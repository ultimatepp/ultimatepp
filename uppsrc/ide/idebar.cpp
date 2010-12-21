#include "ide.h"

#define KEYGROUPNAME "Ide"
#define KEYNAMESPACE IdeKeys
#define KEYFILE      <ide/ide.key>
#include             <CtrlLib/key_source.h>

using namespace IdeKeys;

void Ide::PackageMenu(Bar& menu) {
	Project(menu);
}

void Ide::FileBookmark(Bar& menu) {
	int i;
	for(i = 0; i < 10; i++) {
		const Bookmark& b = bookmark[i];
		String txt = Format("Goto bookmark &%d", i);
		if(!b.file.IsEmpty())
			txt << " (" << bookmark[i].file << ')';
		menu.Add(!b.file.IsEmpty(), txt, THISBACK1(BookKey, K_CTRL_0 + i))
			.Key(K_CTRL_0 + i);
	}
	menu.MenuBreak();
	for(i = 0; i < 10; i++)
		menu.Add("Set", THISBACK1(BookKey, K_SHIFT_CTRL_0 + i))
		    .Key(K_SHIFT_CTRL_0 + i);
}

void Ide::File(Bar& menu) {
	menu.Add(AK_SETMAIN, THISBACK(NewMainPackage))
		.Help("Select global configuration (var), select / add main project package");

	menu.AddMenu(AK_EDITFILE, CtrlImg::open(), THISBACK(EditAnyFile))
		.Help("Select any file in file selector and open it in editor");
	menu.AddMenu(!IsNull(GetOpposite()), AK_OPPOSITE, IdeImg::opposite(), THISBACK(GoOpposite))
		.Help("Switch between source and header file");
	menu.AddMenu(AK_SAVEFILE, CtrlImg::save(), THISBACK(DoSaveFile))
		.Help("Save current file");
	if(!designer)
		menu.AddMenu(CanToggleReadOnly(), AK_READONLY, IdeImg::read_only(), THISBACK(ToggleReadOnly))
			.Check(editor.IsReadOnly())
			.Help("Set / clear read-only flag for current file");

	menu.AddMenu(!designer, AK_PRINT, CtrlImg::print(), THISBACK(Print));

//	menu.Add("Export project", THISBACK(ExportProject))
//		.Help("Copy all project files into given directory");

	if(menu.IsMenuBar())
	{
		menu.Separator();
		menu.Add(AK_CLOSETAB, THISBACK(ClearTab))
		    .Help("Close the current file tab");
		menu.Add(AK_CLOSETABS, THISBACK(ClearTabs))
		    .Help("Close all file tabs");
		if(!designer) {
			menu.Add("Go back", THISBACK(HistoryBk))
			    .Key(K_ALT_LEFT);
			menu.Add("Go forward", THISBACK(HistoryFw))
			    .Key(K_ALT_RIGHT);
			menu.Add("Bookmarks", THISBACK(FileBookmark))
				.Help("Set one of available bookmarks (1..9, 0) on current file");
			menu.MenuSeparator();
		}
		menu.Add("Show/hide bottom pane", THISBACK(SwapBottom))
			.Check(IsBottomShown())
			.Key(K_ESCAPE)
			.Help("Show / hide bottom pane (with console, calc and browser tabs)");
	}

	menu.Add(AK_PACKAGESFILES, THISBACK(SwapPackagesFiles));

	menu.MenuSeparator();

	bool split = editorsplit.GetZoom() < 0;
	menu.Add(AK_SPLIT, THISBACK1(KeySplit, false))
	    .Check(split && editorsplit.IsVert());
	menu.Add(AK_VSPLIT, THISBACK1(KeySplit, true))
	    .Check(split && editorsplit.IsHorz());
	menu.Add(split, AK_SWAP, THISBACK(SwapEditors));

	menu.MenuSeparator();

	menu.Add(AK_STATISTICS, THISBACK(Statistics))
		.Help("Display various statistics");

	menu.Add("Elapsed times..", THISBACK(Times));

	menu.Add(AK_EXIT, THISBACK(Exit));
}

