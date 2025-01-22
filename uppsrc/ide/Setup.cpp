#include "ide.h"

#define LLOG(x) // LOG(x)

int CharFilterVar(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

void DlCharset(DropList& d)
{
	d.Add(CHARSET_UTF8, "UTF8");
	d.Add(TextCtrl::CHARSET_UTF8_BOM, "UTF8 BOM");
	d.Add(TextCtrl::CHARSET_UTF16_LE, "UTF16 LE");
	d.Add(TextCtrl::CHARSET_UTF16_BE, "UTF16 BE");
	d.Add(TextCtrl::CHARSET_UTF16_LE_BOM, "UTF16 LE BOM");
	d.Add(TextCtrl::CHARSET_UTF16_BE_BOM, "UTF16 BE BOM");
	for(int i = 1; i < CharsetCount(); i++)
		d.Add(i, CharsetName(i));
}

class FontSelectManager {
	DropList *face;
	DropList *height;
	Option   *bold;
	Option   *italic;

	void FaceSelect();
	void Select();

public:
	Event<>   WhenAction;

	typedef FontSelectManager CLASSNAME;

	void Set(DropList& _face, DropList& _height,
	         Option& _bold, Option& _italic, bool gui = false);
	void Set(Font f);
	Font Get();
};

void FontSelectManager::FaceSelect() {
	Select();
}

void FontSelectManager::Select() {
	WhenAction();
}

void LoadFonts(DropList *face, Index<String>& fni, bool fixed)
{
	for(int i = 0; i < Font::GetFaceCount(); i++)
		if(!!(Font::GetFaceInfo(i) & Font::FIXEDPITCH) == fixed) {
			String n = Font::GetFaceName(i);
			if(fni.Find(n)
			 < 0) {
				fni.Add(n);
				face->Add(i, n);
			}
		}
}

void FontSelectManager::Set(DropList& _face, DropList& _height,
                            Option& _bold, Option& _italic, bool gui) {
	face = &_face;
	face->WhenAction = THISBACK(FaceSelect);
	height = &_height;
	height->WhenAction = THISBACK(Select);
	bold = &_bold;
	bold->WhenAction = THISBACK(Select);
	italic = &_italic;
	italic->WhenAction = THISBACK(Select);
	face->Clear();
	if(gui) {
		face->Add(Font::ARIAL);
		face->Add(Font::ROMAN);
		face->Add(Font::COURIER);
		for(int i = Font::COURIER + 1; i < Font::GetFaceCount(); i++)
			if(Font::GetFaceInfo(i) & Font::SCALEABLE)
				face->Add(i);

		struct DisplayFace : public Display {
			void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
				int ii = q;
				Font fnt = StdFont();
				if(!(Font::GetFaceInfo(ii) & Font::SPECIAL))
					fnt.Face(ii);
				w.DrawRect(r, paper);
				w.DrawText(r.left, r.top + (r.Height() - fnt.Info().GetHeight()) / 2,
				           Font::GetFaceName((int)q), fnt, ink);
			}
		};
		face->SetDisplay(Single<DisplayFace>());
	}
	else {
		Index<String> fni;
		LoadFonts(face, fni, true);
		face->AddSeparator();
		LoadFonts(face, fni, false);
		face->SetIndex(0);
		height->ClearList();
	}
	for(int i = 6; i < 64; i++)
		height->Add(i);
	FaceSelect();
}

void FontSelectManager::Set(Font f) {
	int fi = f.GetFace();
	if(!face->HasKey(fi)) {
		fi = face->FindValue(f.GetFaceName());
		if(fi < 0)
			fi = Font::COURIER;
		else
			fi = face->GetKey(fi);
	}
	face->SetData(fi);
	FaceSelect();
	height->SetData(f.GetHeight());
	for(int i = 0; i < height->GetCount(); i++) {
		int q = height->GetKey(i);
		if(f.GetHeight() <= q) {
			height->SetData(q);
			break;
		}
	}
	*bold = f.IsBold();
	*italic = f.IsItalic();
}

Font FontSelectManager::Get() {
	Font f(face->GetData(), height->GetData());
	if(*bold) f.Bold();
	if(*italic) f.Italic();
	return f;
}

