#include "CtrlLib.h"

namespace Upp {

Topic HelpWindow::AcquireTopic(const String& topic)
{
	return GetTopic(topic);
}

void HelpWindow::BarEx(Bar& bar)
{}

void HelpWindow::FinishText(RichText& text)
{}

bool HelpWindow::GoTo0(const String& link)
{
	if(IsNull(link) || doing_goto)
		return true;
	bool lnk = false;
	if(current_link != link) {
		Topic t = AcquireTopic(link);
		SetBar();
		if(IsNull(t.text))
			return false;
		label = t.label;
		topic = t.link;
		doing_goto++; // suppress recursive GoTo
		if(~tree != topic)
			tree.FindSetCursor(topic);
		doing_goto--;
		Title(ToUtf32(t.title));
		RichText txt = ParseQTF(t.text);
		FinishText(txt);
		view.Pick(pick(txt), zoom);
		current_link = link;
		lnk = true;
	}
	if(WhenMatchLabel) {
		WString lw = label.ToWString();
		return view.GotoLabel([=](const WString& data) { return WhenMatchLabel(data, lw); }, true, true) || lnk;
	}
	return view.GotoLabel(label, true, true) || lnk;
}

HelpWindow::Pos HelpWindow::GetPos()
{
	Pos p;
	p.link = topic;
	p.scy = view.GetSb();
	return p;
}

bool HelpWindow::GoTo(const String& link)
{
	if(IsNull(link) || doing_goto)
		return false;
	Pos p = GetPos();
	if(GoTo0(link)) {
		if(!IsNull(p.link) && p.link != link) {
			back.Add(p);
			forward.Clear();
		}
		SetBar();
		return true;
	}
	if(link.StartsWith("www.") || link.StartsWith("http") || link.StartsWith("mailto:")) {
		LaunchWebBrowser(link);
		return true;
	}
	return false;
}

void HelpWindow::Back()
{
	Pos p = GetPos();
	if(back.GetCount()) {
		GoTo0(back.Top().link);
		if(back.GetCount()) {
			view.SetSb(back.Top().scy);
			back.Drop();
		}
		if(!IsNull(p.link))
			forward.Add(p);
		SetBar();
	}
}

void HelpWindow::Forward()
{
	Pos p = GetPos();
	if(forward.GetCount()) {
		GoTo0(forward.Top().link);
		if(forward.GetCount()) {
			view.SetSb(forward.Top().scy);
			forward.Drop();
		}
		if(!IsNull(p.link))
			back.Add(p);
		SetBar();
	}
}

void HelpWindow::SetZoom()
{
	zoom.d = 1000000 / Zy(800);
	current_link = Null;
	GoTo0(topic);
	Refresh();
	SetBar();
}

void HelpWindow::FontSize()
{
	int q = zoom.m / 20;
	if(q < 6 || q > 10)
		q = 6;
	else
		q++;
	zoom.m = 20 * q;
	SetZoom();
}

void HelpWindow::Print()
{
#ifndef PLATFORM_PDA
	Topic t = AcquireTopic(topic);
	UPP::Print(ParseQTF(t.text), Size(3968, 6074), 0);
#endif
}

void HelpWindow::Tools(Bar& bar)
{
	bar.Add(back.GetCount(), t_("Go Back"), CtrlImg::go_back(), THISBACK(Back))
	   .Key(K_ALT_LEFT);
	bar.Add(forward.GetCount(), t_("Go Forward"), CtrlImg::go_forward(), THISBACK(Forward))
	   .Key(K_ALT_RIGHT);
	bar.Gap();
	bar.Add(t_("Font size"), MakeZoomIcon(zoom.m / 160.0), THISBACK(FontSize));
	bar.Gap();
#ifndef PLATFORM_PDA
	bar.Add(t_("Print"), CtrlImg::print(), THISBACK(Print));
#endif
	BarEx(bar);
}

void HelpWindow::SetBar()
{
	toolbar.Set(THISBACK(Tools));
}

bool HelpWindow::Key(dword key, int count)
{
	switch(key) {
	case K_ESCAPE:
		Close();
		return true;
	case K_MOUSE_BACKWARD:
		Back();
		return true;
	case K_MOUSE_FORWARD:
		Forward();
		return true;
	}
	return view.Key(key, count);
}

void HelpWindow::ClearTree()
{
	tree.Clear();
}

int HelpWindow::AddTree(int parent, const Image& img, const String& topic, const String& title)
{
	tree_view.NoZoom();
	return tree.Add(parent, img, topic, title, false);
}

void HelpWindow::SortTree(int id)
{
	tree.SortDeep(id);
}

void HelpWindow::SortTree(int id, int (*cmp)(const Value& v1, const Value& v2))
{
	tree.SortDeep(id, cmp);
}

void HelpWindow::SortTree(int id, int (*cmp)(const Value& k1, const Value& v1,
                                             const Value& k2, const Value& v2))
{
	tree.SortDeep(id, cmp);
}

void HelpWindow::FinishTree()
{
	if(!tree.FindSetCursor(topic))
		CurrentOrHome();
}

void HelpWindow::OpenDeep(int id)
{
	tree.OpenDeep(id);
}

void HelpWindow::Ids(int pid, Vector<int>& r)
{
	int n = tree.GetChildCount(pid);
	for(int i = 0; i < n; i++) {
		int id = tree.GetChild(pid, i);
		if(!IsNull(tree.GetValue(id))) {
			r.Add(id);
			Ids(id, r);
		}
	}
}

Vector<int> HelpWindow::Ids()
{
	Vector<int> r;
	Ids(0, r);
	return r;
}

bool HelpWindow::PrevNext(int d, bool allowempty)
{
	Vector<int> r = Ids();
	int id = tree.GetCursor();
	if(id < 0)
		return false;
	int ii = FindIndex(r, id);
	if(ii < 0)
		return false;
	for(;;) {
		ii += d;
		if(ii >= r.GetCount() || ii < 0)
			return false;
		if(!IsNull(tree.Get(r[ii])) || allowempty) {
			tree.SetCursor(r[ii]);
			return true;
		}
	}
}

bool HelpWindow::Next(bool allowempty)
{
	return PrevNext(1, allowempty);
}

bool HelpWindow::Prev(bool allowempty)
{
	return PrevNext(-1, allowempty);
}

void HelpWindow::Serialize(Stream& s)
{
	s % zoom.m;
	s % tree_view;
	SerializePlacement(s);
	SetZoom();
}

void HelpWindow::TreeCursor()
{
	if(!IsNull(tree))
		GoTo(~tree);
}

void HelpWindow::CurrentOrHome()
{
	if(~tree != current_link || IsNull(current_link)) {
		if(!IsNull(current_link) && tree.FindSetCursor(current_link))
			return;
		for(int i = 0; i < tree.GetLineCount(); i++) {
			Value k = tree.Get(tree.GetItemAtLine(i));
			if(!IsNull(k) && tree.FindSetCursor(k))
				break;
		}
	}
}

Vector<int> HelpWindow::ScPositions(const Vector<int>& p)
{
	Vector<int> r;
	for(int i = 0; i < p.GetCount(); i++) {
		int y = max(0, view.GetZoom() * view.Get().GetCaret(p[i], view.GetPage()).top - GetSize().cy / 2);
		int ii = FindLowerBound(r, y);
		if(ii == r.GetCount() || r[ii] != y)
			r.Insert(ii, y);
	}
	return r;
}

bool HelpWindow::Up(const Vector<int>& poslist)
{
	int q = view.GetSb();
	Vector<int> p = ScPositions(poslist);
	for(int i = p.GetCount() - 1; i >= 0; i--)
		if(p[i] < q) {
			view.SetSb(p[i]);
			return view.GetSb() != q;
		}
	return false;
}

bool HelpWindow::Down(const Vector<int>& poslist)
{
	int q = view.GetSb();
	Vector<int> p = ScPositions(poslist);
	for(int i = 0; i < p.GetCount(); i++)
		if(p[i] > q) {
			view.SetSb(p[i]);
			return view.GetSb() != q;
		}
	return false;
}

HelpWindow::HelpWindow()
{
	tree_view.Horz(tree, view);
	tree_view.SetPos(3000);
	Add(tree_view.SizePos());
	tree_view.Zoom(1);
	Sizeable().Zoomable();
	Title(t_("Help"));
	BackPaint();
	view.WhenLink = [=](const String& h) { GoTo(h); };
	AddFrame(toolbar);
	view.SetZoom(Zoom(1, 1));
	zoom.m = 160;
	SetZoom();
	view.Margins(Rect(12, 0, 12, 0));
	SetRect(Ctrl::GetWorkArea().Deflated(80));
	tree.WhenSel = THISBACK(TreeCursor);
	tree.NoRoot();
	Icon(CtrlImg::help());
	SetBar();
	tree.BackPaint();
	view.BackPaintHint();
	view.WhenMouseWheel = [=] (int zdelta, dword keyflags) {
		if(keyflags & K_CTRL) {
			zoom.m = clamp((zoom.m / 5 + sgn(zdelta)) * 5, 60, 600);
			SetZoom();
			return true;
		}
		return false;
	};
}

}
