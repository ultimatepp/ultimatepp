#if 0

#include "TCtrlLibHelp.h"
#pragma hdrstop
#pragma BLITZ_APPROVE

#include <Draw/PixelUtil.h>

#define LAYOUTFILE <TCtrlLib/Help/DlgHelp.lay>
#include <CtrlCore/lay.h>

class DisplayTopicName : public Display {
public:
	DisplayTopicName() {}

	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color i, Color p, dword style) const {
		if(!HelpTopicGet(v).added)
			i = LtRed;
		RichTextDisplay().Paint(draw, rc, HelpTopicGet(v).GetTitle(), i, p, style);
	}
};

static bool RawTopicMatch(const char *pattern, const char *file, const char *& endptr)
{
	for(char c; c = *pattern++;)
		if(c == '*') {
			do
				if(RawTopicMatch(pattern, file, endptr))
					return true;
			while(*file++);
			endptr = pattern;
			return false;
		}
		else if(c == '?') {
			if(*file++ == 0) {
				endptr = pattern - 1;
				return false;
			}
		}
		else if(ToLower(c) != ToLower(*file++)) {
			endptr = pattern - 1;
			return false;
		}
	endptr = file;
	return true;
}

static const char *FindTopicMatch(const char *pattern, const char *file, String& cont)
{
	const char *endptr;
	while(!RawTopicMatch(pattern, file, endptr)) {
		while(!(file[0] == '/' && file[1] == '/'))
			if(*file++ == 0)
				return endptr;
		file += 2;
	}
	if(cont.IsVoid()) {
		cont = endptr;
		return NULL;
	}
	const char *p = cont;
	while(*p && ToLower(*p) == ToLower(*endptr++))
		p++;
	cont.Trim(p - cont.Begin());
	return NULL;
}

struct RichObjectExchangeNew : public RichObjectExchange
{
	RichObjectExchangeNew(RichObjectType *type) : object(type, Null) {}

	virtual RichObject GetRichObject() const                 { return object; }
	virtual void       SetRichObject(const RichObject& data) { object = data; }

	RichObject         object;
};

class DlgHelpEditorLink : public WithHelpEditorLinkLayout<TopWindow>
{
public:
	typedef DlgHelpEditorLink CLASSNAME;
	DlgHelpEditorLink();

	bool Run(String& target, String& text, String& style);

private:
	void OnMask();
	void OnPreview();
	void SyncList();

private:
	Vector<String> map;
	int prev;
	DisplayTopicName tn;
};

RegisterHelpTopicObjectTitle(DlgHelpEditorLink, t_("Link to topic"))

DlgHelpEditorLink::DlgHelpEditorLink()
{
	prev = 0;

	CtrlLayoutOKCancel(*this, DlgHelpEditorLinkHelpTitle());
	Sizeable().MaximizeBox();
	HelpTopic("DlgHelpEditorLink");

	list.AddColumn(t_("Code"), 10);
	list.AddColumnAt(0, t_("Topic name"), 10).SetDisplay(tn);

	dyna_preview.SetZoom(Zoom(DOC_SCREEN_ZOOM, 1024));
	dyna_preview.NoSb();
	book_preview.SetZoom(Zoom(DOC_SCREEN_ZOOM, 1024));
	book_preview.NoSb();

	style.AddList(" (viz str. #)");
	style.AddList(", str. #");
	style.AddList(" (str. #)");
	style.AddList("-|#");

	mask <<= THISBACK(OnMask);
	list.WhenCursor = text <<= style <<= THISBACK(OnPreview);
}

bool DlgHelpEditorLink::Run(String& tg, String& tx, String& st)
{
	map <<= HelpTopicMap().GetKeys();
	Sort(map, GetLanguageInfo());

	CtrlRetriever rtvr;
	rtvr(text,   tx)
		(style,  st);
	mask <<= tg;
	SyncList();
	OnPreview();
	while(TopWindow::Run() == IDOK) {
		if(!list.IsCursor()) {
			BeepExclamation();
			continue;
		}
		tg = list.Get(0);
		rtvr.Retrieve();
		return true;
	}
	return false;
}

void DlgHelpEditorLink::SyncList()
{
	list.Clear();
	String m = mask.GetText().ToString();
	String rem = String::GetVoid();
	const char *best_err = NULL;

	for(int p = 0; p < map.GetCount(); p++) {
		String fn = map[p];
		String rr = rem;
		const char *errptr = FindTopicMatch(m, fn, rr);
		if(!errptr) {
			rem = rr;
			list.Add(fn);
		}
		else if(!best_err || errptr > best_err)
			best_err = errptr;
	}
	if(list.GetCount() == 0) {
		int px = best_err - m.Begin();
		mask.SetSelection(px, mask.GetLength());
		BeepExclamation();
	}
	else if(!rem.IsEmpty() && prev < mask.GetLength()) {
		int l, h;
		mask.GetSelection(l, h);
		if(l == m.GetLength()) {
			mask.SetText((mask + rem).ToWString());
			mask.SetSelection(l, l + rem.GetLength());
		}
	}
	prev = m.GetLength();
	if(list.GetCount() > 0)
		list.SetCursor(0);
}

void DlgHelpEditorLink::OnMask()
{
	SyncList();
}

void DlgHelpEditorLink::OnPreview()
{
	RichPara::CharFormat cfmt;
	RichPara para;
	String target;
	if(list.IsCursor())
		target = list.Get(0);
	para.Cat(FieldTypeLinkID(), EncodeLink(~target, ~text, ~style), cfmt);
	RichText text;
	text.Cat(para); {
		VectorMap<String, Value> vars;
		vars.Add("$link", 1);
		text.EvaluateFields(vars);
	}
	RichText copy;
	copy.Cat(para); {
		VectorMap<String, Value> vars;
		vars.Add("$book", 1);
		vars.Add(~target, 1234);
		copy.EvaluateFields(vars);
	}
	dyna_preview.Pick(text);
	book_preview.Pick(copy);
}

class DlgHelpEditorNewTopic : public WithHelpEditorNewTopicLayout<TopWindow>
{
public:
	typedef DlgHelpEditorNewTopic CLASSNAME;
	DlgHelpEditorNewTopic();

	bool Run(String& space, String& nesting, String& topic, String& folder);

private:
	void OnSpace();
	void OnNesting();
	void OnFolderBrowse();

private:
	ExtendedButton folder_browse;
};

RegisterHelpTopicObjectTitle(DlgHelpEditorNewTopic, t_("New help topic"))

DlgHelpEditorNewTopic::DlgHelpEditorNewTopic()
{
	CtrlLayoutOKCancel(*this, DlgHelpEditorNewTopicHelpTitle());
	HelpTopic("DlgHelpEditorNewTopic");

	Vector<String> hsp = HelpTopicEnumSpace();
	int i;
	for(i = 0; i < hsp.GetCount(); i++)
		dpp_space.Add(Nvl(hsp[i], "::"));
	if(hsp.GetCount() == 1)
		dpp_space.SetIndex(0);
	dpp_space <<= THISBACK(OnSpace);
	dpp_nesting <<= THISBACK(OnNesting);
	folder.AddFrame(folder_browse);
	folder_browse <<= THISBACK(OnFolderBrowse);
}

bool DlgHelpEditorNewTopic::Run(String& spc, String& nest, String& top, String& fold)
{
	dpp_space <<= spc;
	OnSpace();
	dpp_nesting <<= nest;
	OnNesting();
	dpp_topic <<= top;
	folder <<= fold;
	if(TopWindow::Run() != IDOK)
		return false;
	spc = ~dpp_space;
	nest = ~dpp_nesting;
	top = ~dpp_topic;
	fold = ~folder;
	return true;
}

