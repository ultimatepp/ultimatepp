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
	
	// line should start from 1...
	line++;
	
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
		Exclamation(t_("No code at chosen location !"));
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
	int i = 50;
	while(!started && --i)
	{
		GuiSleep(20);
		Ctrl::ProcessEvents();
		ReadGdb(false);
	}
	if(!started)
	{
		Exclamation(t_("Failed to start application"));
		return;
	}
	Lock();
	while(dbg && !stopped)
	{
		GuiSleep(20);
		Ctrl::ProcessEvents();
		ReadGdb(false);
	}
	Unlock();
	if(stopped)
		CheckStopReason();

	started = stopped = false;
	firstRun = false;
	IdeActivateBottom();
}

void Gdb_MI2::Stop()
{
	started = stopped = false;
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
	locals.WhenLeftDouble = THISBACK1(onExploreExpr, &locals);

	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6);
	watches.Inserting().Removing();
	watches.WhenLeftDouble = THISBACK1(onExploreExpr, &watches);

	int c = EditField::GetStdHeight();
	explorer.AddColumn("", 1);
	explorer.AddColumn("", 6) /*.SetDisplay(Single<VisualDisplay>()) */;
	explorerPane.Add(explorerBackBtn.LeftPos(0, c).TopPos(0, c));
	explorerPane.Add(explorerForwardBtn.LeftPos(c + 2, c).TopPos(0, c));
	explorerPane.Add(explorerExprEdit.HSizePos(2 * c + 4).TopPos(0, c));
	explorerPane.Add(explorer.HSizePos().VSizePos(EditField::GetStdHeight(), 0));
	explorer.NoHeader();
	explorer.WhenLeftDouble = THISBACK(onExplorerChild);
	explorer.WhenBar = THISBACK(ExplorerMenu);

	explorerBackBtn.SetImage(DbgImg::ExplorerBack());
	explorerBackBtn <<= THISBACK(onExplorerBack);
	explorerForwardBtn.SetImage(DbgImg::ExplorerFw());
	explorerForwardBtn <<= THISBACK(onExplorerForward);
	explorerBackBtn.Disable();
	explorerForwardBtn.Disable();
	explorerHistoryPos = -1;

	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6);
	autos.WhenLeftDouble = THISBACK1(onExploreExpr, &autos);

	Add(tab.SizePos());
	tab.Add(watches.SizePos(), t_("Watches"));
	tab.Add(locals.SizePos(), t_("Locals"));
	tab.Add(autos.SizePos(), "Autos");
	tab.Add(explorerPane.SizePos(), t_("Explorer"));
	
	Add(frame.HSizePos(FindTabsRight() + 10, 0).TopPos(2, EditField::GetStdHeight()));
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

	started = false;
	stopped = false;
}

