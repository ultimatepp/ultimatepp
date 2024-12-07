#include "ide.h"

#define METHOD_NAME "Ide::" << UPP_FUNCTION_NAME << "(): "

void Ide::SerializeWorkspace(Stream& s) {
	int i;
	int version = 22;
	s / version;
	s.Magic(0x12354);
	if(s.IsStoring()) {
		for(i = 0; i < filedata.GetCount(); i++) {
			String fn = filedata.GetKey(i);
			if(!fn.IsEmpty() && FileExists(fn)) {
				s % fn;
				s % filedata[i].editpos;
				if(version >= 1)
					s % filedata[i].columnline;
			}
		}
		String h;
		s % h;
	}
	else {
		String fn;
		filedata.Clear();
		for(;;) {
			s % fn;
			if(fn.IsEmpty()) break;
			FileData& fd = filedata.GetAdd(fn);
			fd.Clear();
			s % fd.editpos;
			if(version >= 1)
				s % fd.columnline;
		}
	}
	String pk = GetActivePackage();
	s % pk;
	package.FindSetCursor(pk);
	s % tablru;
	s % mainconfigname;
	s % mainconfigparam;
	s % console.verbosebuild;
	s % stoponerrors;
	byte dummy = 0;
	s % dummy;
	s % runarg;
	s % recent_runarg;
	s % rundir;
	s % stdout_file % recent_stdout_file / runmode;
	if(version >= 1)
		s % runexternal;
	if(version >= 11) {
		bool dummy = false;
		s % dummy;
	}
	if(version >= 13)
		s % consolemode;
	if(version >= 14)
		s % console_utf8;
	if(version >= 17)
		s % disable_uhd;
	if(version >= 18)
		s % darkmode;
	if(version >= 21)
		s % minimize;
	s % editfile;
	for(i = 0; i < 10; i++) {
		s % bookmark[i];
	}
	editor.Serialize(s);
	if(version >= 5)
		s % editorsplit;
	if(version == 6) {
		String n;
		int v;
		s / v;
		for(int i = 0; i < 10; i++) {
			s % n;
			s / v;
		}
	}
	if(version >= 8) {
		bool dummyb = false;
		String dummy;
		s % dummyb;
		s % dummy;
	}
	SerializeFindInFiles(s);
	String om;
	s % om;
	s % recentoutput;
	s % recentflags;
	s / editortabsize / indent_amount % indent_spaces;
	if(version < 15)
		for(int j = 0; j < GetIdeModuleCount(); j++)
			GetIdeModule(j).Serialize(s);
	else {
		VectorMap<String, String> module_cfg;
		for(int j = 0; j < GetIdeModuleCount(); j++) {
			IdeModule& m = GetIdeModule(j);
			module_cfg.Add(m.GetID(), StoreAsString(m));
		}
		s % module_cfg;
		for(int j = 0; j < GetIdeModuleCount(); j++) {
			IdeModule& m = GetIdeModule(j);
			LoadFromString(m, module_cfg.Get(m.GetID(), String()));
		}
	}
	SerializeWorkspaceConfigs(s);
	SerializeOutputMode(s);
	SerializeClosed(s);
	if(version >= 10) {
		bool dotabs = tabs_serialize;
		if(version >= 19)
			s % dotabs;
		if(dotabs)
			s % tabs;
	}
	if(version >= 11) {
		s % find_file_search_string;
	}
	if(version >= 16) {
		s % find_file_search_in_current_package;
	}
	if(version >= 12)
		SerializePlacement(s);
	if(version >= 20)
		s % difflru;
	if(version >= 22)
		editor.SerializeNavigatorWorkspace(s);
}

void Ide::SerializeLastMain(Stream& s)
{
	s % main;
	String varsname = GetVarsName();
	s % varsname;
	s % pocfg;
	if(s.IsLoading())
		LoadVars(varsname);
}

void Ide::EditorMode()
{
	Logd() << METHOD_NAME;
	
	editormode = true;
	main = Null;
	pocfg = Null;
	
	// TODO: This is probably bug with CtrlLib tool bar. If
	// we did not call clear here - we will have blocked two icons
	// in editor mode :(. This should be investigated with
	// appropriate test case.
	toolbar.Clear();
	SetBar();
	
	editor.SyncNavigator();
}

bool Ide::IsEditorMode() const
{
	return editormode;
}

