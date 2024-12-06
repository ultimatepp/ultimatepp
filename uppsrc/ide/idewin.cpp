#include "ide.h"

#define IMAGECLASS IdeImg
#define IMAGEFILE  <ide/ide.iml>
#include <Draw/iml_source.h>

void Ide::Skin()
{
	SetToolBar();
	ScanWorkspace();
	if(!hl_custom) {
		editor.DefaultHlStyles();
		UpdateFormat();
	}
}

void Ide::ToggleVerboseBuild() {
	console.verbosebuild = !console.verbosebuild;
	
	SetToolBar();
}

void Ide::ToggleStopOnErrors() {
	stoponerrors = !stoponerrors;
}

void Ide::SwapPackagesFiles()
{
	weframe.Show(weframe.IsShown() ? false : true);
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

bool Ide::IsVerbose() const
{
	return console.verbosebuild;
}

void Ide::PutConsole(const char *s)
{
	console << s << "\n";
}

void Ide::PutVerbose(const char *s)
{
	LOG("VERBOSE: " << s);
	GuiLock __;
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

void Ide::IdeConsoleOnFinish(Event<>  cb)
{
	console.OnFinish(cb);
}

void Ide::IdeProcessEvents()
{
	Ctrl::ProcessEvents();
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

int Ide::IdeGetHydraThreads()
{
	return hydra1_threads;
}

String Ide::IdeGetCurrentBuildMethod()
{
	return method;
}

String Ide::IdeGetCurrentMainPackage()
{
	return main;
}

void Ide::IdePutErrorLine(const String& line)
{
	if(console.verbosebuild) {
		GuiLock __;
		SetBottom(Ide::BERRORS);
		ConsoleRunEnd();
		ConsoleLine(line, true);
	}
}

void Ide::IdeEndDebug()
{
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
		debuglock = 1;
		MakeTitle();
		SetBar();
		Sync();
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
	
	int r = HorzLayoutZoom(170);
#ifdef PLATFORM_COCOA
	AddFrame(toolbar);
	display.RightPos(4, r).VSizePos(2, 3);
#else
	int l = HorzLayoutZoom(350);
	
	menubar.Transparent();
	if(toolbar_in_row) {
		toolbar.SetFrame(NullFrame());
		int tcy = max(mainconfiglist.GetStdSize().cy + DPI(2), toolbar.GetStdHeight());
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
#endif
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
	display.Show(!designer && (menubar.GetSize().cx + display.GetSize().cx < GetSize().cx));
}

void HighlightLine(const String& path, Vector<LineEdit::Highlight>& hln, const WString& ln)
{
	One<EditorSyntax> es = EditorSyntax::Create(EditorSyntax::GetSyntaxForFilename(GetFileName(path)));
	es->IgnoreErrors();
	HighlightOutput hl(hln);
	es->Highlight(ln.Begin(), ln.End(), hl, NULL, 0, 0);
}

void CursorInfoCtrl::Paint(Draw& w)
{
	Size sz = GetSize();
	Size tsz = GetTextSize(text, StdFont());
	int x = sz.cx - tsz.cx;
	int y = (sz.cy - tsz.cy) / 2;
	w.DrawText(x, y, text, StdFont(), SColorText());
}

CursorInfoCtrl::CursorInfoCtrl()
{
	Transparent();
}

Ide::Ide()
{
	DiffDlg::WhenHighlight = callback(HighlightLine);

	editor.theide = this;
	editor.WhenSel << [=] {
		delayed_toolbar.KillSet(150, [=] { SetToolBar(); });
	};
	
	editormode = false;
	
	start_time = GetSysTime();
	stat_build_time = 0;
	build_start_time = Null;
	hydra1_threads = CPU_Cores();
	
	chstyle = 0;

	Sizeable().Zoomable();

	filelist.Columns(2);
	package.Columns(2);

	filetabs = AlignedFrame::TOP;
	auto_enclose = false;
	mark_lines = true;
	
	persistent_find_replace = false;

	idestate = EDITING;
	debuglock = 0;

	menubar.WhenHelp = ~statusbar;
	menubar.AreaLook(1);
	toolbar.WhenHelp = ~statusbar;
	toolbar.AreaLook(1);
	toolbar_in_row = false;
	WhenClose = THISBACK(Exit);

	editor_p.Add(editor.SizePos());
	editorsplit.Vert(editor_p, editor2);
	editorsplit.Zoom(0);
	SyncEditorSplit();
	
	editpane.AddFrame(editor.navigatorframe);

	right_split.Horz(editpane, right);
	right_split.Zoom(0);
	
	SetupError(error, "Message");
	error.AddIndex("NOTES");
	error.ColumnWidths("207 41 834");
	error.WhenBar = THISBACK(ErrorMenu);

	error.WhenSel = THISBACK(SelError);
	error.WhenLeftClick = THISBACK(ShowError);
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
	bottom.Add(calc.SizePos().SetFrame(NullFrame()));
	btabs <<= THISBACK(SyncBottom);
	BTabs();
	
	editor.WhenSelectionChanged << [=] {
		editor2.Illuminate(editor.GetIlluminated());
	};

	pfsplit.SetPos(2000);
	pfsplit.Vert(package, filelist);
	wepane.Add(editor_bottom.SizePos());
	wepane.AddFrame(weframe.Left(pfsplit, HorzLayoutZoom(280)));
	Add(wepane.SizePos());

	editor.topsbbutton.ScrollStyle().NoWantFocus().Show();
	editor.topsbbutton1.ScrollStyle().NoWantFocus().Show();
	tabs <<= THISBACK(TabFile);
	tabs.WhenClose = [=](Value file) { // remove file from Ctrl+Tab logic
		int q = FindIndex(tablru, ~file);
		if(q >= 0)
			tablru.Remove(q);
	};
//	tabs.WhenCloseRest = THISBACK1(CloseRest, &tabs);
//	editor2.SetFrame(NullFrame());
//	editor2.theide = this;
	editor2.topsbbutton.ScrollStyle().NoWantFocus().Show();
	editor2.topsbbutton1.ScrollStyle().NoWantFocus().Show();
	editor2.WhenLeftDown = THISBACK(SwapEditors);
	editor.WhenAction = THISBACK(EditorEdit);
	editor.WhenBar = THISBACK(EditorMenu);
	editor.WhenFontScroll = THISBACK(EditorFontScroll);
	editor.WhenOpenFindReplace = THISBACK(AddHistory);
	editor.WhenPaste = THISBACK(IdePaste);
	
	editor.WhenFindAll << THISFN(FindFileAll);

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
	modelist.Add("Release");

	tabi = 0;
	blocktabs = false;

	package.WhenBar = THISBACK(PackageMenu);
	editor.WhenState = THISBACK(MakeTitle);

	package.NoWantFocus();
	filelist.NoWantFocus();
	filelist.WhenLeftDouble = THISBACK(Group);

	Icon(IdeImg::Package(), IdeImg::Package());

	tfont = editorfont = font2 = veditorfont = CourierZ(12);
	consolefont = font1 = CourierZ(10);
	editortabsize = 4;
	indent_amount = 4;
	indent_spaces = false;
	show_status_bar = false;
	show_tabs = false;
	show_spaces = false;
	warnwhitespace = true;
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
	spellcheck_comments = LNG_ENGLISH;
	wordwrap_comments = true;
#ifdef PLATFORM_COCOA
	setmain_newide = true;
#else
	setmain_newide = false;
#endif
	
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
	hline = vline = true;
	wrap_console_text = true;
	mute_sounds = false;
	line_numbers = false;

	use_target = true;

#ifdef PLATFORM_COCOA
	runmode = RUN_CONSOLE;
#else
	runmode = RUN_WINDOW;
#endif
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

	targetmode = 0;

	doc.WhenTopic = THISBACK(OpenATopic);

	editor.NoCutLine();

	bordercolumn = 96;

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
	
	editfile_isfolder = false;
	editfile_repo = NOT_REPO_DIR;
	
	editfile_line_endings = Null;

	HideBottom();
	SetupBars();
	SetBar();
	
	libclang_options = "-Wno-logical-op-parentheses -Wno-pragma-pack";
	libclang_coptions = "-Wno-logical-op-parentheses -Wno-pragma-pack";

	editor.search.Add(indeximage.RightPos(DPI(1), DPI(16)).VSizePos());
	editor.search.Add(indeximage2.RightPos(DPI(1), DPI(16)).VSizePos());

#ifdef PLATFORM_COCOA
	WhenDockMenu = [=](Bar& bar) {
		bar.Add("Open main package..", [=] {
			Host h;
			CreateHost(h, false, false);
			h.Launch(GetExeFilePath() + " --nosplash");
		});
	};
#endif
}

Ide::~Ide()
{
}

void Ide::DeleteWindows()
{
	for(Ptr<TopWindow> w : window)
		if(w)
			delete w;
}

void Ide::NewWindow(TopWindow *win)
{
	window.RemoveIf([&](int i) { return !window[i]; });
	window.Add(win);
}

String LibClangCommandLine()
{
	GuiLock __;
	IdeContext *q = TheIdeContext();
	return q ? ((Ide *)q)->libclang_options : String();
}

String LibClangCommandLineC()
{
	GuiLock __;
	IdeContext *q = TheIdeContext();
	return q ? ((Ide *)q)->libclang_coptions : String();
}

void IdeShowConsole()
{
	GuiLock __;
	IdeContext *q = TheIdeContext();
	if(q)
		((Ide *)q)->ShowConsole();
}

void Ide::Paint(Draw&) {}
