#include <MySupport/support.h>

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

String	promptTopic;

Topic AcquireTopic(const String& topic) {
	TopicLink tl = ParseTopicLink(topic);
	String path = AppendFileName(AppendFileName(GetExeDirFile(""), tl.group + ".tpp"), tl.topic + ".tpp");
	return ReadTopic(LoadFile(path));
}

Topic ReadTopic(const char *text) {
	Topic topic;
	CParser p(text);

	try {
		while(!p.IsEof()) {
			if(p.Id("TITLE")) {
				p.PassChar('(');
				topic.title = p.ReadString();
				p.PassChar(')');
			}
			else if(p.Id("REF")) {
				p.PassChar('(');
				p.ReadString();
				p.PassChar(')');
			}
			else if(p.Id("TOPIC_TEXT")) {
				p.PassChar('(');
				topic.text << p.ReadString();
				p.PassChar(')');
			}
			else if(p.Id("COMPRESSED")) {
				StringBuffer b;
				b.Reserve(1024);
				while(p.IsInt()) {
					b.Cat(p.ReadInt());
					p.PassChar(',');
				}
				topic.text = ZDecompress(b, b.GetLength());
			}
			else {
				topic.text << p.GetPtr();
				break;
			}
		}
	}
	catch(CParser::Error e) {
		topic.text = String::GetVoid();
		topic.title = e;
	}
	
	return topic;
}

bool HelpWin::GoTo0(const String& link, bool find) {
	int q = link.Find('#');
	if(q >= 0) {
		topic = link.Mid(0, q);
		label = link.Mid(q + 1);
	}
	else {
		topic = link;
		label = Null;
	}
	Topic t = AcquireTopic(topic);
	if(!IsNull(t.text)) {
		Title(t.title.ToWString());
		view = t.text;
		view.GotoLabel(label, true);
		tree.FindSetCursor(topic);
		return true;
	}
	return false;
}

HelpWin::Pos HelpWin::GetPos() {
	Pos p;
	p.link = topic;
	p.scy = view.GetSb();
	return p;
}

void HelpWin::GoTo(const String& link) {
	Pos p = GetPos();
	if(GoTo0(link)) {
		if(!IsNull(p.link))
			back.Add(p);
		forward.Clear();
		SetBar();
		return;
	}
	LaunchWebBrowser(link);
}

void HelpWin::Back() {
	Pos p = GetPos();
	if(back.GetCount() && GoTo0(back.Top().link, false)) {
		if(back.GetCount()) {
			view.SetSb(back.Top().scy);
			back.Drop();
		}
		if(!IsNull(p.link))
			forward.Add(p);
		SetBar();
	}
}

void HelpWin::Forward() {
	Pos p = GetPos();
	if(forward.GetCount() && GoTo0(forward.Top().link, false)) {
		if(forward.GetCount()) {
			view.SetSb(forward.Top().scy);
			forward.Drop();
		}
		if(!IsNull(p.link))
			back.Add(p);
		SetBar();
	}
}

void HelpWin::SetZoom() {
	zoom.d = 1000;
	view.SetZoom(zoom);
	Refresh();
}

void HelpWin::FontSize() {
	int q = zoom.m / 20;
	if(q < 7 || q > 10)
		q = 7;
	else
		q++;
	zoom.m = 20 * q;
	SetZoom();
}

void HelpWin::Print() {
#ifdef PLATFORM_WIN32
	::Print(view.Get(), Size(3968, 6074), 0);
#endif
}

void HelpWin::Tools(Bar& bar) {
	bar.Add(back.GetCount(), t_("Back"), commonImg::back_1_16(), THISBACK(Back)).Key(K_ALT_LEFT);
	bar.Add(forward.GetCount(), t_("Forward"), commonImg::forward_1_16(), THISBACK(Forward)).Key(K_ALT_RIGHT);
	bar.Gap();
	bar.Add(t_("Font size"), commonImg::font_size_1_16(), THISBACK(FontSize)).Key(K_ALT_F);
	bar.Gap();
	bar.Add(t_("Print"), commonImg::print_1_16(), THISBACK(Print)).Key(K_CTRL_P);
}

void HelpWin::SetBar() {
	toolbar.Set(THISBACK(Tools));
}

bool HelpWin::Key(dword key, int) {
	if(key == K_ESCAPE) {
		Close();
		return true;
	}
	return false;
}

void HelpWin::ClearTree() {
	tree_view.Zoom(1);
	tree.Clear();
}

int HelpWin::AddTree(int parent, const Image& img, const String& topic, const String& title) {
	tree_view.NoZoom();
	return tree.Add(parent, img, topic, title, false);
}

void HelpWin::FinishTree() {
	tree.FindSetCursor(topic);
}

void HelpWin::TreeCursor() {
	if(!IsNull(tree)) GoTo(~tree);
}

HelpWin::HelpWin() {
	tree_view.Horz(tree, view);
	tree_view.SetPos(2500);
	Add(tree_view.SizePos());
	tree_view.Zoom(1);
	Sizeable().Zoomable();
	Title(t_("Help in linea"));
	//view.Background(Color(255, 255, 232));
	BackPaint();
	view.WhenLink = THISBACK(GoTo);
	AddFrame(toolbar);
	zoom.m = 200;
	SetZoom();
	view.Margins(2);
	tree <<= THISBACK(TreeCursor);
	tree.NoRoot();
	Icon(commonImg::help_1_16());
	SetBar();
}

void HelpWin::SyncDocTree() {
	ClearTree();
	VectorMap<String, Vector<String> > map;
	String pdir = GetFileFolder(GetExeFilePath()) + "/";
	for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next())
		if(ff.IsFolder()) {
			String group = GetFileTitle(ff.GetName());
			String dir = AppendFileName(pdir, ff.GetName());
			for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next()) {	
				if(ft.IsFile()) {
					if(String(ft.GetName()).Mid(0, 1) != "_") map.GetAdd(group).Add(GetFileTitle(ft.GetName()));
				}
			}
		}
	for(int i = 0; i < map.GetCount(); i++) {
		Vector<String>& group = map[i];
		int pid;
		pid = AddTree(0, commonImg::group_1_16(), "topic://" + GetExeTitle() + "/" + map.GetKey(i), map.GetKey(i));
		for(int j = 0; j < group.GetCount(); j++) {
			String t = "topic://" + GetExeTitle() + "/" + map.GetKey(i) + "/" + group[j];
			String u = GetTopicTitle(GetFileFolder(GetExeFilePath()) + "/" + map.GetKey(i) + ".tpp/" + group[j] + ".tpp");
			AddTree(pid, commonImg::topic_1_16(), t, u);
		}
	}
	FinishTree();
}

void HelpWin::ShowTopics(const String& index) {
	SyncDocTree();
	EnableTree(true);
	OpenHelp(index);
	return;
}

void HelpWin::OpenHelp(const String& topic) {
	if(!IsTreeEnabled()) ClearTree();
	if(!IsOpen()) {
		Rect r = Ctrl::GetWorkArea();
		Size s = r.Size();
		r = r.CenterRect(s/2);
		SetRect(r);
		OpenMain();
	}

	else SetForeground();
	GoTo("topic://" + GetExeTitle() + "/" + topic);
	EnableTree(false);
	return;
}

String GetTopicTitle(const String &path) {
	FileIn in((char*)~path);
	if(!in) return "";
	try {
		String ln = in.GetLine();
		CParser p(ln);
		p.PassId("TITLE");
		p.PassChar('(');
		String title = p.ReadString();
		p.PassChar(')');
		return title;
	}
	catch(CParser::Error&) {}
}
