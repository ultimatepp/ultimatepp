#include "TCtrlLibHelp.h"
#pragma hdrstop
#pragma BLITZ_APPROVE

#include <DocTypes/DocTypes.h>

NAMESPACE_UPP

#define LAYOUTFILE <TCtrlLib/Help/DlgHelpView.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE  <TCtrlLib/Help/DlgHelpView.iml>
#define IMAGECLASS DlgHelpViewImg
#include           <Draw/iml.h>

class ReportPageDraw : public PageDraw
{
public:
	ReportPageDraw(Size page_size);

	void          Write(DocReport& output);

	virtual Draw& Info() { return Page(0); }
	virtual Draw& Page(int i);

private:
	Array<DrawingDraw> pages;
	Size               page_size;
};

ReportPageDraw::ReportPageDraw(Size page_size)
: page_size(page_size)
{
	Page(0);
}

Draw& ReportPageDraw::Page(int i)
{
	while(pages.GetCount() <= i)
		pages.Add().Create(page_size);
	return pages[i];
}

void ReportPageDraw::Write(DocReport& report)
{
	for(int i = 0; i < pages.GetCount(); i++) {
		if(i)
			report.NextPage();
		report.DrawDrawing(Rect(page_size), pages[i]);
	}
}

/*
const Color *ThickWhiteBorder[] = {
	(Color *)4,
	&SWhite,  &SWhite,  &SWhite,  &SWhite,
	&SWhite,  &SWhite,  &SWhite,  &SWhite,
	&SWhite,  &SWhite,  &SWhite,  &SWhite,
	&SWhite,  &SWhite,  &SWhite,  &SWhite,
};

GLOBAL_VARP(BorderFrame, ThickWhiteFrame, (ThickWhiteBorder));
*/

INITBLOCK { RegisterHelpStdItems(); }

class DlgHelpTopicTab : public WithHelpTopicTabLayout<TopWindow>
{
public:
	typedef DlgHelpTopicTab CLASSNAME;
	DlgHelpTopicTab();

	String                              Run(String init, bool index);
	virtual bool                        HotKey(dword key);

private:
	void                                OnLink(const String& link);
	void                                OnOK();
	void                                OnTab();
	void                                OnSearch();

private:
	enum { TOC_TAB, INDEX_TAB, TOPICS_TAB };
	WithHelpTopicIndexLayout<TopWindow> index_page;
	RichTextView                        toc_page;
	RichTextView                        topics_page;
	String                              selection;
	Array<HelpIndexFormattedEntry>      entries;
	bool                                toc_inited;
	bool                                topics_inited;
};

String RunDlgHelpTopicIndex(String init, bool index) { return DlgHelpTopicTab().Run(init, index); }

void OpenHelpTopicIndex(String topic, bool index)
{
	String s = RunDlgHelpTopicIndex(topic, index);
	if(!IsNull(s))
		RunDlgHelpTopic(s);
}

//RegisterHelpTopicObjectTitle(DlgHelpTopicTab, t_("Nápovìda"))

DlgHelpTopicTab::DlgHelpTopicTab()
{
	toc_inited = topics_inited = false;

	CtrlLayoutOKCancel(*this, t_("Help system"));
	HelpTopic("topic://TCtrlLib/DlgHelpTopicTab");

	Sizeable().Zoomable();

	CtrlLayout(index_page);
	ASSERT(tab.GetCount() == TOC_TAB);
	tab.Add(toc_page.HSizePos(4, 4).VSizePos(4, 4), t_("Contents"));
	ASSERT(tab.GetCount() == INDEX_TAB);
	tab.Add(index_page.HSizePos(4, 4).VSizePos(4, 4), t_("Index"));
	ASSERT(tab.GetCount() == TOPICS_TAB);
	tab.Add(topics_page.HSizePos(4, 4).VSizePos(4, 4), t_("All topics"));
	tab <<= THISBACK(OnTab);

	toc_page.Margins(4);
	toc_page.SetZoom(Zoom(180, 1024));
	toc_page.WhenLink = THISBACK(OnLink);

	topics_page.Margins(4);
	topics_page.SetZoom(Zoom(180, 1024));
	topics_page.WhenLink = THISBACK(OnLink);

	index_page.search <<= THISBACK(OnSearch);
//	index_page.list.AutoHideSb().NoHeader().NoGrid();
	index_page.list.AddIndex();
	index_page.list.AddColumn().SetDisplay(RichTextDisplay());
	ok <<= index_page.list.WhenLeftDouble = THISBACK(OnOK);
}