void Ide::UpdateFormat(CodeEditor& editor)
{
	if(!IsActiveFile() || ActiveFile().tabsize <= 0)
		editor.TabSize(editortabsize);
	editor.IndentSpaces(indent_spaces);
	editor.IndentAmount(indent_amount);
	editor.ShowTabs(show_tabs);
	editor.ShowSpaces(show_spaces);
	editor.BlockCaret(block_caret);
	editor.ShowLineEndings(show_tabs);
	editor.WarnWhiteSpace(warnwhitespace);
	editor.NoParenthesisIndent(no_parenthesis_indent);
	editor.HiliteScope(hilite_scope);
	editor.HiliteBracket(hilite_bracket);
	editor.HiliteIfDef(hilite_ifdef);
	editor.BarLine(barline);
	editor.HiliteIfEndif(hilite_if_endif);
	editor.ThousandsSeparator(thousands_separator);
	editor.ShowCurrentLine(hline ? HighlightSetup::GetHlStyle(HighlightSetup::SHOW_LINE).color : (Color)Null);
	editor.ShowCurrentColumn(vline ? HighlightSetup::GetHlStyle(HighlightSetup::SHOW_COLUMN).color : (Color)Null);
	editor.LineNumbers(line_numbers);
	editor.AutoEnclose(auto_enclose);
	editor.MarkLines(mark_lines);
	editor.BorderColumn(bordercolumn, this->editor.GetHlStyle(CodeEditor::SHOW_BORDER).color);
	editor.PersistentFindReplace(persistent_find_replace);
	editor.FindReplaceRestorePos(find_replace_restore_pos);
	editor.Refresh();
}

void Ide::UpdateFormat() {
	SetupEditor();
	UpdateFormat(editor);
	UpdateFormat(editor2);
	console.SetFont(consolefont);
	console.WrapText(wrap_console_text);
	statusbar.Show(show_status_bar);
	SetupBars();
	
	if(!designer) {
		if(filetabs >=0) {
			tabs.SetAlign(filetabs);
			editpane.SetFrame(tabs);
		}
		else
			editpane.SetFrame(NullFrame());
	}

	tabs.Grouping(tabs_grouping);
	tabs.Stacking(tabs_stacking);
	tabs.FileIcons(tabs_icons, false);
	tabs.Crosses(tabs_crosses >= 0, tabs_crosses);
}

void Ide::EditorFontScroll(int d)
{
	if(!IsActiveFile())
		return;
	Package::File& f = ActiveFile();
	if(f.font != 0 || editorsplit.GetZoom() < 0)
		return;
	int h = editorfont.GetCy();
	int q = editorfont.GetHeight();
	while(editorfont.GetCy() == h && (d < 0 ? editorfont.GetCy() > 5 : editorfont.GetCy() < 40))
		editorfont.Height(q += d);
	editor.SetFont(editorfont);
	editor.EditorBarLayout();
}

struct FormatDlg : TabDlg {
	ColorPusher hl_color[CodeEditor::HL_COUNT];
};

void HlPusherFactory(One<Ctrl>& ctrl)
{
	ctrl.Create<ColorPusher>().NotNull().Track();
}

void Ide::ReadHlStyles(ArrayCtrl& hlstyle)
{
	hlstyle.Clear();
	for(int i = 0; i < CodeEditor::HL_COUNT; i++) {
		const HlStyle& s = editor.GetHlStyle(i);
		hlstyle.Add(editor.GetHlName(i), s.color, s.bold, s.italic, s.underline);
	}
}

void SetConsole(EditString *e, const char *text)
{
	*e <<= text;
}

void AddPath(EditString *es)
{
	String s = SelectDirectory();
	if(IsNull(s))
		return;
	String h = ~*es;
	if(h.GetCount() && *h.Last() != ';')
		h << ';';
	h << s;
	*es <<= h;
	es->SetWantFocus();
}

void InsertPath(EditString *es)
{
	String s = SelectDirectory();
	if(IsNull(s))
		return;
	es->Clear();
	*es <<= s;
	es->SetWantFocus();
}

void DlSpellerLangs(DropList& dl)
{
	static Vector<int> lng;
	ONCELOCK {
		VectorMap<int, String> lngs;
		String path = GetExeDirFile("speller") + ';' + ConfigFile("speller") + ';' +
		              GetExeFolder() + ';' + GetConfigFolder() + ';' +
		              getenv("LIB") + ';' + getenv("PATH");
#ifdef PLATFORM_POSIX
		path << "/usr/local/share/upp/speller;/usr/local/share/upp;/usr/share/upp/speller;/usr/share/upp";
#endif
		Vector<String> p = Split(path, ';');
		for(auto dir : p) {
			FindFile ff(AppendFileName(dir, "*.udc"));
			while(ff) {
				int lang = LNGFromText(ff.GetName());
				if(lang)
					lngs.GetAdd(lang) = LNGAsText(lang);
				ff.Next();
			}
		}
		SortByValue(lngs);
		lng = lngs.PickKeys();
	}
	dl.Add(0, "Off");
	for(auto l : lng)
		dl.Add(l, LNGAsText(l));
}

