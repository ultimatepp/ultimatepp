#include "Debuggers.h"
#include <ide/ide.h>

#include "TypeSimplify.h"

#define LDUMP(x) // DDUMP(x)
#define LLOG(x)  // DLOG(x)

void WatchEdit::HighlightLine(int line, Vector<Highlight>& h, int pos)
{
	Color cEven = Blend(SColorInfo, White, 220);
	Color cOdd = Blend(SColorInfo, White, 128);
	for(int i = 0; i < h.GetCount(); i++)
		h[i].paper = (line % 2 ? cOdd : cEven);
}

// fill a pane with data from a couple of arrays without erasing it first
// (avoid re-painting and resetting scroll if not needed)
void Gdb_MI2::FillPane(ArrayCtrl &pane, Index<String> const &nam, Vector<String> const &val)
{
	GuiLock __;
	
	int oldCount = pane.GetCount();
	int newCount = nam.GetCount();
	if(newCount < oldCount)
		for(int i = oldCount - 1; i >= newCount; i--)
			pane.Remove(i);
	for(int i = 0; i < min(oldCount, newCount); i++)
	{
		pane.Set(i, 0, nam[i]);
		pane.Set(i, 1, val[i]);
	}
	for(int i = oldCount; i < newCount; i++)
		pane.Add(nam[i], val[i]);
	
	SyncWidth(pane);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											PUBLIC IDE INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gdb_MI2::DebugBar(Bar& bar)
{
	bar.Add("Stop debugging", DbgImg::StopDebug(), THISBACK(Stop)).Key(K_SHIFT_F5);
	bar.Separator();
#ifdef PLATFORM_POSIX
	bar.Add(!stopped, "Asynchronous break", THISBACK(AsyncBrk));
#endif
	bool b = !IdeIsDebugLock();
	bar.Add(b, "Step into", DbgImg::StepInto(), THISBACK1(Step, disas.HasFocus() ? "exec-step-instruction" : "exec-step")).Key(K_F11);
	bar.Add(b, "Step over", DbgImg::StepOver(), THISBACK1(Step, disas.HasFocus() ? "exec-next-instruction" : "exec-next")).Key(K_F10);
	bar.Add(b, "Step out", DbgImg::StepOut(), THISBACK1(Step, "exec-finish")).Key(K_SHIFT_F11);
	bar.Add(b, "Run to", DbgImg::RunTo(), THISBACK(doRunTo)).Key(K_CTRL_F10);
	bar.Add(b, "Run", DbgImg::Run(), THISBACK(Run)).Key(K_F5);
	bar.MenuSeparator();
	bar.Add(b, "Quick watch", THISBACK(QuickWatch)).Key(K_CTRL_Q);
	bar.MenuSeparator();
	bar.Add(b, "Copy backtrace", THISBACK(CopyStack));
	bar.Add(b, "Copy dissassembly", THISBACK(CopyDisas));
}

static int CharFilterReSlash2(int c)
{
	return c == '\\' ? '/' : c;
}

bool Gdb_MI2::SetBreakpoint(const String& filename, int line, const String& bp)
{
	String file = Filter(host->GetHostPath(NormalizePath(filename)), CharFilterReSlash2);
	
	// gets all breakpoints
	MIValue bps = GetBreakpoints();
	
	// line should start from 1...
	line++;
	
	// check wether we've got already a breakpoint here
	// and remove it
	MIValue brk = pick(bps.FindBreakpoint(file, line));
	if(!brk.IsEmpty())
		if(!MICmd(Format("break-delete %s", brk["number"].Get())))
		{
			Exclamation(t_("Couldn't remove breakpoint"));
			return false;
		}
	
	if(bp.IsEmpty())
		return true;
	else if(bp[0] == 0xe)
		return MICmd(Format("break-insert %s:%d", file, line));
	else
		return MICmd(Format("break-insert -c \"%s\" %s:%d", bp, file, line));
}

bool Gdb_MI2::RunTo()
{
	String bi;
	bool df = disas.HasFocus();
	bool res;
	// sets a temporary breakpoint on cursor location
	// it'll be cleared automatically on first stop
	if(df)
	{
		if(!disas.GetCursor())
			return false;
		res = TryBreak(disas.GetCursor(), true);
	}
	else
		res = TryBreak(IdeGetFileName(), IdeGetFileLine() + 1, true);

	if(!res)
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
		val = MICmd("exec-continue --all");
	
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
		IdeEndDebug();
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

void Gdb_MI2::AsyncBrk()
{
#ifdef PLATFORM_POSIX
	// send interrupt command to debugger
	if(!InterruptDebugger())
		return;
	
	// if successful, wait for some time for 'stopped' flag to become true
	for(int iMsec = 0; iMsec < 1000; iMsec += 20)
	{
		ReadGdb(false);
		Sleep(20);
		Ctrl::ProcessEvents();
		if(stopped)
			break;
	}
#endif
}

#ifdef flagMT
// mutex-protected functions
bool Gdb_MI2::IsThreadRunning(void)
{
	bool b;
	INTERLOCKED_(mutex) {
		b = threadRunning;
	}
	return b;
}

void Gdb_MI2::IncThreadRunning()
{
	INTERLOCKED_(mutex) {
		threadRunning++;
	}
}

void Gdb_MI2::DecThreadRunning()
{
	INTERLOCKED_(mutex) {
		threadRunning--;
	}
}

bool Gdb_MI2::IsStopThread(void)
{
	bool b;
	INTERLOCKED_(mutex) {
		b = stopThread;
	}
	return b;
}

void Gdb_MI2::SetStopThread(bool b)
{
	INTERLOCKED_(mutex) {
		stopThread = b;
	}
}

// shut down threads and wait till done
void Gdb_MI2::ShutDownThreads(void)
{
	// signal all other threads to stop
	SetStopThread(true);
	
	// interrupt any active GDB command
	InterruptCommand();
	
	// ugly hack, otherwise service thread can deadlock
	// MUST CHECK THIS ONE....
	int n = LeaveGuiMutexAll();
	
	// give some time to recover
	do
	{
		Sleep(20);
	}
	while(IsThreadRunning());

	// RE-ENTER GUI MUTEX -- SEE ABOVE...
	EnterGuiMutex(n);
	
	// remove thread stopping flag
	SetStopThread(false);
}


#endif

#ifdef flagMT
// quick exit from service thread when called and 'stopThread' is set
// throws a BreakExc exception
void Gdb_MI2::RaiseIfStop(void)
{
	INTERLOCKED_(mutex) {
		if(!IsMainThread() && stopThread)
			throw BreakExc();
	}
}
#else
void Gdb_MI2::RaiseIfStop(void)
{
}
#endif


void Gdb_MI2::Stop()
{
	stopped = true;
#ifdef flagMT
	// stop debugger threads and wait for termination
	SetStopThread(true);
	while(IsThreadRunning())
		Sleep(20);
#endif
	if(dbg && dbg->IsRunning())
		dbg->Kill();
}

bool Gdb_MI2::IsFinished()
{
	return !dbg->IsRunning() && !IdeIsDebugLock();
}

bool Gdb_MI2::Tip(const String& exp, CodeEditor::MouseTip& mt)
{
	// quick exit
	if(exp.IsEmpty() || !dbg)
		return false;

	int iVal;
	String val;

	// first look into locals....
	iVal = localExpressions.Find(exp);
	if(iVal >= 0)
		val = localValues[iVal];
	// if not fount, look into 'this'
	else
	{
		iVal = thisShortExpressions.Find(exp);
		if(iVal >= 0)
			val = thisValues[iVal];
		else
			return false;
	}

	mt.display = &StdDisplay();
	mt.value = exp + "=" + val;
	mt.sz = mt.display->GetStdSize(String(mt.value) + "X");
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											CONSTRUCTOR / DESTRUCTOR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gdb_MI2::Periodic()
{
	if(TTYQuit())
		Stop();
}

void Gdb_MI2::Setup(ArrayCtrl& a, int x)
{
	a.NoHeader();
	a.AddColumn("", GetStdFontCy() * x * 3);
	a.AddColumn("", 10000);
	a.EvenRowColor();
	a.OddRowColor();
	a.HeaderObject().Absolute();
}

void Gdb_MI2::SyncWidth(ArrayCtrl& a)
{
	GuiLock __;
	int mx = a.GetSize().cx - a.HeaderObject().GetTabWidth(0);
	for(int i = 0; i < a.GetCount(); i++) {
		String h = a.GetConvertedColumn(i, 1);
		mx = max(mx, a.GetDisplay(i, 1).GetStdSize(a.GetConvertedColumn(i, 1)).cx + a.HeaderTab(0).GetMargin() * 2);
	}
	a.HeaderObject().SetTabWidth(1, mx);
}

Gdb_MI2::Gdb_MI2()
{
	CtrlLayout(regs);
	regs.Height(regs.GetLayoutSize().cy);
	AddReg(RPREFIX "ax", &regs.rax);
	AddReg(RPREFIX "bx", &regs.rbx);
	AddReg(RPREFIX "cx", &regs.rcx);
	AddReg(RPREFIX "dx", &regs.rdx);
	AddReg(RPREFIX "si", &regs.rsi);
	AddReg(RPREFIX "di", &regs.rdi);
	AddReg(RPREFIX "bp", &regs.rbp);
	AddReg(RPREFIX "sp", &regs.rsp);
#ifdef CPU_64
	AddReg("r8", &regs.r8);
	AddReg("r9", &regs.r9);
	AddReg("r10", &regs.r10);
	AddReg("r11", &regs.r11);
	AddReg("r12", &regs.r12);
	AddReg("r13", &regs.r13);
	AddReg("r14", &regs.r14);
	AddReg("r15", &regs.r15);
#endif
	regs.Color(SColorLtFace);
	regs.AddFrame(TopSeparatorFrame());
	regs.AddFrame(RightSeparatorFrame());

	Setup(autos);
	autos.WhenLeftDouble = THISBACK1(onExploreExpr, &autos);

	Setup(locals);
	locals.WhenLeftDouble = THISBACK1(onExploreExpr, &locals);

	Setup(members);
	members.WhenLeftDouble = THISBACK1(onExploreExpr, &members);

	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 10);
	watches.EvenRowColor();
	watches.OddRowColor();
	watches.HeaderObject().Absolute();
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
	
	Add(tab.SizePos());
	tab.Add(autos.SizePos(), "Autos");
	tab.Add(locals.SizePos(), t_("Locals"));
	tab.Add(members.SizePos(), t_("This"));
	tab.Add(watches.SizePos(), t_("Watches"));
	tab.Add(explorerPane.SizePos(), t_("Explorer"));
	
	Add(threadSelector.LeftPos(FindTabsRight() + 10, StdFont().GetWidth('X') * 10).TopPos(2, EditField::GetStdHeight()));
	Add(frame.HSizePos(threadSelector.GetRect().right + 10, 0).TopPos(2, EditField::GetStdHeight()));
	frame.Ctrl::Add(dlock.SizePos());
	dlock = "  Running..";
	dlock.SetFrame(BlackFrame());
	dlock.SetInk(Red);
	dlock.NoTransparent();
	dlock.Hide();

	CtrlLayout(quickwatch, "Quick watch");
	quickwatch.close.Cancel() << quickwatch.Breaker(IDCANCEL);
	quickwatch.evaluate.Ok() << quickwatch.Acceptor(IDOK);
	quickwatch.WhenClose = quickwatch.Breaker(IDCANCEL);
	quickwatch.value.SetReadOnly();
	quickwatch.value.SetFont(CourierZ(12));
	quickwatch.value.SetColor(TextCtrl::PAPER_READONLY, White);
	quickwatch.Sizeable().Zoomable();
	quickwatch.NoCenter();
	quickwatch.SetRect(0, 150, 300, 400);
	quickwatch.Icon(DbgImg::QuickWatch());

	Transparent();

	started = false;
	stopped = false;
	
	periodic.Set(-50, THISBACK(Periodic));
	
	localSynced = false;
	thisSynced = false;
	watchesSynced = false;
	explorerSynced = false;
	
#ifdef flagMT
	// no running debug thread
	threadRunning = false;
	stopThread = false;
#endif

}

Gdb_MI2::~Gdb_MI2()
{
	IdeRemoveBottom(*this);
	IdeRemoveRight(disas);
	KillDebugTTY();
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
	int i = 0;
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
	threadSelector.Disable();
	dlock.Show();
}

void Gdb_MI2::Unlock()
{
	if(IdeDebugUnLock())
	{
		watches.Enable();
		locals.Enable();
		frame.Enable();
		threadSelector.Enable();
		dlock.Hide();
	}
}

// format breakpoint line from ide file and line
String Gdb_MI2::BreakPos(String const &file, int line)
{
	return String().Cat() << Filter(host->GetHostPath(NormalizePath(file)), CharFilterReSlash2) << ":" << line;
}

// set breakpoint
MIValue Gdb_MI2::InsertBreakpoint(const char *file, int line)
{
	return MICmd("break-insert " + BreakPos(file, line));
}

// get breakpoints info
MIValue Gdb_MI2::GetBreakpoints(void)
{
	// issue a break-list command
	return pick(MICmd("break-list")["BreakpointTable"]);
}

MIValue Gdb_MI2::GetBreakpoint(int id)
{
	return MIValue();
}

MIValue Gdb_MI2::GetBreakPoint(const char *file, int line)
{
	return MIValue();
}

bool Gdb_MI2::TryBreak(adr_t addr, bool temp)
{
	String bp;
	if(temp)
		bp = "-t ";
	bp += Format("*0x%X", (int64)addr);

	MIValue res = MICmd(String("break-insert ") + bp);
	return !res.IsError();
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
			code = pick(MICmd(Format("data-disassemble -f %s -l %s -n -1 -- 0", file, line))["asm_insns"]);
		}
		else
			// otherwise disassemble some -100 ... +100 bytes around address
			code = pick(MICmd(Format("data-disassemble -s %x -e %x -- 0", (void *)(adr - 100), (void *)(adr + 100)))["asm_insns"]);
			
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
	
	// setup disassembler cursor
	disas.SetCursor(adr);
	disas.SetIp(adr, fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr());

	// update registers
	MIValue rNames = pick(MICmd("data-list-register-names")["register-names"]);
	MIValue rValues = pick(MICmd("data-list-register-values x")["register-values"]);
	Index<String> iNames;
	for(int i = 0; i < rNames.GetCount(); i++)
		iNames.Add(rNames[i]);
	for(int iReg = 0; iReg < regname.GetCount(); iReg++)
	{
		int i = iNames.Find(regname[iReg]);
		String rValue = "****************";
		if(i >= 0)
			rValue = "0000000000000000" + rValues[i]["value"].Get().Mid(2);
#ifdef CPU_64
		rValue = rValue.Right(16);
#else
		rValue = rValue.Right(8);
#endif
		if(reglbl[iReg]->GetText() != rValue)
		{
			reglbl[iReg]->SetLabel(rValue);
			reglbl[iReg]->SetInk(LtRed);
		}
		else
			reglbl[iReg]->SetInk(Black);
	}
			
}

// sync ide display with breakpoint position
void Gdb_MI2::SyncIde(bool fr)
{
	// kill pending update callbacks
#ifndef flagMT
	timeEvent<> .Kill();
	exploreEvent<> .Kill();
#endif

	// get current frame info and level
	MIValue fInfo = pick(MICmd("stack-info-frame")["frame"]);
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

	// setup threads droplist
	{
		threadSelector.Clear();
		MIValue tInfo = MICmd("thread-info");
		if(!tInfo.IsError() && !tInfo.IsEmpty())
		{
			int curThread = atoi(tInfo["current-thread-id"].Get());
			MIValue &threads = tInfo["threads"];
			for(int iThread = 0; iThread < threads.GetCount(); iThread++)
			{
				int id = atoi(threads[iThread]["id"].Get());
				if(id == curThread)
				{
					threadSelector.Add(id, Format("#%03x(*)", id));
					break;
				}
			}
			threadSelector <<= curThread;
		}
	}


	// get the arguments for current frame
	MIValue fArgs = pick(MICmd(Format("stack-list-arguments 1 %d %d", level, level))["stack-args"][0]["args"]);
	if(!fArgs.IsError() && !fArgs.IsEmpty())
	{
		// setup frame droplist
		frame.Clear();
		frame.Add(0, FormatFrame(fInfo, fArgs));
		frame <<= 0;
	}

	// sync disassembly
	SyncDisas(fInfo, fr);
	

	// update vars only on idle times
#ifdef flagMT
	SyncExplorer();
	SyncData();
	CleanupVariables();
#else
	exploreEvent<> .Set(480, THISBACK1(SyncExplorer, DeepClone(Vector<VarItem>())));
	timeEvent<> .Set(500, THISBACK(SyncData));
	timeEvent<> .Set(550, THISBACK(CleanupVariables));
#endif
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
	// data must be synced on stop...
	localSynced = false;
	thisSynced = false;
	watchesSynced = false;
	explorerSynced = false;

	// we need to store stop reason BEFORE interrupting all other
	// threads, otherwise it'll be lost
	MIValue stReason = pick(stopReason);
	
#ifdef PLATFORM_X11
	// try to ungrab X11 on stop.... otherwise mouse clicks are lost
	MICmd("data-evaluate-expression _DBG_Ungrab()");
#endif
	
	// get the reason string
	String reason;
	if(stReason.IsEmpty())
		reason = "unknown reason";
	else
		reason = stReason["reason"];

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
		LogFrame("Hit breakpoint", stReason["frame"]);
		SyncIde();
	}
	else if(reason == "end-stepping-range")
	{
		LogFrame("End stepping range", stReason["frame"]);
		SyncIde();
	}
	else if(reason == "unknown reason")
	{
		PutConsole("Stopped by unknown reason");
		SyncIde();
	}
	else
	{
		// weird stop reasons (i.e., signals, segfaults... may not have a frame
		// data inside
		if(stReason.Find("frame") < 0)
			PutConsole(Format("Stopped, reason '%s'", reason));
		else
			LogFrame(Format("Stopped, reason '%s'", reason), stReason["frame"]);
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
		Stop();
		Exclamation(t_("Step failed - terminating debugger"));
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

	firstRun = false;
	if(stopped)
		CheckStopReason();
	started = stopped = false;
	IdeActivateBottom();

	// reset focus to disassembly pane if was there before
	if(b)
		disas.SetFocus();
}

// setup ide cursor based on disassembler one
void Gdb_MI2::DisasCursor()
{
	if(!disas.HasFocus())
		return;

	// temporary disable disas lost-focus handler
	// otherwise cursor will not be correctly set by following code
	disas.WhenFocus.Clear();

	// to get info of corresponding file/line of an address, the only way
	// we found is to insert a breakpoint, note the position and remove it
	MIValue b = pick(MICmd(Format("break-insert *0x%X", (int64)disas.GetCursor()))["bkpt"]);
	if(b.Find("file") >= 0 && b.Find("line") >= 0)
		IdeSetDebugPos(b["file"], atoi(b["line"].Get()) - 1, DbgImg::DisasPtr(), 1);
	if(b.Find("number") >= 0)
		MICmd(Format("break-delete %s", b["number"].Get()));
	disas.SetFocus();

	// re-enable disas lost focus handler
	disas.WhenFocus = THISBACK(DisasFocus);
}

// reset ide default cursor image when disassembler loose focus
void Gdb_MI2::DisasFocus()
{
	if(!disas.HasFocus())
		IdeSetDebugPos(IdeGetFileName(), IdeGetFileLine(), Null, 1);
}

// create a string representation of frame given its info and args
String Gdb_MI2::FormatFrame(MIValue &fInfo, MIValue &fArgs)
{
	int idx = atoi(fInfo("level", "-1"));
	if(idx < 0)
		return t_("invalid frame info");
	if(!fArgs.IsArray())
		return t_("invalid frame args");
	String func = fInfo("func", "<unknown>");
	String file = fInfo("file", "<unknown>");
	String line = fInfo("line", "<unknown>");
	int nArgs = fArgs.GetCount();
	String argLine;
	for(int iArg = 0; iArg < nArgs; iArg++)
	{
		MIValue &arg = fArgs[iArg];
		if(arg.IsTuple())
		{
			argLine += arg["name"].Get();
			if(arg.Find("value") >= 0)
				argLine << "=" << arg["value"];
		}
		else if(arg.IsString())
			// quick fix....
			argLine << arg.ToString();
		argLine << ',';
	}
	if(!argLine.IsEmpty())
		argLine = "(" + argLine.Left(argLine.GetCount() - 1) + ")";
	return Format("%02d-%s%s at %s:%s", idx, func, argLine, file, line);
}

bool Gdb_MI2::FillDropFrames(int min, int max, bool val)
{
	if(min > max)
		return true;
	
	// get a list of frames
	MIValue frameList = pick(MICmd(Format("stack-list-frames %d %d", min, max))["stack"]);
	if(frameList.IsError() || !frameList.IsArray())
	{
		Exclamation("Couldn't get stack frame list");
		return false;
	}
	
	// get the arguments for all frames, values just for simple types
	MIValue frameArgs = pick(MICmd(Format("stack-list-arguments %d %d %d", val ? 2 : 0, min, max))["stack-args"]);
	if(frameArgs.IsError() || !frameArgs.IsArray())
	{
		Exclamation("Couldn't get stack arguments list");
		return false;
	}
	
	// fill the droplist
	for(int iFrame = 0; iFrame < frameArgs.GetCount(); iFrame++)
	{
		MIValue &fInfo = frameList[iFrame];
		MIValue &fArgs = frameArgs[iFrame]["args"];
		frame.Add(iFrame + min, FormatFrame(fInfo, fArgs));
	}
	return true;
}

// re-fills frame's droplist when dropping it
void Gdb_MI2::DropFrames()
{
	int q = ~frame;
	frame.Clear();

	// getting frame args values is quite slow in gdb
	// so we get them just for 3+3 frames around current
	// frame position; for the rest we just get names
	
	// get stack depth
	int depth = atoi(MICmd("stack-info-depth")["depth"].ToString());
	
	// adjust frame pointer, just in case current depth is less than former
	if(q >= depth)
		q = depth - 1;

	int minVal = max(0, q - 3);
	int maxVal = min(depth - 1, q + 3);

	// first window without values
	if(!FillDropFrames(0, minVal - 1, false))
		return;
	
	// window with values
	if(!FillDropFrames(minVal, maxVal, true))
		return;
	
	// last window without values
	if(!FillDropFrames(maxVal + 1, depth - 1, false))
		return;

	frame <<= q;
}

// shows selected stack frame in editor
void Gdb_MI2::ShowFrame()
{
	int i = (int)~frame;
	if(!MICmd(Format("stack-select-frame %d", i)))
	{
		Exclamation(Format(t_("Couldn't select frame #%d"), i));
		return;
	}

	// data must be synced on frame change
	localSynced = false;
	thisSynced = false;
	watchesSynced = false;
	explorerSynced = false;

	SyncIde(i);
}

// re-fills thread selector droplist on drop
void Gdb_MI2::dropThreads()
{
	int q = ~threadSelector;
	threadSelector.Clear();

	// get a list of all available threads
	MIValue tInfo = MICmd("thread-info");
	MIValue &threads = tInfo["threads"];
	if(!tInfo.IsTuple() || !threads.IsArray())
	{
		Exclamation(t_("couldn't get thread info"));
		return;
	}
	int currentId = atoi(tInfo["current-thread-id"].Get());
	for(int iThread = 0; iThread < threads.GetCount(); iThread++)
	{
		MIValue &t = threads[iThread];
		int id = atoi(t["id"].Get());
		String threadStr = Format("#%03x%s", id, (id == currentId ? "(*)" : ""));
		threadSelector.Add(id, threadStr);
	}

	// data must be resynced on threads drop
	localSynced = false;
	thisSynced = false;
	watchesSynced = false;
	explorerSynced = false;

	threadSelector <<= q;
}

// selects current thread
void Gdb_MI2::showThread(void)
{
	int i = (int)~threadSelector;
	MICmd(Format("thread-select %d", i));

	SyncIde();	
}

struct CapitalLess
{
	bool operator()(String const &a, String const &b) const { return ToUpper(a) < ToUpper(b); }
};

// update local variables on demand
#ifdef flagMT
void Gdb_MI2::SyncLocals()
{
	IncThreadRunning();
	
	try
	{
		VectorMap<String, String> prev = DataMap(locals);
		
		RaiseIfStop();

		// get local vars names
		MIValue locs = MICmd("stack-list-variables 0");
		if(!locs.IsTuple() || locs.Find("variables") < 0)
			throw BreakExc();
	
		// variables are returned as a tuple, named "variables"
		// containing a array of variables
		MIValue lLocs = pick(locs["variables"]);
		if(!lLocs.IsArray())
			throw BreakExc();
	
		Vector<VarItem> localVars;
		for(int iLoc = 0; iLoc < lLocs.GetCount(); iLoc++)
		{
			MIValue &lLoc = lLocs[iLoc];
			String name = lLoc["name"];
	
			// skip 'this', we've a page for it
			if(name == "this")
				continue;
	
			localVars.Add(VarItem(this, name));
		}

		// create a VarItem for each variable and put evaluation results
		// inside local arrays
		localExpressions.Clear();
		localValues.Clear();
	
		for(int iLoc = 0; iLoc < localVars.GetCount(); iLoc++)
		{
			VarItem &vItem = localVars[iLoc];
			localExpressions << vItem.evaluableExpression;
			localValues << vItem.value;
		}
	
		// update locals pane
		FillPane(locals, localExpressions, localValues);
	
		// autos variables can come from members or locals...
		SyncAutos();
		
		// simplify batch
		for(int iLoc = 0; iLoc < localVars.GetCount(); iLoc++)
		{
			VarItem &v = localVars[iLoc];
			while(v.Simplify())
				RaiseIfStop();
			
			localExpressions.Set(iLoc, v.evaluableExpression);
			localValues[iLoc] = v.value;
			{
				GuiLock __;
				locals.Set(iLoc, 1, v.value);
			}
		}
	
		localSynced = true;
	}
	catch(...)
	{
		localSynced = false;
	}
	
	DecThreadRunning();
}
#else
void Gdb_MI2::SyncLocals(const Vector<VarItem>& localVars_)
{
	static VectorMap<String, String> prev;
	Vector<VarItem> localVars = clone(localVars_);
	if(localVars.IsEmpty())
	{
		prev = DataMap(members);;

		// get local vars names
		MIValue locs = MICmd("stack-list-variables 0");
		if(!locs.IsTuple() || locs.Find("variables") < 0)
			return;
	
		// variables are returned as a tuple, named "variables"
		// containing a array of variables
		MIValue lLocs = pick(locs["variables"]);
		if(!lLocs.IsArray())
			return;
	
		for(int iLoc = 0; iLoc < lLocs.GetCount(); iLoc++)
		{
			MIValue &lLoc = lLocs[iLoc];
			String name = lLoc["name"];
	
			// skip 'this', we've a page for it
			if(name == "this")
				continue;
	
			localVars.Add(VarItem(this, name));
		}

		// create a VarItem for each variable and put evaluation results
		// inside local arrays
		localExpressions.Clear();
		localValues.Clear();
	
		for(int iLoc = 0; iLoc < localVars.GetCount(); iLoc++)
		{
			VarItem &vItem = localVars[iLoc];
			localExpressions << vItem.evaluableExpression;
			localValues << vItem.value;
		}
	
		// update locals pane
		FillPane(locals, localExpressions, localValues);
	
		// autos variables can come from members or locals...
		SyncAutos();
	
		timeEvent<> .Set(500, THISBACK1(SyncLocals, DeepClone(localVars)));
		return;
	}
	
	// simplify batch
	for(int iLoc = 0; iLoc < localVars.GetCount(); iLoc++)
	{
		if(localVars[iLoc].Simplify())
		{
			VarItem &v = localVars[iLoc];
			localExpressions.Set(iLoc, v.evaluableExpression);
			localValues[iLoc] = v.value;
			locals.Set(iLoc, 1, v.value);
			SyncAutos();
			timeEvent<> .Set(100, THISBACK1(SyncLocals, DeepClone(localVars)));
			return;
		}
	}
	
	for(int iLoc = 0; iLoc < localVars.GetCount(); iLoc++)
	{
		VarItem &v = localVars[iLoc];
		localExpressions.Set(iLoc, v.evaluableExpression);
		localValues[iLoc] = v.value;
		locals.Set(iLoc, 1, v.value);
	}

	// when finished, mark changed values
	MarkChanged(prev, locals);
	localSynced = true;
}
#endif

// update 'this' inspector data
#ifdef flagMT
void Gdb_MI2::SyncThis()
{
	IncThreadRunning();
	
	try
	{
		VectorMap<String, String> prev = DataMap(members);
		
		RaiseIfStop();

		// create a vari object and evaluate '*this' expression
		VarItem vItem(this);
		vItem.Evaluate("*this");

		RaiseIfStop();

		// get children
		Vector<VarItem> children = vItem.GetChildren();

		RaiseIfStop();

		thisExpressions.Clear();
		thisShortExpressions.Clear();
		thisValues.Clear();
		for(int iVar = 0; iVar < children.GetCount(); iVar++)
		{
			VarItem &v = children[iVar];
			thisExpressions << v.evaluableExpression;
			String exp = v.shortExpression;
			int dot = exp.Find('.');
			if(dot >= 0)
				exp = exp.Mid(dot + 1);
			thisShortExpressions << exp;
			thisValues << v.value;
		}
	
		RaiseIfStop();

		// update 'this' pane
		FillPane(members, thisShortExpressions, thisValues);

		// autos variables can come from members or locals...
		SyncAutos();
	
		// simplify batch
		for(int iVar = 0; iVar < children.GetCount(); iVar++)
		{
			RaiseIfStop();

			VarItem &v = children[iVar];
			while(v.Simplify())
				RaiseIfStop();

			thisExpressions.Set(iVar, v.evaluableExpression);
			thisValues[iVar] = v.value;
			{
				GuiLock __;
				members.Set(iVar, 1, v.value);
			}
			SyncAutos();
		}

		// when finished, mark changed values
		MarkChanged(prev, members);
		thisSynced = true;
	}
	catch(...)
	{
		thisSynced = false;
	}
	
	DecThreadRunning();
}
#else
void Gdb_MI2::SyncThis(const Vector<VarItem>& children_)
{
	static VectorMap<String, String> prev;
	Vector<VarItem> children = clone(children_);
	if(children.IsEmpty())
	{
		prev = DataMap(members);;

		// create a vari object and evaluate '*this' expression
		VarItem vItem(this, "*this");

		// get children
		children = vItem.GetChildren();

		// fill 'this' memners expressions, short expressions and values
		thisExpressions.Clear();
		thisShortExpressions.Clear();
		thisValues.Clear();
		for(int iVar = 0; iVar < children.GetCount(); iVar++)
		{
			VarItem &v = children[iVar];
			thisExpressions << v.evaluableExpression;
			String exp = v.shortExpression;
			int dot = exp.Find('.');
			if(dot >= 0)
				exp = exp.Mid(dot + 1);
			thisShortExpressions << exp;
			thisValues << v.value;
		}
	
		// update 'this' pane
		FillPane(members, thisShortExpressions, thisValues);
	
		// autos variables can come from members or locals...
		SyncAutos();
	
		timeEvent<> .Set(500, THISBACK1(SyncThis, DeepClone(children)));
		return;
	}
	
	// simplify batch
	for(int iVar = 0; iVar < children.GetCount(); iVar++)
	{
		VarItem &v = children[iVar];
		if(v.Simplify())
		{
			thisExpressions.Set(iVar, v.evaluableExpression);
			thisValues[iVar] = v.value;
			members.Set(iVar, 1, v.value);
			SyncAutos();
			timeEvent<> .Set(100, THISBACK1(SyncThis, DeepClone(children)));
			return;
		}
	}
	
	for(int iVar = 0; iVar < children.GetCount(); iVar++)
		members.Set(iVar, 1, children[iVar].value);

	// when finished, mark changed values
	MarkChanged(prev, members);
	thisSynced = true;
}
#endif
		
// sync auto vars treectrl
void Gdb_MI2::SyncAutos()
{
	INTERLOCKED {
		GuiLock __;
	
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
				if(autos.Find(exp) >= 0)
					continue;
				int idx = localExpressions.Find(exp);
				if(idx < 0)
				{
					idx = thisShortExpressions.Find(exp);
					if(idx >= 0)
						autos.Add(exp, thisValues[idx]);
				}
				else
					autos.Add(exp, localValues[idx]);
			}
			p.SkipTerm();
		}
		autos.Sort();
		SyncWidth(autos);
	}
}