void DlgHelpEditorNewTopic::OnSpace()
{
	Vector<String> nest = HelpTopicEnumNesting(~dpp_space);
	dpp_nesting.Clear();
	for(int i = 0; i < nest.GetCount(); i++)
		dpp_nesting.Add(Nvl(nest[i], "::"));
	if(nest.GetCount() == 1)
		dpp_nesting.SetIndex(0);
	OnNesting();
}

void DlgHelpEditorNewTopic::OnNesting()
{
	String sp = ~dpp_space, ne = ~dpp_nesting;
	Vector<String> names = HelpTopicList(sp, ne);
	dpp_topic.ClearList();
	for(int i = 0; i < names.GetCount(); i++) {
		String drl = names[i];
		String s, n, t;
		HelpParseDPP(drl, s, n, t);
		dpp_topic.AddList(t);
	}
}

void DlgHelpEditorNewTopic::OnFolderBrowse()
{
	FileSel fsel;
	fsel <<= ~folder;
	if(fsel.ExecuteSelectDir("Složka tématu"))
		folder <<= ~fsel;
}

class DlgHelpEditorSection : public WithHelpEditorSectionLayout<TopWindow>
{
public:
	typedef DlgHelpEditorSection CLASSNAME;
	DlgHelpEditorSection();

	bool Run(RichTextSection& section);

public:
	Option header_opt[3], footer_opt[3];
	EditField header[3], footer[3];

private:
	void Pump(RichTextSection& section, bool write);
	void UpdateUI();
};

RegisterHelpTopicObjectTitle(DlgHelpEditorSection, "Textový oddíl (tisk)");

DlgHelpEditorSection::DlgHelpEditorSection()
{
	CtrlLayoutOKCancel(*this, DlgHelpEditorSectionHelpTitle());
	HelpTopic("DlgHelpEditorSection");
	static String lcr[] = { t_("Left:"), t_("Center:"), t_("Right:") };
	for(int i = 0; i < 3; i++) {
		header_opt[i].SetLabel(lcr[i]);
		footer_opt[i].SetLabel(lcr[i]);
		header_opt[i] <<= footer_opt[i] <<= THISBACK(UpdateUI);
	}
	columns_opt <<= column_space_opt <<= firstpage_opt <<=
	margin_left_opt <<= margin_top_opt <<= margin_right_opt <<= margin_bottom_opt <<=
	header_space_opt <<= footer_space_opt <<= THISBACK(UpdateUI);
}

static void PumpOpt(int& value, Option& opt, Ctrl& editor, bool write)
{
	if(!write) {
		opt = !IsNull(value);
		editor <<= value;
	}
	else
		value = opt ? (int)~editor : int(Null);
}

void DlgHelpEditorSection::Pump(RichTextSection& section, bool write)
{
	if(!write) {
		columns_opt = !IsNull(section.columns);
		columns <<= !IsNull(section.columns) ? section.columns - 1 : int(Null);
		nested <<= section.nested;
	}
	else {
		section.columns = columns_opt ? Nvl((int)~columns) + 1 : int(Null);
		section.nested = nested;
	}
	PumpOpt(section.column_space, column_space_opt, column_space, write);
	PumpOpt(section.firstpage, firstpage_opt, firstpage, write);
	PumpOpt(section.margin.left, margin_left_opt, margin_left, write);
	PumpOpt(section.margin.top, margin_top_opt, margin_top, write);
	PumpOpt(section.margin.right, margin_right_opt, margin_right, write);
	PumpOpt(section.margin.bottom, margin_bottom_opt, margin_bottom, write);
	PumpOpt(section.header_space, header_space_opt, header_space, write);
	PumpOpt(section.footer_space, footer_space_opt, footer_space, write);
	for(int i = 0; i < 3; i++) {
		if(!write) {
			header_opt[i] = !section.header[i].IsVoid();
			footer_opt[i] = !section.footer[i].IsVoid();
			header[i] <<= section.header[i];
			footer[i] <<= section.footer[i];
		}
		else {
			section.header[i] = (header_opt[i] ? (String)~header[i] : String::GetVoid());
			section.footer[i] = (footer_opt[i] ? (String)~footer[i] : String::GetVoid());
		}
	}
}

bool DlgHelpEditorSection::Run(RichTextSection& section)
{
	Pump(section, false);
	UpdateUI();
	if(TopWindow::Run() != IDOK)
		return false;
	Pump(section, true);
	return true;
}

void DlgHelpEditorSection::UpdateUI()
{
	columns.Enable(columns_opt);
	column_space.Enable(column_space_opt);
	firstpage.Enable(firstpage_opt);
	margin_left.Enable(margin_left_opt);
	margin_top.Enable(margin_top_opt);
	margin_right.Enable(margin_right_opt);
	margin_bottom.Enable(margin_bottom_opt);
	header_space.Enable(header_space_opt);
	footer_space.Enable(footer_space_opt);
	for(int i = 0; i < 3; i++) {
		header[i].Enable(header_opt[i]);
		footer[i].Enable(footer_opt[i]);
	}
}

static HelpTopicInfoMap& StoredHelpTopicMap()
{
	static HelpTopicInfoMap map;
	return map;
}

void StoreHelpInfo()
{
	StoredHelpTopicMap() <<= HelpTopicMap();
}

void LoadAppHelpFile()
{
	HelpInit::Run();
	StoreHelpInfo();
	String dir = AppendFileName(GetFileDirectory(ConfigFile()), "doc.dpp");
	FindFile ff;
	if(ff.Search(AppendFileName(dir, "*.dpp")))
		do
			if(ff.IsFile()) {
				String path = AppendFileName(dir, ff.GetName());
				try {
					HelpTopicLoad(path, LoadFile(path));
				}
				catch(Exc e) {
					if(!PromptOKCancel(NFormat("[* \1%s\1]: %s\nPokraèovat?", path, e)))
						break;
				}
			}
		while(ff.Next());
}

class ConvertParaCount : public Convert
{
public:
	ConvertParaCount() : language(Null) {}

	void          Language(int lang) { language = lang; }

	virtual Value Format(const Value& value) const;

private:
	int           language;
};

Value ConvertParaCount::Format(const Value& value) const
{
//	return Null;
	String topic = value;
	if(IsNull(topic) || IsNull(language))
		return Null;
	RichText rt = ParseQTF(HelpTopicGet(HelpDPPStyle()).GetText(language)
		+ "\r\n" + HelpTopicGet(topic).GetText(language));
	return rt.IsEmpty() ? int(Null) : rt.GetPartCount();
}

class ConvertIndexCount : public Convert
{
public:
	ConvertIndexCount() : language(Null) {}

	void          Language(int lang) { language = lang; }

	virtual Value Format(const Value& value) const;

private:
	int           language;
};

Value ConvertIndexCount::Format(const Value& value) const
{
//	return Null;
	String topic = value;
	if(IsNull(topic) || IsNull(language))
		return Null;
	RichText rt = ParseQTF(HelpTopicGet(HelpDPPStyle()).GetText(language)
		+ "\r\n" + HelpTopicGet(topic).GetText(language));
	Vector<RichValPos> vp = rt.GetValPos(Size(10000, INT_MAX), RichText::INDEXENTRIES);
	Index<WString> ie;
	for(int i = 0; i < vp.GetCount(); i++)
		ie.FindAdd(vp[i].data);
	if(ie.IsEmpty())
		return Null;
	return ie.GetCount();
}

typedef VectorMap< String, VectorMap<int, int> > EditPosMap;
GLOBAL_VAR(EditPosMap, _sPosMap)

class DlgHelpEditor : public TopWindow
{
public:
	typedef WithHelpEditorEditLayout<TopWindow> EditorCtrl;

	typedef DlgHelpEditor CLASSNAME;
	DlgHelpEditor();

