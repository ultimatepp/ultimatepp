#include "Debuggers.h"

void Gdb::DebugBar(Bar& bar)
{
	using namespace PdbKeys;

	bar.Add("Stop debugging", DbgImg::StopDebug(), THISBACK(Stop))
	   .Key(K_SHIFT_F5);
	bar.Separator();
	bool b = !IdeIsDebugLock();
	bar.Add(b, AK_STEPINTO, DbgImg::StepInto(), THISBACK1(Step, disas.HasFocus() ? "stepi"
	                                                                             : "step"));
	bar.Add(b, AK_STEPOVER, DbgImg::StepOver(), THISBACK1(Step, disas.HasFocus() ? "nexti"
	                                                                             : "next"));
	bar.Add(b, AK_STEPOUT, DbgImg::StepOut(), THISBACK1(Step, "finish"));
	bar.Add(b, AK_RUNTO, DbgImg::RunTo(), THISBACK(DoRunTo));
	bar.Add(b, AK_RUN, DbgImg::Run(), THISBACK(Run));
//	bar.Add(b, AK_SETIP, DbgImg::SetIp(), THISBACK(SetIp));
//	bar.Add(!b, AK_STOP, DbgImg::Stop(), THISBACK(BreakRunning));
	bar.MenuSeparator();
	bar.Add(b, AK_AUTOS, THISBACK1(SetTab, 0));
	bar.Add(b, AK_LOCALS, THISBACK1(SetTab, 1));
	bar.Add(b, AK_THISS, THISBACK1(SetTab, 2));
	bar.Add(b, AK_WATCHES, THISBACK1(SetTab, 3));
	bar.Add(b, AK_CLEARWATCHES, THISBACK(ClearWatches));
	bar.Add(b, AK_ADDWATCH, THISBACK(QuickWatch));
	bar.Add(b, AK_CPU, THISBACK1(SetTab, 4));
	bar.MenuSeparator();
	bar.Add(b, "Copy backtrace", THISBACK(CopyStack));
	bar.Add(b, "Copy dissassembly", THISBACK(CopyDisas));
}

void Gdb::CopyStack()
{
	DropFrames();
	String s;
	for(int i = 0; i < frame.GetCount(); i++)
		s << frame.GetValue(i) << "\n";
	WriteClipboardText(s);
}

void Gdb::CopyDisas()
{
	disas.WriteClipboard();
}

int CharFilterReSlash(int c)
{
	return c == '\\' ? '/' : c;
}

String Bpoint(Host& host, const String& file, int line)
{
	return String().Cat() << Filter(host.GetHostPath(NormalizePath(file)), CharFilterReSlash) << ":" << line + 1;
}

bool Gdb::TryBreak(const char *text)
{
	return FindTag(FastCmd(text), "Breakpoint");
}

bool Gdb::SetBreakpoint(const String& filename, int line, const String& bp)
{
	String bi = Bpoint(*host, filename, line);
	if(bp.IsEmpty())
		FastCmd("clear " + bi);
	else if(bp[0]==0xe)
		FastCmd("b " + bi);
	else
		FastCmd("b " + bi + " if " + bp);
	return true;
}

void Gdb::SetDisas(const String& text)
{
	disas.Clear();
	StringStream ss(text);
	while(!ss.IsEof()) {
		String ln = ss.GetLine();
		const char *s = ln;
		while(*s && !IsDigit(*s))
			s++;
		adr_t adr = 0;
		String code, args;
		if(s[0] == '0' && ToLower(s[1]) == 'x')
			adr = (adr_t)ScanInt64(s + 2, NULL, 16);
		int q = ln.Find(">:");
		if(q >= 0) {
			s = ~ln + q + 2;
			while(IsSpace(*s))
				s++;
			while(*s && !IsSpace(*s))
				code.Cat(*s++);
			while(IsSpace(*s))
				s++;
			args = s;
			q = args.Find("0x");
			if(q >= 0)
				disas.AddT(ScanInt(~args + q + 2, NULL, 16));
			disas.Add(adr, code, args);
		}
	}
}

void Gdb::SyncDisas(bool fr)
{
	if(!disas.IsVisible())
		return;
	if(!disas.InRange(addr))
		SetDisas(FastCmd("disas"));
	disas.SetCursor(addr);
	disas.SetIp(addr, fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr());
}

String FormatFrame(const char *s)
{
	if(*s++ != '#')
		return Null;
	while(IsDigit(*s))
		s++;
	while(*s == ' ')
		s++;
	if(s[0] == '0' && ToUpper(s[1]) == 'X') {
		s += 2;
		while(IsXDigit(*s))
			s++;
		while(*s == ' ')
			s++;
		if(s[0] != 'i' && s[1] != 'n')
			return Null;
		s += 2;
		while(*s == ' ')
			s++;
	}
	if(!IsAlpha(*s))
		return Null;
	const char *w = strchr(s, '\r');
	if(w)
		return String(s, w);
	w = strchr(s, '\n');
	if(w)
		return String(s, w);
	return s;
}

