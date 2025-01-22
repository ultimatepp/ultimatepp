#include "Debuggers.h"

#define KEYGROUPNAME "Debugger"
#define KEYNAMESPACE PdbKeys
#define KEYFILE      <ide/Debuggers/Pdb.key>
#include             <CtrlLib/key_source.h>

#ifdef PLATFORM_WIN32

#include <tlhelp32.h>

Vector<DWORD> GetChildProcessList(DWORD processId) {
	Vector<DWORD> child, all, parents;
	
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
		return child;
	
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(proc);
	
	if (!Process32First(hSnap, &proc)) {
		CloseHandle(hSnap);
		return child;
	}
	
	do {
		all << proc.th32ProcessID;
		parents << proc.th32ParentProcessID;
    } while(Process32Next(hSnap, &proc));
	
	CloseHandle(hSnap);
	
	child << processId;
	int init = 0;
	while (true) {
		int count = child.GetCount();
		if (init >= count)
			break;
		for (int cid = init; cid < count; ++cid) {
			for (int i = 0; i < all.GetCount(); ++i) {
				if (parents[i] == child[cid])
					child << all[i];
			}
		}
		init = count;
	}
	child.Remove(0);
	return child;
}

void TerminateChildProcesses(DWORD dwProcessId, UINT uExitCode) {
	Vector<DWORD> children = GetChildProcessList(dwProcessId);
	for (int i = 0; i < children.GetCount(); ++i) {
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, children[i]);
		TerminateProcess(hProcess, uExitCode);
		CloseHandle(hProcess);
	}
}

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "psapi.lib")

#define LLOG(x) // LOG(x)

using namespace PdbKeys;

void Pdb::DebugBar(Bar& bar)
{
	bar.Add(AK_STOP, DbgImg::StopDebug(), THISBACK(Stop));
	bool b = !IdeIsDebugLock();
	bar.Separator();
	bar.Add(b, AK_STEPINTO, DbgImg::StepInto(), THISBACK1(Trace, false));
	bar.Add(b, AK_STEPOVER, DbgImg::StepOver(), THISBACK1(Trace, true));
	bar.Add(b, AK_STEPOUT, DbgImg::StepOut(), THISBACK(StepOut));
	bar.Add(b, AK_RUNTO, DbgImg::RunTo(), THISBACK(DoRunTo));
	bar.Add(b, AK_RUN, DbgImg::Run(), THISBACK(Run));
	bar.Add(b, AK_SETIP, DbgImg::SetIp(), THISBACK(SetIp));
	bar.Add(!b, AK_BREAK, DbgImg::Stop(), THISBACK(BreakRunning));
	bar.MenuSeparator();
	bar.Add(b, AK_AUTOS, THISBACK1(SetTab, 0));
	bar.Add(b, AK_LOCALS, THISBACK1(SetTab, 1));
	bar.Add(b, AK_THISS, THISBACK1(SetTab, 2));
	bar.Add(b, AK_WATCHES, THISBACK1(SetTab, 3));
	bar.Add(b, AK_CLEARWATCHES, THISBACK(ClearWatches));
	bar.Add(b, AK_ADDWATCH, [=] { AddWatch(); });
	bar.Add(b, AK_CPU, THISBACK1(SetTab, 4));
	bar.Add(b, AK_MEMORY, THISBACK1(SetTab, 5));
	bar.Add(b, AK_BTS, THISBACK1(SetTab, 6));
	bar.MenuSeparator();
	bar.Add(b, "Copy backtrace", THISBACK(CopyStack));
	bar.Add(b, "Copy backtrace of all threads", THISBACK(CopyStackAll));
	bar.Add(b, "Copy dissassembly", THISBACK(CopyDisas));
	bar.Add(b, "Copy modules", THISBACK(CopyModules));
}

void Pdb::Tab()
{
	switch(tab.Get()) {
	case TAB_AUTOS: autos.SetFocus(); break;
	case TAB_LOCALS: locals.SetFocus(); break;
	case TAB_THIS: self.SetFocus(); break;
	case TAB_WATCHES: watches.SetFocus(); break;
	case TAB_MEMORY: memory.SetFocus(); break;
	case TAB_BTS: bts.SetFocus(); break;
	}
	Data();
	SyncTreeDisas();
}

void Pdb::SyncTreeDisas()
{
	bool d = tab.Get() == TAB_CPU || IsNull(tree_exp);
	disas.Show(d);
	tree.Show(!d);
}