// sync watches treectrl
#ifdef flagMT
void Gdb_MI2::SyncWatches()
{
	// re-enter if called from main thread
	if(IsMainThread())
	{
		debugThread.Start(THISBACK(SyncWatches));
		return;
	}
	
	INTERLOCKED {
		IncThreadRunning();
		try
		{
			VectorMap<String, String> prev = DataMap(watches);
	
			// get watches expressions and create VarItems for them
			// and put results inside watches control
			Vector<VarItem> watchesVars;
			watchesExpressions.Clear();
			watchesValues.Clear();
			for(int iWatch = 0; iWatch < watches.GetCount(); iWatch++)
			{
				String exp, val;
				{
					GuiLock __;
					exp = watches.Get(iWatch, 0);
				}
				watchesExpressions << exp;
				watchesVars.Add(VarItem(this, exp));
				val = watchesVars.Top().value;
				watchesValues.Add(val);
				{
					GuiLock __;
					watches.Set(iWatch, 1, val);
				}
			}
				
			// simplify batch
			for(int iWatch = 0; iWatch < watchesVars.GetCount(); iWatch++)
			{
				RaiseIfStop();

				VarItem &v = watchesVars[iWatch];
				while(v.Simplify())
					RaiseIfStop();
	
				watchesExpressions.Set(iWatch, v.evaluableExpression);
				watchesValues[iWatch] = v.value;
				{
					GuiLock __;
					watches.Set(iWatch, 1, v.value);
				}
			}
			
			SyncWidth(watches);
	
			// when finished, mark changed values
			MarkChanged(prev, watches);
			watchesSynced = true;
		}
		catch(...)
		{
			watchesSynced = false;
		}
		DecThreadRunning();
	}
}
#else
void Gdb_MI2::SyncWatches(const Vector<VarItem>& watchesVars_)
{
	static VectorMap<String, String> prev;
	Vector<VarItem> watchesVars = clone(watchesVars_);
	if(watchesVars.IsEmpty())
	{
		prev = DataMap(watches);;

		// get watches expressions and create VarItems for them
		// and put results inside watches control
		watchesExpressions.Clear();
		watchesValues.Clear();
		for(int iWatch = 0; iWatch < watches.GetCount(); iWatch++)
		{
			String exp = watches.Get(iWatch, 0);
			watchesExpressions << exp;
			watchesVars.Add(VarItem(this, exp));
			String val = watchesVars.Top().value;
			watchesValues.Add(val);
			watches.Set(iWatch, 1, val);
		}
		
		timeEvent<> .Set(500, THISBACK1(SyncWatches, DeepClone(watchesVars_)));
		return;
	}
	
	// simplify batch
	for(int iWatch = 0; iWatch < watchesVars.GetCount(); iWatch++)
	{
		if(watchesVars[iWatch].Simplify())
		{
			VarItem &v = watchesVars[iWatch];
			watchesExpressions.Set(iWatch, v.evaluableExpression);
			watchesValues[iWatch] = v.value;
			watches.Set(iWatch, 1, v.value);
			timeEvent<> .Set(100, THISBACK1(SyncWatches, DeepClone(watchesVars)));
			return;
		}
	}
	
	for(int iWatch = 0; iWatch < watchesVars.GetCount(); iWatch++)
	{
		VarItem &v = watchesVars[iWatch];
		watchesExpressions.Set(iWatch, v.evaluableExpression);
		watchesValues[iWatch] = v.value;
		watches.Set(iWatch, 1, v.value);
	}

	// when finished, mark changed values
	MarkChanged(prev, watches);
	watchesSynced = true;
}
#endif

