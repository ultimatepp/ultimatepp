#include <ide/QuickTabs/QuickTabs.h>
#include <ide/Common/Common.h>

#define IMAGECLASS Img
#define IMAGEFILE <ide/QuickTabs/QuickTabs.iml>
#include <Draw/iml_source.h>

static String ParseGroup(const String &s)
{
	if(!s.IsEmpty())
	{
		if(s[0] == '/' || s[0] == '\\')
			return s.Mid(1);
		else if(s[1] == ':')
			return s.Mid(3);
	}
	return Null;
}

TabScrollBar::TabScrollBar()
{
	total = 0;
	pos = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	ready = false;
}

void TabScrollBar::UpdatePos(bool update)
{
	sz = GetSize();
	if(total <= 0 || sz.cx <= 0)
		cs = ics = 0;
	else
	{
		cs = sz.cx / ((double) total + 0.5);
		ics = total / ((double) sz.cx);
	}
	size = sz.cx * cs;
	if(update)
		pos = new_pos - start_pos;
	if(pos < 0)
		pos = 0;
	else if(pos + size > sz.cx)
		pos = sz.cx - size;

	ps = total > sz.cx ? pos * ics : 0;
	WhenScroll();
}

void TabScrollBar::Paint(Draw &w)
{
	if(!ready)
	{
		UpdatePos();
		ready = true;
	}
	Size sz = GetSize();
	w.DrawRect(sz, White);
	if(total > sz.cx)
		w.DrawRect(ffloor(pos), 1, fceil(size), 2, Blue);
	else
		w.DrawRect(0, 1, sz.cx, 2, Blue);
}

void TabScrollBar::Layout()
{
	UpdatePos(false);
}

void TabScrollBar::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	old_pos = new_pos = p.x;
	if(p.x < pos || p.x > pos + size)
		start_pos = size / 2;
	else
		start_pos = tabs(p.x - pos);
	UpdatePos();
	Refresh();
}

void TabScrollBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	old_pos = p.x;
}

void TabScrollBar::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;

	new_pos = p.x;
	UpdatePos();
	Refresh();
}

void TabScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	AddPos(-zdelta / 4, true);
}

int TabScrollBar::GetPos() const
{
	return ffloor(ps);
}