String DlgHelpTopicTab::Run(String init, bool index)
{
	tab <<= (index ? INDEX_TAB : TOC_TAB);
	OnTab();
	if(TopWindow::Run() != IDOK)
		return Null;
	return selection;
}

void DlgHelpTopicTab::OnLink(const String& link)
{
	selection = link;
	AcceptBreak(IDOK);
}

//RegisterHelpTopicObjectTitle(DlgHelpTopicSelect, t_("Výbìr tématu nápovìdy"))

void DlgHelpTopicTab::OnOK()
{
	if(tab.Get() == INDEX_TAB && index_page.list.IsCursor()) {
		selection = index_page.list.Get(0);
		if(*selection == '!')
			selection.Remove(0);
		if(selection.Find(';') >= 0) { // select topic
			WithHelpTopicSelectLayout<TopWindow> dlg;
			CtrlLayoutOKCancel(dlg, t_("Help topic selection"));
			dlg.list.AddIndex();
			dlg.list.AddColumn().SetDisplay(RichTextDisplay());
			dlg.list.WhenLeftDouble = dlg.Acceptor(IDOK);
			const char *p = selection;
			while(*p) {
				while(*p == '!' || *p == ' ')
					p++;
				const char *b = p;
				while(*p && *p != ';')
					p++;
				if(p > b) {
					String key(b, p);
					dlg.list.Add(key, HelpTopicGet(key).GetTitle());
				}
				if(*p)
					p++;
			}
			if(dlg.list.GetCount() == 0) {
				Exclamation(NFormat(t_("Invalid help topic [* \1%s\1]."), selection));
				return;
			}
			dlg.list.SetCursor(0);
			if(dlg.list.GetCount() > 1 && dlg.Run() != IDOK)
				return;
			selection = dlg.list.Get(0);
		}
		AcceptBreak(IDOK);
	}
}

void DlgHelpTopicTab::OnTab()
{
	switch(tab.Get()) {
		case TOC_TAB: {
			if(!toc_inited) {
				toc_inited = true;
				RichText toc;
				HelpTopicOnlineToc()(toc);
				toc_page.Pick(toc);
			}
			break;
		}

		case TOPICS_TAB: {
			if(!topics_inited) {
				topics_inited = true;
				RichText topics;
				String lang = LNGAsText(GetCurrentLanguage());
				if(lang.GetLength() > 5)
					lang.Trim(5);
				String fend;
				fend << '$' << ToLower(lang);
				VectorMap<String, Topic> dummy;
				Vector<int> order = GetSortOrder(TopicBase().GetKeys(), GetLanguageInfo());
				for(int po = 0; po < order.GetCount(); po++) {
					int p = order[po];
					String package = TopicBase().GetKey(p);
					if(TopicBase()[p].Find("appdoc") >= 0) {
						bool first = true;
						const Vector<String>& group = TopicBase()[p].Get("appdoc");
						Vector<int> gorder = GetSortOrder(group, GetLanguageInfo());
						for(int io = 0; io < gorder.GetCount(); io++) {
							int i = gorder[io];
							String t = group[i];
							String topic;
							topic << "topic://" << package << "/appdoc/" << t;
							int l = t.Find('$');
							if(l >= 0 && t.Mid(l) == fend) {
								if(first) {
									RichPara head;
									head.format.before = 100;
									head.Cat(package, head.format);
									topics.Cat(head);
									first = false;
								}
								RichPara para;
								para.format.lm = 150;
								para.format.link = topic;
								para.Cat(t.Left(l), para.format);
								topics.Cat(para);
							}
						}
					}
				}
				topics_page.Pick(topics);
			}
			break;
		}
		case INDEX_TAB: {
			if(entries.IsEmpty()) {
				index_page.list.Clear();
				entries = HelpTopicFormatIndex();
				for(int i = 0; i < entries.GetCount(); i++)
					if(entries[i].nesting >= 0) {
						WString s;
						s.Cat('_', entries[i].nesting * 3);
						s.Cat(entries[i].text);
						index_page.list.Add(entries[i].topics, s);
					}

					else
						entries.Remove(i--);
			}
			break;
		}
	}
}

