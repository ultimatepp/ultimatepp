#include "ide.h"

struct GatherLinksIterator : RichText::Iterator {
	Index<String> link;

	virtual bool operator()(int pos, const RichPara& para)
	{
		for(int i = 0; i < para.GetCount(); i++) {
			String l = para[i].format.link;
			int q = l.ReverseFind('#');
			if(q >= 0)
				l.Trim(q);
			if(!IsNull(l))
				link.FindAdd(l);
		}
		return false;
	}
};

void GatherLinks(Index<String>& link, String topic)
{
	int q = topic.ReverseFind('$');
	if(q >= 0)
		topic.Set(q, '_');
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

Index<String> TopicCtrl::idelink;

void TopicCtrl::ScanDirForTpp(const char *dir, const String& rel,
                              Index<String>& donepackage, Index<String>& lang_list)
{
	TopicLink tl;
	for(FindFile pff(AppendFileName(dir, "*.*")); pff; pff.Next()) {
		if(pff.IsFolder() && *pff.GetName() != '.') {
			String pdir = AppendFileName(dir, pff.GetName());
			tl.package = rel + pff.GetName();
			if(donepackage.Find(tl.package) < 0) {
				donepackage.Add(tl.package);
				for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next())
					if(ff.IsFolder()) {
						tl.group = GetFileTitle(ff.GetName());
						String dir = AppendFileName(pdir, ff.GetName());
						for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next())
							if(ft.IsFile()) {
								tl.topic = GetFileTitle(ft.GetName());
								int q = tl.topic.ReverseFind('_');
								String l;
								if(q >= 0) {
									l = ToUpper(tl.topic.Mid(q + 1));
									lang_list.FindAdd(l);
								}
								map.GetAdd(tl.package).GetAdd(tl.group).FindAdd(tl.topic);
							}
					}
			}
			ScanDirForTpp(pdir, tl.package + '/', donepackage, lang_list);
		}
	}
}

int map_serial, topic_serial;

void TopicCtrl::LoadMap()
{
	map.Clear();
	lang_list.Clear();
	Vector<String> upp = GetUppDirs();
	Index<String> donepackage, lang_list;
	for(int i = 0; i < upp.GetCount(); i++)
		ScanDirForTpp(upp[i], String(), donepackage, lang_list);
	Vector<String> l = lang_list.PickKeys();
	Sort(l);
	String lng = ~lang;
	lang.Clear();
	lang.Add("All");
	for(int i = 0; i < l.GetCount(); i++)
		lang.Add(l[i]);
	if(lng.GetCount() && lang.Find(lng))
		lang <<= lng;
	else
	if(lang.Find("EN-US"))
		lang <<= "EN-US";
	else
	if(lang.GetCount())
		lang.SetIndex(0);
}

static String sTopicHome = "topic://ide/app/index_en-us";
static String s_idehelp = "TheIDE help";
static String s_usedpackages = "Used packages";
static String s_otherpackages = "Other packages";
static String s_documents = "Documents";
static String s_reference = "Reference";
static String s_implementation = "Implementation";

inline int sFindN(const String& s)
{
	if(s == s_idehelp) return 0;
	if(s == s_usedpackages) return 1;
	if(s == s_otherpackages) return 2;
	if(s == s_documents) return 3;
	if(s == s_reference) return 4;
	if(s == s_implementation) return 5;
	return 6;
}

int TopicSortOrder(const Value& k1, const Value& v1, const Value& k2, const Value& v2)
{
	String s1 = v1;
	String s2 = v2;
	bool bk1 = IsNull(k1);
	bool bk2 = IsNull(k2);
	int q = (int)bk1 - (int)bk2;
	if(q) return q;
	if(bk1) {
		int q = sFindN(s1) - sFindN(s2);
		if(q) return q;
	}
	return StdValueCompare(v1, v2);
}

