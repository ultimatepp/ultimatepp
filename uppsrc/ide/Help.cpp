#include "ide.h"

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

#define TOPICFILE <ide/app.tpp/all.i>
#include <Core/topic_group.h>

Size MakeLogo(Ctrl& parent, Array<Ctrl>& ctrl)
{
	Image logo = IdeImg::logo();
	Size  isz = logo.GetSize();
	ImageCtrl& l = ctrl.Create<ImageCtrl>();
	Label& v = ctrl.Create<Label>();
	Label& v1 = ctrl.Create<Label>();
	l.SetImage(logo);
	Size sz = Size(isz.cx, isz.cy + 80);
	v = IDE_VERSION;
	v.LeftPos(300, 100).TopPos(90, 40);
	if(sizeof(void *) == 8) {
		v = IDE_VERSION " (64 bit)";
		v.RightPos(3, Ctrl::MINSIZE).TopPos(90, 40);
	}
	l.Add(v);
	v.SetFont(Arial(20));
	v.SetInk(Blend(Gray, Blue));
	v1 = Format("%d`KB", MemoryUsedKb());
	v1.LeftPos(300, 100).BottomPos(20, 12);
	v1.SetFont(Arial(10));
	l.Add(v1);
	parent.Add(ctrl.Create<StaticRect>().Color(White).SizePos());
	parent.Add(l.TopPos(0, isz.cy).LeftPos(0, isz.cx));
	parent.Add(ctrl.Create<StaticRect>().Color(Blue).LeftPos(2, isz.cx - 4).TopPos(isz.cy, 1));
	RichTextView& w = ctrl.Create<RichTextView>();
	w.SetQTF(GetTopic("ide/app/Sponsor$en-us"));
	w.PageWidth(2900);
	w.NoSb();
	w.SetFrame(NullFrame());
	parent.Add(w.TopPos(isz.cy + 3, 99).LeftPos(0, isz.cx));
	return sz;
}

struct Splash : Ctrl {
	Array<Ctrl> ctrl;

	Splash() {
		SetRect(GetWorkArea().CenterRect(MakeLogo(*this, ctrl) + 2));
		SetFrame(BlackFrame());
	}
};

void HideSplash()
{
	if(Single<Splash>().IsOpen())
		Single<Splash>().Close();
}

void ShowSplash()
{
	Single<Splash>().PopUp(NULL, false, false);
	SetTimeCallback(750, callback(HideSplash));
}

bool IsSplashOpen()
{
	return Single<Splash>().IsOpen();
}

struct AboutDlg : TopWindow {
	Array<Ctrl>  ctrl;
	RichTextView about;

	typedef AboutDlg CLASSNAME;

	virtual bool Key(dword key, int) {
		if(key == K_ALT_M)
			MemoryProfileInfo();
		return false;
	}

	AboutDlg() {
		Size isz = MakeLogo(*this, ctrl);
		SetRect(0, 0, 1000, isz.cy);
		about.SetQTF(GetTopic("ide/app/About$en-us"), Zoom(130, 1024));
		about.SetZoom(Zoom(1, 1));
		about.RightPos(0, 1000 - isz.cx).VSizePos();
		about.HMargins(4);
		about.SetFrame(NullFrame());
		Background(PaintRect(ColorDisplay(), SColorPaper()));
		Add(about);
		Title("About TheIDE");
	}
};

void Ide::About()
{
	AboutDlg().Execute();
}

struct GatherLinksIterator : RichText::Iterator {
	Index<String> link;

	virtual bool operator()(int pos, const RichPara& para)
	{
		for(int i = 0; i < para.GetCount(); i++) {
			String l = para[i].format.link;
			if(!IsNull(l))
				link.FindAdd(l);
		}
		return false;
	}
};

void GatherLinks(Index<String>& link, const char *topic)
{
	if(link.Find(topic) < 0) {
		Topic p = GetTopic(topic);
		if(IsNull(p.text))
			return;
		link.Add(topic);
		RichText txt = ParseQTF(p.text);
		SyncTopicFile(txt, topic, ":ide:", p.title);
		GatherLinksIterator ti;
		txt.Iterate(ti);
		for(int i = 0; i < ti.link.GetCount(); i++)
			GatherLinks(link, ti.link[i]);
	}
}