static bool LessEntry(const HelpIndexFormattedEntry& entry, const WString& s)
{
	return GetLanguageInfo().Compare(FromUnicode(MQTFStrip(entry.full_text)), FromUnicode(s)) < 0;
}

void DlgHelpTopicTab::OnSearch()
{
	WString text = ~index_page.search;
	int n = BinFindIndex(entries, index_page.search.GetText(), &LessEntry);
	if(n < index_page.list.GetCount())
		index_page.list.SetCursor(n);
}

bool DlgHelpTopicTab::HotKey(dword key)
{
	if(key >= ' ' && key <= 255 || key == K_BACKSPACE
		|| key == K_LEFT || key == K_RIGHT || key == K_HOME || key == K_END) {
		int l = index_page.search.GetLength(), h = l;
		index_page.search.GetSelection(l, h);
		index_page.search.SetFocus();
		index_page.search.SetSelection(l, h);
		return index_page.search.Key(key, 1);
	}
	return TopWindow::HotKey(key);
}

GLOBAL_VAR(Callback1<Bar&>, HelpTopicBookMenu)
GLOBAL_VAR(Callback1<RichText&>, HelpTopicOnlineToc)

class DlgHelpTopic : public TopWindow
{
public:
	typedef DlgHelpTopic CLASSNAME;
	DlgHelpTopic();

	static ConfigItem&    config();
	virtual void          Serialize(Stream& stream);
	void                  Run(String topic);

	virtual void          Activate();

private:
	void                  OnLink(const String& link);
	bool                  OpenPage(String page, int y = 0);

	void                  Rescan() { toolbar.Set(THISBACK(ToolMain)); }

	TOOL(Main)
		TOOL(Zoom)
		TOOL(Back)
		TOOL(Forward)
		TOOL(Index)
		TOOL(Print)
//		TOOL(BookMenu)
//		TOOL(Book)
//		TOOL(BookPDF)
//		TOOL(BookRTF)

	void                  AddRecent(String page, int y);
	void                  SeekAnchor(const char *anchor);
	void                  SyncPage(WString title, String text /*, String style*/);

private:
	ToolBar               toolbar;
	RichTextView          viewer;
//	QtfCtrl               qtf;
	StatusBar             statusbar;
//	SliderCtrl            zoom;
//	Point                 book_menu_pos;
//	int                   help_topic_seq;

//	const HelpPage       *current_page;
	String                current_topic;
	WString               title;
	String                topic_qtf;
//	String                style_qtf;

	static Vector<String> recent_pages;
	static Vector<int>    recent_y;
	static int            recent_pos;
	static int            recent_zoom;
	enum { MAX_RECENT = 100 };
};

Vector<String> DlgHelpTopic::recent_pages;
Vector<int>    DlgHelpTopic::recent_y;
int            DlgHelpTopic::recent_pos = 0;
int            DlgHelpTopic::recent_zoom = DOC_SCREEN_ZOOM;

void RunDlgHelpTopic(String topic) { DlgHelpTopic().Run(topic); }