void Ide::SetupFormat() {
	FormatDlg dlg;
	dlg.Title("Settings");
	WithSetupFontLayout<ParentCtrl> fnt;
	WithSetupHlLayout<ParentCtrl> hlt;
	WithSetupEditorLayout<ParentCtrl> edt;
	WithSetupIdeLayout<ParentCtrl> ide;
	WithSetupAssistLayout<ParentCtrl> assist;
	WebSearchTab web_search;
	edt.lineends
		.Add(LF, "LF")
		.Add(CRLF, "CRLF")
		.Add(DETECT_LF, "Detect with default LF")
		.Add(DETECT_CRLF, "Detect with default CRLF");
	
	edt.filetabs
		.Add(AlignedFrame::LEFT, "Left")
		.Add(AlignedFrame::TOP, "Top")
		.Add(AlignedFrame::RIGHT, "Right")
		.Add(AlignedFrame::BOTTOM, "Bottom")
		.Add(-1, "Off");
		
	edt.tabs_crosses
		.Add(AlignedFrame::LEFT, "Left")
		.Add(AlignedFrame::RIGHT, "Right")
		.Add(-1, "Off");
	
	dlg.Add(fnt, "Fonts");
	dlg.Add(hlt, "Syntax highlighting");
	dlg.Add(edt, "Editor");
	dlg.Add(assist, "Assist");
	dlg.Add(ide, "IDE");
	dlg.Add(web_search, "Web search");
	dlg.WhenClose = dlg.Acceptor(IDEXIT);

#ifdef PLATFORM_WIN32
	ide.xterm.SetLabel("powershell");
	ide.xterm << [&] { ide.console <<= "powershell.exe"; };
	ide.gnome.SetLabel("cmd");
	ide.gnome << [&] { ide.console <<= "cmd.exe"; };
	ide.kde.Hide();
	ide.mate.Hide();
	ide.lxde.Hide();
#else
	#ifdef PLATFORM_BSD
	ide.kde <<= callback2(SetConsole, &ide.console, "/usr/local/bin/konsole -e");
	ide.gnome <<= callback2(SetConsole, &ide.console, "/usr/local/bin/gnome-terminal -x");
	ide.mate <<= callback2(SetConsole, &ide.console, "/usr/local/bin/mate-terminal -x");
	ide.lxde <<= callback2(SetConsole, &ide.console, "/usr/local/bin/lxterminal -e");
	ide.xterm <<= callback2(SetConsole, &ide.console, "/usr/local/bin/xterm -e");
	#else
	ide.kde <<= callback2(SetConsole, &ide.console, "/usr/bin/konsole -e");
	ide.gnome <<= callback2(SetConsole, &ide.console, "/usr/bin/gnome-terminal -x");
	ide.mate <<= callback2(SetConsole, &ide.console, "/usr/bin/mate-terminal -x");
	ide.lxde <<= callback2(SetConsole, &ide.console, "/usr/bin/lxterminal -e");
	ide.xterm <<= callback2(SetConsole, &ide.console, "/usr/bin/xterm -e");
	#endif
#endif

	FontSelectManager ed, vf, con, f1, f2, tf, gui;
	ed.Set(fnt.face, fnt.height, fnt.bold, fnt.italic);
	vf.Set(fnt.vface, fnt.vheight, fnt.vbold, fnt.vitalic);
	con.Set(fnt.cface, fnt.cheight, fnt.cbold, fnt.citalic);
	tf.Set(fnt.tface, fnt.theight, fnt.tbold, fnt.titalic);
	f1.Set(fnt.face1, fnt.height1, fnt.bold1, fnt.italic1);
	f2.Set(fnt.face2, fnt.height2, fnt.bold2, fnt.italic2);
	gui.Set(ide.face, ide.height, ide.bold, ide.italic, true);

	ed.Set(editorfont);
	vf.Set(veditorfont);
	con.Set(consolefont);
	tf.Set(tfont);
	f1.Set(font1);
	f2.Set(font2);
	gui.Set(gui_font);

	DlCharset(edt.charset);
	edt.tabsize.MinMax(1, 100).NotNull();
	edt.tabsize <<= editortabsize;
	edt.indent_amount.MinMax(1, 100).NotNull();
	edt.indent_amount <<= indent_spaces ? indent_amount : editortabsize;
	edt.indent_amount.Enable(indent_spaces);
	CtrlRetriever rtvr;
	int hs = hilite_scope;

	DlSpellerLangs(edt.spellcheck_comments);
	
	web_search.Load();
	
	assist.std_cpp.Add(14, "C++ 14")
	              .Add(17, "C++ 17")
	              .Add(20, "C++ 20")
	              .Add(23, "C++ 23")
	;

	rtvr
		(hlt.hilite_scope, hs)
		(hlt.hilite_bracket, hilite_bracket)
		(hlt.hilite_ifdef, hilite_ifdef)
		(hlt.hilite_if_endif, hilite_if_endif)
		(hlt.thousands_separator, thousands_separator)
		(hlt.hline, hline)
		(hlt.vline, vline)
		(hlt.bordercolumn, bordercolumn)
		(hlt.hl_custom, hl_custom)

		(edt.indent_spaces, indent_spaces)
		(edt.no_parenthesis_indent, no_parenthesis_indent)
		(edt.showtabs, show_tabs)
		(edt.showspaces, show_spaces)
		(edt.warnwhitespace, warnwhitespace)
		(edt.lineends, line_endings)
		(edt.numbers, line_numbers)
		(edt.bookmark_pos, bookmark_pos)
		(edt.findpicksel, find_pick_sel)
		(edt.findpicktext, find_pick_text)
		(edt.deactivate_save, deactivate_save)
		(edt.filetabs, filetabs)
		(edt.tabs_icons, tabs_icons)
		(edt.tabs_crosses, tabs_crosses)
		(edt.tabs_grouping, tabs_grouping)
		(edt.tabs_stacking, tabs_stacking)
		(edt.tabs_serialize, tabs_serialize)
		(edt.spellcheck_comments, spellcheck_comments)
		(edt.wordwrap_comments, wordwrap_comments)
		(edt.persistent_find_replace, persistent_find_replace)
		(edt.find_replace_restore_pos, find_replace_restore_pos)
		(edt.block_caret, block_caret)
		(edt.bar_branch, bar_branch)

		(assist.enabled, LibClangEnabled)
		(assist.barline, barline)
		(assist.auto_enclose, auto_enclose)
		(assist.commentdp, editor.commentdp)
		(assist.header_guards, header_guards)
		(assist.insert_include, insert_include)
		(assist.mark_lines, mark_lines)
		(assist.qtfsel, qtfsel)
		(assist.assist, editor.auto_assist)
		(assist.auto_indexer, AutoIndexer)
		(assist.relaxed, RelaxedIndexerDependencies)
		(assist.navigator_right, editor.navigator_right)
		(assist.show_errors, editor.show_errors)
		(assist.show_errors_status, editor.show_errors_status)
		(assist.libclang_options, libclang_options)
		(assist.libclang_coptions, libclang_coptions)
		(assist.diagnostics, AssistDiagnostics)
		(assist.indexer_threads, IndexerThreads)
		(assist.parsed_files, ParsedFiles)
		(assist.no_empty_autocomplete, editor.no_empty_autocomplete)
		(assist.blk0_header, blk0_header)
		(assist.std_cpp, LibClangCppVersion)

		(ide.showtime, showtime)
		(ide.show_status_bar, show_status_bar)
		(ide.toolbar_in_row, toolbar_in_row)
		(ide.splash_screen, splash_screen)
		(ide.sort, sort)
		(ide.mute_sounds, mute_sounds)
		(ide.wrap_console_text, wrap_console_text)
		(ide.hydra1_threads, hydra1_threads)
		(ide.chstyle, chstyle)
		(ide.console, HostConsole)
		(ide.output_per_assembly, output_per_assembly)
		(ide.setmain_newide, setmain_newide)
		(ide.gui_font, gui_font_override)
		(ide.search_downloads, search_downloads)
	;
	hlt.hlstyle.AddColumn("Style");
	hlt.hlstyle.AddColumn("Color").Ctrls(HlPusherFactory);
	hlt.hlstyle.AddColumn("Bold").Ctrls<Option>();
	hlt.hlstyle.AddColumn("Italic").Ctrls<Option>();
	hlt.hlstyle.AddColumn("Underline").Ctrls<Option>();
	hlt.hlstyle.ColumnWidths("211 80 45 45 80");
	hlt.hlstyle.EvenRowColor().NoHorzGrid().SetLineCy(EditField::GetStdHeight() + 2);
	ReadHlStyles(hlt.hlstyle);
	edt.charset <<= (int)default_charset;
	edt.tabsize.WhenAction = rtvr <<=
		hlt.hlstyle.WhenCtrlsAction = ed.WhenAction = tf.WhenAction =
		con.WhenAction = f1.WhenAction = f2.WhenAction = dlg.Breaker(222);
	hlt.hlstyle.WhenCtrlsAction << [&] { hlt.hl_custom <<= true; };
	ide.showtimeafter <<= Nvl((Date)FileGetTime(ConfigFile("version")), GetSysDate() - 1);
	hlt.hl_restore <<= dlg.Breaker(333);
	hlt.hl_restore_white <<= dlg.Breaker(334);
	hlt.hl_restore_dark <<= dlg.Breaker(335);
	
	for(auto sk : GetAllChSkins())
		ide.chstyle.Add(ide.chstyle.GetCount(), sk.b);
	
	ide.chstyle << dlg.Breaker(336);

	String usc_path = GetHomeDirFile("usc.path");
	ide.uscpath <<= LoadFile(usc_path);
	DirSelect(ide.uscpath, ide.uscpath_sel);

	ide.upphub <<= LoadFile(DefaultHubFilePath());
	ide.upphub.NullText(ConfigFile("UppHub"));
	DirSelect(ide.upphub, ide.upphub_sel);
	
	fnt.defaults << [&] {
		Ide def;

		ed.Set(def.editorfont);
		vf.Set(def.veditorfont);
		con.Set(def.consolefont);
		tf.Set(def.tfont);
		f1.Set(def.font1);
		f2.Set(def.font2);
	};

	for(;;) {
		int c = dlg.Run();

		SetHubDir(~ide.upphub);

		if(IsNull(ide.uscpath))
			FileDelete(usc_path);
		else
			Upp::SaveFile(usc_path, ~ide.uscpath);

		editorfont = ed.Get();
		tfont = tf.Get();
		veditorfont = vf.Get();
		consolefont = con.Get();
		font1 = f1.Get();
		font2 = f2.Get();
		gui_font = gui.Get();
		
		editortabsize = Nvl((int)~edt.tabsize, 4);
		rtvr.Retrieve();
		console.SetSlots(minmax(hydra1_threads, 1, 256));
		hilite_scope = hs;
		if(indent_spaces)
			indent_amount = ~edt.indent_amount;
		else {
			indent_amount = editortabsize;
			edt.indent_amount <<= editortabsize;
		}
		edt.indent_amount.Enable(indent_spaces);
		default_charset = (byte)(int)~edt.charset;
		for(int i = 0; i < CodeEditor::HL_COUNT; i++)
			editor.SetHlStyle(i, hlt.hlstyle.Get(i, 1), hlt.hlstyle.Get(i, 2),
			                     hlt.hlstyle.Get(i, 3), hlt.hlstyle.Get(i, 4));
		UpdateFormat();
		
		web_search.Save();
		
		if(c == IDEXIT)
			break;
		if(c == 222)
			hlstyle_is_default = false;
		if(c == 333 && PromptYesNo("Restore default highlighting colors?")) {
			editor.DefaultHlStyles();
			SetupEditor();
			ReadHlStyles(hlt.hlstyle);
			hlstyle_is_default = true;
			hlt.hl_custom <<= true;
		}
		if(c == 334 && PromptYesNo("Set white theme?")) {
			editor.WhiteTheme(false);
			SetupEditor();
			ReadHlStyles(hlt.hlstyle);
			hlstyle_is_default = false;
			hlt.hl_custom <<= true;
		}
		if(c == 335 && PromptYesNo("Set dark theme?")) {
			editor.DarkTheme(false);
			SetupEditor();
			ReadHlStyles(hlt.hlstyle);
			hlstyle_is_default = false;
			hlt.hl_custom <<= true;
		}
		if(c == 336) {
			auto v = GetAllChSkins();
			Ctrl::SetSkin(v[clamp((int)~ide.chstyle, 0, v.GetCount() - 1)].a);
		}
	}
	FileSetTime(ConfigFile("version"), ToTime(~ide.showtimeafter));
	FinishConfig();
	SaveConfig();
	
	auto v = GetAllChSkins();
	Ctrl::SetSkin(v[clamp(chstyle, 0, v.GetCount() - 1)].a);
	
	if(HasLibClang()) {
		for(int cpp = 0; cpp < 2; cpp++) {
			Clang h;
			if(!h.Parse(GetHomeDirFile(cpp ? "test.cpp" : "test.c"), "", GetCurrentIncludePath(), GetCurrentDefines(), 0)) {
				Exclamation("libclang additional compiler options do not seem do be correct. Assist might not work.");
				return;
			}
		}
		CurrentFileDeleteCache();
		editor.SyncCurrentFile();
	}
}

void Ide::FinishConfig()
{
	if(filelist.IsCursor()) {
		FlushFile();
		FileCursor();
	}
	SaveLoadPackage();
}

bool Ide::IsPersistentFindReplace()
{
	return persistent_find_replace;
}