bool ParsePos(const String& s, String& fn, int& line, adr_t & adr)
{
	const char *q = FindTag(s, "\x1a\x1a");
	if(!q) return false;
	q += 2;
	Vector<String> p = Split(q + 2, ':');
	p.SetCount(5);
	fn = String(q, q + 2) + p[0];
	line = atoi(p[1]);
	try {
		CParser pa(p[4]);
		pa.Char2('0', 'x');
		if(pa.IsNumber(16))
			adr = (adr_t)pa.ReadNumber64(16);
	}
	catch(CParser::Error) {}
	return true;
}

void Gdb::CheckEnd(const char *s)
{
	if(!dbg) {
		Stop();
		return;
	}
	if(FindTag(s, "Program exited normally.")) {
		Stop();
		return;
	}
	const char *q = FindTag(s, "Program exited with code ");
	if(q) {
		PutConsole(q);
		Stop();
		return;
	}
}

String Gdb::Cmdp(const char *cmdline, bool fr)
{
	expression_cache.Clear();
	String s = Cmd(cmdline);
	if(ParsePos(s, file, line, addr)) {
		IdeSetDebugPos(GetLocalPath(file), line - 1, fr ? DbgImg::FrameLinePtr()
		                                                : DbgImg::IpLinePtr(), 0);
		IdeSetDebugPos(GetLocalPath(file), line - 1,
		               disas.HasFocus() ? fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr()
		                                : Image(), 1);
		SyncDisas(fr);
		autoline.Clear();
		for(int i = -4; i <= 4; i++)
			autoline << ' ' << IdeGetLine(line + i);
	}
	else {
		file = Null;
		try {
			CParser pa(s);
			pa.Char2('0', 'x');
			if(pa.IsNumber(16))
				addr = (adr_t)pa.ReadNumber64(16);
		}
		catch(CParser::Error) {}
		SyncDisas(fr);
	}
	frame.Clear();
	frame.Add(0, FormatFrame(FastCmd("frame")));
	frame <<= 0;
	threads.Clear();
	s = FastCmd("info threads");
	StringStream ss(s);
	int active_thread = -1;
	while(!ss.IsEof()) {
		String s = ss.GetLine();
		CParser p(s);
		try {
			bool active = p.Char('*');
			if(p.IsNumber()) {
				int id = p.ReadInt();
				threads.Add(id, String().Cat() << "Thread " << id);
				if(active)
					active_thread = id;
			}
			threads.GoBegin();
		}
		catch(CParser::Error) {}
	}
	if(active_thread >= 0)
		threads <<= active_thread;
	if(threads.GetCount() == 0)
		Stop();
	Data();
	return s;
}

bool Gdb::RunTo()
{
	String bi;
	bool df = disas.HasFocus();
	if(df) {
		if(!disas.GetCursor())
			return false;
		bi = Sprintf("*0x%X", disas.GetCursor());
	}
	else
		bi = Bpoint(*host, IdeGetFileName(), IdeGetFileLine());
	if(!TryBreak("b " + bi)) {
		Exclamation("No code at chosen location !");
		return false;
	}
	String e = Cmdp(firstrun ? "run" : "continue");
	firstrun = false;
	FastCmd("clear " + bi);
	if(df)
		disas.SetFocus();
	CheckEnd(e);
	IdeActivateBottom();
	return true;
}

void Gdb::Run()
{
	CheckEnd(Cmdp(firstrun ? "run" : "continue"));
	firstrun = false;
	IdeActivateBottom();
}

void Gdb::Step(const char *cmd)
{
	bool b = disas.HasFocus();
	String s = Cmdp(cmd);
	if(b) disas.SetFocus();
	CheckEnd(s);
	IdeActivateBottom();
}

void Gdb::DisasCursor()
{
	if(!disas.HasFocus())
		return;
	int line;
	String file;
	adr_t addr;
	if(ParsePos(FastCmd(Sprintf("info line *0x%X", disas.GetCursor())), file, line, addr))
		IdeSetDebugPos(file, line - 1, DbgImg::DisasPtr(), 1);
	disas.SetFocus();
}

void Gdb::DisasFocus()
{
//	if(!disas.HasFocus())
//		IdeSetDebugPos(file, 0, Null, 1);
}