	virtual void                        Serialize(Stream& stream);
	static ConfigItem&                  config();

	void                                Run(Ctrl *ctrl, const Image& still);

	virtual void                        ChildGotFocus();
	virtual bool                        HotKey(dword key);

	TOOL(Main)

	TOOL(File)
		TOOL(FileOpen)
		TOOL(FileSave)
//		TOOL(FileSaveAs)
		TOOL(FileTopicIndex)
		TOOL(FileTopicView)
		TOOL(FileClose)

	TOOL(Edit)
		TOOL(EditUp)
		TOOL(EditDown)
		TOOL(EditGoto)
		TOOL(EditFormula)
		TOOL(EditLink)
		TOOL(EditSection)
		TOOL(EditNewTopic)
		TOOL(EditPaste)

	TOOL(System)
//		TOOL(SystemExport)
		TOOL(SystemExportProject)

	void                                Rescan() { menubar.Set(THISBACK(ToolMain)); toolbar.Set(THISBACK(ToolMain)); }

private:
	static bool                         SaveTopic(String topic, int lang, const EditorCtrl& editor);
	static String                       LoadTopic(String topic, int lang, EditorCtrl& editor);

	void                                OnTopicListCursor();
	void                                OnTopicListAction();
	void                                OnTopicLanguage();
	void                                OnAuxLanguage();

	void                                OnDPPSpace();
	void                                OnDPPNesting();

	void                                OnScreenshot();
	void                                OnPasteIcon();

	void                                UpdateTopicList() { OnDPPSpace(); }
	void                                UpdateAvail();
	void                                UpdateIconModules();
	void                                UpdateIconGallery();

	bool                                SaveFile();
	bool                                PromptSave();

private:
	Splitter                            hsplitter;
	ArrayCtrl                           topic_list;
	enum { C_DRL, C_NAME, C_COUNT };
	Splitter                            vsplitter;
	EditorCtrl                          edit, aux_edit;
	TabCtrl                             aux_tab;
	Ctrl                                aux_pane;
	WithHelpEditorIconLayout<TopWindow> aux_icons;
	EditorCtrl                         *last_edit;
	ConvertParaCount                    cv_para;
	ConvertIndexCount                   cv_index;
	bool                                is_dirty;
	Label                               dpp_space_tag;
	DropList                            dpp_space;
	Label                               dpp_nesting_tag;
	DropList                            dpp_nesting;

	Ctrl                               *master;
	One<Ctrl>                           screenshot;

	MenuBar                             menubar;
	ToolBar                             toolbar;
	StatusBar                           statusbar;

	String                              old_topic;
	int                                 old_lang1, old_lang2;
	String                              recent_dlg;
	String                              recent_exp_dir;
	String                              init_topic;

	enum { IFRM_NONE, IFRM_BUTTON, IFRM_INSET };
};

void RunDlgHelpEditor(Ctrl *ctrl, const Image& still) { DlgHelpEditor().Run(ctrl, still); }

RegisterHelpTopicObjectTitle(DlgHelpEditor, t_("Application documentation editor"))

DlgHelpEditor::DlgHelpEditor()
{
	master = NULL;
	is_dirty = false;
//	topic_text.SpellCheck();
//	topic_aux.SpellCheck();

	//CtrlLayout(*this, "Editor aplikaèní dokumentace");
	Title(DlgHelpEditorHelpTitle().ToWString());
	HelpTopic("DlgHelpEditor");
	WhenClose = THISBACK(OnFileClose);
	Sizeable().Zoomable();
	AddFrame(menubar);
	AddFrame(toolbar);
	AddFrame(statusbar);
	Add(hsplitter.SizePos());
	hsplitter.Horz(topic_list, vsplitter);
	hsplitter.SetPos(3000);
	topic_list.AutoHideSb();
	topic_list.NoVertGrid();
	ASSERT(topic_list.GetIndexCount() == C_DRL);
	topic_list.AddIndex();
	ASSERT(topic_list.GetIndexCount() == C_NAME);
	topic_list.AddColumn(t_("Topic"), 20);
	topic_list.AddColumnAt(C_DRL, t_("Para"), 4).SetConvert(cv_para).Cache();
	topic_list.AddColumnAt(C_DRL, t_("Index"), 4).SetConvert(cv_index).Cache();
	topic_list.WhenCursor = THISBACK(OnTopicListCursor);
	topic_list <<= THISBACK(OnTopicListAction);
	vsplitter.Vert(edit, aux_tab);
	CtrlLayout(edit);
	aux_tab.Add(aux_pane.SizePos(), t_("Screenshot"));
	aux_pane.NoWantFocus().Transparent();
	CtrlLayout(aux_edit);
	int wd = 4000; //edit.text.GetPage().cx;
	edit.text.SetPage(Size(wd, 0x10000000));
	aux_edit.text.SetPage(Size(wd, 0x10000000));
	aux_tab.Add(aux_edit.SizePos(), t_("Text"));
	CtrlLayout(aux_icons);
	aux_tab.Add(aux_icons.SizePos(), t_("Icons"));
	aux_icons.module.AddIndex();
	aux_icons.module.AddColumn();
	aux_icons.module.WhenCursor = THISBACK(UpdateIconGallery);
	aux_icons.gallery.Columns(3);
	aux_icons.frame <<= IFRM_BUTTON;
	aux_icons.size <<= 80;
	aux_icons.raise <<= -3;
	aux_icons.gallery.WhenLeftDouble = THISBACK(OnPasteIcon);
	last_edit = &edit;
	edit.text.WhenRefreshBar = aux_edit.text.WhenRefreshBar = THISBACK(Rescan);
//	edit.topic_module.SetReadOnly();
	edit.title.SetFont(StdFont().Bold());
	aux_edit.title.SetFont(StdFont().Bold());
	edit.title.SetConvert(CFormatConvert());
	aux_edit.title.SetConvert(CFormatConvert());
//	menubar <<= THISBACK(ToolMain);

	dpp_space_tag.SetLabel(t_("Namespace: "));
	dpp_space <<= THISBACK(OnDPPSpace);
	dpp_nesting_tag.SetLabel(t_("Nesting: "));
	dpp_nesting <<= THISBACK(OnDPPNesting);

	Vector<String> hsp = HelpTopicEnumSpace();
	dpp_space.Add(String::GetVoid(), t_("(all)"));
	int i;
	for(i = 0; i < hsp.GetCount(); i++)
		dpp_space.Add(Nvl(hsp[i], "::"));
	if(hsp.GetCount() == 1)
		dpp_space.SetIndex(1);

	Vector<int> lcodes = HelpTopicEnumLang();
	if(FindIndex(lcodes, LNG_CZECH) < 0) lcodes.Add(LNG_CZECH);
	if(FindIndex(lcodes, LNG_ENGLISH) < 0) lcodes.Add(LNG_ENGLISH);
	aux_edit.language.Add(Null, "---");
	old_lang1 = old_lang2 = Null;
	for(i = 0; i < lcodes.GetCount(); i++) {
		int l = lcodes[i];
		String n = LNGAsText(l);
		edit.language.Add(l, n);
		aux_edit.language.Add(l, n);
		if(l == GetCurrentLanguage())
			old_lang1 = l;
		else if(IsNull(old_lang2))
			old_lang2 = l;
	}
	edit.language <<= old_lang1;
	edit.language <<= THISBACK(OnTopicLanguage);
	aux_edit.language <<= old_lang2;
	aux_edit.language <<= THISBACK(OnAuxLanguage);

	ActiveFocus(topic_list);
	UpdateAvail();
	UpdateIconModules();
}

CONFIG_ITEM(DlgHelpEditor::config, "DlgHelpEditor", 6, 3, 6)

