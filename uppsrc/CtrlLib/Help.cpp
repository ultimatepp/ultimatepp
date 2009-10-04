#include "CtrlLib.h"

NAMESPACE_UPP

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
	if(IsNull(link) || current_link == link)
		return true;
	Topic t = AcquireTopic(link);
	SetBar();
	if(!IsNull(t.text)) {
		label = t.label;
		topic = t.link;
		Title(FromUtf8(t.title));
		RichText txt = ParseQTF(t.text);
		FinishText(txt);
		view.Pick(txt, zoom);
		view.GotoLabel(label, true);
		current_link = link;
		tree.FindSetCursor(topic);
		return true;
	}
	return false;
}

HelpWindow::Pos HelpWindow::GetPos()
{
	Pos p;
	p.link = topic;
	p.scy = view.GetSb();
	return p;
}

void HelpWindow::GoTo(const String& link)
{
	if(IsNull(link) || current_link == link)
		return;
	Pos p = GetPos();
	if(GoTo0(link)) {
		if(!IsNull(p.link))
			back.Add(p);
		forward.Clear();
		SetBar();
		return;
	}
	if(link.StartsWith("www.") || link.StartsWith("http") || link.StartsWith("mailto:"))
		LaunchWebBrowser(link);
}

void HelpWindow::Back()
{
	Pos p = GetPos();
	if(back.GetCount() && GoTo0(back.Top().link)) {
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
	if(forward.GetCount() && GoTo0(forward.Top().link)) {
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
	zoom.d = 1000;
	current_link = Null;
	GoTo0(topic);
	Refresh();
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
	bar.Add(t_("Font size"), CtrlImg::font_size(), THISBACK(FontSize));
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
	if(key == K_ESCAPE) {
		Close();
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
	tree.FindSetCursor(topic);
}

void HelpWindow::OpenDeep(int id)
{
	tree.OpenDeep(id);
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
	if(~tree != current_link) {
		if(tree.FindSetCursor(current_link))
			return;
		for(int i = 0; i < tree.GetLineCount(); i++) {
			Value k = tree.Get(tree.GetItemAtLine(i));
			if(!IsNull(k) && tree.FindSetCursor(k))
				break;
		}
	}
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
	view.WhenLink = THISBACK(GoTo);
	AddFrame(toolbar);
	view.SetZoom(Zoom(1, 1));
	zoom.m = 160;
	SetZoom();
	view.Margins(Rect(12, 12, 2, 2));
	SetRect(Ctrl::GetWorkArea().Deflated(80));
	tree.WhenSel = THISBACK(TreeCursor);
	tree.NoRoot();
	Icon(CtrlImg::help());
	SetBar();
	tree.BackPaint();
	view.BackPaintHint();
}

END_UPP_NAMESPACE