void TopicCtrl::SyncDocTree()
{
	Vector<String> ss = Split((String)~search, ' ');
	Vector<int> sdx;
	for(int i = 0; i < ss.GetCount(); i++)
		sdx.Add(TopicWordIndex(ToUtf8(ToLower(FromUtf8(ss[i])))));

	ClearTree();

	static Index<String> idelink;
	String hdx = "topic://ide/app/index$en-us";
	if(idelink.GetCount() == 0)
		GatherLinks(idelink, hdx);
	int ide = AddTree(0, IdeImg::Package(), "\3" + hdx, "TheIDE help");
	for(int i = 0; i < idelink.GetCount(); i++)
		if(idelink[i] != hdx && MatchTopicLink(idelink[i], sdx))
			AddTree(ide, TopicImg::Topic(), "\3" + idelink[i], GetTopic(idelink[i]).title);

	Index<String> used;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		used.Add(wspc[i]);

	VectorMap<String, VectorMap<String, Vector<String> > > map;
	Vector<String> upp = GetUppDirs();
	String lng = ~lang;
	lang.Clear();
	lang.Add("All");
	Index<String> li;
	for(int i = 0; i < upp.GetCount(); i++) {
		TopicLink tl;
		for(FindFile pff(AppendFileName(upp[i], "*.*")); pff; pff.Next()) {
			if(pff.IsFolder()) {
				String pdir = AppendFileName(upp[i], pff.GetName());
				tl.package = pff.GetName();
				for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next())
					if(ff.IsFolder()) {
						tl.group = GetFileTitle(ff.GetName());
						String dir = AppendFileName(pdir, ff.GetName());
						for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next())
							if(ft.IsFile()) {
								tl.topic = GetFileTitle(ft.GetName());
								int q = tl.topic.ReverseFind('$');
								String l;
								if(q >= 0) {
									l = ToUpper(tl.topic.Mid(q + 1));
									li.FindAdd(l);
								}
								String link = TopicLinkString(tl);
								if(idelink.Find(link) < 0 && MatchTopicLink(link, sdx) &&
								   (lng == "All" || lng == l))
									map.GetAdd(tl.package).GetAdd(tl.group).Add(tl.topic);
							}
					}
			}
		}
	}
	Vector<String> sli = li.PickKeys();
	Sort(sli);
	for(int i = 0; i < sli.GetCount(); i++)
		lang.Add(sli[i]);
	if(lang.Find(lng))
		lang <<= lng;

	int usid = AddTree(0, IdeImg::Package(), Null, "Used packages");
	int otid = AddTree(0, IdeImg::Package(), Null, "Other packages");

	for(int i = 0; i < map.GetCount(); i++) {
		TopicLink tl;
		tl.package = map.GetKey(i);
		int pid = AddTree(used.Find(tl.package) >= 0 ? usid : otid,
		                      TopicImg::Package(), Null, tl.package);
		VectorMap<String, Vector<String> >& package = map[i];
		for(int i = 0; i < package.GetCount(); i++) {
			tl.group = package.GetKey(i);
			if(all || tl.group == "src" || tl.group == "srcdoc" || tl.group == "srcimp") {
				int gid = AddTree(pid, Null, "\1" + tl.group, tl.group);
				Vector<String> group = package[i];
				for(int i = 0; i < group.GetCount(); i++) {
					tl.topic = group[i];
					String p = TopicLinkString(tl);
					String t = GetTopicTitle(p);
					AddTree(gid, TopicImg::Topic(), p, t);
				}
			}
		}
	}
	SortTree(ide);
	SortTree(usid);
	SortTree(otid);
	FinishTree();
	if(sdx.GetCount())
		OpenDeep();
}

Vector<String> GetTypeRefLinks(const String& t, String &label)
{
	const char *tp[] = { "", "::struct", "::class", "::union", "::typedef", "::enum" };
	Vector<String> f;
	for(int i = 0; i < __countof(tp); i++) {
		label = t + tp[i];
		f = GetRefLinks(label);
		if(f.GetCount())
			break;
	}
	return f;
}