bool Pdb::Key(dword key, int count)
{
	if(key >= 32 && key < 65535 && tab.Get() == TAB_LOCALS) {
		watches.DoInsertAfter();
		Ctrl* f = GetFocusCtrl();
		if(f && watches.HasChildDeep(f))
			f->Key(key, count);
		return true;
	}
	return Ctrl::Key(key, count);
}

#define CONFIGNAME "pdb debugger"

void Pdb::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	memory.SerializeSettings(s);
	Splitter dummy;
	s % dummy;
	s % show_type;
}

INITBLOCK
{
	RegisterGlobalConfig(CONFIGNAME);
}

bool Pdb::Create(Host& local, const String& exefile, const String& cmdline, bool clang_)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;
	String cl;
	if(exefile.Find(' ') >= 0)
		cl << '\"' << exefile << '\"';
	else
		cl << exefile;
	if(!IsNull(cmdline))
		cl << ' ' << ToSystemCharset(cmdline);

	clang = clang_;

	Buffer<char> cmd(cl.GetLength() + 1);
	memcpy(cmd, cl, cl.GetLength() + 1);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	Buffer<char> env(local.environment.GetCount() + 1);
	memcpy(env, ~local.environment, local.environment.GetCount() + 1);
	bool h = CreateProcess(exefile, cmd, NULL, NULL, TRUE,
	                       /*NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE|*/DEBUG_ONLY_THIS_PROCESS/*|DEBUG_PROCESS*/,
	                       ~env, NULL, &si, &pi);

	if(!h) {
		Exclamation("Error creating process&[* " + DeQtf(exefile) + "]&" +
		            "Windows error: " + DeQtf(GetLastErrorMessage()));
		return false;
	}
	hProcess = pi.hProcess;
	mainThread = pi.hThread;
	hProcessId = pi.dwProcessId;
	mainThreadId = pi.dwThreadId;

#ifdef CPU_64
	BOOL _64;
	win64 = IsWow64Process(hProcess, &_64) && !_64;
	LLOG("Win64 app: " << win64);
	disas.Mode64(win64);
#else
	win64 = false;
#endif

	if(win64)
		memory.SetTotal(I64(0xffffffffffff));
	else
		memory.SetTotal(0x80000000);
	
	CloseHandle(pi.hThread);

	IdeSetRight(rpane);
	IdeSetBottom(*this);
	
	SyncTreeDisas();
	
	LoadFromGlobal(*this, CONFIGNAME);

	if(!SymInitialize(hProcess, 0, FALSE)) {
		Error();
		Exclamation("Failed to load symbols");
		return false;
	}
	SymSetOptions(SYMOPT_LOAD_LINES|SYMOPT_UNDNAME|SYMOPT_NO_UNQUALIFIED_LOADS);

	lock = 0;
	stop = false;
	refreshmodules = true;
	terminated = false;

	running = true;
	
	break_running = false;

	RunToException();
	
	return !terminated;
}

INITBLOCK {
	RegisterWorkspaceConfig("pdb-debugger");
}

void Pdb::SerializeSession(Stream& s)
{
	int version = 0;
	s / version;
	int n = watches.GetCount();
	s / n;
	if(n < 0)
		LoadingError();
	for(int i = 0; i < n; i++) {
		String w;
		if(s.IsStoring())
			w = watches.Get(i, 0);
		s % w;
		if(s.IsLoading())
			watches.Add(w);
	}
}

struct CpuRegisterDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		Font fnt = Courier(Ctrl::HorzLayoutZoom(12));
		static int cx1 = GetTextSize("EFLAGS12", fnt().Bold()).cx +
		                 GetTextSize("0000 0000 0000 0000", fnt).cx;
		String name;
		String value;
		String odd;
		SplitTo((String)q, '|', name, value, odd);
		w.DrawRect(r, odd != "1" || (style & CURSOR) ? paper : Blend(SColorMark, SColorPaper, 220));
		int i = value.GetLength() - 4;
		while(i > 0) {
			value.Insert(i, ' ');
			i -= 4;
		}
		Size tsz = GetTextSize(value, fnt);
		int tt = r.top + max((r.Height() - tsz.cy) / 2, 0);
		w.DrawText(r.left, tt, name, fnt().Bold(), ink);
		w.DrawText(r.left + cx1 - tsz.cx, tt, value, fnt, ink);
	}
};

