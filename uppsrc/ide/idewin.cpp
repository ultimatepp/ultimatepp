#include "ide.h"

#define IMAGECLASS IdeImg
#define IMAGEFILE  <ide/ide.iml>
#include <Draw/iml_source.h>

#include <art/BlueBar/BlueBar.h>

void ChStdSkinBlue()
{
	ChStdSkin();
	InstallBlueBar();
}

void ChHostSkinBlue()
{
	ChHostSkin();
	InstallBlueBar();
}

void ChClassicSkinBlue()
{
	ChClassicSkin();
	InstallBlueBar();
}

void Ide::SyncCh()
{
	switch(chstyle) {
	case 0:
		SetSkin(ChHostSkin);
		break;
	case 1:
		SetSkin(ChStdSkin);
		break;
	case 2:
		SetSkin(ChClassicSkin);
		break;
	case 3:
		SetSkin(ChHostSkinBlue);
		break;
	case 4:
		SetSkin(ChStdSkinBlue);
		break;
	}
}

void Ide::ToggleVerboseBuild() {
	console.verbosebuild = !console.verbosebuild;
}

void Ide::ToggleStopOnErrors() {
	stoponerrors = !stoponerrors;
}

void Ide::SwapPackagesFiles()
{
	wesplit.Zoom(wesplit.GetZoom() == 1 ? -1 : 1);
}

void IdePutErrorLine(const String& line)
{
	Ide *ide = dynamic_cast<Ide *>(TheIde());
	if(ide && ide->console.verbosebuild) {
		ide->SetBottom(Ide::BERRORS);
		ide->ConsoleRunEnd();
		ide->ConsoleLine(line, true);
	}
}

void Ide::ConsoleClear()
{
	console <<= Null;
}

void Ide::ConsoleCopy()
{
	console.Copy();
}

void Ide::ConsolePaste()
{
	String s = ReadClipboardText();
	if(!IsNull(s)) {
		s.Insert(0, '\n');
		int len = console.GetLength();
		console.Insert(len, s.ToWString());
		console.SetCursor(len + 1);
	}
}

void Ide::PutConsole(const char *s)
{
	console << s << "\n";
}

void Ide::PutVerbose(const char *s)
{
	if(console.verbosebuild) {
		PutConsole(s);
		console.Sync();
	}
}

bool Ide::IdeIsBuilding() const
{
	return idestate == Ide::BUILDING;
}

String Ide::IdeGetOneFile() const
{
	return onefile;
}

int Ide::IdeConsoleExecute(const char *cmdline, Stream *out, const char *envptr, bool quiet, bool noconvert)
{
	return console.Execute(cmdline, out, envptr, quiet, noconvert);
}

int Ide::IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet, bool noconvert)
{
	ShowConsole();
	console.Input(true);
	int r = console.Execute(cmdline, out, envptr, quiet, noconvert);
	console.Input(false);
	return r;
}

int Ide::IdeConsoleExecute(One<AProcess> pick_ process, const char *cmdline, Stream *out, bool quiet)
{
	return console.Execute(pick(process), cmdline, out, quiet);
}

int Ide::IdeConsoleAllocSlot()
{
	return console.AllocSlot();
}

bool Ide::IdeConsoleRun(const char *cmdline, Stream *out, const char *envptr, bool quiet, int slot, String key, int blitz_count)
{
	return console.Run(cmdline, out, envptr, quiet, slot, key, blitz_count);
}

bool Ide::IdeConsoleRun(One<AProcess> pick_ process, const char *cmdline, Stream *out, bool quiet, int slot, String key, int blitz_count)
{
	return console.Run(pick(process), cmdline, out, quiet, slot, key, blitz_count);
}

void Ide::IdeConsoleFlush()
{
	console.Flush();
}

void Ide::IdeConsoleBeginGroup(String group)
{
	console.BeginGroup(group);
}

void Ide::IdeConsoleEndGroup()
{
	console.EndGroup();
}

bool Ide::IdeConsoleWait()
{
	return console.Wait();
}

bool Ide::IdeConsoleWait(int slot)
{
	console.Wait(slot);
	return true;
}

void Ide::IdeConsoleOnFinish(Callback cb)
{
	console.OnFinish(cb);
}