void DlgHelpEditor::Serialize(Stream& stream)
{
	SerializePlacement(stream);
	stream % hsplitter % vsplitter;
	stream % recent_exp_dir;
	if(config() >= 4) {
		String module;
		if(aux_icons.module.IsCursor())
			module = aux_icons.module.Get(0);
		stream % CtrlData<int>(aux_icons.frame)
			% CtrlData<int>(aux_icons.size)
			% CtrlData<double>(aux_icons.raise)
			% module;
		if(stream.IsLoading())
			aux_icons.module.FindSetCursor(module);
	}
	if(config() >= 5)
		stream % aux_icons.gallery;
	if(config() >= 6)
		stream % CtrlData<int>(aux_icons.grayed)
			% CtrlData<int>(aux_icons.etched);
}

void DlgHelpEditor::Run(Ctrl *master_, const Image& still_)
{
	master = master_;
	if(master) {
		screenshot = GetDlgShot(master_, still_);
		*screenshot <<= THISBACK(OnScreenshot);
		aux_pane.Add(screenshot->SizePos());
		Ctrl *m = master;
		while(IsNull(init_topic = m->GetHelpTopic()))
			if(m->GetParent())
				m = m->GetParent();
			else if(m->GetOwner())
				m = m->GetOwner();
			else
				break;
	}

	ReadConfigSelf();
	Open();
	Rescan();
	OnDPPSpace();
	OnTopicLanguage();
	OnTopicListCursor();
	ActiveFocus(edit.text);
	TopWindow::Run();
	OnTopicListCursor();
//	topic_list.KillCursor();
	WriteConfigSelf();
//	text = editor.Get();
//	String text = ~editor;
//	PromptOK(String() << "Output text:\n\n" << text);
}

void DlgHelpEditor::ToolMain(Bar& bar)
{
	bar.Add(t_("File"), THISBACK(ToolFile))
		.Help(t_("Help files maintenance"));
	bar.Add(t_("Edit"), THISBACK(ToolEdit))
		.Help(t_("Edit theme topics"));
	if(bar.IsToolBar()) {
		bar.Separator();
		bar.Add(dpp_space_tag);
		bar.Add(dpp_space, 120);
		bar.Separator();
		bar.Add(dpp_nesting_tag);
		bar.Add(dpp_nesting, 120);
	}
	bar.Add(t_("System"), THISBACK(ToolSystem))
		.Help(t_("System tools"));
	if(bar.IsToolBar()) {
		last_edit->text.DefaultBar(bar);
	}
/*
	if(bar.IsToolBar()) {

		bar_edit->StyleTool(bar);
		bar.Gap();
		bar_edit->FontTools(bar);
		bar.Gap();
		bar_edit->InkTool(bar);
		bar_edit->PaperTool(bar);
		bar.Gap();
		bar_edit->LanguageTool(bar);
		bar_edit->SpellCheckTool(bar);
		bar.Break();
		bar_edit->IndexEntryTool(bar);
		bar.Break();
		bar_edit->HyperlinkTool(bar);
		bar.Gap();
		bar_edit->ParaTools(bar);
		bar.Gap();
		bar_edit->PrintTool(bar);
	}
*/
}

void DlgHelpEditor::ToolFile(Bar& bar)
{
	ToolFileOpen(bar);
	ToolFileSave(bar);
//	ToolFileSaveAs(bar);
	bar.MenuSeparator();
	ToolFileTopicIndex(bar);
	ToolFileTopicView(bar);
	bar.MenuSeparator();
	ToolFileClose(bar);
}

void DlgHelpEditor::ToolFileOpen(Bar& bar)
{
	bar.Add(t_("Open"), CtrlImg::open(), THISBACK(OnFileOpen))
		.Key(K_CTRL_O)
		.Help(t_("Read help files from a given disk file"));
}

//RegisterHelpTopicObjectTitle(DlgHelpEditorMissingTopics, s_(DlgHelpEditorMissingTopicsHelpTitle))

void DlgHelpEditor::OnFileOpen()
{
	FileSelector fsel;
	fsel.Type("Dokumentace (*.dpp, *.dpx)", "*.dpp;*.dpx");
	fsel.AllFilesType();
	fsel.DefaultExt("dpp");
	fsel.Multi();
	if(fsel.ExecuteOpen(t_("Open"))) {
		int cs = topic_list.GetCursor();
		int sc = topic_list.GetCursorSc();
		topic_list.KillCursor();
		Progress progress("Naèítám soubory", fsel.GetCount());
		Index<String> loaded;
		for(int i = 0; i < fsel.GetCount(); i++)
			try {
				if(progress.StepCanceled())
					break;
				String data = LoadFile(fsel[i]);
				if(IsNull(data))
					throw Exc(t_("error loading file"));
				HelpTopicLoad(fsel[i], data, &loaded);
			}
			catch(Exc e) {
				if(!PromptOKCancel(NFormat("Chyba v souboru [* \1%s\1]:\n[* \1%s\1]\nPokraèovat?", fsel[i], e)))
					break;
			}
		UpdateTopicList();
		if(!topic_list.FindSetCursor(loaded[0], C_DRL))
			topic_list.SetCursor(cs);
		topic_list.ScCursor(sc);
	}
	UpdateAvail();
}

void DlgHelpEditor::ToolFileSave(Bar& bar)
{
	bar.Add(t_("Save"), CtrlImg::save(), THISBACK(OnFileSave))
		.Key(K_CTRL_S)
		.Help(t_("Save help topics into a common application file"));
}

void DlgHelpEditor::OnFileSave()
{
	SaveFile();
}

bool DlgHelpEditor::SaveFile()
{
	OnTopicListCursor();
	String outpath = GetFileFolder(ConfigFile());
	try {
		Vector<String> folders = HelpTopicEnumTextFolders();
		Progress progress("Ukládám nápovìdu", folders.GetCount());
		for(int i = 0; i < folders.GetCount(); i++) {
			if(progress.StepCanceled())
				throw AbortExc();
			Vector<String> files;
			HelpTopicSave(files, folders[i], StoredHelpTopicMap(), outpath);
		}
	}
	catch(Exc e) {
		Exclamation(NFormat(t_("Error writing help into folder [* \1%s\1]."), outpath));
		return false;
	}
	is_dirty = false;
	return true;
}

/*
void DlgHelpEditor::ToolFileSaveAs(Bar& bar)
{
	bar.Add(s_(DlgHelpEditorToolFileSaveAs), CtrlImg::save_as(), THISBACK(OnFileSaveAs))
		.Help(s_(DlgHelpEditorToolFileSaveAsHelp));
}
*/

/*
void DlgHelpEditor::OnFileSaveAs()
{
	SaveFile(Null);
}
*/

void DlgHelpEditor::ToolFileTopicIndex(Bar& bar)
{
	bar.Add(t_("Contents, index"), THISBACK(OnFileTopicIndex))
		.Help(t_("Show help contents and search index"));
}

void DlgHelpEditor::OnFileTopicIndex()
{
	OnTopicListCursor();
	OpenHelpTopicIndex(old_topic);
}

void DlgHelpEditor::ToolFileTopicView(Bar& bar)
{
	bar.Add(topic_list.IsCursor(), "Prohlížeè tématu", THISBACK(OnFileTopicView))
		.Help("Zobrazit aktivní téma v prohlížeèi dokumentace");
}

void DlgHelpEditor::OnFileTopicView()
{
	if(!topic_list.IsCursor()) {
		BeepExclamation();
		return;
	}
	String drl = topic_list.Get(C_DRL);
	RunDlgHelpTopic(drl);
}

void DlgHelpEditor::ToolFileClose(Bar& bar)
{
	bar.Add(t_("Close"), THISBACK(OnFileClose))
		.Help(t_("Close documentation editor"));
}