void Ide::EditSpecial(Bar& menu)
{
	menu.Add(AK_SPACESTOTABS, THISBACK(EditMakeTabs))
		.Help("Convert leading blanks on each line to tabs");
	menu.Add(AK_TABSTOSPACES, THISBACK(EditMakeSpaces))
		.Help("Convert all tabs to spaces");
	menu.Add(AK_TRANSLATESTRING, THISBACK(TranslateString))
		.Help("Mark the current selection as translated string");
	menu.Add(AK_SWAPCHARS, THISBACK(SwapChars))
	    .Help("Transpose characters");
	menu.Add(AK_COPYWORD, THISBACK(CopyWord))
	    .Help("Copy the current identifier to the clipboard");
	menu.Add(AK_FORMATCODE, THISBACK(FormatCode))
	    .Help("Reformat code in editor");
	menu.Add(AK_TOUPPER, THISBACK(TextToUpper))
	    .Help("Convert letters in selection to uppercase"); 
	menu.Add(AK_TOLOWER, THISBACK(TextToLower))
	    .Help("Convert letters in selection to lowercase"); 
	menu.Add(AK_TOASCII, THISBACK(TextToAscii))
		.Help("Covert text to 7-bit ASCII removing all accents and special symbols");
	menu.Add(AK_INITCAPS, THISBACK(TextInitCaps))
	    .Help("Capitalize the first character of words in selection"); 
	menu.Add(AK_SWAPCASE, THISBACK(SwapCase))
	    .Help("Swap the case of letters in selection"); 
}

void Ide::SearchMenu(Bar& menu)
{
	if(!designer) {
		menu.Add(AK_FIND, THISBACK(EditFind))
			.Help("Search for text or text pattern");
		menu.Add(AK_REPLACE, THISBACK(EditReplace))
			.Help("Search for text or text pattern, with replace option");
//		menu.Add(AK_FINDSEL, THISBACK(EditFindReplacePickText))
//			.Help("Show find / replace dialog & set active text as the 'find' text");

		menu.Add(AK_FINDNEXT, THISBACK(EditFindNext))
			.Help("Find next occurrence");
		menu.Add(AK_FINDPREV, THISBACK(EditFindPrevious))
			.Help("Find previous occurrence");

		menu.MenuSeparator();

		menu.Add(AK_FINDSTRING, THISBACK1(FindString, false))
			.Help("Find any ordinary string constant (\"\" - delimited)");
		menu.Add(AK_FINDSTRINGBACK, THISBACK1(FindString, true))
			.Help("Find any ordinary string constant (\"\" - delimited) backwards");	
		menu.MenuSeparator();
	}
	menu.Add(AK_FINDINFILES, THISBACK1(FindInFiles, false))
		.Help("Find text or text pattern in subtree of given path");
	menu.Add(AK_REPLACEINFILES, THISBACK1(FindInFiles, true))
		.Help("Find text or text pattern in subtree of given path, with replace option(s)");
	menu.Add(AK_FINDFILE, THISBACK(FindFileName))
		.Help("Locate file by filename (use *, ? when you're not sure)");
}