void Ide::IdeSetRight(Ctrl& ctrl)
{
	right.Add(ctrl.SizePos());
	right_split.Zoom(-1);
}

void Ide::IdeRemoveRight(Ctrl& ctrl)
{
	ctrl.Remove();
	if(!right.GetFirstChild())
		right_split.Zoom(0);
}

bool Ide::IdeIsDebug() const
{
	return debugger;
}

String GetCurrentBuildMethod()
{
	Ide *ide = dynamic_cast<Ide *>(TheIde());
	return ide ? ide->method : String();
}

String GetCurrentMainPackage()
{
	Ide *ide = dynamic_cast<Ide *>(TheIde());
	return ide ? ide->main : String();
}

int GetHydraThreads()
{
	Ide *ide = dynamic_cast<Ide *>(TheIde());
	return ide ? ide->hydra1_threads : CPU_Cores();
}

String GetAndroidSDKPath()
{
	Ide *ide = dynamic_cast<Ide *>(TheIde());
	return ide ? ide->androidSDKPath : String();
}

void Ide::IdeEndDebug()
{
	console.Kill();
	debugger.Clear();
	debuglock = 0;
	MakeTitle();
	editor.EnableBreakpointing();
	SetBar();
	editor.HidePtr();
	for(int i = 0; i < 2; i++)
		posfile[i].Clear();
	PosSync();
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String file = SourcePath(n, pk.file[i]);
			ValidateBreakpoints(Filedata(file).lineinfo);
		}
	}
	editor.ValidateBreakpoints();
}

void Ide::IdeSetDebugPos(const String& file, int line, const Image& img, int i)
{
	posfile[i] = file;
	posline[i] = line;
	posimg[i] = img;
	EditFile(file);
	editor.GotoLine(line);
	PosSync();
	Size sz = editor.GetPageSize();
	Point p = editor.GetScrollPos();
	int l = editor.GetCursorLine();
	if(sz.cy > 8 && l - p.y > sz.cy - 4)
		editor.SetScrollPos(Point(p.x, max(0, l - 4)));
	SetForeground();
	editor.SetFocus();
}

void Ide::IdeHidePtr()
{
	editor.HidePtr();
}

bool Ide::IdeDebugLock()
{
	if(debuglock == 0) {
		editor.DisableBreakpointing();
		MakeTitle();
		SetBar();
		Sync();
		debuglock = 1;
		return true;
	}
	debuglock++;
	return false;
}

bool Ide::IdeDebugUnLock()
{
	debuglock--;
	ASSERT(debuglock >= 0);
	if(debuglock == 0) {
		MakeTitle();
		editor.EnableBreakpointing();
		SetBar();
		return true;
	}
	return false;
}

bool Ide::IdeIsDebugLock() const
{
	return debuglock;
}

String Ide::IdeGetFileName() const
{
	return !designer ? editfile : Null;
}

int Ide::IdeGetFileLine()
{
	return !designer ? editor.GetNoLine(editor.GetCursorLine()) : 0;
}

String Ide::IdeGetLine(int i) const
{
	if(i >= 0 && i < editor.GetLineCount())
		return editor.GetUtf8Line(i);
	else
		return Null;
}

void Ide::IdeSetBar()
{
	SetBar();
}

void Ide::SetupBars()
{
	ClearFrames();
	int r = HorzLayoutZoom(100);
	int l = HorzLayoutZoom(350);
	menubar.Transparent();
	if(toolbar_in_row) {
		toolbar.SetFrame(NullFrame());
		int tcy = max(mainconfiglist.GetStdSize().cy + VertLayoutZoom(2), toolbar.GetStdHeight());
		bararea.Add(menubar.LeftPos(0, l).VCenterPos(menubar.GetStdHeight()));
		bararea.Add(toolbar.HSizePos(l, r).VCenterPos(tcy));
		bararea.Add(display.RightPos(4, r).VSizePos(2, 3));
		bararea.Height(max(menubar.GetStdHeight(), tcy));
		AddFrame(bararea);
		toolbar.Transparent();
	}
	else {
		bararea.Add(menubar.LeftPos(0, l).VCenterPos(menubar.GetStdHeight()));
		bararea.Add(display.RightPos(4, r).VSizePos(2, 3));
		bararea.Height(menubar.GetStdHeight());
		AddFrame(bararea);
		AddFrame(TopSeparatorFrame());
		AddFrame(toolbar);
		toolbar.NoTransparent();
	}
	AddFrame(statusbar);
	SetBar();
}