void DlgHelpEditor::OnFileClose()
{
//	if(PromptSave())
	AcceptBreak(IDOK);
}

bool DlgHelpEditor::PromptSave()
{
	OnTopicListCursor();
	if(!is_dirty)
		return true;
	switch(PromptYesNoCancel(t_("Do you want to save changes to the documentation?"))) {
	case 1:  return SaveFile();
	case 0:  return true;
	default: return false;
	}
}

void DlgHelpEditor::ToolEdit(Bar& bar)
{
/*	bar_edit->UndoTool(bar);
	bar_edit->RedoTool(bar);
	bar.Gap();
	bar_edit->CutTool(bar);
	bar_edit->CopyTool(bar);
	ToolEditPaste(bar);
	bar.Gap();
*/	last_edit->text.FindReplaceTool(bar);
	bar.Separator();
	ToolEditUp(bar);
	ToolEditDown(bar);
	ToolEditGoto(bar);
	bar.Separator();
	ToolEditFormula(bar);
	ToolEditLink(bar);
	ToolEditSection(bar);
	ToolEditNewTopic(bar);
}

/*
void DlgHelpEditor::ToolEditPaste(Bar& bar)
{
	bar.Add("Paste", CtrlImg::paste(), THISBACK(OnEditPaste))
	   .Key(K_CTRL_V);
}
*/

/*
void DlgHelpEditor::OnEditPaste()
{
	int rtf = GetClipboardFormatCode("Rich Text Format");
	if(!IsClipboardFormatAvailable(GetClipboardFormatCode("QTF"))
	&& IsClipboardFormatAvailable(rtf)) {
		String rtfdata = ReadClipboard(rtf);
		RichText rt = ParseRTF(rtfdata);
		if(!rt.IsEmpty()) {
			bar_edit->PasteText(rt);
			return;
		}
	}
	bar_edit->Paste();
}
*/

void DlgHelpEditor::ToolEditUp(Bar& bar)
{
	bar.Add(t_("Previous theme"), THISBACK(OnEditUp))
		.Key(K_ALT_UP)
		.Help(t_("Go up one theme in the list"));
}

void DlgHelpEditor::OnEditUp()
{
	if(topic_list.GetCursor() > 0)
		topic_list.SetCursor(topic_list.GetCursor() - 1);
	ActiveFocus(last_edit->text);
}

void DlgHelpEditor::ToolEditDown(Bar& bar)
{
	bar.Add(t_("Next topic"), THISBACK(OnEditDown))
		.Key(K_ALT_DOWN)
		.Help(t_("Go down one topic in the list"));
}

void DlgHelpEditor::OnEditDown()
{
	if(topic_list.GetCursor() + 1 < topic_list.GetCount())
		topic_list.SetCursor(topic_list.GetCursor() + 1);
	ActiveFocus(last_edit->text);
}

void DlgHelpEditor::ToolEditGoto(Bar& bar)
{
	bar.Add(t_("Go to..."), THISBACK(OnEditGoto))
		.Key(K_CTRL_D)
		.Help(t_("Locate help topic using initial letters or wildcard mask (*, ?)"));
}

RegisterHelpTopicObjectTitle(DlgHelpEditorGoto, t_("Go to topic"));

void DlgHelpEditor::OnEditGoto()
{
	WithHelpEditorGotoLayout<TopWindow> ffdlg;
	CtrlLayoutOKCancel(ffdlg, DlgHelpEditorGotoHelpTitle());
	ffdlg.HelpTopic("DlgHelpEditorGoto");
	ffdlg.list.AddColumn(t_("Code"), 10);
	DisplayTopicName tn;
	ffdlg.list.AddColumnAt(0, t_("Topic name"), 10).SetDisplay(tn);
	ffdlg.mask <<= ffdlg.Breaker(IDYES);
	int prev = 0;
	Vector<String> map;
	map <<= HelpTopicMap().GetKeys();
	Sort(map, GetLanguageInfo());

	for(;;) {
		ffdlg.list.Clear();
		String mask = ffdlg.mask.GetText().ToString();
		String rem = String::GetVoid();
		const char *best_err = NULL;

		for(int p = 0; p < map.GetCount(); p++) {
			String fn = map[p];
			String rr = rem;
			const char *errptr = FindTopicMatch(mask, fn, rr);
			if(!errptr) {
				rem = rr;
				ffdlg.list.Add(fn);
			}
			else if(!best_err || errptr > best_err)
				best_err = errptr;
		}
		if(ffdlg.list.GetCount() == 0) {
			int px = best_err - mask.Begin();
			ffdlg.mask.SetSelection(px, ffdlg.mask.GetLength());
			BeepExclamation();
		}
		else if(!rem.IsEmpty() && prev < mask.GetLength()) {
			int l, h;
			ffdlg.mask.GetSelection(l, h);
			if(l == mask.GetLength()) {
				ffdlg.mask.SetText((mask + rem).ToWString());
				ffdlg.mask.SetSelection(l, l + rem.GetLength());
			}
		}
		prev = mask.GetLength();
		if(ffdlg.list.GetCount() > 0)
			ffdlg.list.SetCursor(0);

		switch(ffdlg.Run()) {
		case IDCANCEL:
			return;

		case IDOK:
			if(ffdlg.list.IsCursor()) {
				if(topic_list.FindSetCursor(ffdlg.list.Get(0))) {
					ActiveFocus(last_edit->text);
					return;
				}
				BeepExclamation();
			}
			break;

		case IDYES:
			break;
		}
	}
}

void DlgHelpEditor::ToolEditFormula(Bar& bar)
{
	bar.Add(!!master, t_("Formula"), THISBACK(OnEditFormula))
		.Key(K_CTRL_W)
		.Help(t_("Open formula editor"));
}

void DlgHelpEditor::OnEditFormula()
{
	RichObjectExchangeNew ren(RichObjectTypeFormula());
	RichObjectTypeFormula()->DefaultAction(ren);
	if(!IsNull(ren.object.GetData())) {
		if(last_edit->text.IsSelection())
			last_edit->text.RemoveSelection();
		RichPara rpara;
		rpara.Cat(ren.object, RichPara::CharFormat());
		RichText rtext;
		rtext.Cat(rpara);
		last_edit->text.PasteText(rtext);
	//	BeepInformation();
		ActiveFocus(last_edit->text);
	}
}

void DlgHelpEditor::ToolSystem(Bar& bar)
{
//	ToolSystemExport(bar);
	ToolSystemExportProject(bar);
}

/*
void DlgHelpEditor::ToolSystemExport(Bar& bar)
{
	bar.Add(s_(DlgHelpEditorToolSystemExport), THISBACK(OnSystemExport))
		.Key(K_CTRL_E)
		.Help(s_(DlgHelpEditorToolSystemExportHelp));
}
*/

/*
void DlgHelpEditor::OnSystemExport()
{
	OnTopicListCursor();
	FileSel fsel;
	fsel.ActiveDir(recent_exp_dir);
	if(!fsel.ExecuteSelectDir(s_(DlgHelpEditorSystemExportDirSelTitle)))
		return;
	recent_exp_dir = ~fsel;
	Vector<String> mmap = HelpTopicEnumTextFolders(&StoredHelpTopicMap());
	Progress progress(s_(DlgHelpEditorSystemExportProgress), mmap.GetCount());
	int count_done = 0;
	for(int i = 0; i < mmap.GetCount(); i++) {
		String mod = mmap[i];
		String outfile = AppendFileName(~fsel, GetFileTitle(mod) + ".hpp");
		String diffile = HelpTopicSave(mod, StoredHelpTopicMap());
		if(IsNull(diffile))
			DeleteFile(outfile);
		else if(!::SaveFile(outfile, diffile)) {
			if(!PromptOKCancel(NFormat(s_(DlgHelpEditorSystemExportSaveError), outfile)))
				break;
		}
		else
			count_done++;
		if(progress.StepCanceled()) {
			PromptOK(s_(DlgHelpEditorSystemExportAbort));
			return;
		}
	}
	PromptOK(NFormat(s_(DlgHelpEditorSystemExportCountInfo),
		count_done, ~fsel));
}
*/