DlgHelpTopic::DlgHelpTopic()
//: current_page(0)
{
//	help_topic_seq = 0;

	Title(t_("Help viewer"));
	MaximizeBox().Sizeable();

//	zoom.SetRect(0, 0, 60, 16);
//	zoom.MinMax(100, 300).Step(10);
//	zoom <<= THISBACK(OnZoom);
//	zoom <<= recent_zoom;

//	dialog.WhenClose = dialog.Breaker(IDCANCEL); // TopWindow WhenClose bug; remove when corrected
	AddFrame(toolbar);
	AddFrame(statusbar);

	Add(viewer.SizePos());
	viewer.Margins(4);
//	viewer.SetZoom(Zoom(180, 1024));
	viewer.WhenLink = THISBACK(OnLink);
//	viewer.SetReadOnly();
//	Add(qtf.SizePos());
//	qtf.WhenLink = THISBACK(OnLink);
//	qtf.BackPaint();
//	qtf.WantFocus();
//	qtf.SetFrame(InsetFrame());
//	qtf.Background(White).Margins(10);
//	qtf.Zoom(160);
}

CONFIG_ITEM(DlgHelpTopic::config, "DlgHelpTopic", 1, 1, 1)

void DlgHelpTopic::Serialize(Stream& stream)
{
	SerializePlacement(stream);
}

void DlgHelpTopic::Run(String topic)
{
	ReadConfigSelf();
	OnZoom(); // update zoom
	if(OpenPage(topic)) {
		Open();
		AddRecent(topic, viewer.GetSb());
		TopWindow::Run();
		WriteConfigSelf();
	}
}

void DlgHelpTopic::Activate()
{
/*
	if(!IsNull(current_topic) && HelpTopicGetSeq() != help_topic_seq) {
		help_topic_seq = HelpTopicGetSeq();
		const HelpTopicInfo& info = HelpTopicGet(current_topic);
		String title = info.GetTitle();
		String text = info.GetText();
		String style = HelpTopicGet(HelpDPPStyle()).GetText();
		if(title != title_qtf || text != topic_qtf / * || style != style_qtf* /)
			SyncPage(title, text / *, style* /);
	}
*/
}

bool DlgHelpTopic::OpenPage(String topic, int y)
{
	Topic topicdata = GetTopic(topic);
	if(IsNull(topicdata.text)) {
		String langcode = LNGAsText(GetCurrentLanguage() & ~(-1 << 20));
		String ltopic = topic;
		ltopic << '$' << ToLower(langcode);
		topicdata = GetTopic(ltopic);
	}
/*
	const ArrayMap<String, HelpTopicInfo>& map = HelpTopicMap();
	int t = map.Find(topic);
	if(t < 0) {
		String appdoc = HelpAppDPP(topic);
		t = map.Find(appdoc);
		if(t < 0) {
			Exclamation(NFormat("Téma nápovìdy [* \1%s\1] nebylo v systému nalezeno.", topic));
			return false;
		}
		topic = appdoc;
	}
	//HelpIndex::Get().AddVisited(page.GetID()); // mark page as visited

	//int lang = GetCurrentLanguage();
	const HelpTopicInfo& info = map[t];
	String title = info.GetTitle();
	String text = info.GetText();
*/
	String title = topicdata.title;
	String text = topicdata.text;
	if(IsNull(text)) {
		Exclamation(NFormat(t_("Help topic [* \1%s\1] not found."), topic));
		return false;
	}
//	String style = HelpTopicGet(HelpDPPStyle()).GetText();
	current_topic = topic;
	HelpTopic(current_topic);
	SyncPage(FromUtf8(title), text /*, style*/);
	ActiveFocus(viewer);
	viewer.SetSb(y);

	return true;
}