void TabScrollBar::SetPos(int p, bool dontscale)
{
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::AddPos(int p, bool dontscale)
{
	pos += total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

int TabScrollBar::GetTotal() const
{
	return total;
}

void TabScrollBar::SetTotal(int t)
{
	bool upd = total < t;
	total = t;
	UpdatePos(upd);
	Refresh();
}

void TabScrollBar::AddTotal(int t)
{
	total += t;
	UpdatePos();
	Refresh();
}

void TabScrollBar::GoEnd()
{
	pos = total;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::GoBegin()
{
	pos = 0;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::Set(const TabScrollBar& t)
{
	total = t.total;
	pos = t.pos;
	ps = t.ps;
	Refresh();
}

QuickTabs::QuickTabs()
{
	AddFrame(sc.Height(QT_SBHEIGHT));
	sc.WhenScroll = THISBACK(Scroll);
	highlight = -1;
	active = -1;
	cross = -1;
	target = -1;
	crosses = true;
	file_icons = false;
	grouping = true;
	isctrl = false;
	isdrag = false;
	id = -1;

	Group &g = groups.Add();
	g.name = "All";
	group = 0;

	SetStyle(TabCtrl::StyleDefault());
	BackPaint();
}

void QuickTabs::CloseAll()
{
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
		if(i != active)
			tabs.Remove(i);

	sc.SetTotal(tabs[0].cx);
	MakeGroups();
	Repos();
	SetCursor(0);
}

int QuickTabs::GetNextId()
{
	return ++id;
}

void QuickTabs::Menu(Bar& bar)
{
	bar.Add(tabs.GetCount() > 1, "Close", THISBACK1(Close, highlight));
	bar.Separator();
	int cnt = groups.GetCount();
	for(int i = 0; i < cnt; i++)
	{
		String name = Format("%s (%d)", groups[i].name, groups[i].count);
		Bar::Item &it = i > 0 ? bar.Add(name, THISBACK1(GroupMenu, i))
		                      : bar.Add(name, THISBACK1(DoGrouping, i));
		if(i == group)
			it.Image(Img::CHK);
		if(i == 0 && cnt > 1)
			bar.Separator();
	}
	bar.Separator();

	bar.Add("Close all tabs except current", THISBACK(CloseAll));
	bar.Add("Close non-project tabs", WhenCloseRest);
}

void QuickTabs::GroupMenu(Bar &bar, int n)
{
	bar.Add("Set active", THISBACK1(DoGrouping, n));
	bar.Add("Close", THISBACK1(DoCloseGroup, n));
}

bool Tab::HasMouse(const Point& p)
{
	return visible && p.x >= x && p.x < x + cx;
}

bool Tab::HasMouseCross(const Point& p, int h)
{
	if(!visible)
		return false;

	Size isz = Img::CR0().GetSize();
	int iy = (h - isz.cy) / 2;
	int ix = x + cx - isz.cx - QT_MARGIN;

	return p.x >= ix && p.x < ix + isz.cx &&
	       p.y >= iy && p.y < iy + isz.cy;
}


int QuickTabs::FindGroup(const String& g)
{
	for(int i = 0; i < groups.GetCount(); i++)
		if(groups[i].path == g)
			return i;
	return -1;
}

void QuickTabs::MakeGroups()
{
	groups[0].count = tabs.GetCount();
	groups[0].first = 0;
	groups[0].last = tabs.GetCount() - 1;

	for(int i = 1; i < groups.GetCount(); i++)
	{
		groups[i].count = 0;
		groups[i].first = 1000000;
		groups[i].last = 0;
	}

	for(int i = 0; i < tabs.GetCount(); i++)
	{
		Tab &tab = tabs[i];
		String s = ParseGroup(tab.group);
		if(IsNull(s))
			continue;
		int n = FindGroup(tab.group);
		if(n < 0)
		{
			Group &g = groups.Add();
			g.count = 1;
			g.first = 1000000;
			g.last = 0;
			g.name = s;
			g.path = tab.group;
			g.active = tab.id;
			n = groups.GetCount() - 1;
		}
		else
		{
			groups[n].count++;
			groups[n].last = i;
		}

		if(i < groups[n].first)
			groups[n].first = i;
		if(i > groups[n].last)
			groups[n].last = i;
	}


	int removed = 0;
	for(int i = 1; i < groups.GetCount(); i++)
		if(groups[i].count == 0)
		{
			groups.Remove(i - removed);
			removed++;
		}
	if(group > groups.GetCount() - 1 && group > 0)
		group--;

}

void QuickTabs::DoGrouping(int n)
{
	group = n;
	Repos();
	SetCursor(-1);
}

void QuickTabs::DoCloseGroup(int n)
{
	int cnt = groups.GetCount();
	if(cnt <= 0)
		return;

	if(cnt == n)
		--group;

	String group = groups[n].path;

	for(int i = tabs.GetCount() - 1; i >= 0; i--)
	{
		String s = ParseGroup(tabs[i].group);
		if(IsNull(s))
			continue;
		if(group == tabs[i].group && tabs.GetCount() > 1)
			tabs.Remove(i);
	}
	if(cnt > 1)
		groups.Remove(n);
	MakeGroups();
	Repos();
	SetCursor(-1);
}

void QuickTabs::DrawTab(Draw &w, int i)
{
	Size sz = GetSize();

	Size isz = Img::CR0().GetSize();
	int cnt = tabs.GetCount();

	Tab &t = tabs[i];

	int lx = i > 0 ? style->extendleft : 0;

	int x = t.x - sc.GetPos() + style->margin - lx;

	bool ac = i == active;
	bool hl = i == highlight;

	int ndx = !IsEnabled() ? CTRL_DISABLED :
		       ac ? CTRL_PRESSED :
		       hl ? CTRL_HOT : CTRL_NORMAL;

	const Value& sv = (cnt == 1 ? style->both : i == 0 ? style->first : i == cnt - 1 ? style->last : style->normal)[ndx];

	if(ac)
		ChPaint(w, x - style->sel.left,
		           0,
		           t.cx + style->sel.right + style->sel.left + lx,
		           t.cy + style->sel.bottom,
		           sv);
	else
		ChPaint(w, x,
		           style->sel.top,
		           t.cx + lx,
		           t.cy - style->sel.top,
		           sv);

	int h = sz.cy + (ac ? 0 : style->sel.top);

	if(crosses && tabs.GetCount() > 1)
		w.DrawImage(x + t.cx - isz.cx - QT_MARGIN,
		            (h - isz.cy) / 2,
		            (ac || hl) ? (cross == i ? Img::CR2 : ac ? Img::CR1 : Img::CR0) : Img::CR0);

	if(file_icons)
	{
		//Image icon = Img::ARH;
		Image icon = IdeFileImage(t.name);
		w.DrawImage(x + QT_MARGIN, (h - icon.GetSize().cx) / 2, icon);
	}

	String fn = GetFileName(t.name);
	int ty = (h - t.tsz.cy) / 2 + 1;
	DrawFileName(w, x + QT_MARGIN + (QT_FILEICON + QT_SPACEICON) * int(file_icons), ty,
	             t.tsz.cx, t.tsz.cy, fn.ToWString(),
	             false, StdFont(),  SColorLabel(), LtBlue, Null, Null, false);

}

void QuickTabs::Paint(Draw &w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorFace());
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Color(128, 128, 128));

	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].visible && i != active)
			DrawTab(w, i);

	if(active >= 0)
		DrawTab(w, active);


	if(target >= 0)
	{
		int last = GetLast();
		int first = GetFirst();

		if(target != active && target != active + 1)
		{
			int x = (target == last + 1 ? tabs[last].Right() : tabs[target].x)
			        - sc.GetPos() - (target <= first ? 1 : 2)
			        + style->margin - (target > 0 ? style->extendleft : 0);

			int y = style->sel.top;
			int cy = sz.cy - y;
			Color c(255, 0, 0);
			w.DrawRect(x + 1, y, 2, cy, c);
			w.DrawRect(x, y, 4, 1, c);
			w.DrawRect(x, y + cy - 1, 4, 1, c);
		}
	}
}