Gdb_MI2::~Gdb_MI2()
{
	IdeRemoveBottom(*this);
	IdeRemoveRight(disas);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											PRIVATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// find free space at right of tabs (we should probably add something to TabCtrl for that..)
int Gdb_MI2::FindTabsRight(void)
{
	if(!tab.GetCount())
		return 0;
	
	int lastTab = tab.GetCount() - 1;
	int i1 = 0, i2 = 10000;
	
	// bisect up it finds a point on last tab...
	int iTab = -1;
	int i;
	while(iTab != lastTab)
	{
		i = (i1 + i2) / 2;
		iTab = tab.GetTab(Point(i, 0));
		if(iTab < 0)
			i2 = i;
		else
			i1 = i;
	}
	
	// now scan for righ tab edge
	i1 = i; i2 = i + 10000;
	while(abs(i1 - i2) > 2)
	{
		i = (i1 + i2) / 2;
		iTab = tab.GetTab(Point(i, 0));
		if(iTab == -1)
			i2 = i;
		else
			i1 = i;
	}
	return i;
}

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
			started = true;
			stopReason.Clear();
			continue;
		}
		else if(s.StartsWith("*stopped"))
		{
			stopped = true;
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

	dbg->Write(String("-") + cmdLine + "\n");

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

void Gdb_MI2::SyncDisas(MIValue &fInfo, bool fr)
{
	if(!disas.IsVisible())
		return;

	// get current frame's address
	adr_t adr = stou(~fInfo["addr"].Get().Mid(2), NULL, 16);
	if(!disas.InRange(adr))
	{
		MIValue code;
		
		// if frame is inside a source file, disassemble current function
		if(fInfo.Find("file") >= 0 && fInfo.Find("line") >= 0)
		{
			String file = fInfo["file"];
			String line = fInfo["line"];
			code = MICmd(Format("data-disassemble -f %s -l %s -n -1 -- 0", file, line))["asm_insns"];
		}
		else
			// otherwise disassemble some -100 ... +100 bytes around address
			code = MICmd(Format("data-disassemble -s %x -e %x -- 0", (void *)(adr - 100), (void *)(adr + 100)))["asm_insns"];
			
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
	autoLine = "";
	if(fInfo.Find("file") >= 0 && fInfo.Find("line") >= 0)
	{
		String file = GetLocalPath(fInfo["file"]);
		int line = atoi(fInfo["line"].Get());
		if(FileExists(file))
		{
			IdeSetDebugPos(GetLocalPath(file), line - 1, fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr(), 0);
			IdeSetDebugPos(GetLocalPath(file), line - 1, disas.HasFocus() ? fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr() : Image(), 1);
			autoLine = IdeGetLine(line - 2) + ' ' + IdeGetLine(line - 1) + ' ' + IdeGetLine(line);
		}
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
	
	SyncData();
	SyncDisas(fInfo, fr);
}

// logs frame data on console
void Gdb_MI2::LogFrame(String const &msg, MIValue &frame)
{
	String file = frame("file", "<unknown>");
	String line = frame("line", "<unknown>");
	String function = frame("function", "<unknown>");
	String addr = frame("addr", "<unknown>");

	PutConsole(Format(msg + " at %s, function '%s', file '%s', line %s", addr, function, file, line));
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
		LogFrame("Hit breakpoint", stopReason["frame"]);
		SyncIde();
	}
	else
	{
		LogFrame(Format("Stopped, reason '%s'", reason), stopReason["frame"]);
		SyncIde();
	}
}

void Gdb_MI2::Step(const char *cmd)
{
	bool b = disas.HasFocus();

	MIValue res = MICmd(cmd);

	int i = 50;
	while(!started && --i)
	{
		GuiSleep(20);
		Ctrl::ProcessEvents();
		ReadGdb(false);
	}
	if(!started)
	{
		Exclamation(t_("Failed to start application"));
		return;
	}

	Lock();
	ReadGdb(false);
	while(dbg && !stopped)
	{
		GuiSleep(20);
		Ctrl::ProcessEvents();
		ReadGdb(false);
	}
	Unlock();
	if(b)
		disas.SetFocus();

	firstRun = false;
	if(stopped)
		CheckStopReason();
	started = stopped = false;
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
	String func = fInfo("func", "<unknown>");
	String file = fInfo("file", "<unknown>");
	String line = fInfo("line", "<unknown>");
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

// update variables on demand (locals, watches....)
void Gdb_MI2::UpdateVars(void)
{
	MIValue updated = MICmd("var-update 2 *")["changelist"];
	for(int iUpd = 0; iUpd < updated.GetCount(); iUpd++)
	{
		String varName = updated[iUpd]["name"];
		int iVar;

		// local variables
		if( (iVar = localVarNames.Find(varName)) >= 0)
		{
			if( updated[iUpd].Find("value") >= 0)
				localVarValues[iVar] = updated[iUpd]["value"];
		}
		
		// watches
		if( (iVar = watchesNames.Find(varName)) >= 0)
		{
			if( updated[iUpd].Find("value") >= 0)
				watchesValues[iVar] = updated[iUpd]["value"];
		}
		
		// autos
		if( (iVar = autosNames.Find(varName)) >= 0)
		{
			if( updated[iUpd].Find("value") >= 0)
				autosValues[iVar] = updated[iUpd]["value"];
		}
	}
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
		if(locIdx.Find(localVarExpressions[iVar]) < 0)
		{
			String varName = localVarNames.Pop();
			MICmd("var-delete \"" + varName + "\"");
			localVarExpressions.Pop();
			localVarValues.Pop();
			localVarTypes.Pop();
		}
	}
	
	// then we shall add missing variables
	for(int iLoc = 0; iLoc < locIdx.GetCount(); iLoc++)
	{
		if(localVarExpressions.Find(locIdx[iLoc]) < 0)
		{
			MIValue var = MICmd(String("var-create - * \"") + locIdx[iLoc] + "\"");
			
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
	UpdateVars();
}

// update stored watches values on demand
void Gdb_MI2::UpdateWatches(void)
{
	// gets variable names from control
	Index<String> exprs;
	for(int i = 0; i < watches.GetCount(); i++)
		exprs.Add(watches.Get(i, 0));
	
	// purge stored watches not available anymore
	for(int iVar = watchesNames.GetCount() - 1; iVar >= 0;  iVar--)
	{
		if(exprs.Find(watchesExpressions[iVar]) < 0)
		{
			String varName = watchesNames.Pop();
			MICmd("var-delete \"" + varName + "\"");
			watchesExpressions.Pop();
			watchesValues.Pop();
			watchesTypes.Pop();
		}
	}

	// then we shall add missing variables
	for(int i = 0; i < exprs.GetCount(); i++)
	{
		if(watchesExpressions.Find(exprs[i]) < 0)
		{
			// the '@' means we create a dynamic variable
			MIValue var = MICmd(String("var-create - @ \"") + exprs[i] + "\"");
			
			// sometimes it has problem creating vars... maybe because they're
			// still not active; we just skip them
			if(var.IsError() || var.IsEmpty())
				continue;
			watchesNames.Add(var["name"]);
			watchesExpressions.Add(exprs[i]);
			watchesTypes.Add(var["type"]);
			watchesValues.Add(var["value"]);
		}
	}

	// and finally, we do an update to refresh changed variables
	UpdateVars();
}

// update stored auto values on demand
void Gdb_MI2::UpdateAutos(void)
{
	// gets variable names from control
	Index<String> exprs;
	for(int i = 0; i < autos.GetCount(); i++)
		exprs.Add(autos.Get(i, 0));
	
	// purge stored autos not available anymore
	for(int iVar = autosNames.GetCount() - 1; iVar >= 0;  iVar--)
	{
		if(exprs.Find(autosExpressions[iVar]) < 0)
		{
			String varName = autosNames.Pop();
			MICmd("var-delete \"" + varName + "\"");
			autosExpressions.Pop();
			autosValues.Pop();
			autosTypes.Pop();
		}
	}

	// then we shall add missing variables
	for(int i = 0; i < exprs.GetCount(); i++)
	{
		if(autosExpressions.Find(exprs[i]) < 0)
		{
			// the '@' means we create a dynamic variable
			MIValue var = MICmd(String("var-create - @ \"") + exprs[i] + "\"");
			
			// sometimes it has problem creating vars... maybe because they're
			// still not active; we just skip them
			if(var.IsError() || var.IsEmpty())
				continue;
			autosNames.Add(var["name"]);
			autosExpressions.Add(exprs[i]);
			autosTypes.Add(var["type"]);
			autosValues.Add(var["value"]);
		}
	}

	// and finally, we do an update to refresh changed variables
	UpdateVars();
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
	// update local watches cache, if needed
	UpdateWatches();
	
	// re-fill the control
	VectorMap<String, String> prev = DataMap(watches);
	for(int i = 0; i < watches.GetCount(); i++)
	{
		String expr = watches.Get(i, 0);
		int idx = watchesExpressions.Find(expr);
		if(idx >= 0)
			watches.Set(i, 1, "(" + watchesTypes[idx] + ")" + watchesValues[idx]);
		else
			watches.Set(i, 1, t_("<can't evaluate expression>"));
	}
	MarkChanged(prev, watches);
}

// sync auto vars treectrl
void Gdb_MI2::SyncAutos()
{
	VectorMap<String, String> prev = DataMap(autos);
	autos.Clear();

	// read expressions around cursor line
	CParser p(autoLine);
	while(!p.IsEof())
	{
		if(p.IsId())
		{
			String exp = p.ReadId();
			for(;;)
			{
				if(p.Char('.') && p.IsId())
					exp << '.';
				else
				if(p.Char2('-', '>') && p.IsId())
					exp << "->";
				else
					break;
				exp << p.ReadId();
			}
			if(autos.Find(exp) < 0)
				autos.Add(exp, "");
		}
		p.SkipTerm();
	}
	autos.Sort();

	// update autos cache, if needed
	UpdateAutos();

	for(int i = autos.GetCount() -1; i >= 0; i--)
	{
		String expr = autos.Get(i, 0);
		int idx = autosExpressions.Find(expr);
		if(idx >= 0)
			autos.Set(i, 1, "(" + autosTypes[idx] + ")" + autosValues[idx]);
		else
			autos.Remove(i);
	}

	MarkChanged(prev, autos);
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
	MICmd("interpreter-exec console \"source " + fName + "\"");
	FileDelete(fName);
}

// format watch line
String Gdb_MI2::FormatWatchLine(String exp, String const &val, int level)
{
	if(exp.GetCount() < 20)
		exp = exp + String(' ', 20);
	else
		exp = exp.Left(17) + "...";
	exp = exp.Left(20) + " = " + val;
	return String(' ', level * 4) + exp;
}

// deep watch current quickwatch variable
void Gdb_MI2::WatchDeep(String const &parentExp, String const &var, int level)
{
	MIValue childInfo = MICmd("var-list-children 1 \"" + var + "\" 0 100");
	int nChilds = min(atoi(childInfo["numchild"].Get()), 100);
	if(nChilds)
	{
		MIValue &childs = childInfo["children"];
		for(int i = 0; i < childs.GetCount(); i++)
		{
			MIValue child = childs[i];
			String exp = child["exp"];
			
			// handle pseudo children...
			while(exp == "public" || exp == "private" || exp == "protected")
			{
				child = MICmd(String("var-list-children 1 \"") + child["name"] + "\"")["children"][0];
				exp = child["exp"];
			}
			if(isdigit(exp[0]))
				exp = '[' + exp + ']';

			String type = child("type", "");
			if(!type.IsEmpty())
				type = "(" + type + ")";
			String value = child["value"];
			
			// try to format nicely results...
			quickwatch.value <<= (String)~quickwatch.value + "\n" + FormatWatchLine(parentExp + exp, type + value, level);
			
			// recursive deep watch
			WatchDeep(exp, child["name"], level + 1);
		}
	}
}

// opens quick watch dialog
void Gdb_MI2::QuickWatch()
{
	for(;;)
	{
		int q = quickwatch.Run();
		if(q == IDCANCEL)
			break;
		MIValue v = MICmd("var-create - @ " + (String)~quickwatch.expression);
		if(!v.IsError())
		{
			String exp = ~quickwatch.expression;
			String type = v("type", "");
			if(!type.IsEmpty())
				type = "(" + type + ")";
			String value = v["value"];
			quickwatch.value <<= FormatWatchLine(exp, type + value, 0);
			quickwatch.expression.AddHistory();
			String name = v["name"];
			WatchDeep(exp, name, 1);
			MICmd("var-delete " + name);
		}
		else
			quickwatch.value <<= t_("<can't evaluate expression>");
	}
	quickwatch.Close();
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

/////////////////////////////////////  EXPLORER //////////////////////////////////////////////////////

void Gdb_MI2::doExplore(String const &expr, String var, bool isChild, bool appendHistory)
{
	// set the expression inside expression editor
	explorerExprEdit = expr;

	// store current expr as "parent expr" to be used
	// when displaying childs... GDB is not reliable on this
	explorerParentExpr = expr;
	
	// try to find a suitable already created var inside history
	// if not, and if not a child, we shall create it
	if(var.IsEmpty())
	{
		// empty var is allowed ONLY for non-childs exploring
		if(isChild)
			return;
		
		int iVar = explorerHistoryExpressions.Find(expr);
		if(iVar < 0)
		{
			MIValue v = MICmd("var-create - @ \"" + expr + "\"");
			if(v.IsEmpty() || v.IsError())
				return;
			var = v["name"];
		}
		else
			var = explorerHistoryVars[iVar];
	}

	// update the history : trim it from past current position
	// and append it at end
	if(appendHistory)
	{
		if(explorerHistoryPos >= 0)
		{
			// frees all non-child variables following current position
			for(int i = explorerHistoryPos + 1; i < explorerHistoryVars.GetCount(); i++)
			{
				if(!explorerHistoryChilds[i] && explorerHistoryVars[i] != var)
				{
					MICmd("var-delete \"" + explorerHistoryVars[i] + "\"");
				}
			}
			explorerHistoryPos++;
			explorerHistoryExpressions.Trim(explorerHistoryPos);
			explorerHistoryVars.Trim(explorerHistoryPos);
			explorerHistoryChilds.Trim(explorerHistoryPos);
		}
		else
			explorerHistoryPos = 0;
		explorerHistoryExpressions.Add(expr);
		explorerHistoryVars.Add(var);
		explorerHistoryChilds.Add(isChild);
	}
	
	// here we've finally got variable to explore, just read it
	// and set into first row of explorer ArrayCtrl
	String value = MICmd("var-evaluate-expression \"" + var + "\"")["value"];
	String type = MICmd("var-info-type \"" + var + "\"")["type"];
	explorer.Clear();
	explorerChildVars.Clear();
	explorer.Add("=", "(" + type + ")" + value, var);
	
	// now we shall add variable children... we limit them to a number of 100
	// which should be anough. Alternative would be to display them in ranges,
	// but this over-complicates the application.
	MIValue childInfo = MICmd("var-list-children 1 \"" + var + "\" 0 100");
	int nChilds = min(atoi(childInfo["numchild"].Get()), 100);
	if(nChilds)
	{
		MIValue &childs = childInfo["children"];
		for(int i = 0; i < childs.GetCount(); i++)
		{
			MIValue child = childs[i];
			String exp = child["exp"];
			
			// handle pseudo children...
			while(exp == "public" || exp == "private" || exp == "protected")
			{
				child = MICmd(String("var-list-children 1 \"") + child["name"] + "\"")["children"][0];
				exp = child["exp"];
			}
			if(isdigit(exp[0]))
				exp = '[' + exp + ']';
			String type = child("type", "");
			if(!type.IsEmpty())
				type = "(" + type + ")";
			String value = child["value"];
			explorer.Add(exp, type + value);
			explorerChildVars.Add(child["name"]);
		}
	}
	
	explorerBackBtn.Enable(explorerHistoryPos > 0);
	explorerForwardBtn.Enable(explorerHistoryPos < explorerHistoryVars.GetCount() - 1);
}

// 
void Gdb_MI2::onExploreExpr(ArrayCtrl *what)
{
	String expr;
	if(!what)
	{
		// if expression don't come from another ArrayCtrl
		// we use the expression editbox
		expr = ~explorerExprEdit;
	}
	{
		// otherwise, we use the expression from sending ArrayCtrl
		int line = what->GetCursor();
		if(line >= 0)
			expr = what->Get(line, 0);
	}
	// nothing to do on empty expression
	if(expr == "")
		return;
	doExplore(expr, "", false, true);
	
	// activate explorer tab
	tab.Set(3);
}

void Gdb_MI2::onExplorerChild()
{
	// click on first line (value line) does nothing
	int line = explorer.GetCursor();
	if(line < 1)
		return;
	if(--line < explorerChildVars.GetCount())
	{
		String var = explorerChildVars[line];
		String varExp = var;
		String expr = MICmd("var-info-expression \"" + var + "\"")["exp"];
		if(expr[0] != '[' && expr[0] != '.')
			if(isdigit(expr[0]))
				expr = '[' + expr + ']';
			else
				expr = '.' + expr;
		expr = explorerParentExpr + expr;
		doExplore(expr, var, true, true);
	}
}

void Gdb_MI2::onExplorerBack()
{
	if(explorerHistoryPos < 1)
		return;
	explorerHistoryPos--;
	String expr = explorerHistoryExpressions[explorerHistoryPos];
	String var = explorerHistoryVars[explorerHistoryPos];
	bool isChild = explorerHistoryChilds[explorerHistoryPos];
	doExplore(expr, var, isChild, false);
}

void Gdb_MI2::onExplorerForward()
{
	if(explorerHistoryPos >= explorerHistoryVars.GetCount() - 1)
		return;
	explorerHistoryPos++;
	String expr = explorerHistoryExpressions[explorerHistoryPos];
	String var = explorerHistoryVars[explorerHistoryPos];
	bool isChild = explorerHistoryChilds[explorerHistoryPos];
	doExplore(expr, var, isChild, false);
}

void Gdb_MI2::ExplorerMenu(Bar& bar)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

// create GDB process and initializes it
bool Gdb_MI2::Create(One<Host> _host, const String& exefile, const String& cmdline)
{
	host = _host;
	dbg = host->StartProcess("gdb " + GetHostPath(exefile) + " --interpreter=mi -q");
	if(!dbg) {
		Exclamation(t_("Error invoking gdb !"));
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