void DlgHelpTopic::SyncPage(WString title, String text /*, String style*/)
{
	title = title;
	topic_qtf = text;
//	style_qtf = style;

	WString dlgt = title;
	if(IsNull(dlgt))
		dlgt = t_("Help viewer");
	else
		dlgt << t_(" - help viewer");
	Title(dlgt);

/*
	RichText out;

	RichPara title_para;
	title_para.format.after = 150;
	title_para.format.tab.Add().pos = 10000;
	RichPara::CharFormat format;
//	format.paper = Color(224, 224, 224);
	format.Face(Font::ARIAL).Height(150).Bold();
	MQTFCat(title_para, title_qtf, format);

	out.Cat(title_para);
	AppendQTF(out, style_qtf + "\r\n" + topic_qtf);
*/
//	int progress_pos;
//	Progress progress("Pøipravuji téma (%d)");
	RichText txt = ParseQTF(topic_qtf);
	RichText out;
	out.SetStyles(txt.GetStyles());
	RichPara title_para;
	title_para.format = out.GetStyle(out.GetStyleId("Heading")).format;
	title_para.Cat(title, title_para.format);
	out.Cat(title_para);
	out.CatPick(txt);

//	RichText out = HelpTopicExpand(/* style_qtf */ Null, title_qtf, topic_qtf, "Heading", false, 0, NoFormat(), Null, progress, progress_pos);
//	VectorMap<String, Value> eval;
//	eval.Add("$book", 1);
//	out.EvaluateFields(eval);

	int sb = viewer.GetSb();
	viewer.Pick(out);
	viewer.SetSb(sb);

//	help_style = 0;
}

void DlgHelpTopic::AddRecent(String topic, int y)
{
	recent_pages.SetCount(recent_pos);
	recent_y.SetCount(recent_pos);
	if(recent_pos == 0 || topic != recent_pages[recent_pos - 1] || y != recent_y[recent_pos - 1]) {
		recent_pages.Add(topic);
		recent_y.Add(y);
	}
	if((recent_pos = recent_pages.GetCount()) >= MAX_RECENT) {
		int over = recent_pos - MAX_RECENT;
		recent_pages.Remove(0, over);
		recent_y.Remove(0, over);
		recent_pos -= over;
	}
	Rescan();
}

void DlgHelpTopic::OnLink(const String& link)
{
	String new_topic = link;
	String old_topic = current_topic;
	int old_sb = viewer.GetSb();
	if(OpenPage(new_topic)) {
		AddRecent(old_topic, old_sb);
//		SeekAnchor(s);
		AddRecent(new_topic, viewer.GetSb());
	}
}

/*
void DlgHelpTopic::SeekAnchor(const char *a)
{
	if(!a)
		return;
	while(*a && *a != '#')
		a++;
	if(*a != '#')
		return;

	const Vector<ValueRect>& vrc = qtf.GetValueRects();
	int y = 0;
	for(int i = 0; i < vrc.GetCount(); i++) {
		String s = vrc[i].value;
		const char *p = s;
		if(*p != '=' || *++p != '#')
			continue;
		if(!strcmp(p, a)) {
			int t = vrc[i].rect.top;
			if(y == 0 || t < y)
				y = t;
		}
	}
	qtf.SetSb(y + qtf.GetTm());
}
*/

void DlgHelpTopic::OnBack()
{
	if(recent_pos >= 2) {
		int t = --recent_pos - 1;
		OpenPage(recent_pages[t], recent_y[t]);
	}
}

void DlgHelpTopic::OnForward()
{
	if(recent_pos < recent_pages.GetCount()) {
		int t = recent_pos++;
		OpenPage(recent_pages[t], recent_y[t]);
	}
}

void DlgHelpTopic::OnIndex()
{
	String page = RunDlgHelpTopicIndex();
	if(!IsNull(page))
		OpenPage(page);
/*
	String page = HelpIndex::Get().Run(HelpIndex::INDEX_TAB);
	const HelpPage *pp = HelpIndex::Get().Find(page);
	if(pp) {
		AddRecent(*current_page, dialog.qtf.GetSb());
		OpenPage(*pp);
		SeekAnchor(page);
		AddRecent(*pp, dialog.qtf.GetSb());
	}
*/
}