void QuickTabs::Scroll()
{
	Refresh();
}

int QuickTabs::GetWidth(int n)
{
	Tab &t = tabs[n];
	t.tsz = GetTextSize(GetFileName(t.name), StdFont());
	return QT_MARGIN * 2 + t.tsz.cx + (QT_SPACE + Img::CR0().GetSize().cx) * crosses
	                                + (QT_FILEICON + QT_SPACEICON) * file_icons;
}

void QuickTabs::Add(const char *name, bool make_active)
{
	Tab &t = tabs.Add();

//	int cnt = tabs.GetCount() - 1; Mirek: unused
	t.name = name;
	t.group = GetFileFolder(name);
	t.id = GetNextId();
	MakeGroups();
	Repos();
	SetCursor(tabs.GetCount() - 1);
}

int QuickTabs::GetWidth()
{
//	int j = GetLast(); Mirek: unused
	return tabs[GetLast()].Right() + style->margin * 2;
}

int QuickTabs::GetHeight()
{
	return style->tabheight + style->sel.top /*+ style->sel.bottom */ + QT_SBHEIGHT + QT_SBSEPARATOR;
}

void QuickTabs::Repos()
{
	if(!tabs.GetCount())
		return;

	String g = GetGroupName();

	int j = 0;
	bool first = true;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		bool v = IsNull(g) ? true : g == tabs[i].group;
		if(v)
		{
			tabs[i].x = first ? 0 : tabs[j].Right();
			j = i;
			first = false;
		}
		else
			tabs[i].x = 0;

		tabs[i].visible = v;
		tabs[i].y = 0;
		tabs[i].cx = GetWidth(i);
		tabs[i].cy = style->tabheight + style->sel.top;
	}
	sc.SetTotal(GetWidth());
}

int QuickTabs::Find(int id)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].id == id)
			return i;
	return -1;
}

int QuickTabs::GetNext(int n)
{
	for(int i = n + 1; i < tabs.GetCount(); i++)
		if(tabs[i].visible)
			return i;
	return -1;
}