void Ide::Edit(Bar& menu) {
	if(designer) {
		if(FileExists(designer->GetFileName())) {
			menu.Add(AK_EDITASTEXT, THISBACK(EditAsText))
			    .Help("Edit as text file (do not use a designer)");
			menu.MenuSeparator();
		}
		if(menu.IsMenuBar())
			designer->EditMenu(menu);
	}
	else {
		if(GetFileExt(editfile) == ".t") {
			if(editastext.Find(editfile) >= 0)
				menu.Add(AK_DESIGNER, THISBACK(EditUsingDesigner))
				    .Help("Edit converted strings");
			else				
				menu.Add(AK_EDITASTEXT, THISBACK(EditAsText))
				    .Help("Edit raw strings");
			menu.MenuSeparator();
		}
		else
		if(editastext.Find(editfile) >= 0) {
			menu.Add(AK_DESIGNER, THISBACK(EditUsingDesigner))
			    .Help("Edit using the designer (not as text)");
			menu.MenuSeparator();
		}
		Bar::Item& (Bar::*add)(const char *s, const Image& m, Callback cb) = &Bar::Add;
		if(toolbar_in_row) add = &MenuBar::AddMenu;
		(menu.*add)("Undo", CtrlImg::undo(), callback(&editor, &LineEdit::Undo))
			.Key(K_CTRL_Z)
			.Help("Undo changes to text");
		(menu.*add)("Redo", CtrlImg::redo(), callback(&editor, &LineEdit::Redo))
			.Key(K_SHIFT|K_CTRL_Z)
			.Help("Redo undone changes");
		if(!toolbar_in_row || menu.IsMenuBar())
			menu.Separator();
		(menu.*add)("Cut", CtrlImg::cut(), callback(&editor, &LineEdit::Cut))
			.Key(K_CTRL_X)
			.Help("Cut selection and place it on the system clipboard");
		(menu.*add)("Copy", CtrlImg::copy(), callback(&editor, &LineEdit::Copy))
			.Key(K_CTRL_C)
			.Help("Copy current selection on the system clipboard");
		(menu.*add)("Paste", CtrlImg::paste(), THISBACK(EditPaste))
			.Key(K_CTRL_V)
			.Help("Insert text from clipboard at cursor location");

		if(!toolbar_in_row || menu.IsMenuBar())
			menu.Separator();

		(menu.*add)("Select All", CtrlImg::copy(), callback(&editor, &LineEdit::SelectAll));		
	}

	menu.MenuSeparator();
	menu.Add("Find and Replace", THISBACK(SearchMenu));

	if(!designer && menu.IsMenuBar()) {
		menu.Add("Insert", THISBACK(InsertMenu));
		menu.Add("Advanced", THISBACK(EditSpecial));
	}

	if(editor.GetLineCount() && editor.GetUtf8Line(0) == "$uvs: PENDING CONFLICT") {
		menu.MenuSeparator();
		menu.Add("Resolve pending uvs conflict", THISBACK(ResolveUvsConflict))
		.Help("Merge $uvs: pending conflicts generated by UVS series of versioning software");
	}
}

bool Ide::HasMacros()
{
	const Array<IdeMacro>& mlist = UscMacros();
	if(!mlist.IsEmpty())
		for(int i = 0; i < mlist.GetCount(); i++) {
			const IdeMacro& m = mlist[i];
			if(!IsNull(m.menu))
				return true;
		}
	return false;
}

void Ide::MacroMenu(Bar& menu)
{
	const Array<IdeMacro>& mlist = UscMacros();
	if(!mlist.IsEmpty() && menu.IsMenuBar()) {
		VectorMap< String, Vector<int> > submenu_map;
		for(int i = 0; i < mlist.GetCount(); i++) {
			const IdeMacro& m = mlist[i];
			if(!IsNull(m.menu))
				if(IsNull(m.submenu))
					submenu_map.GetAdd(Null).Add(i);
				else
					submenu_map.GetAdd(m.menu).Add(i);
		}
		if(!submenu_map.IsEmpty()) {
			Vector<int> order = GetSortOrder(submenu_map.GetKeys());
			for(int o = 0; o < order.GetCount(); o++) {
				String m = submenu_map.GetKey(order[o]);
				Vector<int>& mx = submenu_map[order[o]];
				if(!IsNull(m))
					menu.Add(m, THISBACK1(EditMacroMenu, mx));
				else
					EditMacroMenu(menu, mx);
			}
		}
	}
}