void Ide::SaveLastMain()
{
	StoreToFile(THISBACK(SerializeLastMain), ConfigFile("lastmain.cfg"));
}

void Ide::LoadLastMain()
{
	bool editor = IsEditorMode();
	LoadFromFile(THISBACK(SerializeLastMain), ConfigFile("lastmain.cfg"));
	if(editor)
		main = Null;
}

void Sentinel(Stream& s, const char *txt)
{
	String h;
	h << "<123456789:" << txt << ":123456789>";
	Buffer<char> hh(h.GetLength());
	memcpy(hh, h, h.GetLength());
	s.SerializeRaw((byte *)~hh, h.GetLength());
}

void Ide::Serialize(Stream& s)
{
	int version = 31;
	Sentinel(s, "before 12341234");
	s.Magic(0x12341234);
	Sentinel(s, "after magic");
	s / version;
	Sentinel(s, "before first FileSel");
	s % AnySourceFs();
	Sentinel(s, "after AnySourceFs");
	s % BasedSourceFs();
	Sentinel(s, "after BasedSourceFs");
	s % AnyPackageFs();
	Sentinel(s, "after AnyPackageFs");
	s % pfsplit;
	if (version >= 12)
		s % weframe;
	else {
		Splitter dummy;
		s % dummy;
	}
	package.SerializeSettings(s);
	filelist.SerializeSettings(s);
	s % editorfont;
	s % tfont;
	s % veditorfont;
	s % consolefont;
	s % font1;
	s % font2;
	s % show_status_bar;
	s % toolbar_in_row;
	s % filetabs;
	s % auto_enclose;
	s % persistent_find_replace;
	if(version >= 4) {
		s % find_replace_restore_pos;
	}
	s % show_tabs;
	if(version >= 18)
		s % show_spaces;
	if(version >= 29)
		s % block_caret;
	if(version >= 30)
		s % bar_branch;
	if(version >= 7)
		s % warnwhitespace;
	s % tabs_icons;
	s % tabs_crosses;
	s % tabs_grouping;
	s % tabs_serialize;
	s % tabs_stacking;
	if(version >= 11)
		s % spellcheck_comments;
	if(version >= 13) {
		s % wordwrap_comments;
		s % wordwrap;
	}
	if(version >= 16) {
		s % setmain_newide;
	}
	bool dummy_force_crlf = false;
	s % dummy_force_crlf;
	if(version >= 1)
		s % line_endings;
	s % no_parenthesis_indent;
	s % hilite_scope;
	s % hilite_if_endif;
	s % hilite_bracket;
	s % hilite_ifdef;
	if(version >= 31)
		s % hl_custom;
	if(version >= 3)
		s % thousands_separator;
	if(version >= 5)
		s % hline;
	if(version >= 15)
		s % vline;
	s % barline;
	s % qtfsel;
	s % wrap_console_text;
	s % mute_sounds;
	s % line_numbers;
	s % bookmark_pos;
	s % use_target;
	s % default_charset;
	s % header_guards;
	s % insert_include;
	if(version >= 23)
		s % libclang_options;
	if(version >= 24)
		s % libclang_coptions;
	SerializeGlobalConfigs(s);
	doc.Serialize(s);
	s % right_split;
	s % splash_screen;
	s % editor.auto_assist;
	if(version >= 9)
		s % AutoIndexer;
	if(version >= 27)
		s % RelaxedIndexerDependencies;
	if(version >= 10) {
		bool dummy = false;
		s % dummy;
	}
	if(version >= 21) {
		bool dummy = false;
		s % dummy;
	}
	s % editor.commentdp;
	s % bordercolumn;
	Color dummy_color;
	s % dummy_color;
	if(version >= 20)
		s % find_pick_sel % find_pick_text % deactivate_save;
	s % hydra1_threads;
	if(s.IsLoading())
		console.SetSlots(hydra1_threads);
	if(version < 14) {
		int dummy_gdb_selector = 0;
		s % dummy_gdb_selector;
	}
	s % doc;
	s % chstyle;
	if(version < 25) { // deprecated
		bool	astyle_BracketIndent;
		bool	astyle_NamespaceIndent;
		bool	astyle_BlockIndent;
		bool	astyle_CaseIndent;
		bool	astyle_ClassIndent;
		bool	astyle_LabelIndent;
		bool	astyle_SwitchIndent;
		bool	astyle_PreprocessorIndent;
		int		astyle_MinInStatementIndentLength;
		int		astyle_MaxInStatementIndentLength;
		bool	astyle_BreakClosingHeaderBracketsMode;
		bool	astyle_BreakElseIfsMode;
		bool	astyle_BreakOneLineBlocksMode;
		bool	astyle_SingleStatementsMode;
		bool	astyle_BreakBlocksMode;
		bool	astyle_BreakClosingHeaderBlocksMode;
		int		astyle_BracketFormatMode;
		int		astyle_ParensPaddingMode;
		bool	astyle_ParensUnPaddingMode;
		bool	astyle_OperatorPaddingMode;
		bool	astyle_EmptyLineFill;
		bool	astyle_TabSpaceConversionMode;
		WString	astyle_TestBox;

		s % astyle_BracketIndent;
		s % astyle_NamespaceIndent;
		s % astyle_BlockIndent;
		s % astyle_CaseIndent;
		s % astyle_ClassIndent;
		s % astyle_LabelIndent;
		s % astyle_SwitchIndent;
		s % astyle_PreprocessorIndent;
		s % astyle_MinInStatementIndentLength;
		s % astyle_MaxInStatementIndentLength;
		s % astyle_BreakClosingHeaderBracketsMode;
		s % astyle_BreakElseIfsMode;
		s % astyle_BreakOneLineBlocksMode;
		s % astyle_SingleStatementsMode;
		s % astyle_BreakBlocksMode;
		s % astyle_BreakClosingHeaderBlocksMode;
		s % astyle_BracketFormatMode;
		s % astyle_ParensPaddingMode;
		s % astyle_ParensUnPaddingMode;
		s % astyle_OperatorPaddingMode;
		s % astyle_EmptyLineFill;
		s % astyle_TabSpaceConversionMode;
		s % astyle_TestBox;
	}
	
	if(version >= 6 && version < 8) {
		String dummy;
		s % dummy;
	}
	s % HostConsole;
	editor.SerializeNavigator(s);
	s % showtime;
	s % DiffFs();
	s % sort;
	s % output_per_assembly;
	s.Magic();
	if(version >= 17)
		s % hlstyle_is_default;
	if(version >= 19) {
		s % gui_font % gui_font_override;
	}
	
	if(version >= 22) {
		ClangConfigSerialize(s);
	}
	
	if(version >= 26)
		s % prefer_clang_format;
	
	if(version >= 28)
		s % blk0_header;

#ifdef PLATFORM_WIN32
	if(s.IsLoading() && HostConsole == "/usr/bin/xterm -e")
		HostConsole = "powershell.exe";
#endif
}