void DlgHelpTopic::OnPrint()
{
	DocReport report;
	Size page = report.GetPageSize();
	ReportPageDraw rpd(page);
	PaintInfo info;
	info.sell = info.selh = 0;
	info.zoom = Zoom(1024, 1024);
	int doty = page.cy / info.zoom;
//	info.bottom = PageY(0, page.cy);
	viewer.Get().Paint(rpd, Rect(page), info);
	rpd.Write(report);
	report.Perform();
/*
	RichText rtext;
	AppendQtf(rtext, "[=A4* " + title_qtf);
	RichText::Format format;
	(RichText::CharFormat&)format = Arial(100);
	rtext.AppendPara(format);
	rtext.AppendPara(format);
	AppendQtf(rtext, topic_qtf);
	Report report;
	report.Qtf(AsQtf(rtext));
	report.Perform();
*/
}

/*
void DlgHelpTopic::ToolBookMenu(Bar& bar)
{
	bar.Add("Pøíruèka", DlgHelpViewImg::book(), THISBACK(OnBook))
		.Key(K_SHIFT_CTRL_P)
		.Help("Vysázet uživatelskou pøíruèku pøímo pro tisk");
#ifdef _DEBUG
#ifdef PLATFORM_WIN32
	bar.Add("Uložit PDF", DlgHelpViewImg::book_pdf(), THISBACK(OnBookPDF))
		.Help("Uložit uživatelskou pøíruèku ve formátu PDF pro Adobe Acrobat Reader");
#endif
	bar.Add("Uložit RTF", DlgHelpViewImg::book_rtf(), THISBACK(OnBookRTF))
		.Help("Uložit pøíruèku ve formátu RTF (pøenositelný formátovaný text)");
#endif
}
*/

/*
void DlgHelpTopic::OnBookRTF()
{
	FileSel fsel;
	fsel.Type("RTF soubory (*.rtf)", "*.rtf")
	.AllFilesType()
	.DefaultExt("rtf");
	static String recent;
	fsel <<= Nvl(recent, ConfigFile(GetExeTitle() + ".rtf"));
	if(fsel.ExecuteSaveAs("Uložit pøíruèku (RTF)")) {
		recent = ~fsel;
		String manual = CreateHelpRTF();
		if(!IsNull(manual)) {
			if(!SaveFile(recent, manual))
				Exclamation(NFormat("Nelze uložit soubor [* \1%s\1] ([* %d] B).", recent, manual.GetLength()));
		}
	}
}
*/

/*
#ifdef PLATFORM_WIN32
void DlgHelpTopic::OnBookPDF()
{
	FileSel fsel;
	fsel.Type("PDF soubory (*.pdf)", "*.pdf")
	.AllFilesType()
	.DefaultExt("pdf");
	static String recent;
	fsel <<= Nvl(recent, ConfigFile(GetExeTitle() + ".pdf"));
	if(fsel.ExecuteSaveAs("Uložit pøíruèku (PDF)")) {
		recent = ~fsel;
		String manual = CreateHelpPDF();
		if(!IsNull(manual)) {
			if(!SaveFile(recent, manual))
				Exclamation(NFormat("Nelze uložit soubor [* \1%s\1] ([* %d] B).", recent, manual.GetLength()));
		}
	}
}
#endif
*/

void DlgHelpTopic::OnZoom()
{
//	dialog.qtf.Zoom(~zoom);
//	dialog.qtf.RefreshLayout(); // todo: Fidler's bug, remove as soon as possible
}