void Ide::EditMacroMenu(Bar& menu, const Vector<int>& mx)
{
	const Array<IdeMacro>& mlist = UscMacros();
	Vector<String> names;
	Vector<int> index;
	names.Reserve(mx.GetCount());
	for(int i = 0; i < mx.GetCount(); i++)
		if(mx[i] >= 0 && mx[i] < mlist.GetCount()) {
			const IdeMacro& m = mlist[mx[i]];
			names.Add(Nvl(m.submenu, m.menu));
			index.Add(mx[i]);
		}
	IndexSort(names, index);
	for(int i = 0; i < index.GetCount(); i++)
		menu.Add(names[i], THISBACK1(EditMacro, index[i]))
			.Key(mlist[index[i]].hotkey);
}

void Ide::EditMacro(int i)
{
	const Array<IdeMacro>& mlist = UscMacros();
	if(i >= 0 && i < mlist.GetCount()) {
		const IdeMacro& m = mlist[i];
		try {
			Vector<EscValue> arg;
			::Execute(UscGlobal(), &macro_api, m.code, arg, 1000000);
		}
		catch(Exc e) {
			PutConsole(e);
		}
	}
}

void Ide::Setup(Bar& menu) {
	menu.Add("Be verbose", THISBACK(ToggleVerboseBuild))
		.Check(console.verbosebuild)
		.Help("Log detailed description of build and debug");
	menu.Add("Environment..", THISBACK(SetupFormat))
		.Help("Fonts, tabs, indentation, status bar");
	menu.Add("Abbreviations..", THISBACK(Abbreviations))
		.Help("Edit abbreviation keywords and code");
	menu.Add("Keyboard shortcuts..", callback(EditKeys))
		.Help("Edit key bindings");
	menu.Add("Build methods..", THISBACK(SetupBuildMethods))
	    .Help("Setup build methods");
#ifdef PLATFORM_WIN32
	menu.Add("Automatic setup..", THISBACK(AutoSetup))
	    .Help("Automatic setup of build methods..");
#endif
}

void Ide::ProjectSvn(Bar& menu)
{
	Vector<String> w = SvnDirs();
	String p = GetFileFolder(PackagePath(actualpackage));
	if(IsSvnDir(p))
		w.Insert(0, p);
	for(int i = 0; i < w.GetCount(); i++)
		menu.Add("Synchronize " + w[i], IdeImg::svn_dir(), THISBACK1(SyncSvnDir, w[i]));
	menu.Add("Synchronize everything..", IdeImg::svn(), THISBACK(SyncSvn));
}

void Ide::Project(Bar& menu) {
	if(menu.IsToolBar() && !debugger)
	{
		mainconfiglist.Enable(idestate == EDITING);
		buildmode.Enable(idestate == EDITING);
		menu.Add(mainconfiglist, 120);
		menu.Gap(4);
		menu.Add(buildmode, 120);
		menu.Separator();
	}
	WorkspaceWork::PackageMenu(menu);
	menu.MenuSeparator();
	menu.Add(AK_ORGANIZER, IdeImg::package_organizer(), THISBACK(EditWorkspace))
		.Help("Package dependencies, compiler & linker options, output path override");
	menu.Add(AK_CUSTOM, THISBACK(CustomSteps))
		.Help("Building intermediate files using custom commands / applications");
	if(menu.IsMenuBar())
		menu.Add(AK_MAINCONFIG, IdeImg::main_package(), THISBACK(MainConfig))
			.Help("Configuring compiler, operating system, output application parameters, custom flags");
	menu.Separator();
	menu.Add(AK_SYNCT, IdeImg::Language(), THISBACK1(SyncT, 0))
	    .Help("Synchronize all language translation files of current workspace");
	menu.AddMenu(AK_TRIMPORT, IdeImg::Language(), THISBACK1(SyncT, 1))
	    .Help("Import runtime translation file");
	menu.AddMenu(AK_TREXPORT, IdeImg::Language(), THISBACK1(SyncT, 2))
	    .Help("Export runtime translation file");
	if(OldLang())
		menu.Add("Convert s_ -> t_", THISBACK(ConvertST));
	FilePropertiesMenu(menu);
	if(SvnDirs().GetCount())
		if(menu.IsMenuBar())
			menu.Add("SVN", THISBACK(ProjectSvn));
		else
			menu.Add("SVN Synchronize everything..", IdeImg::svn(), THISBACK(SyncSvn));
}