void Gdb::DropFrames()
{
	int i = 0;
	int q = ~frame;
	frame.Clear();
	for(;;) {
		String s = FormatFrame(FastCmd(Sprintf("frame %d", i)));
		if(IsNull(s)) break;
		frame.Add(i++, s);
	}
	frame <<= q;
}

void Gdb::SwitchFrame()
{
	int i = (int)~frame;
	Cmdp(Sprintf("frame %d", i), i);
}

void Gdb::SwitchThread()
{
	int i = (int)~threads;
	Cmdp(Sprintf("thread %d", i), i);
}

bool Gdb::Key(dword key, int count)
{
	if(key >= 32 && key < 65535 && tab.Get() == 2) {
		watches.DoInsertAfter();
		Ctrl* f = GetFocusCtrl();
		if(f && watches.HasChildDeep(f))
			f->Key(key, count);
		return true;
	}
	return Ctrl::Key(key, count);
}

bool Gdb::Create(One<Host>&& _host, const String& exefile, const String& cmdline, bool console)
{
	host = pick(_host);
	dbg = host->StartProcess(GdbCommand(console) + GetHostPath(exefile));
	if(!dbg) {
		Exclamation("Error invoking gdb !");
		return false;
	}
	IdeSetBottom(*this);
	IdeSetRight(disas);

	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);
	frame.WhenDrop = THISBACK(DropFrames);
	frame <<= THISBACK(SwitchFrame);
	
	threads <<= THISBACK(SwitchThread);

	watches.WhenAcceptEdit = THISBACK(Data);
	tab <<= THISBACK(Data);

	Cmd("set prompt " GDB_PROMPT);
	Cmd("set disassembly-flavor intel");
	Cmd("set exec-done-display off");
	Cmd("set annotate 1");
	Cmd("set height 0");
	Cmd("set width 0");
	Cmd("set confirm off");
	Cmd("set print asm-demangle");
	Cmd("set print static-members off");
	Cmd("set print vtbl off");
	Cmd("set print repeat 0");
	Cmd("set print null-stop");

	if(!IsNull(cmdline))
		Cmd("set args " + cmdline);


	firstrun = true;

	return true;
}

Gdb::~Gdb()
{
	IdeRemoveBottom(*this);
	IdeRemoveRight(disas);
	KillDebugTTY();
}

void Gdb::Periodic()
{
	if(TTYQuit())
		Stop();
}

Gdb::Gdb()
{
	locals.NoHeader();
	locals.AddColumn("", 1);
	locals.AddColumn("", 6);
	locals.EvenRowColor();
	locals.WhenSel = THISBACK1(SetTree, &locals);
	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6);
	watches.Inserting().Removing();
	watches.EvenRowColor();
	watches.WhenSel = THISBACK1(SetTree, &watches);
	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6);
	autos.EvenRowColor();
	autos.WhenSel = THISBACK1(SetTree, &autos);
	self.NoHeader();
	self.AddColumn("", 1);
	self.AddColumn("", 6);
	self.EvenRowColor();
	self.WhenSel = THISBACK1(SetTree, &self);
	cpu.Columns(3);
	cpu.ItemHeight(Courier(Ctrl::HorzLayoutZoom(12)).GetCy());

	pane.Add(tab.SizePos());
	tab.Add(autos.SizePos(), "Autos");
	tab.Add(locals.SizePos(), "Locals");
	tab.Add(watches.SizePos(), "Watches");
	tab.Add(self.SizePos(), "this");
	tab.Add(cpu.SizePos(), "CPU");
	pane.Add(threads.LeftPosZ(300, 100).TopPos(2));
	pane.Add(frame.HSizePosZ(404, 0).TopPos(2));
	split.Horz(pane, tree.SizePos());
	split.SetPos(8000);
	Add(split);

	tree.WhenOpen = THISBACK(TreeExpand);

	frame.Ctrl::Add(dlock.SizePos());
	dlock = "  Running..";
	dlock.SetFrame(BlackFrame());
	dlock.SetInk(Red);
	dlock.NoTransparent();
	dlock.Hide();

	CtrlLayoutOKCancel(quickwatch, "Watch");
	quickwatch.WhenClose = quickwatch.Breaker(IDCANCEL);
	quickwatch.value.SetReadOnly();
	quickwatch.value.SetFont(CourierZ(11));
	quickwatch.Sizeable().Zoomable();
	quickwatch.SetRect(0, 150, 300, 400);

	Transparent();

	periodic.Set(-50, THISBACK(Periodic));
}

One<Debugger> GdbCreate(One<Host>&& host, const String& exefile, const String& cmdline, bool console)
{
	Gdb *dbg = new Gdb;
	if(!dbg->Create(pick(host), exefile, cmdline, console)) {
		delete dbg;
		return NULL;
	}
	return dbg;
}