void SetupError(ArrayCtrl& error, const char *s)
{
	error.AddColumn("File").SetDisplay(Single<Ide::FoundFileDisplay>());
	error.AddColumn("Line").SetDisplay(Single<Ide::TopAlignedDisplay>());
	error.AddColumn(s);
	error.AddIndex("INFO");
	error.ColumnWidths("184 44 298");
	error.NoWantFocus();
}

void Ide::Layout()
{
	display.Show(menubar.GetSize().cx + display.GetSize().cx < GetSize().cx);
}

static void sHighlightLine(const String& path, Vector<LineEdit::Highlight>& hln, const WString& ln)
{
	One<EditorSyntax> es = EditorSyntax::Create(EditorSyntax::GetSyntaxForFilename(GetFileName(path)));
	es->IgnoreErrors();
	HighlightOutput hl(hln);
	es->Highlight(ln.Begin(), ln.End(), hl, NULL, 0, 0);
}

Ide::Ide()
{
	DiffDlg::WhenHighlight = callback(sHighlightLine);

	editor.theide = this;
	editor.WhenSel = THISBACK(SetToolBar);
	
	editormode = false;
	
	start_time = GetSysTime();
	stat_build_time = 0;
	build_start_time = Null;
	hydra1_threads = CPU_Cores();
	
	gdbSelector = 1;

	chstyle = 0;

	Sizeable().Zoomable();

	display.SetFrame(ThinInsetFrame());
	display.SetAlign(ALIGN_CENTER);

	filelist.Columns(2);
	package.Columns(2);

	filetabs = AlignedFrame::TOP;
	auto_enclose = false;
	mark_lines = true;
	
	persistent_find_replace = false;

	idestate = EDITING;
	debuglock = 0;

	menubar.WhenHelp = statusbar;
	menubar.AreaLook(1);
	toolbar.WhenHelp = statusbar;
	toolbar.AreaLook(1);
	toolbar_in_row = false;
	SetupBars();
	SetBar();
	WhenClose = THISBACK(Exit);

	editorsplit.Vert(editor, editor2);
	editorsplit.Zoom(0);
	SyncEditorSplit();

	right_split.Horz(editpane, right);
	right_split.Zoom(0);
	
	SetupError(error, "Message");
	error.AddIndex("NOTES");
	error.ColumnWidths("207 41 834");

	SetupError(ffound, "Source");
	ffound.ColumnWidths("207 41 834");
	ffound.ColumnAt(0).SetDisplay(Single<FoundFileDisplay>());
	ffound.ColumnAt(2).SetDisplay(Single<FoundDisplay>());
	ffound.WhenBar = THISBACK(FFoundMenu);

	error.WhenSel = THISBACK(SelError);
	error.WhenLeftClick = THISBACK(ShowError);
	ffound.WhenSel = ffound.WhenLeftClick = THISBACK(ShowFound);
	console.WhenLine = THISBACK1(ConsoleLine, false);
	console.WhenRunEnd = THISBACK(ConsoleRunEnd);
	
	addnotes = false;
	removing_notes = false;

	editor_bottom.Vert(right_split, bottom);
	console.WhenBar = THISBACK(ConsoleMenu);
	editor_bottom.SetPos(8000);
	bottom.SetFrame(btabs);
	bottom.Add(console.SizePos().SetFrame(NullFrame()));
	bottom.Add(error.SizePos().SetFrame(NullFrame()));
	bottom.Add(ffound.SizePos().SetFrame(NullFrame()));
	bottom.Add(calc.SizePos().SetFrame(NullFrame()));
	btabs <<= THISBACK(SyncBottom);
	BTabs();

	pfsplit.SetPos(2000);
	pfsplit.Vert(package, filelist);
	wesplit.Horz(pfsplit, editor_bottom);
	wesplit.SetPos(2000);
	Add(wesplit);

	editor.topsbbutton.ScrollStyle().NoWantFocus().Show();
	editor.topsbbutton1.ScrollStyle().NoWantFocus().Show();
	tabs <<= THISBACK(TabFile);
//	tabs.WhenCloseRest = THISBACK1(CloseRest, &tabs);
	editor2.SetFrame(NullFrame());
	editor2.topsbbutton.ScrollStyle().NoWantFocus().Show();
	editor2.topsbbutton1.ScrollStyle().NoWantFocus().Show();
	editor2.WhenLeftDown = THISBACK(SwapEditors);
	editor.WhenAction = THISBACK(AddHistory);
	editor.WhenBar = THISBACK(EditorMenu);
	editor.WhenFontScroll = THISBACK(EditorFontScroll);
	editor.WhenOpenFindReplace = THISBACK(AddHistory);
	editor.WhenPaste = THISBACK(IdePaste);

#ifdef CPU_64
	editor.MaxLength(700000000);
	editor2.MaxLength(700000000);
#endif

	macro_api = MacroEditor();

	mainconfiglist.WhenClick = THISBACK(MainConfig);
	mainconfiglist <<= THISBACK(OnMainConfigList);
	mainconfiglist.NoDropFocus();
	mainconfiglist.NoWantFocus();

	buildmode.WhenClick = THISBACK(SetupOutputMode);
	buildmode.NoWantFocus();
	buildmode.Tip("Output mode");
	buildmode.AddButton().Tip("Build method").Left() <<= THISBACK(DropMethodList);
	buildmode.AddButton().Tip("Build mode") <<= THISBACK(DropModeList);
	methodlist.Normal();
	methodlist.WhenSelect = THISBACK(SelectMethod);
	modelist.Normal();
	modelist.WhenSelect = THISBACK(SelectMode);
	modelist.Add("Debug");
	modelist.Add("Optimal");
	modelist.Add("Speed");
	modelist.Add("Size");

	tabi = 0;
	blocktabs = false;

	package.WhenBar = THISBACK(PackageMenu);
	editor.WhenState = THISBACK(MakeTitle);

	package.NoWantFocus();
	filelist.NoWantFocus();
	filelist.WhenLeftDouble = THISBACK(Group);

	Icon(IdeImg::Package(), IdeImg::Package());

	tfont = editorfont = font2 = Courier(13);
	veditorfont = consolefont = font1 = Courier(11);
	editortabsize = 4;
	indent_amount = 4;
	indent_spaces = false;
	show_status_bar = false;
	show_tabs = false;
	tabs_icons = false;
	tabs_crosses = AlignedFrame::RIGHT;
	tabs_grouping = true;
	tabs_stacking = false;
	tabs_serialize = true;
	no_parenthesis_indent = false;
#ifdef PLATFORM_POSIX
	line_endings = DETECT_LF;
#else
	line_endings = DETECT_CRLF;
#endif
	/*
		astyle code formatter control vars
		added 2008.01.27 by Massimo Del Fedele
	*/
	astyle_BracketIndent = false;
	astyle_NamespaceIndent = true;
	astyle_BlockIndent = false;
	astyle_CaseIndent = true;
	astyle_ClassIndent = true;
	astyle_LabelIndent = true;
	astyle_SwitchIndent = true;
	astyle_PreprocessorIndent = false;
	astyle_MinInStatementIndentLength = 2;
	astyle_MaxInStatementIndentLength = 20;
	astyle_BreakClosingHeaderBracketsMode = true;
	astyle_BreakElseIfsMode = true;
	astyle_BreakOneLineBlocksMode = true;
	astyle_SingleStatementsMode = true;
	astyle_BreakBlocksMode = true;
	astyle_BreakClosingHeaderBlocksMode = true;
	astyle_BracketFormatMode = astyle::BREAK_MODE;
	astyle_ParensPaddingMode = astyle::PAD_BOTH;
	astyle_ParensUnPaddingMode = true;
	astyle_OperatorPaddingMode = true;
	astyle_EmptyLineFill = false;
	astyle_TabSpaceConversionMode = false;
	astyle_TestBox = "#include <stdio.h>\n#ifndef __abcd_h\n#include <abcd.h>\n#endif\n\nvoid test(int a, int b)\n{\n  /* this is a switch */\n  switch(a)\n\n  {\n    case 1:\n      b = 2;\n      break;\n    case 2:\n      b = 4;\n      break;\n    default:\n    break;\n  }\n\n  /* this are more statements on one line */\n  a = 2*a;b=-5;a=2*(b+2)*(a+3)/4;\n\n  /* single line blocks */\n  {int z;z = 2*a+b;}\n\n  /* loop */\n  for(int i = 0;i< 10;i++) { a = b+2*i;}\n\n}\n";
	
	androidSDKPath = "";
	
	console.WhenSelect = THISBACK(FindError);
	console.SetSlots(hydra1_threads);

	editor.WhenSelection = THISBACK(DoDisplay);
	stoponerrors = true;
	hilite_scope = 1;
	hilite_bracket = 1;
	hilite_ifdef = 1;
	barline = true;
	qtfsel = true;
	hilite_if_endif = false;
	thousands_separator = true;
	hline = true;
	wrap_console_text = true;
	mute_sounds = false;
	line_numbers = false;

	use_target = true;

	runmode = RUN_WINDOW;
	runexternal = false;
	consolemode = 0;
	console_utf8 = false;

	browser_closeesc = true;

	bookmark_pos = true;

	header_guards = true;
	insert_include = 1;

	SetTimeCallback(-20, THISBACK(Periodic), TIMEID_PERIODIC);

	editor.WhenBreakpoint = THISBACK(OnBreakpoint);

	ConstructFindInFiles();

	ff.style <<= STYLE_NO_REPLACE;

	default_charset = CHARSET_UTF8;

	HideBottom();
	TheIde(this);

	targetmode = 0;

	doc.WhenTopic = THISBACK(OpenATopic);

	editor.NoCutLine();

	bordercolumn = 96;
	bordercolor = SColorFace();

	state_icon = -1;

	histi = 0;

	doc_serial = -1;

	showtime = true;
	
	editor.WhenTip = THISBACK(EditorTip);
	editor.WhenCtrlClick = THISBACK(CtrlClick);
	
	find_pick_sel = true;
	find_pick_text = false;
	
	deactivate_save = true;
	
	output_per_assembly = true;
	
	issaving = 0;
	isscanning = 0;
	
	linking = false;
	
	error_count = 0;
	warning_count = 0;
	
	editor.WhenUpdate = THISBACK(TriggerAssistSync);

	editfile_isfolder = editfile_svn = false;
}