void Ide::FilePropertiesMenu(Bar& menu)
{
	menu.MenuSeparator();
	menu.Add(IsActiveFile(), AK_FILEPROPERTIES, THISBACK(FileProperties))
		.Help("File properties stored in package");
	menu.Add(IsActiveFile(), AK_SAVEENCODING, THISBACK(ChangeCharset))
	    .Help("Convert actual file to different encoding");
	menu.AddMenu(IsActiveFile() && !IsFolder(editfile), AK_DIFF, IdeImg::Diff(), THISBACK(Diff))
	    .Help("Show differences between the project and arbitrary files");
	menu.AddMenu(IsActiveFile() && !IsFolder(editfile), AK_PATCH, IdeImg::Patch(), THISBACK(Patch))
	    .Help("Show differences with patch file applied");
	if(IsSvnDir(GetFileFolder(editfile)))
		menu.AddMenu(IsActiveFile() && !IsFolder(editfile), AK_SVNDIFF, IdeImg::SvnDiff(), THISBACK(SvnHistory))
		    .Help("Show svn history of file");
}

void Ide::BuildFileMenu(Bar& menu)
{
	bool b = idestate == EDITING && !IdeIsDebugLock();
	menu.Add(b, "Compile " + GetFileName(editfile), IdeImg::Source(), THISBACK(FileCompile))
		.Key(AK_COMPILEFILE)
		.Help("Compile current file");
	menu.Add(b, "Preprocess " + GetFileName(editfile), IdeImg::Header(), THISBACK1(Preprocess, false))
		.Key(AK_PREPROCESSFILE)
		.Help("Preprocess current file into temporary file & open in editor");
	if(GetMethodVars(method).Get("BUILDER", "") == "GCC")
		menu.Add(b, "Show assembler code for " + GetFileName(editfile), THISBACK1(Preprocess, true))
			.Key(AK_ASSEMBLERCODE)
			.Help("Compile the file into assembler code");
}

void Ide::BuildPackageMenu(Bar& menu)
{
	int pi = GetPackageIndex();
	bool b = !IdeIsDebugLock() && idestate == EDITING && pi >= 0 && pi < IdeWorkspace().GetCount();
	menu.Add(b, AK_BUILDPACKAGE, THISBACK(PackageBuild))
		.Help("Build current package");
	menu.Add(b, AK_CLEANPACKAGE, THISBACK(PackageClean))
		.Help("Remove all intermediate files of the current package");
}