String recent_topic;

Topic TopicCtrl::AcquireTopic(const String& t)
{
	recent_topic = t;
	String topic = t;
	internal = (byte)*topic < 32;
	if(*topic == '\3')
		return GetTopic(topic.Mid(1));
	if(*topic == '\1') {
		Topic t;
		t.title = "Summary";
		String group = topic.Mid(1);
		const Workspace& wspc = GetIdeWorkspace();
		for(int i = 0; i < wspc.GetCount(); i++) {
			String package = wspc[i];
			String path = AppendFileName(AppendFileName(PackageDirectory(wspc[i]), group + ".tpp"), "index$en-us.tpp");
			if(FileExists(path)) {
				t.text << "[R5* " << wspc[i] << "]&" << ReadTopic(LoadFile(path)).text << "&&&";
			}
		}
		return t;
	}
	if(*topic == '\2')
		topic = topic.Mid(1);
	if(topic[0] == ':' && topic[1] == ':') {
		String lbl;
		Vector<String> link = GetTypeRefLinks(topic, lbl);
		if(link.GetCount() == 0)
			return Topic();
		if(link.GetCount() == 1)
			topic = link[0];
		else {
			WithSimpleListLayout<TopWindow> dlg;
			CtrlLayoutOKCancel(dlg, "Choose one of more link targets");
			dlg.list.AddKey();
			dlg.list.AddColumn("Topic");
			for(int i = 0; i < link.GetCount(); i++)
				dlg.list.Add(link[i], GetTopicTitle(link[i]));
			dlg.list.SetCursor(0);
			if(dlg.Run() != IDOK || !dlg.list.IsCursor())
				return Topic();
			topic = dlg.list.GetKey();
		}
		if(lbl.GetCount())
			topic << '#' << lbl;
	}
	TopicLink tl = ParseTopicLink(topic);
	if(!IsNull(tl.package)) {
		String path = AppendFileName(
						AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp"),
						tl.topic + ".tpp");
		Topic t = ReadTopic(LoadFile(path));
		t.label = tl.label;
		tl.label.Clear();
		t.link = TopicLinkString(tl);
		return t;
	}
	return Topic();
}

struct HighlightWords : RichText::Iterator {
	Index<String> words;
	struct Pos : Moveable<Pos> { int pos, len; };
	Vector<Pos>   pos;

	virtual bool operator()(int tpos, const RichPara& para) {
		WString text = para.GetText();
		const wchar *s = text;
		for(;;) {
			while(!IsLetter(*s) && *s)
				s++;
			if(*s == '\0')
				break;
			WStringBuffer wb;
			const wchar *b = s;
			while(IsLetter(*s))
				wb.Cat(ToLower(*s++));
			if(words.Find(FromUnicode(wb)) >= 0) {
				Pos& p = pos.Add();
				p.pos = int(b - ~text) + tpos;
				p.len = int(s - b);
			}
		}
		return false;
	}
};

void TopicCtrl::FinishText(RichText& text)
{
	if(!showwords)
		return;
	Vector<String> ss = Split((String)~search, ' ');
	if(ss.GetCount() == 0)
		return;
	HighlightWords hw;
	hw.words = ss;
	text.Iterate(hw);
	RichText::FormatInfo fi;
	fi.charvalid = RichText::PAPER|RichText::INK;
	fi.paravalid = 0;
	fi.paper = SColorHighlight();
	fi.ink = SColorHighlightText();
	for(int i = 0; i < hw.pos.GetCount(); i++)
		text.ApplyFormatInfo(hw.pos[i].pos, fi, hw.pos[i].len);
}

void TopicCtrl::OpenTopic()
{
	WhenTopic();
}

void TopicCtrl::Search()
{
	if(issearch) {
		search <<= Null;
		issearch = false;
	}
	else {
		search.AddHistory(20);
		issearch = true;
	}
	SyncDocTree();
	SetBar();
}

void TopicCtrl::SearchWord(const String& s)
{
	issearch = false;
	search <<= s;
	Search();
}

void TopicCtrl::ShowWords()
{
	showwords = !showwords;
	SetBar();
	GoTo(GetCurrent());
}