int QuickTabs::GetPrev(int n)
{
	for(int i = n - 1; i >= 0; i--)
		if(tabs[i].visible)
			return i;
	return -1;
}

void QuickTabs::Clear()
{
	tabs.Clear();
	Refresh();
}

QuickTabs& QuickTabs::FileIcons(bool b)
{
	file_icons = b;
	Repos();
	return *this;
}

QuickTabs& QuickTabs::Crosses(bool b)
{
	crosses = b;
	Repos();
	return *this;
}

QuickTabs& QuickTabs::Grouping(bool b)
{
	grouping = b;
	return *this;
}

void QuickTabs::LeftDown(Point p, dword keyflags)
{
	if(keyflags & K_SHIFT)
	{
		highlight = -1;
		Refresh();
		SetCapture();
		oldp = p;
		return;
	}

	isctrl = keyflags & K_CTRL;
	if(isctrl)
		return;

	if(cross != -1)
		Close(cross);
	else if(highlight >= 0)
		SetCursor(highlight);
}

void QuickTabs::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
}

void QuickTabs::RightDown(Point p, dword keyflags)
{
	MenuBar::Execute(THISBACK(Menu), GetMousePos());
}

void QuickTabs::MiddleDown(Point p, dword keyflags)
{
	Close(highlight);
}

void QuickTabs::MiddleUp(Point p, dword keyflags)
{
}

int QuickTabs::GetTargetTab(Point p)
{
	p.x += sc.GetPos();

	int f = GetFirst();
	int l = GetLast();

	if(tabs[f].visible && p.x < tabs[f].x + tabs[f].cx / 2)
		return f;

	for(int i = l; i >= f; i--)
		if(tabs[i].visible && p.x >= tabs[i].x + tabs[i].cx / 2)
			return i == l ? i + 1 : GetNext(i);
	return -1;
}

void QuickTabs::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sc.AddPos(-zdelta / 4, true);
	MouseMove(p, 0);
}

void QuickTabs::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
	{
		sc.AddPos(p.x - oldp.x, true);
		oldp = p;
		return;
	}

	p.x += sc.GetPos() - style->margin;
	Size sz = GetSize();

	int h = highlight;
	bool iscross = false;
	bool istab = false;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		Point np(p.x + (i > 0 ? style->extendleft : 0), p.y);
		if(tabs[i].HasMouse(np))
		{
			istab = true;
			int h = sz.cy + (active == i ? 0 : style->sel.top);
			iscross = crosses ? tabs[i].HasMouseCross(np, h) : false;
			if(highlight != i || (iscross && cross != i))
			{
				cross = iscross ? i : -1;
				highlight = i;
				Refresh();
				return;
			}
		}
	}

	if(!istab && h >= 0)
	{
		highlight = -1;
		Refresh();
	}

	if(!iscross && cross >= 0)
	{
		cross = -1;
		Refresh();
	}
}

void QuickTabs::MouseLeave()
{
	if(isdrag)
		return;

	highlight = -1;
	cross = -1;
	Refresh();
}

void QuickTabs::FrameLayout(Rect &r)
{
	LayoutFrameTop(r, this, GetHeight());
	r.top += 2;
	r.left += 2;
	r.right -= 2;
	r.bottom -= 2;
}

void QuickTabs::FrameAddSize(Size& sz)
{
	sz += 4;
	sz.cy += GetHeight();
}

void QuickTabs::FramePaint(Draw& w, const Rect& rr)
{
	Rect r = rr;
	r.top += GetHeight();
	ViewFrame().FramePaint(w, r);
}

void QuickTabs::DragAndDrop(Point p, PasteClip& d)
{
	int c = GetTargetTab(p);
	int tab = isctrl ? highlight : active;

	bool sametab = c == tab || c == tab + 1;
	bool internal = AcceptInternal<QuickTabs>(d, "tabs");

	if(!sametab && d.IsAccepted())
	{
		if(internal)
		{
			int id = tabs[active].id;
			Tab t = tabs[tab];
			tabs.Insert(c, t);
			tabs.Remove(tab + int(c < tab));
			active = Find(id);
			isdrag = false;
			MakeGroups();
			Repos();
			return;
		}
		else if(d.IsPaste())
		{
			CancelMode();
			return;
		}
	}
	else
	{
		//d.Reject();
		//unfortunately after Reject DragLeave stops working until d is accepted
	}

	if(c != target)
	{
		target = c;
		Refresh();
	}
}