void Ide::BuildMenu(Bar& menu) {
	bool b = !IdeIsDebugLock();
	menu.Add(AK_OUTPUTMODE, THISBACK(SetupOutputMode))
	    .Help("Setup how to build the target");
	if(idestate == BUILDING)
		menu.Add(b, "Stop build", IdeImg::build_stop(), THISBACK(StopBuild))
			.Key(AK_BUILD)
			.Help("Stop building");
	else
		menu.Add(b, "Build", IdeImg::build_make(), THISBACK(DoBuild))
			.Key(AK_BUILD)
			.Help("Perform minimal application rebuild");
	b = b && idestate == EDITING;
	menu.Add(b, AK_CLEAN, THISBACK(Clean))
		.Help("Remove all intermediate files");
	menu.Add(b, AK_REBUILDALL, IdeImg::build_rebuild_all(), THISBACK(RebuildAll))
		.Help("Remove all intermediate files & build");
	menu.Add(b, AK_CLEANUPPOUT, THISBACK(CleanUppOut))
		.Help("Remove all files and subdirectories in the output & intermediate directory (see Base setup)");

//	menu.MenuSeparator();

//	menu.Add(b, AK_CREATEMAKEFILE, THISBACK(CreateMakefile))
//		.Help("Create makefile enabling IDE-independent project building");

	menu.MenuSeparator();

	if(menu.IsMenuBar()) {
		BuildPackageMenu(menu);
		menu.MenuSeparator();
	}

	BuildFileMenu(menu);

	menu.MenuSeparator();

	menu.Add("Stop on errors", THISBACK(ToggleStopOnErrors))
		.Check(stoponerrors)
		.Help("Stop build after package when the package has errors");

	menu.MenuSeparator();

	menu.Add(GetConsole().GetLineCount(), AK_FINDNEXTERROR, THISBACK(FindNextError))
		.Help("Find next " + String(btabs.GetCursor() == BCONSOLE2 ? "position" : "error line")
	           + "according to console pane");
	menu.Add(GetConsole().GetLineCount(), AK_FINDPREVERROR, THISBACK(FindPrevError))
		.Help("Find previous " + String(btabs.GetCursor() == BCONSOLE2 ? "position" : "error line")
	          + "according to console pane");
#if defined(PLATFORM_WIN32) || defined(PLATFORM_LINUX)
	menu.MenuSeparator();
	menu.Add(!IsNull(target), "Open output directory", THISBACK(OpenOutputFolder));
#endif
}

void Ide::DebugMenu(Bar& menu)
{
	bool b = idestate == EDITING && !IdeIsDebugLock();
	if(debugger) {
		debugger->DebugBar(menu);
		menu.Separator();
	}
	else {
		if(console.IsRunning())
			menu.Add("Stop !", THISBACK(StopDebug))
			    .Help("Stop controlled process");
		menu.Add(AK_RUNOPTIONS, THISBACK(RunArgs))
			.Help("Current directory, command line, stdout redirection");
		menu.Add(b, AK_EXECUTE, IdeImg::debug_run(), THISBACK(BuildAndExecute))
			.Help("Build and execute the application");
		menu.Add(b, AK_DEBUG, THISBACK1(BuildAndDebug, false))
			.Help("Build application & run debugger");
		menu.Add(b, AK_DEBUGTO, THISBACK1(BuildAndDebug, true))
			.Help("Build application & run to cursor in debugger");
		menu.Add(b, AK_DEBUGEXT, THISBACK(BuildAndExtDebug))
			.Help("Build application & run external debugger (see Base setup, default \"msdev.exe\")");
		menu.Add(b, AK_DEBUGFILEEXT, THISBACK(BuildAndExtDebugFile))
			.Help("Build application & run external debugger, trying to start with current file");
	#ifdef PLATFORM_POSIX
		if(IsValgrind())
			menu.Add(b, AK_VALGRIND, THISBACK(Valgrind))
				.Help("Build application & run in valgring");
	#endif

		menu.Separator();
	}
	menu.Add(!editfile.IsEmpty() /*&& !debuglock*/, AK_BREAKPOINT, THISBACK(DebugToggleBreak))
		.Help("Set / clear breakpoint on current line");
//	menu.Add(!editfile.IsEmpty(), AK_CONDBREAKPOINT, THISBACK(ConditionalBreak))
//		.Help("Edit conditional breakpoint");
	menu.Add(!editfile.IsEmpty() /*&& !debuglock*/, AK_CLEARBREAKPOINTS, THISBACK(DebugClearBreakpoints))
		.Help("Clear all breakpoints");
	menu.Separator();
	menu.Add(target.GetCount() && FileExists(GetLogPath()), AK_OPENLOG, THISBACK(OpenLog));
}

