#include "Debuggers.h"

#include "PrettyPrinters.brc"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											PUBLIC IDE INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gdb_MI2::DebugBar(Bar& bar)
{
	bar.Add("Stop debugging", THISBACK(Stop)).Key(K_SHIFT_F5);
	bar.Separator();
	bool b = !IdeIsDebugLock();
	bar.Add(b, "Step into", DbgImg::StepInto(), THISBACK1(Step, disas.HasFocus() ? "exec-step-instruction" : "exec-step")).Key(K_F11);
	bar.Add(b, "Step over", DbgImg::StepOver(), THISBACK1(Step, disas.HasFocus() ? "exec-next-instruction" : "exec-next")).Key(K_F10);
	bar.Add(b, "Step out", DbgImg::StepOut(), THISBACK1(Step, "exec-finish")).Key(K_SHIFT_F11);
	bar.Add(b, "Run to", DbgImg::RunTo(), THISBACK(doRunTo)).Key(K_CTRL_F10);
	bar.Add(b, "Run", DbgImg::Run(), THISBACK(Run)).Key(K_F5);
	bar.Separator();
	bar.Add(b, "Quick watch", THISBACK(QuickWatch)).Key(K_CTRL_Q);
	bar.MenuSeparator();
	bar.Add(b, "Copy backtrace", THISBACK(CopyStack));
	bar.Add(b, "Copy dissassembly", THISBACK(CopyDisas));
}

static int CharFilterReSlash(int c)
{
	return c == '\\' ? '/' : c;
}

bool Gdb_MI2::SetBreakpoint(const String& filename, int line, const String& bp)
{
	String file = Filter(host->GetHostPath(NormalizePath(filename)), CharFilterReSlash);
	
	// gets all breakpoints
	MIValue bps = GetBreakpoints();
	
	// check wether we've got already a breakpoint here
	// and remove it
	MIValue brk = bps.FindBreakpoint(file, line);
	if(!brk.IsEmpty())
		ASSERT(!MICmd("break-delete " + brk["number"]).IsError());
	
	if(bp.IsEmpty())
		return true;
	else if(bp[0] == 0xe)
		return !MICmd("break-insert " + Format("%s:%d", file, line)).IsError();
	else
		return !MICmd("break-insert " + Format("-c \"%s\" %s:%d", bp, file, line)).IsError();
}

bool Gdb_MI2::RunTo()
{
	String bi;
	bool df = disas.HasFocus();
	if(df)
	{
		if(!disas.GetCursor())
			return false;
		bi = Sprintf("*0x%X", disas.GetCursor());
	}
	else
		bi = BreakPos(IdeGetFileName(), IdeGetFileLine());

	// sets a temporary breakpoint on cursor location
	// it'll be cleared automatically on first stop
	if(!TryBreak(IdeGetFileName(), IdeGetFileLine(), true))
	{
		Exclamation("No code at chosen location !");
		return false;
	}

	Run();
	if(df)
		disas.SetFocus();
	return true;
}

void Gdb_MI2::Run()
{
	MIValue val;
	if(firstRun)
		val = MICmd("exec-run");
	else
		val = MICmd("exec-continue");

	while(dbg && isRunning)
	{
		ReadGdb(false);
		GuiSleep(20);
		Ctrl::ProcessEvents();
	}
	if(!isRunning)
		CheckStopReason();

	firstRun = false;
	IdeActivateBottom();
}

void Gdb_MI2::Stop()
{
	if(dbg && dbg->IsRunning())
		dbg->Kill();
}

bool Gdb_MI2::IsFinished()
{
	return !dbg->IsRunning() && !IdeIsDebugLock();
}