// sync data tabs, depending on which tab is shown
void Gdb_MI2::SyncData()
{
#ifdef flagMT
	
	// update stored 'this' member data
	// also used for tooltips and 'this' pane page
	if(IsStopThread())
		return;
	INTERLOCKED_(mutex) {
		if(!thisSynced)
			debugThread.Start(THISBACK(SyncThis));
	}
	
	// updated locals variables
	if(IsStopThread())
		return;
	INTERLOCKED_(mutex) {
		if(!localSynced)
			debugThread.Start(THISBACK(SyncLocals));
	}

	//update watches
	if(IsStopThread())
		return;
	INTERLOCKED_(mutex) {
		if(!watchesSynced)
			debugThread.Start(THISBACK(SyncWatches));
	}	

#else

	// update stored 'this' member data
	// also used for tooltips and 'this' pane page
	if(!thisSynced)
		SyncThis();
	
	// updated locals variables
	if(!localSynced)
		SyncLocals();

	// update watches
	if(!watchesSynced)
		SyncWatches();
	
#endif
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
	if(key == K_ENTER && explorerExprEdit.HasFocus())
	{
		onExploreExpr();
		return true;
	}
	return Ctrl::Key(key, count);
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
void Gdb_MI2::WatchDeep0(String parentExp, String const &var, int level, int &maxRemaining)
{
	// avoid endless recursion for circularly linked vars
	if(--maxRemaining <= 0)
		return;
	
	MIValue childInfo = MICmd("var-list-children 1 \"" + var + "\" 0 100");
	if(!childInfo || !childInfo.IsTuple())
		return;
	int nChilds = min(atoi(childInfo("numchild", "-1")), 100);
	if(nChilds <= 0)
		return;

	MIValue &childs = childInfo["children"];
	for(int i = 0; i < childs.GetCount() && maxRemaining > 0; i++)
	{
		MIValue child = pick(childs[i]);
		String exp = child["exp"];
		
		// handle pseudo children...
/*
		while(exp == "public" || exp == "private" || exp == "protected")
		{
			child = MICmd(String("var-list-children 1 \"") + child["name"] + "\"")["children"][0];
			exp = child["exp"];
		}
*/
		if(isdigit(exp[0]))
		{
			exp = '[' + exp + ']';
			if(parentExp.Mid(parentExp.GetCount() - 1, 1) == ".")
				parentExp = parentExp.Left(parentExp.GetCount() - 1);
		}
		if(exp[0] != '.' && exp[0] != '[')
			exp = '.' + exp;

		String type = child("type", "");
		if(!type.IsEmpty())
			type = "(" + type + ")";
		String value = child["value"];
		
		// try to format nicely results...
		quickwatch.value <<= (String)~quickwatch.value + "\n" + FormatWatchLine(parentExp + exp, type + value, level);
		
		// recursive deep watch
		WatchDeep0(exp, child["name"], level + 1, maxRemaining);
	}
}

void Gdb_MI2::WatchDeep(String parentExp, String const &name)
{
	// this is to avoid circular endless recursion
	// we limit the total watched (sub)variables to this count
	int maxRemaining = 300;
	
	WatchDeep0(parentExp, name, 1, maxRemaining);
}

// opens quick watch dialog
void Gdb_MI2::QuickWatch()
{
	// try to figure out if we've got the cursor in some interesting
	// place... if it is, grab the expression from there
	// otherwise let it unchanged
	Ctrl *c = GetFocusCtrl();
	if(typeid(*c) == typeid(AssistEditor))
	{
		AssistEditor *a = dynamic_cast<AssistEditor *>(c);
		String s = a->ReadIdBack(a->GetCursor());
		quickwatch.expression <<= s;
	}
	else if(c == &autos)
	{
		int i = autos.GetCursor();
		if(i >= 0)
			quickwatch.expression <<= autos.Get(i, 0);
	}
	else if(c == &locals)
	{
		int i = locals.GetCursor();
		if(i >= 0)
			quickwatch.expression <<= locals.Get(i, 0);
	}
	else if(c == &watches)
	{
		int i = watches.GetCursor();
		if(i >= 0)
			quickwatch.expression <<= watches.Get(i, 0);
	}
	else if(c == &explorer || c == &explorerExprEdit)
	{
		quickwatch.expression <<= ~explorerExprEdit;
	}
	
	for(;;)
	{
		String exp = ~quickwatch.expression;
		if(!exp.IsEmpty())
		{
			MIValue v = MICmd("var-create - @ " + exp);
			if(!v.IsError())
			{
				String type = v("type", "");
				if(!type.IsEmpty())
					type = "(" + type + ")";
				String value = v["value"];
				quickwatch.value <<= FormatWatchLine(exp, type + value, 0);
				quickwatch.expression.AddHistory();
				String name = v["name"];
				WatchDeep(exp, name);
				MICmd("var-delete " + name);
			}
			else
				quickwatch.value <<= t_("<can't evaluate expression>");
		}
		else
			quickwatch.value.Clear();
		int q = quickwatch.Run();
		if(q == IDCANCEL)
			break;
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

//////////////////////////////////////////////////////////////////////////////////////////////////////

// create GDB process and initializes it
bool Gdb_MI2::Create(One<Host>&& _host, const String& exefile, const String& cmdline, bool console)
{
	host = pick(_host);
	dbg = host->StartProcess(GdbCommand(console) + GetHostPath(exefile) + " --interpreter=mi2" /*" -q"*/);
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

	threadSelector.WhenDrop = THISBACK(dropThreads);
	threadSelector <<= THISBACK(showThread);

#ifdef flagMT
	watches.WhenAcceptEdit = THISBACK(SyncWatches);
#else
	watches.WhenAcceptEdit = THISBACK1(SyncWatches, DeepClone(Vector<VarItem>()));
#endif
	// this one will allow asynchronous break of running app
// 2012-07-08 -- DISABLED because of GDB bugs...
//	MICmd("gdb-set target-async 1");
	MICmd("gdb-set pagination off");

// Don't enable this one -- brings every sort of bugs with
// It was useful to issue Asynchronous break, but too many bugs
// to be useable
//	MICmd("gdb-set non-stop on");

//	MICmd("gdb-set interactive-mode off");

	MICmd("gdb-set disassembly-flavor intel");
	MICmd("gdb-set exec-done-display off");
	MICmd("gdb-set annotate 1");
	MICmd("gdb-set height 0");
	MICmd("gdb-set width 0");
	MICmd("gdb-set confirm off");
	MICmd("gdb-set print asm-demangle");
	
	// limit array printing to 200 elements
	MICmd("gdb-set print elements 200");
	
	// do NOT print repeated counts -- they confuse the parser
	MICmd("gdb-set print repeat 0");

	// we wanna addresses, we skip them in parser if useless
//	MICmd("gdb-set print address off");

	// print true objects having their pointers (if have vtbls)
	MICmd("gdb-set print object on");
	
	// print symbolic addresses, if found
	MICmd("gdb-set print symbol on");
	
	// do not search of symbolic address near current one
	MICmd("gdb-set print max-symbolic-offset 1");
	
	// avoids debugger crash if caught inside ungrabbing function
	// (don't solves all cases, but helps...)
	MICmd("gdb-set unwindonsignal on");

//	MICmd("gdb-set interactive on");

	if(!IsNull(cmdline))
		MICmd("gdb-set args " + cmdline);
	
	firstRun = true;

	return true;
}

One<Debugger> Gdb_MI2Create(One<Host>&& host, const String& exefile, const String& cmdline, bool console)
{
	Gdb_MI2 *dbg = new Gdb_MI2;
	if(!dbg->Create(pick(host), exefile, cmdline, console)) {
		delete dbg;
		return NULL;
	}
	return dbg;
}
