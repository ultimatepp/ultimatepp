#include "Debuggers.h"

#define METHOD_NAME UPP_METHOD_NAME("Gdb")

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
	bar.Add(!b && pid, AK_BREAK, DbgImg::Stop(), THISBACK(BreakRunning));
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
	bar.Add(b, "Copy backtrace of all threads", THISBACK(CopyStackAll));
	bar.Add(b, "Copy dissassembly", THISBACK(CopyDisas));
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
		if(s[0] == 'i' && s[1] == 'n')
			s += 2;
		while(*s == ' ')
			s++;
	}
	String result;
	const char *w = strchr(s, '\r');
	if(!w)
		w = strchr(s, '\n');
	if(w)
		result = String(s, w);
	else
		result = s;
	if(!IsAlpha(*s)) {
		int q = result.ReverseFind(' ');
		if(q >= 0)
			result = result.Mid(q + 1);
	}
	return result.GetCount() > 2 ? result : Null;
}

void Gdb::CopyStack()
{
	if(IdeIsDebugLock())
		return;
	DropFrames();
	String s;
	for(int i = 0; i < frame.GetCount(); i++)
		s << frame.GetValue(i) << "\n";
	WriteClipboardText(s);
}

void Gdb::CopyStackAll()
{
	String s = FastCmd("info threads");
	StringStream ss(s);
	String r;
	while(!ss.IsEof()) {
		String s = ss.GetLine();
		CParser p(s);
		try {
			p.Char('*');
			if(p.IsNumber()) {
				int id = p.ReadInt();
				r << "----------------------------------\r\n"
				  << "Thread: " << id << "\r\n\r\n";

				FastCmd(Sprintf("thread %d", id));

				int i = 0;
				for(;;) {
					String s = FormatFrame(FastCmd("frame " + AsString(i++)));
					if(IsNull(s)) break;
					r << s << "\r\n";
				}
				r << "\r\n";
			}
		}
		catch(CParser::Error) {}
	}
	FastCmd("thread " + ~~threads);
	WriteClipboardText(r);
}

void Gdb::CopyDisas()
{
	if(IdeIsDebugLock())
		return;
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
	if(IdeIsDebugLock()) {
		BreakRunning();
		bp_filename = filename;
		bp_line = line;
		bp_val = bp;
		return true;
	}

	String bi = Bpoint(*host, filename, line);

	String command;
	if(bp.IsEmpty())
		command = "clear " + bi;
	else if(bp[0]==0xe || bp == "1")
		command = "b " + bi;
	else
		command = "b " + bi + " if " + bp;
	return !FastCmd(command).IsEmpty();
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

String Gdb::Cmdp(const char *cmdline, bool fr, bool setframe)
{
	expression_cache.Clear();
	IdeHidePtr();
	String s = Cmd(cmdline);
	exception.Clear();
	if(!running_interrupted) {
		int q = s.Find("received signal SIG");
		if(q >= 0) {
			int l = s.ReverseFind('\n', q);
			if(l < 0)
				l = 0;
			int r = s.Find('\n', q);
			if(r < 0)
				r = s.GetCount();
			if(l < r)
				exception = s.Mid(l, r - l);
		}
	}
	else {
		running_interrupted = false;
	}
	
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
	if(setframe) {
		frame.Clear();
		frame.Add(0, FormatFrame(FastCmd("frame")));
		frame <<= 0;
	}
	
	if (dbg->IsRunning()) {
		if (IsProcessExitedNormally(s))
			Stop();
		else {
			s = ObtainThreadsInfo();
			
			ObtainData();
		}
	}
	return s;
}

bool Gdb::IsProcessExitedNormally(const String& cmd_output) const
{
	const auto phrase = String().Cat() << "(process " << pid << ") exited normally";
	return cmd_output.Find(phrase) >= 0;
}

String Gdb::ObtainThreadsInfo()
{
	threads.Clear();
	String output = FastCmd("info threads");
	StringStream ss(output);
	int active_thread = -1;
	bool main = true;
	while(!ss.IsEof()) {
		String s = ss.GetLine();
		CParser p(s);
		try {
			bool is_active = p.Char('*');
			if(!p.IsNumber())
				continue;
			
			int id = p.ReadInt();
				
			String name;
			while (!p.IsEof()) {
				if (p.IsString()) {
					name = p.ReadString();
					break;
				}
					
				p.SkipTerm();
			}
			
			AttrText text(String() << "Thread " << id);
			if (!name.IsEmpty())
				text.Set(text.ToString() << " (" << name << ")");
			if(is_active) {
				active_thread = id;
				text.Bold();
			}
			if(main) {
				text.Underline();
				main = false;
			}
			threads.Add(id, text);
			threads.GoBegin();
		}
		catch(CParser::Error e) {
			Loge() << METHOD_NAME << e;
		}
	}
		
	if(active_thread >= 0)
		threads <<= active_thread;
	if(threads.GetCount() == 0) {
		String error = t_(
			"Failed to obtain information about threads. "
			"Make sure your application posses debug info. "
			"The debugger and debugge proceses will be stoped!");
		
		Loge() << METHOD_NAME << error;
		ErrorOK(error);
		
		Stop();
	}
	
	return output;
}

void Gdb::ShowException()
{
	if(exception.GetCount())
		ErrorOK(exception);
	exception.Clear();
}

String Gdb::DoRun()
{
	if(firstrun) {
		firstrun = false;
		Cmd("start");
		String s = Cmd("info inferior");
		int q = s.FindAfter("process");
		pid = atoi(~s + q);
		IdeSetBar();
	}
	
	String s;
	for(;;) {
		ClearCtrls();
		s = Cmdp("continue");
		if(IsNull(bp_filename))
			break;
		if(!IdeIsDebugLock())
			SetBreakpoint(bp_filename, bp_line, bp_val);
		bp_filename.Clear();
	}
	ShowException();
	return s;
}

bool Gdb::RunTo()
{
	if(IdeIsDebugLock())
		return false;
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
		Exclamation("No code at chosen location!");
		return false;
	}
	String e = DoRun();
	FastCmd("clear " + bi);
	if(df)
		disas.SetFocus();
	CheckEnd(e);
	IdeActivateBottom();
	return true;
}