void TopicCtrl::All()
{
	all = !all;
	SyncDocTree();
	SetBar();
}

void TopicCtrl::Lang()
{
	SyncDocTree();
	SetBar();
}

bool TopicCtrl::Key(dword key, int count)
{
	if(key == K_ENTER && search.HasFocus()) {
		Search();
		return true;
	}
	return HelpWindow::Key(key, count);
}

void  TopicCtrl::BarEx(Bar& bar)
{
	bar.Gap();
	bar.Add(lang, HorzLayoutZoom(60));
	bar.Add("All topics", IdeImg::HelpAll(), THISBACK(All))
	   .Check(all);
	bar.Gap(HorzLayoutZoom(30));
	bar.Add(search_label, HorzLayoutZoom(50));
	bar.Add(search, HorzLayoutZoom(300));
	bar.Add("Full text search", IdeImg::HelpSearch(), THISBACK(Search))
	   .Check(issearch);
	bar.Add("Highlight search keywords in topic", IdeImg::ShowWords(), THISBACK(ShowWords))
	   .Check(showwords);
	bar.GapRight();
	bar.Separator();
	bar.Add(!internal && GetCurrent().StartsWith("topic:"),
	        TopicImg::Topic(), THISBACK(OpenTopic));
}

void TopicCtrl::Serialize(Stream& s)
{
	int version = 2;
	s / version;
	search.SerializeList(s);
	if(version >= 1)
		s % showwords;
	if(version >= 2)
		s % all;
}

struct HelpDes : public IdeDesigner {
	TopicCtrl *topic;

	virtual String GetFileName() const              { return HELPNAME; }
	virtual void   Save()                           {}
	virtual void   EditMenu(Bar& menu)              {}
	virtual Ctrl&  DesignerCtrl()                   { return *topic; }
	virtual void   SetFocus()                       { topic->SetFocus(); }
};

bool IsHelpName(const char *path)
{
	return strcmp(path, HELPNAME) == 0;
}

struct HelpModule : public IdeModule {
	virtual Image FileIcon(const char *path) {
		return IsHelpName(path) ? IdeImg::help() : Null;
	}
	virtual IdeDesigner *CreateDesigner(Ide *ide, const char *path, byte cs) {
		if(IsHelpName(path)) {
			GetRefLinks("");
			if(ide->doc.GetCurrent().IsEmpty()) {
				ide->doc.SyncDocTree();
				ide->doc.GoTo(Nvl(recent_topic, "\3topic://ide/app/index$en-us"));
			}
			HelpDes *d = new HelpDes;
			d->topic = &ide->doc;
			return d;
		}
		return false;
	}
	virtual void Serialize(Stream& s) {
		s % recent_topic;
	}
};

INITBLOCK {
	RegisterIdeModule(Single<HelpModule>());
}

TopicCtrl::TopicCtrl()
{
	Icon(IdeImg::doc());
	search_label = "Search: ";
	showwords = issearch = all = false;
	lang.Add("All");
	lang.Add("EN-US");
	lang <<= "EN-US";
	lang <<= THISBACK(Lang);
	lang.Tip("Language"),
	search.Tip("Full text search");
	internal = true;
}

void Ide::ShowTopics()
{
	if(designer && designer->GetFileName() == HELPNAME) {
		CycleFiles();
		tabi = 0;
		return;
	}
	if(doc_serial != TopicEditor::GetSerial()) {
		GetRefLinks("");
		doc.SyncDocTree();
		doc.GoTo("\3topic://ide/app/index$en-us");
		doc_serial = TopicEditor::GetSerial();
	}
	EditFile(HELPNAME);
}

void Ide::SearchTopics()
{
	String s = editor.GetWord();
	GetRefLinks("");
	doc.SyncDocTree();
	doc.GoTo("\3topic://ide/app/index$en-us");
	EditFile(HELPNAME);
	if(s.GetLength())
		doc.SearchWord(s);
}

void Ide::RefreshBrowser()
{
	browser.Refresh();
	doc.SyncDocTree();
}

void Ide::ShowTopic(String link)
{
	ShowTopics();
	doc.GoTo(link + '#' + browser.GetItem());
}
