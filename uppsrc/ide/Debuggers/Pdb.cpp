#include "Debuggers.h"

#ifdef COMPILER_MSC

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "psapi.lib")

#define LLOG(x) // LOG(x)

#define KEYGROUPNAME "PdbDebugger"
#define KEYNAMESPACE PdbKeys
#define KEYFILE      <ide/Debuggers/Pdb.key>
#include             <CtrlLib/key_source.h>

using namespace PdbKeys;

void Pdb::DebugBar(Bar& bar)
{
	bar.Add(AK_STOP, THISBACK(Stop));
	bool b = !IdeIsDebugLock();
	bar.Separator();
	bar.Add(b, AK_STEPINTO, DbgImg::StepInto(), THISBACK1(Trace, false));
	bar.Add(b, AK_STEPOVER, DbgImg::StepOver(), THISBACK1(Trace, true));
	bar.Add(b, AK_STEPOUT, DbgImg::StepOut(), THISBACK(StepOut));
	bar.Add(b, AK_RUNTO, DbgImg::RunTo(), THISBACK(DoRunTo));
	bar.Add(b, AK_RUN, DbgImg::Run(), THISBACK(Run));
	bar.Add(b, AK_SETIP, DbgImg::SetIp(), THISBACK(SetIp));
	bar.Add(!b, AK_STOP, DbgImg::Stop(), THISBACK(BreakRunning));
	bar.MenuSeparator();
	bar.Add(b, AK_AUTOS, THISBACK1(SetTab, 0));
	bar.Add(b, AK_LOCALS, THISBACK1(SetTab, 1));
	bar.Add(b, AK_WATCHES, THISBACK1(SetTab, 2));
	bar.Add(b, AK_CLEARWATCHES, THISBACK(ClearWatches));
	bar.Add(b, AK_ADDWATCH, THISBACK(AddWatch));
	bar.Add(b, AK_EXPLORER, THISBACK(DoExplorer));
	bar.Add(b, AK_MEMORY, THISBACK1(SetTab, 4));
	bar.MenuSeparator();
	bar.Add(b, "Copy backtrace", THISBACK(CopyStack));
	bar.Add(b, "Copy dissassembly", THISBACK(CopyDisas));
}

void Pdb::Tab()
{
	switch(tab.Get()) {
	case 0: autos.SetFocus(); break;
	case 1: locals.SetFocus(); break;
	case 2: watches.SetFocus(); break;
	case 3: explorer.SetFocus(); break;
	case 4: memory.SetFocus(); break;
	}
	Data();
}

bool Pdb::Key(dword key, int count)
{
	if(key >= 32 && key < 65535 && tab.Get() == 2) {
		watches.DoInsertAfter();
		Ctrl* f = GetFocusCtrl();
		if(f && watches.HasChildDeep(f))
			f->Key(key, count);
		return true;
	}
	if(key == K_ENTER && expexp.HasFocus()) {
		Explore(~expexp);
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
	s % split;
}

INITBLOCK
{
	RegisterGlobalConfig(CONFIGNAME);
}

bool Pdb::Create(One<Host> local, const String& exefile, const String& cmdline)
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
		cl << ' ' << cmdline;

	Buffer<char> cmd(cl.GetLength() + 1);
	memcpy(cmd, cl, cl.GetLength() + 1);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	Buffer<char> env(local->GetEnvironment().GetCount() + 1);
	memcpy(env, ~local->GetEnvironment(), local->GetEnvironment().GetCount() + 1);
	bool h = CreateProcess(exefile, cmd, NULL, NULL, TRUE,
	                       NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE|DEBUG_ONLY_THIS_PROCESS|DEBUG_PROCESS,
	                       ~env, NULL, &si, &pi);
	if(!h) {
		Exclamation("Error creating process&[* " + DeQtf(exefile) + "]&" +
		            "Windows error: " + DeQtf(GetLastErrorMessage()));
		return false;
	}
	hProcess = pi.hProcess;
	CloseHandle(pi.hThread);

	IdeSetBottom(*this);
	IdeSetRight(disas);

	LoadFromGlobal(*this, CONFIGNAME);

	if(!SymInitialize(hProcess, 0, FALSE)) {
		Error();
		return false;
	}
	SymSetOptions(SYMOPT_LOAD_LINES|SYMOPT_UNDNAME|SYMOPT_NO_UNQUALIFIED_LOADS);

	lock = 0;
	stop = false;
	refreshmodules = true;
	terminated = false;

	running = true;

	RunToException();
//	Sync();

	return true;
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
	for(int i = 0; i < n; i++) {
		String w;
		if(s.IsStoring())
			w = watches.Get(i, 0);
		s % w;
		if(s.IsLoading())
			watches.Add(w);
	}
}