void TopicCtrl::SyncDocTree()
{
	if(map_serial != topic_serial) {
		LoadMap();
		map_serial = topic_serial;
	}
	
	Vector<String> ss = Split((String)~search, ' ');
	
	if(ss.GetCount() && !SyncRefsFinished) {
		SyncRefsShowProgress = true;
		return;
	}
	
	Vector<String> sdx;
	for(int i = 0; i < ss.GetCount(); i++)
		sdx.Add(ToUtf8(ToLower(ToUtf32(ss[i]))));

	ClearTree();

	if(idelink.GetCount() == 0)
		GatherLinks(idelink, sTopicHome);
	int ide = 0;
	bool idefirst = true;
	if(MatchTopicLink(sTopicHome, sdx)) {
		ide = AddTree(0, IdeImg::book(), sTopicHome, s_idehelp);
		idefirst = false;
	}
	for(int i = 0; i < idelink.GetCount(); i++) {
		if(idelink[i] != sTopicHome && MatchTopicLink(idelink[i], sdx)) {
			if(idefirst) {
				ide = AddTree(0, IdeImg::Package(), sTopicHome, s_idehelp);
				idefirst = false;
			}
			AddTree(ide, TopicImg::Topic(), idelink[i], GetTopic(idelink[i]).title);
		}
	}

	Index<String> used;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		used.Add(wspc[i]);

	int usid = 0;
	bool usedfirst = true;
	int otid = 0;
	bool otherfirst = true;

	String lng = ~lang;
	for(int i = 0; i < map.GetCount(); i++) {
		TopicLink tl;
		tl.package = map.GetKey(i);
		bool packagefirst = true;
		int pid = 0;
		VectorMap<String, Index<String> >& group = map[i];
		for(int i = 0; i < group.GetCount(); i++) {
			tl.group = group.GetKey(i);
			if(all || tl.group == "src" || tl.group == "srcdoc" || tl.group == "srcimp") {
				String n = tl.group;
				if(n == "src")
					n = s_reference;
				if(n == "srcdoc")
					n = s_documents;
				if(n == "srcimp")
					n = s_implementation;
				int gid = 0;
				bool groupfirst = true;
				const Index<String>& topic = group[i];
				for(int i = 0; i < topic.GetCount(); i++) {
					tl.topic = topic[i];
					int q = tl.topic.ReverseFind('_');
					String l;
					if(q >= 0)
						l = ToUpper(tl.topic.Mid(q + 1));
					String link = TopicLinkString(tl);
					if(idelink.Find(link) < 0 && MatchTopicLink(link, sdx) && (lng == "All" || lng == l)) {
						int pd;
						if(used.Find(tl.package) >= 0) {
							if(usedfirst) {
								usid = AddTree(0, IdeImg::book(), Null, s_usedpackages);
								usedfirst = false;
							}
							pd = usid;
						}
						else {
							if(otherfirst) {
								otid = AddTree(0, IdeImg::book(), Null, s_otherpackages);
								otherfirst = false;
							}
							pd = otid;
						}
						if(packagefirst) {
							pid = AddTree(pd, TopicImg::Package(), Null, tl.package);
							packagefirst = false;
						}
						if(groupfirst) {
							gid = AddTree(pid, Null, Null, n);
							groupfirst = false;
						}
						String p = TopicLinkString(tl);
						String t = GetTopicTitle(p);
						AddTree(gid, TopicImg::Topic(), p, t);
					}
				}
			}
		}
	}
	SortTree(0, TopicSortOrder);
	FinishTree();
	if(sdx.GetCount()) {
		OpenDeep();
		CurrentOrHome();
	}
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
	String current = GetCurrent();
	String topic = t;
	if(*topic == '#')
		topic = current + topic;
	recent_topic = topic;
	internal = (byte)*topic < 32;
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
	if(topic.StartsWith("topic://ide/app/"))
		return GetTopic(topic);
	TopicLink tl = ParseTopicLink(topic);
	if(!IsNull(tl.package)) {
		int q = tl.topic.ReverseFind('$');
		if(q >= 0)
			tl.topic.Set(q, '_');
		Topic t = ReadTopic(LoadFile(AppendFileName(
							AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp"),
							tl.topic + ".tpp")));
		t.label = tl.label;
		tl.label.Clear();
		t.link = TopicLinkString(tl);
		return t;
	}
	return Topic();
}

                 

struct HighlightWords : RichText::Iterator {
	Vector<String> words;
	struct Pos : Moveable<Pos> { int pos, len; };
	Vector<Pos>   pos;

	int FindPattern(const String& x) {
		for(int i = 0; i < words.GetCount(); i++)
			if(x.StartsWith(words[i]))
				return words[i].GetCount();
		return -1;
	}

	virtual bool operator()(int tpos, const RichPara& para) {
		WString text = para.GetText();
		const wchar *s = text;
		for(;;) {
			while(!IsAlpha(*s) && !IsDigit(*s) && *s)
				s++;
			if(*s == '\0')
				break;
			String wb;
			const wchar *b = s;
			while(IsAlpha(*s) || IsDigit(*s))
				wb.Cat(ToUpper(*s++));
			int q = FindPattern(wb);
			if(q >= 0) {
				Pos& p = pos.Add();
				p.pos = int(b - ~text) + tpos;
				p.len = q;
			}
		}
		return false;
	}
};

void TopicCtrl::FinishText(RichText& text)
{
	spos.Clear();
	if(!showwords)
		return;
	Vector<String> ss = Split((String)~search, ' ');
	
	if(ss.GetCount() == 0)
		return;
	HighlightWords hw;
	hw.words = pick(ss);
	text.Iterate(hw);
	RichText::FormatInfo fi;
	fi.charvalid = RichText::PAPER|RichText::INK;
	fi.paravalid = 0;
	fi.paper = SColorHighlight();
	fi.ink = SColorHighlightText();
	for(int i = 0; i < hw.pos.GetCount(); i++) {
		text.ApplyFormatInfo(hw.pos[i].pos, fi, hw.pos[i].len);
		spos.Add(hw.pos[i].pos);
	}
}