Ide::~Ide()
{
	TheIde(NULL);
}

void DelTemps() {
	FindFile ff(ConfigFile("*.tmp"));
	while(ff) {
		DeleteFile(ConfigFile(ff.GetName()));
		ff.Next();
	}
}

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )
#endif

void Ide::Paint(Draw&) {}

extern int MemoryProbeFlags;

void Uninstall();

bool SilentMode;

#ifdef PLATFORM_WIN32
#include <Wincon.h>

void Puts(const char *s)
{
	dword dummy;
	if(!SilentMode)
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), s, (int)strlen(s), &dummy, NULL);
}
#endif


#ifdef PLATFORM_POSIX
void Puts(const char *s)
{
	if(!SilentMode)
		puts(s);
}
#endif

bool splash_screen;

int CommaSpace(int c)
{
	return c == ',' ? ' ' : c;
}

void ReduceCache()
{
	String cfgdir = ConfigFile("cfg");
	FindFile ff(AppendFileName(cfgdir, "*.*"));
	while(ff) {
		if(ff.IsFile()) {
			String fn = ff.GetName();
			String ext = GetFileExt(fn);
			if(ext != ".aux" && ext != ".cfg")
				if((Date)Time(ff.GetLastAccessTime()) < GetSysDate() - 14)
					DeleteFile(AppendFileName(cfgdir, fn));
		}
		ff.Next();
	}
}