void DlgHelpEditor::ToolSystemExportProject(Bar& bar)
{
	bar.Add(t_("Export into project"), THISBACK(OnSystemExportProject))
		.Key(K_CTRL_E)
		.Help(t_("Export help topics into the application source code"));
}

RegisterHelpTopicObjectTitle(DlgHelpEditorProjectExport, t_("Export help into project"));

void DlgHelpEditor::OnSystemExportProject()
{
	OnTopicListCursor();
	Vector<String> mmap = HelpTopicEnumTextFolders();
	if(mmap.IsEmpty()) {
		Exclamation(t_("No help files found."));
		return;
	}

	int count_done = 0;
	WithHelpEditorExportProjectLayout<TopWindow> dlgexp;
	CtrlLayoutOKCancel(dlgexp, DlgHelpEditorProjectExportHelpTitle());
	dlgexp.list.AddColumn(t_("Folder"), 50);
	dlgexp.list.AddColumn("Poèet", 20);
//	dlgexp.list.AddColumn(s_(DlgHelpEditorSystemExportProjectLengthColumn), 20);
//	dlgexp.list.AddColumn(s_(DlgHelpEditorSystemExportProjectNewColumn), 20);
	ArrayOption todo;
	todo.AddColumn(dlgexp.list, t_("Overwrite"), 10);
	dlgexp.list.AddIndex(); // data
	Progress progress("Procházím seznam témat...", mmap.GetCount());
	String host_dph_fn = AppendFileName(HelpTopicHostDir(), "host.dph");
	String host_dph;
	for(int i = 0; i < mmap.GetCount(); i++) {
		if(progress.StepCanceled()) {
			Exclamation(t_("Export aborted by user."));
			return;
		}
		int fdir = HelpTopicGetFolderDirty(mmap[i], StoredHelpTopicMap());
		dlgexp.list.Add(mmap[i], fdir, fdir > 0);
	}

	dlgexp.list.SetCursor(0);
	if(dlgexp.Run() != IDOK)
		return;

	Vector<String> files; {
		Progress progress(t_("Exporting changed files"), dlgexp.list.GetCount());
		for(int i = 0; i < dlgexp.list.GetCount(); i++) {
			if(progress.StepCanceled()) {
				Exclamation(t_("Export aborted by user."));
				return;
			}
			dlgexp.list.SetCursor(i);
			bool do_save = (int)dlgexp.list.Get(2);

			String fold = dlgexp.list.Get(0);
//				String data = dlgexp.list.Get(4);
			try {
				host_dph << HelpTopicSave(files, fold, StoredHelpTopicMap(), Null, !do_save);
			}
			catch(Exc e) {
				if(!PromptOKCancel(NFormat(t_("Error exporting folder [* \1%s\1]: [* \1%s\1]. Continue?"), fold, e)))
					return;
			}
		}
	}
	if(!IsSameTextFile(LoadFile(host_dph_fn), host_dph)) {
		RealizePath(host_dph_fn);
		if(!::SaveFileBackup(host_dph_fn, host_dph))
			Exclamation(NFormat("Nelze uložit soubor [* \1%s\1].", host_dph_fn));
		else
			files.Add(host_dph_fn);
	}

	if(files.IsEmpty())
		Exclamation(t_("No help files were saved."));
	else {
		dlgexp.Close();
		WithHelpEditorSavedFilesLayout<TopWindow> sfdlg;
		sfdlg.Sizeable().MaximizeBox();
		CtrlLayoutExit(sfdlg, "Export byl úspìšnì dokonèen");
		sfdlg.list.AddColumn();
		Append(sfdlg.list, files);
		sfdlg.count.SetLabel(FormatInt(files.GetCount()));
		sfdlg.Run();
	}
}

/*
void DlgHelpEditor::OnEditDialog()
{
	DlgHelpEditorDialogObject dlg;
	if(dlg.Run(recent_dlg)) {
		RichPara para;
		para.Cat(RichObject(RichObjectTypeDialogHelp(), recent_dlg));
		RichText text;
		text.Cat(para);
		String s = AsQTF(text);
		LOG("RichText: " << s);
		LOGHEXDUMP(s, s.GetLength());
#ifdef PLATFORM_WIN32
		WriteClipboard(RegisterClipboardFormat("Ultimate Development QTF format"), s);
#else
		WriteClipboardText(s);
#endif
//		editor.Paste();
	}
}
*/

void DlgHelpEditor::ToolEditLink(Bar& bar)
{
	bar.Add(topic_list.IsCursor(), "Odkaz", THISBACK(OnEditLink))
		.Key(K_CTRL_L)
		.Help("Vložit do textu odkaz na zvolené téma nápovìdy");
}

static int LocateField(const RichText& rt, int cursor, Id fieldid, String& param)
{
	RichPos rpos;
	if(cursor < rt.GetLength() && (rpos = rt.GetRichPos(cursor)).field == fieldid) {
		param = rpos.fieldparam;
		return cursor;
	}
	if(cursor > 0 && (rpos = rt.GetRichPos(cursor - 1)).field == fieldid) {
		param = rpos.fieldparam;
		return cursor - 1;
	}
	return -1;
}

void DlgHelpEditor::OnEditLink()
{
	if(!topic_list.IsCursor()) {
		BeepExclamation();
		return;
	}
	DlgHelpEditorLink linkdlg;
//	String s, n, t;
//	HelpParseDPP((String)topic_list.Get(C_DRL), s, n, t);
	String drl, text, style;
	String param;
	int cur = last_edit->text.GetCursor();
	const RichText& rt = last_edit->text.Get();
	int old_pos = LocateField(rt, cur, FieldTypeLinkID(), param);
	int old_len = 0;
	RichPara::CharFormat fmt = last_edit->text.GetFormatInfo();
	if(old_pos >= 0) {
		old_len = 1;
		DecodeLink(param, drl, text, style);
		fmt = rt.GetFormatInfo(old_pos, 1);
	}
	else {
		old_pos = cur;
		int l, h;
		if(cur < rt.GetLength()) {
			RichText::FormatInfo fi = rt.GetFormatInfo(cur, 1);
			if(!IsNull(fi.link)) {
				while(old_pos > 0 && rt.GetFormatInfo(old_pos - 1, 1).link == fi.link)
					--old_pos;
				while(cur < rt.GetLength() && rt.GetFormatInfo(cur, 1).link == fi.link)
					cur++;
				old_len = cur - old_pos;
				drl = fi.link;
				WString wtext;
				for(int i = 0; i < old_len; i++)
					wtext.Cat(last_edit->text[old_pos + i]);
				text = wtext.ToString();
			}
			else if(last_edit->text.GetSelection(l, h)) {
				old_pos = l;
				old_len = h - l;
				WString rtt;
				while(l < h)
					rtt.Cat(rt[l++]);
				text = rtt.ToString();
			}
			fmt = rt.GetFormatInfo(old_pos, old_len);
			fmt.link = Null;
		}
	}
	if(linkdlg.Run(/*Nvl(s, "::"), Nvl(n, "::"),*/ drl, text, style)) {
		if(old_len > 0) {
			last_edit->text.Move(old_pos, false);
			last_edit->text.RemoveText(old_len);
		}
		RichPara para;
		para.Cat(FieldTypeLinkID(), EncodeLink(drl, text, style), fmt);
		RichText text;
		text.Cat(para);
		last_edit->text.Move(old_pos, false);
		last_edit->text.PasteText(text);
	}
}