void Gdb::BreakRunning()
{
	Logd() << METHOD_NAME << "PID: " << pid << "\n";
	
	auto error = gdb_utils->BreakRunning(pid);
	if(!error.IsEmpty()) {
		Loge() << METHOD_NAME << error;
		ErrorOK(error);
		return;
	}
	
	running_interrupted = true;
}

void Gdb::Run()
{
	if(IdeIsDebugLock())
		return;
	String s = DoRun();
	CheckEnd(s);
	IdeActivateBottom();
}

void Gdb::Step(const char *cmd)
{
	if(IdeIsDebugLock())
		return;
	bool b = disas.HasFocus();
	String s = Cmdp(cmd);
	if(b) disas.SetFocus();
	CheckEnd(s);
	IdeActivateBottom();
	ShowException();
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
}

void Gdb::DropFrames()
{
	if(frame.GetCount() < 2)
		LoadFrames();
}

void Gdb::LoadFrames()
{
	if(frame.GetCount())
		frame.Trim(frame.GetCount() - 1);
	int i = frame.GetCount();
	int n = 0;
	for(;;) {
		String s = FormatFrame(FastCmd(Sprintf("frame %d", i)));
		if(IsNull(s))
			break;
		if(n++ >= max_stack_trace_size) {
			frame.Add(Null, Sprintf("<load more> (%d loaded)", frame.GetCount()));
			break;
		}
		frame.Add(i++, s);
	}
}

void Gdb::SwitchFrame()
{
	int i = ~frame;
	if(IsNull(i)) {
		i = frame.GetCount() - 1;
		LoadFrames();
		frame <<= i;
	}
	Cmdp("frame " + AsString(i), i, false);
}

void Gdb::FrameUpDown(int dir)
{
	if(frame.GetCount() < 2)
		LoadFrames();
	int q = frame.GetIndex() + dir;
	if(q >= 0 && q < frame.GetCount()) {
		frame.SetIndex(q);
		SwitchFrame();
	}
}

void Gdb::SwitchThread()
{
	int i = ~threads;
	Cmdp("thread " + AsString(i));
}