void Ide::BrowseMenu(Bar& menu) {
	menu.AddMenu(AK_NAVIGATOR, IdeImg::Navigator(), THISBACK(ToggleNavigator))
	     .Check(editor.IsNavigator());
	menu.Add(AK_SEARCHCODE, THISBACK(SearchCode));
	menu.Add(!designer, AK_GOTO, THISBACK(Goto));
	menu.Add(AK_GOTOGLOBAL, THISBACK(GotoGlobal));
	menu.Add(!designer, AK_JUMPS, THISBACK(ContextGoto));
	menu.Add(!designer, AK_SWAPS, THISBACK(SwapS));
	menu.Add(!designer, AK_ASSIST, callback(&editor, &AssistEditor::Assist));
	menu.Add(!designer, AK_DCOPY, callback(&editor, &AssistEditor::DCopy));
	menu.Add(!designer, AK_VIRTUALS, callback(&editor, &AssistEditor::Virtuals));
	menu.Add(!designer, AK_THISBACKS, callback(&editor, &AssistEditor::Thisbacks));
	menu.Add(!designer, AK_COMPLETE, callback(&editor, &AssistEditor::Complete));
	menu.Add(!designer, AK_ABBR, callback(&editor, &AssistEditor::Abbr));
	menu.Add(!designer, "Insert", THISBACK(InsertMenu));
	menu.MenuSeparator();
	menu.Add("Rescan code", THISBACK(RescanCode));
	menu.MenuSeparator();
	menu.AddMenu(AK_CALC, IdeImg::calc(), THISBACK1(ToggleBottom, BCALC))
	     .Check(IsBottomShown() && btabs.GetCursor() == BCALC);
	menu.AddMenu(AK_QTF, IdeImg::qtf(), THISBACK(Qtf));
	menu.MenuSeparator();
	menu.Add(AK_BROWSETOPICS, IdeImg::help(), THISBACK(ShowTopics));
	menu.Add(AK_SEARCHTOPICS, THISBACK(SearchTopics));
	menu.Add("About..", THISBACK(About));
}

void Ide::MainMenu(Bar& menu) {
	menu.Add("File", THISBACK(File))
		.Help("Package & file functions, exports, bookmarks");
	menu.Add("Edit", THISBACK(Edit))
		.Help("Clipboard, find & replace, spaces / tabs conversion, scope highlighting");
	if(HasMacros())
		menu.Add("Macro", THISBACK(MacroMenu))
			.Help("Editor & IDE macros");
	menu.Add("Project", THISBACK(Project))
		.Help("Package organizer, custom steps, configuration manager");
	menu.Add("Build", THISBACK(BuildMenu))
		.Help("Building & debugging, minor build options, showing errors");
	menu.Add("Debug", THISBACK(DebugMenu))
		.Help("Debugger commands (currently supports gdb-connection only)");
	menu.Add("Assist", THISBACK(BrowseMenu))
		.Help("Informations, code browsing and assistance");
	menu.Add("Setup", THISBACK(Setup))
		.Help("Paths, editor settings, connection to remote host");
}

void Ide::MainTool(Bar& bar)
{
	Edit(bar);
	if(debugger)
		DebugMenu(bar);
	else
	if(!designer)
		bar.Separator();
	Project(bar);
	BuildMenu(bar);
	BrowseMenu(bar);
}

void Ide::ConsoleMenu(Bar& menu)
{
	menu.Add("Clear", THISBACK(ConsoleClear))
		.Help("Empty system console");
	menu.Add("Copy", THISBACK(ConsoleCopy))
		.Key(K_CTRL_C)
		.Help("Copy selection on system clipboard");
	menu.Add("Paste", THISBACK(ConsolePaste))
		.Key(K_CTRL_V)
		.Help("Append selection to system console");
}

void Ide::SetBar()
{
	menubar.Set(THISBACK(MainMenu));
	toolbar.Set(THISBACK(MainTool));
}