void QuickTabs::CancelMode()
{
	isdrag = false;
	target = -1;
	Refresh();
}

void QuickTabs::LeftDrag(Point p, dword keyflags)
{
	if(keyflags & K_SHIFT)
		return;
	if(highlight < 0)
		return;

	isdrag = true;
	DoDragAndDrop(InternalClip(*this, "tabs"));
}

void QuickTabs::DragEnter()
{
}

void QuickTabs::DragLeave()
{
	target = -1;
	Refresh();
}

void QuickTabs::DragRepeat(Point p)
{
	if(target >= 0)
	{
		int dx = GetDragScroll(this, p, 16).x;
		if(dx != 0)
			sc.AddPos(dx);
	}
}

void QuickTabs::SetCursor(int n)
{
	if(tabs.GetCount() == 0)
		return;

	if(n < 0)
	{
		n = max(0, Find(GetActiveGroup()));
		active = -1;
	}

	bool is_all = IsGroupAll();
	bool same_group = tabs[n].group == GetGroupName();

	if((same_group || is_all) && active == n)
		return;

	active = n;

	if(!is_all && !same_group)
	{
		SetGroup(tabs[n].group);
		Repos();
	}

	SetActiveGroup(tabs[n].id);

	int cx = tabs[n].x - sc.GetPos();
	if(cx < 0)
		sc.AddPos(cx - 10);
	else
	{
		cx = tabs[n].x + tabs[n].cx - GetSize().cx - sc.GetPos();
		if(cx > 0)
			sc.AddPos(cx + 10);
	}
	if(HasMouse())
		MouseMove(GetMouseViewPos(), 0);

	UpdateActionRefresh();
}

void QuickTabs::Close(int n)
{
	if(tabs.GetCount() <= 1)
		return;

	if(n == active)
	{
		int c = Find(tabs[n].id);
		int nc = GetNext(c);
		if(nc < 0)
			nc = max(0, GetPrev(c));

		SetActiveGroup(tabs[nc].id);
	}
	sc.AddTotal(-tabs[n].cx);
	tabs.Remove(n);
	MakeGroups();
	Repos();
	SetCursor(-1);
}

bool QuickTabs::FindSetFile(const String& fn)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].name == fn) {
			SetCursor(i);
			return true;
		}
	return false;
}

void QuickTabs::SetAddFile(const String& fn)
{
	if(IsNull(fn))
		return;
	if(FindSetFile(fn))
		return;
	Add(fn, true);
	Refresh();
}

void QuickTabs::RenameFile(const String& fn, const String& nn)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].name == fn)
		{
			tabs[i].name = nn;
			tabs[i].group = GetFileFolder(nn);
		}
	Repos();
}

void QuickTabs::Set(const QuickTabs& t)
{
	active = t.active;
	file_icons = t.file_icons;
	crosses = t.crosses;
	grouping = t.grouping;

	cross = -1;
	highlight = -1;
	target = -1;

	tabs.SetCount(t.GetCount());
	for(int i = 0; i < t.GetCount(); i++)
	{
		tabs[i].name = t.tabs[i].name;
		tabs[i].group = t.tabs[i].group;
	}
	MakeGroups();
	SetGroup(t.GetGroup());
	Repos();
	sc.Set(t.sc);
}

void QuickTabs::SerializeTabs(Stream& s)
{
	int version = 0;
	s / version;
	s % active % file_icons % crosses % grouping;
	cross = -1;
	highlight = -1;
	target = -1;
	int n = tabs.GetCount();
	s % n;
	tabs.SetCount(n);
	for(int i = 0; i < tabs.GetCount(); i++)
		s % tabs[i].name % tabs[i].group;
	MakeGroups();
	int g = GetGroup();
	s % g;
	SetGroup(g);
	Repos();
}