Time Ide::ConfigTime()
{
	return FileGetTime(ConfigFile());
}

void Ide::SaveConfig()
{
	SaveChangedFile(ConfigFile(), StoreAsString(*this));
	if(GetIniKey("DebugClipboard") == "1") {
		SaveChangedFile(ConfigFile() + ".bak", StoreAsString(*this));
		StoreToFile(*this, ConfigFile() + ".bak1");
	}
	SaveChangedFile(ConfigFile("ide.key"), StoreKeys());
	SaveChangedFile(ConfigFile("ide.colors"), editor.StoreHlStyles());
	config_time = ConfigTime();
}

void Ide::SaveConfigOnTime()
{
	if(ConfigTime() == config_time)
		SaveConfig();
}

void Ide::LoadConfig()
{
	if(!LoadFromFile(*this) && GetIniKey("DebugClipboard") == "1") {
		Exclamation("LoadConfig has failed!");
		if(!LoadFromFile(*this, ConfigFile() + ".bak")) {
			Exclamation("LoadConfig .bak has failed!");
			if(!LoadFromFile(*this, ConfigFile() + ".bak1"))
				Exclamation("LoadConfig .bak1 has failed!");
		}
	}
	RestoreKeys(LoadFile(ConfigFile("ide.key")));
	if(hlstyle_is_default)
		editor.DefaultHlStyles();
	else
		editor.LoadHlStyles(LoadFile(ConfigFile("ide.colors")));
	config_time = FileGetTime(ConfigFile());
	UpdateFormat();
	if(filelist.IsCursor()) {
		FlushFile();
		FileCursor();
	}
	SaveLoadPackage();
}