void DlgHelpEditor::ToolEditSection(Bar& bar)
{
	bar.Add(topic_list.IsCursor(), "Tiskový oddíl", THISBACK(OnEditSection))
		.Help("Vložit do textu znaèku zaèátku nového tiskového oddílu");
}

void DlgHelpEditor::OnEditSection()
{
	if(!topic_list.IsCursor()) {
		BeepExclamation();
		return;
	}
	DlgHelpEditorSection secdlg;
	RichTextSection sect;
	int cur = last_edit->text.GetCursor();
	String param;
	int pos = LocateField(last_edit->text.Get(), cur, FieldTypeSectionID(), param);
	if(pos >= 0)
		sect = DecodeSection(param);
	if(secdlg.Run(sect)) {
		RichPara::CharFormat fmt = last_edit->text.GetFormatInfo();
		if(pos >= 0) {
			last_edit->text.Move(pos, false);
			last_edit->text.RemoveText(1);
		}
		RichPara para;
		para.Cat(FieldTypeSectionID(), EncodeSection(sect), fmt);
		RichText text;
		text.Cat(para);
		last_edit->text.Move(cur, false);
		last_edit->text.PasteText(text);
	}
}

void DlgHelpEditor::ToolEditNewTopic(Bar& bar)
{
	bar.Add("Nové téma", THISBACK(OnEditNewTopic))
		.Help("Vytvoøit nové téma nápovìdy");
}

void DlgHelpEditor::OnEditNewTopic()
{
	String drl;
	if(topic_list.IsCursor())
		drl = topic_list.Get(C_DRL);
	String space, nesting, topic, folder;
	HelpParseDPP(drl, space, nesting, topic);
	DlgHelpEditorNewTopic newdlg;
	while(newdlg.Run(space, nesting, topic, folder)) {
		drl = HelpFormatDPP(space, nesting, topic);
		if(HelpTopicMap().Find(drl) >= 0) {
			Exclamation(NFormat("Objekt [* \1%s\1] již v systému dokumentace existuje.", drl));
			continue;
		}
		HelpTopicSet(drl, AppendFileName(folder, ""), ~edit.language, "", "");
		dpp_space <<= space;
		OnDPPSpace();
		dpp_nesting <<= nesting;
		OnDPPNesting();
		topic_list.FindSetCursor(drl);
		return;
	}
}

bool DlgHelpEditor::SaveTopic(String topic_name, int lang, const WithHelpEditorEditLayout<TopWindow>& editor)
{
	_sPosMap().GetAdd(topic_name).GetAdd(lang) = editor.text.GetCursor();
	if(IsNull(topic_name) || IsNull(lang) || !editor.title.IsModified() && !editor.text.IsModified())
		return false;
	if(lang != GetCurrentLanguage())
		SetLanguage(lang);
	int s = HelpTopicMap().Find(HelpDPPStyle());
	int f = HelpTopicMap().Find(topic_name);
	if(f < 0)
		return false;
	byte cs = GetLangStdCharset(lang);
	HelpTopicInfo& topic = HelpTopicMap()[f];
	String old_tit = ~editor.title;
	String old_styles = StylesAsQTF(editor.text.Get(), cs);
	String old_txt;
	if(!editor.text.Get().IsEmpty())
	 	old_txt = BodyAsQTF(editor.text.Get(), cs);
	if(old_tit == topic.GetDefaultTitle())
		old_tit = Null;
	String tqtf;
	if(s >= 0)
		tqtf = HelpTopicMap()[s].GetText(lang) + "\r\n";
	if(old_txt == BodyAsQTF(ParseQTF(tqtf + topic.GetDefaultText()), cs)) //topic.GetText(old_language))
		old_txt = Null;
	bool is_dirty = (old_tit != topic.GetTitle(lang)
	|| old_txt != BodyAsQTF(ParseQTF(tqtf + topic.GetText(lang)), cs));
	topic.Set(lang, old_tit, old_txt);
	if(s >= 0)
		HelpTopicMap()[s].Set(lang, Null, old_styles);
	return is_dirty;
}

String DlgHelpEditor::LoadTopic(String topic_name, int lang, WithHelpEditorEditLayout<TopWindow>& editor)
{
	int s = HelpTopicMap().Find(HelpDPPStyle());
	int f = HelpTopicMap().Find(topic_name);
	bool is_topic = (f >= 0 && !IsNull(lang));
	editor.title.Enable(is_topic);
	editor.text.Enable(is_topic);
	String app_title, title;
	RichText text;
	if(is_topic) {
		if(lang != GetCurrentLanguage() && lang != 0 && !IsNull(lang))
			SetLanguage(lang);
		RichText styles;
		if(s >= 0)
			styles = ParseQTF(HelpTopicMap()[s].GetText(lang));
		HelpTopicInfo& topic = HelpTopicMap()[f];
		title = topic.GetTitle();
		text = ParseQTF(topic.GetText());
		text.OverrideStyles(styles.GetStyles(), true);
		app_title = NFormat(t_("%s: %s - documentation editor"), topic_name, title);
	}
	editor.title <<= title;
	editor.text.Pick(text);
	editor.title.ClearModify();
	editor.text.ClearModify();
	int pos = _sPosMap().GetAdd(topic_name).Get(lang, 0);
	if(pos >= 0 && pos <= editor.text.GetLength())
		editor.text.Move(pos);
	return app_title;
}

void DlgHelpEditor::OnTopicListCursor()
{
	int lang = GetCurrentLanguage();
	if(!IsNull(old_topic)) {
		String space, nesting, item;
		HelpParseDPP(old_topic, space, nesting, item);
		if(nesting != "sys") {
			if(SaveTopic(old_topic, old_lang1, edit))
				is_dirty = true;
			if(SaveTopic(old_topic, old_lang2, aux_edit))
				is_dirty = true;
			int ff = topic_list.Find(old_topic);
			if(ff >= 0)
				topic_list.InvalidateCache(ff);
	//			topic_list.Set(ff, 1, !IsNull(old_txt) ? 1 : 0);
		}
	}
	old_topic = Null;
	if(topic_list.IsCursor())
		old_topic = topic_list.Get(0);
	old_lang1 = ~edit.language;
	old_lang2 = ~aux_edit.language;

	String app_title = Nvl(LoadTopic(old_topic, old_lang1, edit), LoadTopic(old_topic, old_lang2, aux_edit));
	if(GetCurrentLanguage() != lang)
		SetLanguage(lang);
	if(IsNull(app_title))
		app_title = t_("Documentation editor"),
	Title(MQTFStrip(app_title).ToWString());
}

void DlgHelpEditor::UpdateAvail()
{
//	int cs = topic_list.GetCursor();
//	topic_list.KillCursor();
	int lng = ~edit.language;
	topic_list.ClearCache();
//	const HelpTopicInfoMap& hmap = HelpTopicMap();
//	for(int i = 0; i < topic_list.GetCount(); i++)
//	{
//		String topic = topic_list.Get(i, 0);
//		int f = hmap.Find(topic);
//		topic_list.Set(i, C_, f >= 0 && !IsNull(hmap[f].GetText(lng)) ? 1 : 0);
//	}
//	if(cs >= 0)
//		topic_list.SetCursor(cs);
}

void DlgHelpEditor::OnTopicLanguage()
{
	int c = topic_list.GetCursor();
	topic_list.KillCursor();
	int lang = ~edit.language;
	byte cs = GetLangStdCharset(lang);
	cv_para.Language(lang);
	cv_index.Language(lang);
	topic_list.ClearCache();
//	edit.topic_title.Charset(cs);
//	topic_text.Charset(cs);
	UpdateAvail();
	if(c >= 0)
		topic_list.SetCursor(c);
}