void DlgHelpTopic::ToolMain(Bar& bar)
{
	bar.Add(recent_pos >= 2, t_("Previous"), DlgHelpViewImg::back(), THISBACK(OnBack))
		.Key(K_ALT_LEFT)
		.Help(t_("Return to previous help topic"));
	bar.Add(recent_pos < recent_pages.GetCount(), t_("Next"), DlgHelpViewImg::forward(), THISBACK(OnForward))
		.Key(K_ALT_RIGHT)
		.Help(t_("Move forward in order of viewing help topics"));
	bar.Separator();
	bar.Add(t_("Search index"), DlgHelpViewImg::index(), THISBACK(OnIndex))
		.Key(K_CTRL_F)
		.Help(t_("Show / select theme from the alphabetic index"));
	bar.Separator();
	bar.Add(t_("Print"), DlgHelpViewImg::print(), THISBACK(OnPrint))
		.Key(K_CTRL_P)
		.Help(t_("Print current topic on the printer"));
	HelpTopicBookMenu()(bar);
}

class DlgHelpLang : public WithHelpLangLayout<TopWindow>
{
public:
	typedef DlgHelpLang CLASSNAME;
	DlgHelpLang();

	bool Run(VectorMap<int, String>& ext_lang, int& init_lang);

private:
	void OnExtLangEdit();
	void OnLangFileBrowse();

private:
	DropList lang_edit;
	EditField lang_file;
	ExtendedButton lang_file_browse;
};

bool RunDlgHelpLang(VectorMap<int, String>& ext_lang, int& init_lang) { return DlgHelpLang().Run(ext_lang, init_lang); }

//RegisterHelpTopicObjectTitle(DlgHelpLang, t_("Application languages"))

DlgHelpLang::DlgHelpLang()
{
	CtrlLayoutOKCancel(*this, t_("Application languages"));
	HelpTopic("DlgHelpLang");

	ext_lang.AutoHideSb();
	ext_lang.AddColumn(t_("Language code"), 10).SetConvert(lang_edit).Edit(lang_edit);
	ext_lang.AddColumn(t_("Language file"), 30).Edit(lang_file);
	ext_lang.Inserting().Removing().NoAskRemove();
	ext_lang.WhenArrayAction = THISBACK(OnExtLangEdit);

	lang_file.AddFrame(lang_file_browse);
	lang_file_browse <<= THISBACK(OnLangFileBrowse);

	for(const int *p = GetAllLanguages(); *p; p++)
		lang_edit.Add(*p, LNGAsText(*p));
}

bool DlgHelpLang::Run(VectorMap<int, String>& el, int& il)
{
	int i;
	for(i = 0; i < el.GetCount(); i++)
		ext_lang.Add(el.GetKey(i), el[i]);
	OnExtLangEdit();
	init_lang <<= il;
	if(TopWindow::Run() != IDOK)
		return false;
	el.Clear();
	for(i = 0; i < ext_lang.GetCount(); i++)
		el.Add(ext_lang.Get(i, 0), ext_lang.Get(i, 1));
	il = ~init_lang;
	return true;
}

void DlgHelpLang::OnExtLangEdit()
{
	int oi = ~init_lang;
	init_lang.Clear();
	const int *x = GetAllLanguages();
	Index<int> app_lang;
	while(*x)
		app_lang.Add(*x++);
	for(int i = 0; i < ext_lang.GetCount(); i++)
		app_lang.FindAdd(ext_lang.Get(i, 0));
	Index<int> lmask;
	for(int i = 0; i < app_lang.GetCount(); i++) {
		int l = app_lang[i], l0 = SetLNGCharset(l, CHARSET_DEFAULT);
		if(lmask.Find(l0) < 0) {
			lmask.Add(l0);
			init_lang.Add(l, LNGAsText(l0));
		}
	}
	init_lang <<= (init_lang.FindKey(oi) < 0 ? app_lang[0] : oi);
}

void DlgHelpLang::OnLangFileBrowse()
{
	FileSelector fsel;
	fsel.Type(t_("Language files (*.lng)"), t_("*.lng"));
	fsel.DefaultExt("lng");
	fsel.AllFilesType();
	fsel <<= String(~lang_file_browse);
	if(fsel.ExecuteOpen(t_("Language file"))) {
		lang_file_browse <<= ~fsel;
		lang_file_browse.Action();
	}
}

END_UPP_NAMESPACE