Pdb::Pdb()
:	visual_display(this) {
	hWnd = NULL;
	hProcess = INVALID_HANDLE_VALUE;
	current_frame = NULL;

	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6).SetDisplay(visual_display);
	autos.WhenEnterRow = THISBACK1(SetTreeA, &autos);
	autos.WhenBar = [=](Bar& bar) { DataMenu(autos, bar); };
	autos.EvenRowColor();
	autos.WhenLeftDouble << [=] { AddWatch(autos.GetKey()); };

	locals.NoHeader();
	locals.AddColumn("", 1);
	locals.AddColumn("", 6).SetDisplay(visual_display);
	locals.WhenEnterRow = THISBACK1(SetTreeA, &locals);
	locals.WhenBar = [=](Bar& bar) { DataMenu(locals, bar); };
	locals.EvenRowColor();
	locals.WhenLeftDouble << [=] { AddWatch(locals.GetKey()); };

	self.NoHeader();
	self.AddColumn("", 1);
	self.AddColumn("", 6).SetDisplay(visual_display);
	self.WhenEnterRow = THISBACK1(SetTreeA, &self);
	self.WhenBar = [=](Bar& bar) { DataMenu(self, bar); };
	self.EvenRowColor();
	self.WhenLeftDouble << [=] { AddWatch(self.GetKey()); };

	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6).SetDisplay(visual_display);
	watches.Moving();
	watches.WhenEnterRow = THISBACK1(SetTreeA, &watches);
	watches.WhenBar = THISBACK(WatchesMenu);
	watches.WhenAcceptEdit = THISBACK(Data);
	watches.WhenDrop = THISBACK(DropWatch);
	watches.EvenRowColor();

	tab.Add(autos.SizePos(), "Autos");
	tab.Add(locals.SizePos(), "Locals");
	tab.Add(self.SizePos(), "this");
	tab.Add(watches.SizePos(), "Watches");
	tab.Add(cpu.SizePos(), "CPU");
	tab.Add(memory.SizePos(), "Memory");
	tab.Add(bts.SizePos(), "Threads");

	cpu.Columns(4);
	cpu.ItemHeight(Courier(Ctrl::HorzLayoutZoom(12)).GetCy());
	cpu.SetDisplay(Single<CpuRegisterDisplay>());

	memory.pdb = this;

	dlock = "  Running..";
	dlock.SetFrame(BlackFrame());
	dlock.SetInk(Red);
	dlock.NoTransparent();
	dlock.Hide();
	framelist.Ctrl::Add(dlock.SizePos());

	pane.Add(tab.SizePos());
	pane.Add(threadlist.LeftPosZ(370, 60).TopPos(2, EditField::GetStdHeight()));
	int bcx = min(EditField::GetStdHeight(), DPI(16));
	pane.Add(framelist.HSizePos(Zx(434), 2 * bcx).TopPos(2, EditField::GetStdHeight()));
	pane.Add(frame_up.RightPos(bcx, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_up.SetImage(DbgImg::FrameUp());
	frame_up << [=] { FrameUpDown(-1); };
	frame_up.Tip("Previous Frame");
	pane.Add(frame_down.RightPos(0, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_down.SetImage(DbgImg::FrameDown());
	frame_down << [=] { FrameUpDown(1); };
	frame_down.Tip("Next Frame");

	Add(pane.SizePos());

	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);

	memory.WhenGoto = THISBACK(MemoryGoto);

	tab <<= THISBACK(Tab);

	framelist <<= THISBACK(SetFrame);
	threadlist <<= THISBACK(SetThread);

	tree.WhenOpen = THISBACK(TreeExpand);
	tree.WhenBar = THISFN(TreeMenu);
	tree.WhenLeftDouble = THISFN(TreeWatch);
	
	rpane.Add(disas.SizePos());
	rpane.Add(tree.SizePos());

	FileIn in(ConfigFile("TreeTypes.txt"));
	while(!in.IsEof()) {
		String type = in.GetLine();
		String desc = in.GetLine();
		treetype.Add(type, desc);
	}
	StringStream ss(WorkspaceConfigData("pdb-debugger"));
	Load(callback(this, &Pdb::SerializeSession), ss);
	
	LoadPrettyScripts();
}

void Pdb::CopyStack()
{
	String s;
	for(int i = 0; i < framelist.GetCount(); i++) {
		s << framelist.GetValue(i);
		FilePos fp = GetFilePos(frame[i].pc);
		if(fp)
			s << " at " << fp.path << " " << fp.line + 1;
		s << "\n";
	}
	WriteClipboardText(s);
}

void Pdb::CopyStackAll()
{
	String s;
	for(int i = 0; i < threads.GetCount(); i++) {
		int thid = threads.GetKey(i);
		s << "----------------------------------\r\n"
		  << "Thread ID: " << Format("0x%x", thid) << "\r\n\r\n";
		for(const auto& f : Backtrace(threads[i]))
			s << f.text << "\r\n";
		s << "\r\n";
	}
	WriteClipboardText(s);
}

void Pdb::CopyDisas()
{
	disas.WriteClipboard();
}

void Pdb::CopyModules()
{
	String s;
	for(const ModuleInfo& f : module)
		s << f.path << ", base 0x" << FormatIntHex((void *)f.base)
		  << ", size: 0x" << FormatIntHex(f.size) << "\n";
	WriteClipboardText(s);
}

void Pdb::Stop()
{
	if(!terminated) {
		terminated = true;
		SaveTree();
		String fn = ConfigFile("TreeTypes.txt");
		FileOut out(fn);
		for(int i = 0; i < treetype.GetCount(); i++)
			out << treetype.GetKey(i) << "\r\n" << treetype[i] << "\r\n";
		StringStream ss;
		Store(callback(this, &Pdb::SerializeSession), ss);
		WorkspaceConfigData("pdb-debugger") = ss;
		if(hProcess != INVALID_HANDLE_VALUE) {
			for(int i = 0; i < 10; i++) {
				if(DebugActiveProcessStop(processid))
					break;
				Sleep(100);
			}
			TerminateChildProcesses(hProcessId, 0);
			TerminateProcess(hProcess, 0);
			dword exitcode = STILL_ACTIVE;
			int start = msecs();
			while(GetExitCodeProcess(hProcess, &exitcode) && exitcode == STILL_ACTIVE && msecs(start) < 1000)
				Sleep(100);
			if(exitcode == STILL_ACTIVE)
				Exclamation("Unable to kill debugee. Please restart theide.");
			while(threads.GetCount())
				RemoveThread(threads.GetKey(0)); // To CloseHandle
			UnloadModuleSymbols();
			SymCleanup(hProcess);
			CloseHandle(hProcess);
		}
		StoreToGlobal(*this, CONFIGNAME);
		IdeRemoveBottom(*this);
		IdeRemoveRight(rpane);
	}
}

bool Pdb::IsFinished()
{
	return terminated;
}

Pdb::~Pdb()
{
	Stop();
}

One<Debugger> PdbCreate(Host& host, const String& exefile, const String& cmdline, bool clang)
{
	One<Debugger> dbg;
	if(!dbg.Create<Pdb>().Create(host, exefile, cmdline, clang))
		dbg.Clear();
	return dbg;
}

#define LAYOUTFILE <ide/Debuggers/Pdb.lay>
#include <CtrlCore/lay.h>

#define TOPICFILE <ide/Debuggers/app.tpp/all.i>
#include <Core/topic_group.h>

struct PDBExpressionDlg : WithEditPDBExpressionLayout<TopWindow> {
	Pdb *pdb;

	void Sync();

	typedef PDBExpressionDlg CLASSNAME;

	PDBExpressionDlg(const char *title, String& brk, Pdb *pdb);
};

void PDBExpressionDlg::Sync()
{
	if(pdb)
		value <<= RawPickToValue(pick(pdb->Visualise(~text)));
}

PDBExpressionDlg::PDBExpressionDlg(const char *title, String& brk, Pdb *pdb)
:	pdb(pdb)
{
	CtrlLayoutOKCancel(*this, title);
	help.SetQTF(GetTopic("ide/Debuggers/app/PDBExpressions_en-us"));
	help.Background(White());
	help.SetFrame(ViewFrame());
	text <<= brk;
	text <<= THISBACK(Sync);
	value.SetDisplay(pdb->visual_display);
	value.Show(pdb);
	value_lbl.Show(pdb);
	Sync();
}

bool EditPDBExpression(const char *title, String& brk, Pdb *pdb)
{
	PDBExpressionDlg dlg(title, brk, pdb);
	if(dlg.Execute() != IDOK)
		return false;
	brk = ~dlg.text;
	return true;
}

#endif