bool Gdb_MI2::Tip(const String& exp, CodeEditor::MouseTip& mt)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											CONSTRUCTOR / DESTRUCTOR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Gdb_MI2::Gdb_MI2()
{
	CtrlLayout(regs);
	regs.Height(regs.GetLayoutSize().cy);
	AddReg("eax", &regs.eax);
	AddReg("ebx", &regs.ebx);
	AddReg("ecx", &regs.ecx);
	AddReg("edx", &regs.edx);
	AddReg("esi", &regs.esi);
	AddReg("edi", &regs.edi);
	AddReg("ebp", &regs.ebp);
	AddReg("esp", &regs.esp);
	regs.Color(SColorLtFace);
	regs.AddFrame(TopSeparatorFrame());
	regs.AddFrame(RightSeparatorFrame());

	locals.NoHeader();
	locals.AddColumn("", 1);
	locals.AddColumn("", 6);
	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6);
	watches.Inserting().Removing();
	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6);
	Add(tab.SizePos());
	tab.Add(watches.SizePos(), "Watches");
	tab.Add(locals.SizePos(), "Locals");
	tab.Add(autos.SizePos(), "Autos");
	Add(frame.HSizePos(200, 0).TopPos(2, EditField::GetStdHeight()));
	frame.Ctrl::Add(dlock.SizePos());
	dlock = "  Running..";
	dlock.SetFrame(BlackFrame());
	dlock.SetInk(Red);
	dlock.NoTransparent();
	dlock.Hide();

	CtrlLayoutOKCancel(quickwatch, "Quick watch");
	quickwatch.WhenClose = quickwatch.Breaker(IDCANCEL);
	quickwatch.value.SetReadOnly();
	quickwatch.value.SetFont(Courier(12));
	quickwatch.Sizeable().Zoomable();
	quickwatch.NoCenter();
	quickwatch.SetRect(0, 150, 300, 400);
	quickwatch.Icon(DbgImg::QuickWatch());

	Transparent();

	isRunning = false;
}