void DlgHelpEditor::OnAuxLanguage()
{
	OnTopicListCursor();
}

void DlgHelpEditor::OnTopicListAction()
{
//	topic_list.Sync();
}

void DlgHelpEditor::ChildGotFocus()
{
	if(edit.HasFocusDeep() && last_edit != &edit) {
		last_edit = &edit;
		Rescan();
	}
	else if(aux_edit.HasFocusDeep() && last_edit != &aux_edit) {
		last_edit = &aux_edit;
		Rescan();
	}
}

bool DlgHelpEditor::HotKey(dword key)
{
	if(key == K_ESCAPE) {
		vsplitter.Zoom(vsplitter.GetZoom() == -1 ? 0 : -1);
		ActiveFocus(edit.text);
		return true;
	}
	return Ctrl::HotKey(key);
}

void DlgHelpEditor::OnScreenshot()
{
	if(!screenshot)
		return;
	Value v = ~*screenshot;
	if(IsNull(v))
		return;
	if(IsTypeRaw<Image>(v)) {
		Image dwg = v;
		Size dot_size = PixelsToDots(ScreenInfo(), dwg.GetSize());
		RichObject object("PNG", PngEncoder().SaveImage(dwg), dot_size);
		//CreateDrawingObject(dwg, dot_size);
		if(last_edit->text.IsSelection())
			last_edit->text.RemoveSelection();
		RichPara rpara;
		rpara.Cat(object, last_edit->text.GetFormatInfo());
		RichText rtext;
		rtext.Cat(rpara);
		last_edit->text.PasteText(rtext);
	//	BeepInformation();
		ActiveFocus(last_edit->text);
	}
}

void DlgHelpEditor::OnDPPSpace()
{
	Vector<String> nest = HelpTopicEnumNesting(~dpp_space);
	dpp_nesting.Clear();
	dpp_nesting.Add(Null, t_("(all)"));
	for(int i = 0; i < nest.GetCount(); i++)
		dpp_nesting.Add(Nvl(nest[i], "::"));
	dpp_nesting <<= Null;
	if(nest.GetCount() == 1)
		dpp_nesting.SetIndex(1);
	OnDPPNesting();
}

void DlgHelpEditor::OnDPPNesting()
{
	String sp = ~dpp_space, ne = ~dpp_nesting;
	Vector<String> names = HelpTopicList(sp, ne);
	topic_list.Clear();
	topic_list.SetCount(names.GetCount());
	int i;
	for(i = 0; i < names.GetCount(); i++) {
		String drl = names[i], name;
		if(!IsNull(sp) || !IsNull(ne)) {
			String space, nesting, topic;
			HelpParseDPP(drl, space, nesting, topic);
			if(IsNull(space))
				space = "::";
			if(IsNull(nesting))
				nesting = "::";
			if(IsNull(sp))
				if(IsNull(ne))
					name << space << "//" << nesting << "//";
				else
					name << space << "//";
			else if(IsNull(ne))
				name << nesting << "//";
			name << topic;
		}
		topic_list.Set(i, C_DRL, drl);
		topic_list.Set(i, C_NAME, Nvl(name, drl));
	}

	if(!IsNull(init_topic)) {
		if(!topic_list.FindSetCursor(init_topic, C_DRL)
		&& !topic_list.FindSetCursor(HelpAppDPP(init_topic), C_DRL))
			topic_list.SetCursor(0);
		init_topic = Null;
	}
}

void DlgHelpEditor::UpdateIconModules()
{
	Index<String> mods;
	const Vector<String>& names = ImageCache::Get().GetNameMap().GetKeys();
	for(int i = 0; i < names.GetCount(); i++) {
		const char *p = names[i], *e = p;
		while(*e && !(e[0] == ':' && e[1] == ':'))
			e++;
		if(*e && e > p)
			mods.FindAdd(String(p, e));
	}
	Vector<String> mlist = mods.PickKeys();
	Sort(mlist, GetLanguageInfo());
	aux_icons.module.Clear();
	aux_icons.module.Add(Null, "(všechny)");
	for(int i = 0; i < mlist.GetCount(); i++)
		aux_icons.module.Add(mlist[i], mlist[i]);
}

void DlgHelpEditor::UpdateIconGallery()
{
	aux_icons.gallery.Clear();
	aux_icons.gallery.Enable(aux_icons.module.IsCursor());
	Size limit_size(aux_icons.gallery.GetIconWidth(), aux_icons.gallery.GetItemHeight());
	if(aux_icons.module.IsCursor()) {
		String module = aux_icons.module.Get(0);
		int ml = module.GetLength();
		const VectorMap<String, Image>& icons = ImageCache::Get().GetNameMap();
		Vector<String> out;
		Vector<Image> outicon;
		for(int i = 0; i < icons.GetCount(); i++) {
			String n = icons.GetKey(i);
			if(ml) {
				if(memcmp(n, module, ml) || n[ml] != ':' || n[ml + 1] != ':')
					continue;
				n.Remove(0, ml + 2);
			}
			out.Add(n);
			Image out_image = icons[i];

			if(out_image.GetHeight() > limit_size.cx || out_image.GetWidth() > limit_size.cy) {
				Size outsize = GetFitSize(out_image.GetSize(), limit_size);
				AlphaArray array = ImageToAlphaArray(out_image);
				AlphaArray out(outsize, -3);
				PixelCopyAntiAliasMaskOut(out, outsize, array, array.GetSize(), false);
				out_image = AlphaArrayToImage(out);
			}
			outicon.Add(out_image);
		}
		IndexSort(out, outicon, GetLanguageInfo());
		for(int i = 0; i < out.GetCount(); i++) {
			aux_icons.gallery.Add(out[i], outicon[i]);
		}
	}
}

void DlgHelpEditor::OnPasteIcon()
{
	if(!aux_icons.Accept())
		return;
	if(!aux_icons.module.IsCursor() || !aux_icons.gallery.IsCursor()) {
		BeepExclamation();
		return;
	}
	String module = aux_icons.module.Get(0);
	if(!IsNull(module))
		module.Cat("::");
	module.Cat(aux_icons.gallery.GetCurrentName());
	Image img = ImageCache::Get().GetNameMap().Get(module, Null);
	if(IsNull(img)) {
		BeepExclamation();
		return;
	}
	const ColorF *border = NULL;
	switch((int)~aux_icons.frame) {
		case IFRM_BUTTON: border = ButtonBorder(); break;
		case IFRM_INSET:  border = InsetBorder(); break;
	}
	int width = (border ? (int)*border + 2 : 0);
	Size outsize = img.GetSize() + 2 * width;
	Image outimg(outsize);
	ImageDraw idraw(outimg);
	idraw.DrawRect(outsize, width ? SLtGray() : White());
	DrawBorder(idraw, outsize, border);
	idraw.DrawImage(width, width, img, aux_icons.etched ? Image::ETCHED : aux_icons.grayed ? Image::GRAYED : 0);
	idraw.Close();
	Size dot_size = PixelsToDots(ScreenInfo(), outimg.GetSize());
	RichObject object("PNG", PngEncoder().DotSize(dot_size).SaveImage(outimg));
	object.SetSize(object.GetPhysicalSize() * (int)~aux_icons.size / 100);
	object.KeepRatio(true);
	object.SetYDelta(fround((double)~aux_icons.raise * (-600.0 / 72.0)));
	//CreateDrawingObject(dwg, dot_size);
	if(last_edit->text.IsSelection())
		last_edit->text.RemoveSelection();
	RichPara rpara;
	rpara.Cat(object, last_edit->text.GetFormatInfo());
	RichText rtext;
	rtext.Cat(rpara);
	last_edit->text.PasteText(rtext);
//	BeepInformation();
	ActiveFocus(last_edit->text);
}

#endif