void Gdb::ClearCtrls()
{
	threads.Clear();
	disas.Clear();
	
	locals.Clear();
	watches.Clear();
	autos.Clear();
	self.Clear();
	cpu.Clear();
	
	tree.Clear();
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

	dbg = host->StartProcess(GdbCommand(console) + host->NormalizeExecutablePath(exefile));

	if (!dbg) {
		ErrorOK("Error while invoking gdb!");
		return false;
	}

	IdeSetBottom(*this);
	IdeSetRight(disas);

	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);
	frame.WhenDrop = THISBACK(DropFrames);
	frame <<= THISBACK(SwitchFrame);
	
	threads <<= THISBACK(SwitchThread);

	watches.WhenAcceptEdit = THISBACK(ObtainData);
	tab <<= THISBACK(ObtainData);

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

#ifdef PLATFORM_WIN32
	Cmd("set new-console on");
#endif

	if(!IsNull(cmdline))
		Cmd("set args " + cmdline);

	firstrun = true;
	running_interrupted = false;

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
	: gdb_utils(GdbUtilsFactory().Create())
{
	auto Mem = [=](Bar& bar, ArrayCtrl& h) {
		String s = h.GetKey();
		if(s.GetCount()) {
			if(!IsAlpha(*s))
				s = '(' + s + ')';
			bar.Add("Memory at &&" + s, [=] { tab.Set(memory); MemoryLoad('&' + s, 2048, true); });
			bar.Add("Memory at " + s, [=] { tab.Set(memory); MemoryLoad(s, 2048, true); });
		}
	};
	locals.NoHeader();
	locals.AddColumn("", 1);
	locals.AddColumn("", 6);
	locals.EvenRowColor();
	locals.WhenSel = THISBACK1(SetTree, &locals);
	locals.WhenBar = [=](Bar& bar) { Mem(bar, locals); };
	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6);
	watches.Inserting().Removing();
	watches.EvenRowColor();
	watches.WhenSel = THISBACK1(SetTree, &watches);
	watches.WhenBar = [=](Bar& bar) { Mem(bar, watches); };
	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6);
	autos.EvenRowColor();
	autos.WhenSel = THISBACK1(SetTree, &autos);
	autos.WhenBar = [=](Bar& bar) { Mem(bar, autos); };
	self.NoHeader();
	self.AddColumn("", 1);
	self.AddColumn("", 6);
	self.EvenRowColor();
	self.WhenSel = THISBACK1(SetTree, &self);
	self.WhenBar = [=](Bar& bar) { Mem(bar, self); };
	cpu.Columns(3);
	cpu.ItemHeight(Courier(Ctrl::HorzLayoutZoom(12)).GetCy());

	pane.Add(tab.SizePos());
	tab.Add(autos.SizePos(), "Autos");
	tab.Add(locals.SizePos(), "Locals");
	tab.Add(watches.SizePos(), "Watches");
	tab.Add(self.SizePos(), "this");
	tab.Add(cpu.SizePos(), "CPU");
	tab.Add(memory.SizePos(), "Memory");
	pane.Add(threads.LeftPosZ(330, 150).TopPos(2));

	memory.WhenGotoDlg = [=] { MemoryGoto(); };

	int bcx = min(EditField::GetStdHeight(), DPI(16));
	pane.Add(frame.HSizePos(Zx(484), 2 * bcx).TopPos(2));
	pane.Add(frame_up.RightPos(bcx, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_up.SetImage(DbgImg::FrameUp());
	frame_up << [=] { FrameUpDown(-1); };
	pane.Add(frame_down.RightPos(0, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_down.SetImage(DbgImg::FrameDown());
	frame_down << [=] { FrameUpDown(1); };


	split.Horz(pane, tree.SizePos());
	split.SetPos(8000);
	Add(split);

	tree.WhenOpen = THISBACK(OnTreeExpand);
	tree.WhenBar = THISBACK(OnTreeBar);

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
	auto dbg = MakeOne<Gdb>();
	if(!dbg->Create(pick(host), exefile, cmdline, console))
		return nullptr;
	return pick(dbg); // CLANG does not like this without pick
}