Gdb_MI2::~Gdb_MI2()
{
	IdeRemoveBottom(*this);
	IdeRemoveRight(disas);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											PRIVATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// lock/unlock debugger controls
void Gdb_MI2::Lock()
{
	IdeDebugLock();
	watches.Disable();
	locals.Disable();
	frame.Disable();
	dlock.Show();
}

void Gdb_MI2::Unlock()
{
	if(IdeDebugUnLock())
	{
		watches.Enable();
		locals.Enable();
		frame.Enable();
		dlock.Hide();
	}
}

// read debugger output analyzing command responses and async output
// things are quite tricky because debugger output seems to be
// slow and we have almost no terminator to stop on -- (gdb) is not
// so reliable as it can happen (strangely) in middle of nothing
MIValue Gdb_MI2::ParseGdb(String const &output, bool wait)
{
	MIValue res;

	// parse result data
	StringStream ss(output);
	while(!ss.IsEof())
	{
		String s = TrimBoth(ss.GetLine());
		
		// check 'running' and 'stopped' async output
		if(s.StartsWith("*running"))
		{
			isRunning = true;
			stopReason.Clear();
			continue;
		}
		else if(s.StartsWith("*stopped"))
		{
			isRunning = false;
			s = '{' + s.Mid(9) + '}';
			stopReason = MIValue(s);
			continue;
		}
		
		// skip asynchronous responses
		// in future, we could be gather/use them
		if(s[0] == '*'|| s[0] == '=')
			continue;
		
		// here handling of command responses
		// we're not interested either, as we use MI interface
		if(s[0] == '~')
			continue;
		
		// here handling of target output
		// well, for now discard this one too, but it should go on console output
		if(s[0] == '~')
			continue;
	
		// here handling of gdb log/debug message
		// not interesting here
		if(s[0] == '&')
			continue;
		
		// now we SHALL have something starting with any of
		// // "^done", "^running", "^connected", "^error" or "^exit" records
		if(s.StartsWith("^done") || s.StartsWith("^running"))
		{
			// here we've got succesful command output in list form, if any
			// shall skip the comma; following can be a serie of pairs,
			// or directly an array of maps in form of :
			// [{key="value",key="value",...},{key="value"...}...]
			
			int i = 5; // just skip shortest, ^done
			while(s[i] && s[i] != ',')
				i++;
			if(!s[i])
				continue;
			i++;
			if(!s[i])
				continue;
			res = MIValue(s.Mid(i));
			continue;
		}
		else if(s.StartsWith("^error"))
		{
			// first array element is reserved for command result status
			s = s.Right(12); // '^error,msg=\"'
			s = s.Left(s.GetCount() - 1);
			res.SetError(s);
		}
		else
			continue;
	}
	return res;
}

MIValue Gdb_MI2::ReadGdb(bool wait)
{
	String output;
	MIValue res;

	if(wait)
	{
		// this path is for locking read -- we NEED cmd output
		int retries = 4;
		while(dbg)
		{
			String s;
			dbg->Read(s);
			
			// we really NEED an answer
			if(s.IsEmpty() && output.IsEmpty())
				continue;
			
			// on first empty string, we check if we really
			// got a valid answer
			if(s.IsEmpty())
			{
				res = ParseGdb(output);
				
				// if no valid answer (or, well, an empty answer..)
				// we retry up to 4 times
				if(res.IsEmpty() && --retries)
				{
					Sleep(20);
					continue;
				}
				return res;
			}
			output += s;
		}
	}
	else
	{
		// this path is for NON locking read -- we just cleanup
		// streaming output, mostly
		while(dbg)
		{
			String s;
			dbg->Read(s);
			if(s.IsEmpty())
				break;
			output += s;
		}
	}
	if(output.IsEmpty())
		return res;
	return ParseGdb(output);
}

// new-way commands using GDB MI interface
// on input  : MI interface command line
// on output : an MIValue containing GDB output
// STREAM OUTPUT
// ~						command response
// @						target output
// &						gdb log/debug messages
//
// RESULT RECORDS
// "^done" [ "," results ]
// "^running"				same as "^done"
//	"^connected"			gdb has connected to a remote target.
//	"^error" "," c-string	The operation failed. The c-string contains the corresponding error message.
//	"^exit"					gdb has terminate
//
// ASYNCHRONOUS RECORDS
// *running,thread-id="thread"
// *stopped,reason="reason",thread-id="id",stopped-threads="stopped",core="core"
// =thread-group-added,id="id"
// =thread-group-removed,id="id"
// =thread-group-started,id="id",pid="pid"
// =thread-group-exited,id="id"[,exit-code="code"]
// =thread-created,id="id",group-id="gid"
// =thread-exited,id="id",group-id="gid"
// =thread-selected,id="id"
// =library-loaded,...
// =library-unloaded,...
// =breakpoint-created,bkpt={...}
// =breakpoint-modified,bkpt={...}
// =breakpoint-deleted,bkpt={...}
//
// FRAME INFO INSIDE RESPONSES
// level		The level of the stack frame. The innermost frame has the level of zero. This field is always present.
// func		The name of the function corresponding to the frame. This field may be absent if gdb is unable to determine the function name.
// addr		The code address for the frame. This field is always present.
// file		The name of the source files that correspond to the frame's code address. This field may be absent.
// line		The source line corresponding to the frames' code address. This field may be absent.
// from		The name of the binary file (either executable or shared library) the corresponds to the frame's code address. This field may be absent. 

// THREAD INFO INSIDE RESPONSES
// id			The numeric id assigned to the thread by gdb. This field is always present.
// target-id	Target-specific string identifying the thread. This field is always present.
// details		Additional information about the thread provided by the target. It is supposed to be human-readable and not interpreted by the frontend. This field is optional.
// state		Either `stopped' or `running', depending on whether the thread is presently running. This field is always present.
// core		The value of this field is an integer number of the processor core the thread was last seen on. This field is optional. 
//
// REMARKS : by now, we just handle synchronous output and check asynchronous one just to detect
// debugger run/stop status -- all remaining asynchrnonous output is discarded
MIValue Gdb_MI2::MICmd(const char *cmdLine)
{

	// sends command to debugger and get result data

	// should handle dbg unexpected termination ?
	if(!dbg || !dbg->IsRunning() || IdeIsDebugLock())
		return MIValue();
	
	// consume previous output from gdb... don't know why sometimes
	// is there and gives problems to MI interface. We shall maybe
	// parse and store it somewhere
	ReadGdb(false);
	Lock();
	dbg->Write(String("-") + cmdLine + "\n");
	Unlock();
	
	return ReadGdb();
}

// format breakpoint line from ide file and line
String Gdb_MI2::BreakPos(String const &file, int line)
{
	return String().Cat() << Filter(host->GetHostPath(NormalizePath(file)), CharFilterReSlash) << ":" << line + 1;
}

// set breakpoint
MIValue Gdb_MI2::InsertBreakpoint(const char *file, int line)
{
	return MICmd("break-insert" + BreakPos(file, line));
}

// get breakpoints info
MIValue Gdb_MI2::GetBreakpoints(void)
{
	// issue a break-list command
	return MICmd("break-list")["BreakpointTable"];
}

MIValue Gdb_MI2::GetBreakpoint(int id)
{
	return MIValue();
}

MIValue Gdb_MI2::GetBreakPoint(const char *file, int line)
{
	return MIValue();
}

bool Gdb_MI2::TryBreak(String const &file, int line, bool temp)
{
	String bp;
	if(temp)
		bp = "-t ";
	bp += BreakPos(file, line);
	MIValue res = MICmd(String("break-insert ") + bp);
	return !res.IsError();
}

/*
void Gdb_MI2::SetDisas(const String& text)
{
	disas.Clear();
	StringStream ss(text);
	while(!ss.IsEof()) {
		String ln = ss.GetLine();
		CParser p(ln);
		if(p.Char2('0', 'x')) {
			dword adr = p.IsNumber(16) ? p.ReadNumber(16) : 0;
			String code;
			String args;
			int level = 0;
			while(!p.IsEof()) {
				if(p.Char(':') && level == 0)
					break;
				else
				if(p.Char('<'))
					level++;
				else
				if(p.Char('>'))
					level--;
				else
					p.GetChar();
			}
			p.Spaces();
			if(p.IsId()) {
				code = p.ReadId();
				for(;;) {
					if(p.Spaces())
						args.Cat(' ');
					if(p.IsEof())
						break;
					if(p.Char2('0', 'x')) {
						dword adr = 0;
						if(p.IsNumber(16))
							adr = p.ReadNumber(16);
						String fname;
						bool   usefname = false;
						if(p.Char('<')) {
							const char *b = p.GetPtr();
							int level = 1;
							usefname = true;
							while(!p.IsEof()) {
								if(p.Char('>') && --level == 0) {
									fname = String(b, p.GetPtr() - 1);
									break;
								}
								if(p.Char('<'))
									level++;
								if(p.Char('+'))
									usefname = false;
								else {
									p.GetChar();
									p.Spaces();
								}
							}
						}
						args << (usefname ? fname : Sprintf("0x%X", adr));
						disas.AddT(adr);
					}
					else
					if(p.Id("DWORD"))
						args.Cat("dword ");
					else
					if(p.Id("WORD"))
						args.Cat("word ");
					else
					if(p.Id("BYTE"))
						args.Cat("byte ");
					else
					if(p.Id("PTR"))
						args.Cat("ptr ");
					else
						args.Cat(p.GetChar());
				}
			}
			disas.Add(adr, code, args);
		}
	}
}
*/

void Gdb_MI2::SyncDisas(MIValue &fInfo, bool fr)
{
	if(!disas.IsVisible())
		return;

	// get current frame's address
	adr_t adr = stou(~fInfo["addr"].Get().Mid(2), NULL, 16);
	if(!disas.InRange(adr))
	{
		String file = fInfo["file"];
		String line = fInfo["line"];
		MIValue code = MICmd(Format("data-disassemble -f %s -l %s -n -1 -- 0", file, line))["asm_insns"];
		disas.Clear();
		for(int iLine = 0; iLine < code.GetCount(); iLine++)
		{
			MIValue &line = code[iLine];
			adr_t address = stou(~line["address"].Get().Mid(2), NULL, 16);
			String inst = line["inst"];
			int spc = inst.Find(' ');
			String opCode, operand;
			if(spc >= 0)
			{
				opCode = inst.Left(spc);
				operand = TrimBoth(inst.Mid(spc));
			}
			else
				opCode = inst;
			disas.Add(address, opCode, operand);
		}
	}
}

// sync ide display with breakpoint position
void Gdb_MI2::SyncIde(bool fr)
{
	// get current frame info and level
	MIValue fInfo = MICmd("stack-info-frame")["frame"];
	int level = atoi(fInfo["level"].Get());
	
	// if we got file and line info, we can sync the source editor position
	if(fInfo.Find("file") >= 0 && fInfo.Find("line") >= 0)
	{
		String file = fInfo["file"];
		int line = atoi(fInfo["line"].Get());
		IdeSetDebugPos(GetLocalPath(file), line - 1, fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr(), 0);
		IdeSetDebugPos(GetLocalPath(file), line - 1, disas.HasFocus() ? fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr() : Image(), 1);
	}

	// i we got an address, we can sync the assembly position
	if(fInfo.Find("addr") >= 0)
	{
	}

	// get the arguments for current frame
	MIValue fArgs = MICmd(Format("stack-list-arguments 1 %d %d", level, level))["stack-args"][0]["args"];
	
	// setup droplist
	frame.Clear();
	frame.Add(0, FormatFrame(fInfo, fArgs));
	frame <<= 0;
	
	SyncLocals();
	SyncDisas(fInfo, fr);
}

// check for stop reason
void Gdb_MI2::CheckStopReason(void)
{
	ASSERT(!stopReason.IsEmpty());
	String reason = stopReason["reason"];
	if(reason == "exited-normally")
	{
		Stop();
		PutConsole("Program exited normally.");
	}
	else if(reason == "exited")
	{
		Stop();
		PutConsole("Program exited with code ");
	}
	else if(reason == "breakpoint-hit")
	{
		MIValue &v = stopReason["frame"];
		String file 	= v["file"];
		String line 	= v["line"];
		String function = v["func"];
		String addr 	= v["addr"];
		PutConsole(Format("Hit breakpoint at %s, function '%s', file '%s', line %s", addr, function, file, line));
		SyncIde();
	}
	else
	{
		MIValue &v = stopReason["frame"];
		String file 	= v["file"];
		String line 	= v["line"];
		String function = v["func"];
		String addr 	= v["addr"];
		PutConsole(Format("Stopped at %s, function '%s', file '%s', line %s, reason '%s'", addr, function, file, line, stopReason["reason"].Get()));
		SyncIde();
	}
}

void Gdb_MI2::Step(const char *cmd)
{
	bool b = disas.HasFocus();
	MIValue res = MICmd(cmd);
	if(b)
		disas.SetFocus();

	if(!isRunning)
		CheckStopReason();
	IdeActivateBottom();
}

// setup ide cursor based on disassembler one
void Gdb_MI2::DisasCursor()
{
/*
	if(!disas.HasFocus())
		return;
	int line;
	String file;
	adr_t addr;
	if(ParsePos(FastCmd(Sprintf("info line *0x%X", disas.GetCursor())), file, line, addr))
		IdeSetDebugPos(file, line - 1, DbgImg::DisasPtr(), 1);
	disas.SetFocus();
*/
}

// reset ide default cursor image when disassembler loose focus
void Gdb_MI2::DisasFocus()
{
//	if(!disas.HasFocus())
//		IdeSetDebugPos(file, 0, Null, 1);
}

// create a string representation of frame given its info and args
String Gdb_MI2::FormatFrame(MIValue &fInfo, MIValue &fArgs)
{
	int idx = atoi(fInfo["level"].Get());
	String func = fInfo["func"];
	String file = fInfo["file"];
	String line = fInfo["line"];
	int nArgs = fArgs.GetCount();
	String argLine;
	for(int iArg = 0; iArg < nArgs; iArg++)
	{
		argLine += fArgs[iArg]["name"];
		if(fArgs[iArg].Find("value") >= 0)
			argLine << "=" << fArgs[iArg]["value"];
		argLine << ',';
	}
	if(!argLine.IsEmpty())
		argLine = "(" + argLine.Left(argLine.GetCount() - 1) + ")";
	return Format("%02d-%s%s at %s:%s", idx, func, argLine, file, line);
}

// re-fills frame's droplist when dropping it
void Gdb_MI2::DropFrames()
{
	int q = ~frame;
	frame.Clear();
	
	// get a list of frames
	MIValue frameList = MICmd("stack-list-frames")["stack"];
	frameList.AssertArray();
	
	// get the arguments for all frames, values just for simple types
	MIValue frameArgs = MICmd("stack-list-arguments 1")["stack-args"];
	frameArgs.AssertArray();
	
	// fill the droplist
	for(int iFrame = 0; iFrame < frameArgs.GetCount(); iFrame++)
	{
		MIValue &fInfo = frameList[iFrame];
		MIValue &fArgs = frameArgs[iFrame]["args"];
		frame.Add(iFrame, FormatFrame(fInfo, fArgs));
	}
	frame <<= q;
}

// shows selected stack frame in editor
void Gdb_MI2::ShowFrame()
{
	int i = (int)~frame;
	MICmd(Format("stack-select-frame %d", i));
	SyncIde(i);
}

// update local variables on demand
void Gdb_MI2::UpdateLocalVars(void)
{
	// we try to speed-up at most reading of local vars; this
	// because we need to keep them updated at each step and
	// re-reading as a whole is too time expensive.
	// So, at first we build a list of local variable NAMES only
	MIValue loc = MICmd("stack-list-variables 0")["variables"];
	Index<String>locIdx;
	for(int iLoc = 0; iLoc < loc.GetCount(); iLoc++)
		locIdx.Add(loc[iLoc]["name"]);
	
	// then we 'purge' stored local variables that are not present
	// anymore... that can happen, for example, exiting from a loop
	for(int iVar = localVarNames.GetCount() - 1; iVar >= 0; iVar--)
	{
		if(locIdx.Find(localVarNames[iVar]) < 0)
		{
			localVarNames.Pop();
			localVarExpressions.Pop();
			localVarValues.Pop();
			localVarTypes.Pop();
		}
	}
	
	// then we shall add missing variables
	for(int iLoc = 0; iLoc < locIdx.GetCount(); iLoc++)
	{
		if(localVarNames.Find(locIdx[iLoc]) < 0)
		{
			MIValue var = MICmd(String("var-create - * ") + locIdx[iLoc]);
			
			// sometimes it has problem creating vars... maybe because they're
			// still not active; we just skip them
			if(var.IsError() || var.IsEmpty())
				continue;
			localVarNames.Add(var["name"]);
			localVarExpressions.Add(locIdx[iLoc]);
			localVarTypes.Add(var["type"]);
			localVarValues.Add(var["value"]);
		}
	}
	
	// and finally, we do an update to refresh changed variables
	MIValue updated = MICmd("var-update 2 *")["changelist"];
	for(int iUpd = 0; iUpd < updated.GetCount(); iUpd++)
	{
		String varName = updated[iUpd]["name"];
		int iVar;
		if( (iVar = localVarNames.Find(varName)) < 0)
			continue;
		if( updated[iUpd].Find("value") < 0)
			continue;
		localVarValues[iVar] = updated[iUpd]["value"];
	}
}
	

void Gdb_MI2::SyncLocals()
{
	// update local vars cache, if needed
	UpdateLocalVars();
	
	// reload ide control
	VectorMap<String, String> prev = DataMap(locals);
	locals.Clear();
	for(int i = 0; i < localVarNames.GetCount(); i++)
		locals.Add(localVarExpressions[i], "(" + localVarTypes[i] + ")" + localVarValues[i]);
	MarkChanged(prev, locals);
}

// sync watches treectrl
void Gdb_MI2::SyncWatches()
{
/*
	VectorMap<String, String> prev = DataMap(watches);
	for(int i = 0; i < watches.GetCount(); i++)
		watches.Set(i, 1, Print(watches.Get(i, 0)));
	MarkChanged(prev, watches);
*/
}

// sync auto vars treectrl
void Gdb_MI2::SyncAutos()
{
/*
	VectorMap<String, String> prev = DataMap(autos);
	autos.Clear();
	CParser p(autoline);
	while(!p.IsEof()) {
		if(p.IsId()) {
			String exp = p.ReadId();
			for(;;) {
				if(p.Char('.') && p.IsId())
					exp << '.';
				else
				if(p.Char2('-', '>') && p.IsId())
					exp << "->";
				else
					break;
				exp << p.ReadId();
			}
			if(autos.Find(exp) < 0) {
				String val = Print(exp);
				if(!IsNull(val) && val.Find('(') < 0)
					autos.Add(exp, val);
			}
		}
		p.SkipTerm();
	}
	autos.Sort();
	MarkChanged(prev, autos);
*/
}

// sync data tabs, depending on which tab is shown
void Gdb_MI2::SyncData()
{
	switch(tab.Get()) {
	case 0:
		SyncWatches();
		break;
		
	case 1:
		SyncLocals();
		break;
		
	case 2:
		SyncAutos();
		break;
	}
}

// watches arrayctrl key handling
bool Gdb_MI2::Key(dword key, int count)
{
	if(key >= 32 && key < 65535 && tab.Get() == 2)
	{
		watches.DoInsertAfter();
		Ctrl* f = GetFocusCtrl();
		if(f && watches.HasChildDeep(f))
			f->Key(key, count);
		return true;
	}
	return Ctrl::Key(key, count);
}

// sends pretty-printing scripts
void Gdb_MI2::SendPrettyPrinters(void)
{
	String fName = GetTempFileName();
	fName = fName.Left(fName.GetCount() - 3) + "py";
	SaveFile(fName, (const char *)PrettyPrinters);
	DLOG(MICmd("interpreter-exec console \"source " + fName + "\"").Dump());
	FileDelete(fName);
}

// opens quick watch dialog
void Gdb_MI2::QuickWatch()
{
/*
	for(;;) {
		int q = quickwatch.Run();
		if(q == IDCANCEL)
			break;
		FastCmd("set print pretty on");
		String s = FastCmd("p " + (String)~quickwatch.expression);
		const char *a = strchr(s, '=');
		if(a) {
			a++;
			while(*a == ' ')
				a++;
			quickwatch.value <<= a;
			quickwatch.expression.AddHistory();
		}
		else
			quickwatch.value <<= s;
		FastCmd("set print pretty off");
	}
	quickwatch.Close();
*/
}

// copy stack frame list to clipboard
void Gdb_MI2::CopyStack()
{
	DropFrames();
	String s;
	for(int i = 0; i < frame.GetCount(); i++)
		s << frame.GetValue(i) << "\n";
	WriteClipboardText(s);
}

// copy disassembly listing to clipboard
void Gdb_MI2::CopyDisas()
{
	disas.WriteClipboard();
}

// create GDB process and initializes it
bool Gdb_MI2::Create(One<Host> _host, const String& exefile, const String& cmdline)
{
	host = _host;
	dbg = host->StartProcess("gdb " + GetHostPath(exefile) + " --interpreter=mi -q");
	if(!dbg) {
		Exclamation("Error invoking gdb !");
		return false;
	}
	IdeSetBottom(*this);
	IdeSetRight(disas);

	disas.AddFrame(regs);
	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);
	frame.WhenDrop = THISBACK(DropFrames);
	frame <<= THISBACK(ShowFrame);

	watches.WhenAcceptEdit = THISBACK(SyncData);
	tab <<= THISBACK(SyncData);

	MICmd("gdb-set disassembly-flavor intel");
	MICmd("gdb-set exec-done-display off");
	MICmd("gdb-set annotate 1");
	MICmd("gdb-set height 0");
	MICmd("gdb-set width 0");
	MICmd("gdb-set confirm off");
	MICmd("gdb-set print asm-demangle");

	if(!IsNull(cmdline))
		MICmd("gdb-set args " + cmdline);
	
	// enable pretty printing
	SendPrettyPrinters();
	MICmd("enable-pretty-printing");

	firstRun = true;

	return true;
}

One<Debugger> Gdb_MI2Create(One<Host> host, const String& exefile, const String& cmdline)
{
	Gdb_MI2 *dbg = new Gdb_MI2;
	if(!dbg->Create(host, exefile, cmdline))
	{
		delete dbg;
		return NULL;
	}
	return dbg;
}