void TopicCtrl::OpenTopic()
{
	WhenTopic();
}

void TopicCtrl::Search()
{
	int l, h;
	ClearCurrentLink();
	search.GetSelection(l, h);
	SyncDocTree();
	SetBar();
	search.SetFocus();
	search.SetSelection(l, h);
}

void TopicCtrl::SearchWord(const String& s)
{
	search <<= ToUpper(s);
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

void TopicCtrl::FocusSearch()
{
	search.SetFocus();
}

void TopicCtrl::Prev()
{
	if(!Up(spos))
		HelpWindow::Prev();
}

void TopicCtrl::Next()
{
	if(!Down(spos))
		HelpWindow::Next();
}

void  TopicCtrl::BarEx(Bar& bar)
{
	bar.Gap();
	bar.Add(lang, HorzLayoutZoom(60));
	bar.Add("All topics", IdeImg::HelpAll(), THISBACK(All))
	   .Check(all);
	bar.Gap(HorzLayoutZoom(30));
	bar.Add(search, HorzLayoutZoom(300));
	bar.Add(search.GetLength(), "Previous", IdeImg::GoPrev(), THISBACK(Prev));
	bar.Add(search.GetLength(), "Next", IdeImg::GoNext(), THISBACK(Next));
	
	bar.AddKey(K_CTRL_F, THISBACK(FocusSearch));
/*	bar.Add("Highlight search keywords in topic", IdeImg::ShowWords(), THISBACK(ShowWords))
	   .Check(showwords);*/
	bar.Add(!internal && GetCurrent().StartsWith("topic:"), "Display on full screen",
	        IdeImg::show(), THISBACK(SShow));
	bar.GapRight();
	bar.Separator();
	bar.Add(!internal && GetCurrent().StartsWith("topic:"), "Edit topic",
	        TopicImg::Topic(), THISBACK(OpenTopic));
}

void TopicCtrl::Serialize(Stream& s)
{
	int version = 3;
	s / version;
	if(version < 3) {
		WithDropChoice<EditString> dummy;
		dummy.SerializeList(s);
	}
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
	
	~HelpDes()                                      { topic->Remove(); }
};

bool IsHelpName(const char *path)
{
	return strcmp(path, HELPNAME) == 0;
}

struct HelpModule : public IdeModule {
	virtual String       GetID() { return "HelpModule"; }
	virtual Image FileIcon(const char *path) {
		return IsHelpName(path) ? IdeImg::help() : Null;
	}
	virtual IdeDesigner *CreateDesigner(Ide *ide, const char *path, byte cs) {
		if(IsHelpName(path)) {
			topic_serial++;
			GetRefLinks("");
			ide->doc.SyncDocTree();
			ide->doc.GoTo(Nvl(recent_topic, sTopicHome));
			HelpDes *d = new HelpDes;
			d->topic = &ide->doc;
			return d;
		}
		return NULL;
	}
	virtual void Serialize(Stream& s) {
		s % recent_topic;
	}
};

INITBLOCK {
	RegisterIdeModule(Single<HelpModule>());
}

int CharFilterTopicSearch(int c)
{
	if(c == ' ') return c;
	c = CharFilterDefaultToUpperAscii(c);
	return IsAlNum(c) ? c : 0;
}

TopicCtrl::TopicCtrl()
{
	showwords = true;
	all = false;
	lang <<= THISBACK(Lang);
	lang.Tip("Language"),
	search.NullText("Search", StdFont().Italic(), SColorDisabled());
	search.Tip("Full text search");
	search <<= THISBACK(Search);
	search.SetFilter(CharFilterTopicSearch);
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
		doc.GoTo(sTopicHome);
		doc_serial = TopicEditor::GetSerial();
	}
	EditFile(HELPNAME);
}

void Ide::ShowTopicsWin()
{
	windoc.Icon(IdeImg::help_win(), IdeImg::help_win_large());
	if(windoc.IsOpen())
		windoc.SetForeground();
	else {
		topic_serial++;
		GetRefLinks("");
		windoc.SyncDocTree();
		windoc.GoTo(sTopicHome);
		windoc.OpenMain();
	}
}

void Ide::SearchTopics()
{
	String s = editor.GetWord();
	GetRefLinks("");
	doc.SyncDocTree();
	doc.GoTo(sTopicHome);
	EditFile(HELPNAME);
	if(s.GetLength())
		doc.SearchWord(s);
}

void Ide::RefreshBrowser()
{
	editor.SyncNavigator();
	doc.SyncDocTree();
}

void Ide::ViewIdeLogFile()
{
	OpenLog(GetIdeLogPath());
}