bool IsAssembly(const String& s)
{
	Vector<String> varlist;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile())
			if(GetFileTitle(ff.GetName()) == s)
				return true;
	Vector<String> l = Split(s, ',');
	for(int i = 0; i < l.GetCount(); i++)
		if(FindFile(NormalizePath(l[i])).IsFolder())
			return true;
	return false;
}

#ifdef flagMAIN
GUI_APP_MAIN
#else
void AppMain___()
#endif
{
#ifdef flagTEST_HIDPI
	Font fnt = GetStdFont();
	SetStdFont(fnt.Height(2 * fnt.GetHeight()));
	GUI_HiDPI_Write(1);
	DDUMP(GUI_HiDPI());
#endif
	SetLanguage(LNG_ENGLISH);
	SetDefaultCharset(CHARSET_UTF8);

	const Vector<String>& arg = CommandLine();

	bool firstinstall = false;

	SetVppLogSizeLimit(200000000);
#ifdef _DEBUG
	MemoryLimitKb(1000000);
#endif

#ifdef PLATFORM_POSIX
	LoadUpdaterCfg();

	String home = Environment().Get("UPP_HOME", Null);
	if(!IsNull(home))
		SetHomeDirectory(home);
	FindFile ff(ConfigFile("*.var"));
	if(!ff) {
		if(!Install())
			return;
		SaveFile(ConfigFile("version"), IDE_VERSION);
		firstinstall = true;
	}
#endif

#ifdef _DEBUG
//	Ctrl::ShowRepaint(20); _DBG_
#endif

#ifdef PLATFORM_WIN32
	if(!CheckLicense())
		return;
	firstinstall = !IsNull(LoadFile(GetExeDirFile("install.upp")));
#ifdef flagTESTINSTALL
	firstinstall = true;
#endif
	if(firstinstall) {
		if(!Install())
			return;
		SaveFile(ConfigFile("version"), IDE_VERSION);
	}
#endif

	if(!FileExists(BlitzBaseFile()))
		ResetBlitz();

	for(int i = 0; i < arg.GetCount(); i++) {
		if(arg[i] == "-uninstall") {
			Uninstall();
			return;
		}
		if(!firstinstall && arg[i] == "-install" && !Install()) return;

	#ifdef PLATFORM_WIN32
		if(arg[i] == "!") {
			String cmdline;
			for(++i; i < arg.GetCount(); i++) {
				if(!IsNull(cmdline))
					cmdline << ' ';
				cmdline << arg[i];
			}
			int n = cmdline.GetLength() + 1;
			Buffer<char> cmd(n);
			memcpy(cmd, cmdline, n);
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			AllocConsole();
			SetConsoleTitle(cmdline);
			int time = msecs();
			if(CreateProcess(NULL, cmd, &sa, &sa, TRUE,
				             NORMAL_PRIORITY_CLASS,
			                 NULL, NULL, &si, &pi)) {
				WaitForSingleObject(pi.hProcess, INFINITE);
				dword exitcode = 255;
				GetExitCodeProcess(pi.hProcess, &exitcode);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				Puts("<--- Finished in " + GetPrintTime(time) + ", exitcode: " + AsString(exitcode) + " --->");
			}
			else
				Puts("Unable to launch " + cmdline);
			char h[1];
			dword dummy;
			ReadFile(GetStdHandle(STD_INPUT_HANDLE), h, 1, &dummy, NULL);
			return;
		}
	#endif
	}

#ifndef _DEBUG
	try {
#endif
		void RegisterLayDes(); RegisterLayDes();
		void RegisterIconDes(); RegisterIconDes();

		splash_screen = true;

		Ide ide;
		ide.Maximize();
		bool clset = false;
		if(arg.GetCount() && findarg(arg[0], "?", "--help", "-?", "/?") >= 0) {
			Cout() << "Usage: theide assembly package\n"
			          "       theide assembly package build_method [-[a][b][e][r][s][S][v][1][2][m][d][M][l][x][X][Hn]] [+FLAG[,FLAG]...] [out]\n"
			          "       theide -f [file..]\n"
			          "       theide [file..] // autodetection mode\n"
			;
			return;
		}
		if(arg.GetCount() >= 2 && IsAlpha(arg[0][0]) && IsAlpha(arg[1][0]) && IsAssembly(arg[0]) && arg[0] != "-f") {
			bool build = arg.GetCount() >= 3 && IsAlpha(arg[2][0]);
		#ifdef PLATFORM_WIN32
			if(build) {
				HMODULE hDLL = LoadLibrary ("kernel32");
				bool attach = false;
				if(hDLL) {
					typedef BOOL (WINAPI *AttachConsoleType)(DWORD dwProcessId);
					AttachConsoleType AttachConsole;
					AttachConsole = (AttachConsoleType) GetProcAddress(hDLL, "AttachConsole");
					if(AttachConsole)
						attach = AttachConsole((DWORD)-1);
				}
				if(!attach)
					AllocConsole();
			}
		#endif
			for(int i = 3; i < arg.GetCount(); i++)
				if(arg[i][0] == '-') {
					String x = arg[i];
					for(int i = 1; i < x.GetCount(); i++)
						if(x[i] == 'l')
							SilentMode = true;
				}
			if(!LoadVars(arg[0])) {
				if(build)
					Puts("TheIDE: Invalid assembly\n");
				else
					Exclamation("Invalid assembly!");
				SetExitCode(2);
				return;
			}
			if(!FileExists(SourcePath(arg[1], GetFileTitle(arg[1]) + ".upp"))) {
				if(build)
					Puts("TheIDE: Package does not exist\n");
				else
					Exclamation("Package does not exist!");
				SetExitCode(2);
				return;
			}
			if(build) {
				ide.SetMain(arg[1]);
				clset = true;
				bool stoponerror = false;
				const Workspace& wspc = ide.IdeWorkspace();
				if(!wspc.GetCount())
					return;
				const Array<Package::Config>& f = wspc.GetPackage(0).config;
				if(f.GetCount())
					ide.mainconfigparam = f[0].param;
				String m = arg[2];
				if(!FileExists(ConfigFile((String)m + ".bm"))) {
					SilentMode = false;
					Puts("TheIDE: Invalid build method\n");
					SetExitCode(3);
					return;
				}
				ide.method <<= m;
				ide.debug.def.blitz = ide.release.def.blitz = 0;
				ide.debug.def.debug = 2;
				ide.release.def.debug = 0;
				ide.debug.package.Clear();
				ide.release.package.Clear();
				ide.debug.linkmode = ide.release.linkmode = 0;
				ide.release.createmap = ide.debug.createmap = false;
				ide.targetmode = 0;
				ide.console.console = true;
				bool clean = false;
				bool makefile = false;
				bool deletedir = false;
				int  exporting = 0;
				String mkf;
				for(int i = 3; i < arg.GetCount(); i++)
					if(arg[i][0] == '>')
						ide.mainconfigparam = Filter(~arg[i] + 1, CommaSpace);
					else
					if(arg[i][0] == '-') {
						String x = arg[i];
						for(int i = 1; i < x.GetCount(); i++)
							switch(x[i]) {
							case 'a':
								clean = true;
								break;
							case 'r':
								ide.targetmode = 1;
								break;
							case '1':
								ide.targetmode = 2;
								break;
							case '2':
								ide.targetmode = 3;
								break;
							case 'm':
								ide.release.createmap = ide.debug.createmap = true;
								break;
							case 'b':
								ide.release.def.blitz = ide.debug.def.blitz = 1;
								break;
							case 's':
								ide.debug.linkmode = ide.release.linkmode = 1;
								break;
							case 'd':
								ide.debug.def.debug = 0;
								break;
							case 'S':
								ide.debug.linkmode = ide.release.linkmode = 2;
								break;
							case 'e':
								stoponerror = true;
								break;
							case 'M':
								makefile = true;
								break;
							case 'v':
								ide.console.verbosebuild = true;
								break;
							case 'l':
								break;
							case 'x':
								exporting = 1;
								break;
							case 'X':
								exporting = 2;
								break;
							case 'k':
								deletedir = false;
								break;
							default:
								SilentMode = false;
								Puts("Invalid build option(s)");
								SetExitCode(3);
								return;
							}
					}
					else {
						ide.debug.target_override = ide.release.target_override = true;
						ide.debug.target = ide.release.target = mkf = arg[i];
					}
				if(clean)
					ide.Clean();
				if(exporting) {
					mkf = GetFullPath(mkf);
					Cout() << mkf << '\n';
					RealizeDirectory(mkf);
					if(makefile)
						ide.ExportMakefile(mkf);
					else
						ide.ExportProject(mkf, exporting == 2, false, deletedir);
				}
				else
				if(makefile) {
					ide.SaveMakeFile(IsNull(mkf) ? "Makefile" : mkf, false);
					SetExitCode(0);
				}
				else
				if(ide.Build())
					SetExitCode(0);
				else {
					if(stoponerror)
						PromptOK("TheIDE: Error while building " + DeQtf(arg[1]) + "!");
					SetExitCode(1);
				}
				return;
			}
		}
		
		ide.LoadConfig();

		if(arg.GetCount() == 1) {
			if(arg[0].EndsWith(".upp")) {
				Vector<String> names = Split(arg[0], DIR_SEP);
				int last = names.GetCount() - 1;
				for(int i = last; i >= 0; --i) {
					if (IsAssembly(names[i])) {
						String package = names[last];
						package.Remove(package.GetCount() - 4, 4);
						LoadVars(names[i]);
						ide.SetMain(package);
						clset = true;
						break;
					}
				}
			}
		} else {
			if(arg.GetCount() == 2 && IsAssembly(arg[0])) {
				LoadVars(arg[0]);
				ide.SetMain(arg[1]);
				clset=true;
			}
		}
		ide.LoadAbbr();

		ide.SyncCh();

		DelTemps();
		
		if(arg.GetCount() && !clset) {
			Vector<String> dir = Split(LoadFile(GetHomeDirFile("usc.path")), ';');
			for(int i = 0; i < dir.GetCount(); i++)
				ide.UscProcessDirDeep(dir[i]);
			for(int i = 0; i < arg.GetCount(); i++)
				if(arg[i] != "-f") {
					ide.EditFile(NormalizePath(arg[i]));
					ide.FileSelected();
				}
			clset = true;
			ide.EditorMode();
		}

		if(splash_screen && !ide.IsEditorMode()) {
			ShowSplash();
			Ctrl::ProcessEvents();
		}

	#ifdef PLATFORM_POSIX
		int p=UpdaterCfg().period;
		if(!IsNull(p)) {
			int next=GetUtcTime()-UpdaterCfg().last+abs(p)*60;
			if(p <= 0 || next <= 0)
				ide.PostCallback(callback1(&ide,&Ide::CheckUpdates,false),0);
			if(p != 0)
				ide.SetTimeCallback(max(0, next),callback1(&ide,&Ide::SetUpdateTimer,abs(p)));
		}
	#endif

		ide.editor_bottom.Zoom(0);
		ide.right_split.Zoom(0);
		if(FileExists(ConfigFile("developide"))) {
	#ifdef PLATFORM_WIN32
			InstallCrashDump();
	#endif
			Ini::user_log = true;
		}
		
		String ppdefs = ConfigFile("global.defs");
	#ifndef _DEBUG
		if(!FileExists(ppdefs))
	#endif
			SaveFile(ppdefs, GetStdDefs());
		
		SetPPDefs(LoadFile(ppdefs));
		
		ide.LoadLastMain();
		if(clset || ide.OpenMainPackage()) {
			ide.SaveLastMain();
			ide.isscanning++;
			ide.MakeTitle();
			if(!ide.IsEditorMode())
				SyncRefs();
			ide.FileSelected();
			ide.isscanning--;
			ide.MakeTitle();
			if(!IdeExit)
				ide.Run();
			ide.SaveConfigOnTime();
			ide.SaveLastMain();
		}
	#ifdef PLATFORM_POSIX
		StoreAsXMLFile(UpdaterCfg(),"SourceUpdater",ConfigFile("updates.xml"));
	#endif
		SaveCodeBase();
		DelTemps();
		ReduceCache();
#ifndef _DEBUG
	}
	catch(const CParser::Error& e) {
		Exclamation("Parser error " + e);
		LOG("!!!!! Parser error " + e);
	}
	catch(const Exc& e) {
		Exclamation("Exception " + e);
		LOG("!!!!! Exception " << e);
	}
#ifdef PLATFORM_POSIX
	catch(...) {
		Exclamation("Unknown exception !");
		LOG("!!!!! Unknown exception");
	}
#endif
#endif
}