Pdb::Pdb()
{
	hProcess = INVALID_HANDLE_VALUE;

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
	locals.AddColumn("", 6).SetDisplay(Single<VisualDisplay>());
	locals.WhenEnterRow = THISBACK1(SetTreeA, &locals);
	locals.WhenBar = THISBACK(LocalsMenu);
	locals.WhenLeftDouble = THISBACK1(ExploreKey, &locals);

	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6).SetDisplay(Single<VisualDisplay>());
	watches.Inserting().Removing();
	watches.WhenEnterRow = THISBACK1(SetTreeA, &watches);
	watches.WhenBar = THISBACK(WatchesMenu);
	watches.WhenLeftDouble = THISBACK1(ExploreKey, &watches);

	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6).SetDisplay(Single<VisualDisplay>());
	autos.WhenEnterRow = THISBACK1(SetTreeA, &autos);
	autos.WhenBar = THISBACK(AutosMenu);
	autos.WhenLeftDouble = THISBACK1(ExploreKey, &autos);

	int c = EditField::GetStdHeight();
	explorer.AddColumn("", 1);
	explorer.AddColumn("", 6).SetDisplay(Single<VisualDisplay>());
	explorer_pane.Add(exback.LeftPos(0, c).TopPos(0, c));
	explorer_pane.Add(exfw.LeftPos(c + 2, c).TopPos(0, c));
	explorer_pane.Add(expexp.HSizePos(2 * c + 4).TopPos(0, c));
	explorer_pane.Add(explorer.HSizePos().VSizePos(EditField::GetStdHeight(), 0));
	explorer.NoHeader();
	explorer.WhenEnterRow = THISBACK(ExplorerTree);
	explorer.WhenLeftDouble = THISBACK(ExpExp);
	explorer.WhenBar = THISBACK(ExplorerMenu);

	exback.SetImage(DbgImg::ExplorerBack());
	exback <<= THISBACK(ExBack);
	exfw.SetImage(DbgImg::ExplorerFw());
	exfw <<= THISBACK(ExFw);
	exback.Disable();
	exfw.Disable();

	tab.Add(autos.SizePos(), "Autos");
	tab.Add(locals.SizePos(), "Locals");
	tab.Add(watches.SizePos(), "Watches");
	tab.Add(explorer_pane.SizePos(), "Explorer");
	memory.cdb = this;
	tab.Add(memory.SizePos(), "Memory");

	dlock = "  Running..";
	dlock.SetFrame(BlackFrame());
	dlock.SetInk(Red);
	dlock.NoTransparent();
	dlock.Hide();
	framelist.Ctrl::Add(dlock.SizePos());

	pane.Add(tab.SizePos());
	pane.Add(threadlist.LeftPosZ(320, 60).TopPos(2, EditField::GetStdHeight()));
	pane.Add(framelist.HSizePosZ(384, 0).TopPos(2, EditField::GetStdHeight()));
	split.Horz(pane, tree.SizePos());
	split.SetPos(8000);
	Add(split);

	disas.AddFrame(regs);
	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);

	memory.WhenGoto = THISBACK(MemoryGoto);

	tab <<= THISBACK(Tab);

	framelist <<= THISBACK(SetFrame);
	threadlist <<= THISBACK(SetThread);

	watches.WhenAcceptEdit = THISBACK(Data);
	watches.WhenDrop = THISBACK(DropWatch);
	tab <<= THISBACK(Data);

	tree.WhenOpen = THISBACK(TreeExpand);

	FileIn in(ConfigFile("TreeTypes.txt"));
	while(!in.IsEof()) {
		String type = in.GetLine();
		String desc = in.GetLine();
		treetype.Add(type, desc);
	}
	StringStream ss(WorkspaceConfigData("pdb-debugger"));
	Load(callback(this, &Pdb::SerializeSession), ss);
}

void Pdb::CleanupOnExit()
{
	if(hProcess != INVALID_HANDLE_VALUE) {
		while(threads.GetCount())
			RemoveThread(threads.GetKey(0));
		UnloadModuleSymbols();
		SymCleanup(hProcess);
		CloseHandle(hProcess);
		hProcess = INVALID_HANDLE_VALUE;
	}
}

void Pdb::CopyStack()
{
	String s;
	for(int i = 0; i < framelist.GetCount(); i++)
		s << framelist.GetValue(i) << "\n";
	WriteClipboardText(s);
}

void Pdb::CopyDisas()
{
	disas.WriteClipboard();
}

Pdb::~Pdb()
{
	SaveTree();
	String fn = ConfigFile("TreeTypes.txt");
	FileOut out(fn);
	for(int i = 0; i < treetype.GetCount(); i++)
		out << treetype.GetKey(i) << "\r\n" << treetype[i] << "\r\n";
	StringStream ss;
	Store(callback(this, &Pdb::SerializeSession), ss);
	WorkspaceConfigData("pdb-debugger") = ss;
	if(hProcess != INVALID_HANDLE_VALUE) {
		if(!running)
			ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
		TerminateProcess(hProcess, -1);
		do {
			if(!WaitForDebugEvent(&event, INFINITE))
				break;
			if(!ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE))
				break;
		}
		while(event.dwDebugEventCode != EXIT_PROCESS_DEBUG_EVENT);
		CleanupOnExit();
	}
	StoreToGlobal(*this, CONFIGNAME);
	IdeRemoveBottom(*this);
	IdeRemoveRight(disas);
}

One<Debugger> PdbCreate(One<Host> host, const String& exefile, const String& cmdline)
{
	One<Debugger> dbg;
	if(!dbg.Create<Pdb>().Create(host, exefile, cmdline))
		dbg.Clear();
	return dbg;
}

#endif
