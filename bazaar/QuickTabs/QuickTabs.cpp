#include <QuickTabs/QuickTabs.h>
//#include <Common/Common.h>

#define IMAGECLASS Img
#define IMAGEFILE <QuickTabs/QuickTabs.iml>
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

void FloatFrame::FrameLayout(Rect &r)
{
	switch(layout)
	{
		case LAYOUT_LEFT:
			LayoutFrameLeft(r, this, size);
			break;
		case LAYOUT_TOP:
			LayoutFrameTop(r, this, size);
			break;
		case LAYOUT_RIGHT:
			LayoutFrameRight(r, this, size);
			break;
		case LAYOUT_BOTTOM:
			LayoutFrameBottom(r, this, size);
			break;
	}
	r.top += border;
	r.left += border;
	r.right -= border;
	r.bottom -= border;
}

void FloatFrame::FrameAddSize(Size& sz)
{
	sz += border * 2;
	switch(layout)
	{
		case LAYOUT_LEFT:
			sz.cx += size;
			break;
		case LAYOUT_TOP:
			sz.cy += size;
			break;
		case LAYOUT_RIGHT:
			sz.cx -= size;
			break;
		case LAYOUT_BOTTOM:
			sz.cy -= size;
			break;		
	}	
}


void FloatFrame::FramePaint(Draw& w, const Rect& r)
{
	if(border > 0)
	{
		Rect n = r;
		switch(layout)
		{
			case LAYOUT_LEFT:
				n.left += size;
				break;
			case LAYOUT_TOP:
				n.top += size;
				break;
			case LAYOUT_RIGHT:
				n.right -= size;
				break;
			case LAYOUT_BOTTOM:
				n.bottom -= size;
				break;		
		}
		FieldFrame().FramePaint(w, n);
	}
	else
		FrameCtrl<Ctrl>::FramePaint(w, r);		
}


void FloatFrame::Fix(Size& sz)
{
	if(!horizontal)
		Swap(sz.cx, sz.cy);
}

void FloatFrame::Fix(Point& p)
{
	if(!horizontal)
		Swap(p.x, p.y);
}

TabScrollBar::TabScrollBar()
{
	FloatFrame::layout = FloatFrame::TOP;
	FloatFrame::size = QT_SBHEIGHT;
	FloatFrame::border = 0;
	
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
	Fix(sz);
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
	
	w.DrawRect(sz, /*Yellow*/White);
	
	if(horizontal)
	{
		if(total > sz.cx)
			w.DrawRect(ffloor(pos), 1, fceil(size), 2, Blue);
		else
			w.DrawRect(0, 1, sz.cx, 2, Blue);
	}
	else
	{
		if(total > sz.cy)
			w.DrawRect(1, ffloor(pos), 2, fceil(size), Blue);
		else
			w.DrawRect(1, 0, 2, sz.cy, Blue);
	}
}

void TabScrollBar::Layout()
{
	UpdatePos(false);
}

void TabScrollBar::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	Fix(p);
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
	Fix(p);
	old_pos = p.x;
}

void TabScrollBar::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;

	Fix(p);
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
	Fix(sz);
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::AddPos(int p, bool dontscale)
{
	Fix(sz);
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
	
	FloatFrame::layout = FloatFrame::TOP;
	FloatFrame::border = 0;
	FloatFrame::size = GetHeight();	
	
	AddFrame(sc);	
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

bool Tab::HasMouseCross(const Point& p, int w, int h)
{
	if(!visible)
		return false;

	Size isz = Img::CR0().GetSize();
	int iy = (h - isz.cy) / 2;
	int ix = x + cx - isz.cx - w - QT_MARGIN;

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

void QuickTabs::DrawLabel(Draw& w, int x, int y, Tab& t)
{
	String fn = GetFileName(t.name);

	if(horizontal)
		DrawFileName(w, x + QT_MARGIN + (QT_FILEICON + QT_SPACEICON) * int(file_icons), (y - t.tsz.cy) / 2,
	             t.tsz.cx, t.tsz.cy, fn.ToWString(),
	             false, StdFont(), Black, LtBlue, 
	             Null, Null, false);
	else
		w.DrawText((y + t.tsz.cy) / 2 + 1, 
				x + QT_MARGIN + (QT_FILEICON + QT_SPACEICON) * int(file_icons), 
				2700, fn.ToWString(), 
				style->font); 
}

void QuickTabs::DrawTab(Draw& w, Size& sz, int i)
{
	Size isz 	= Img::CR0().GetSize();
	int cnt 	= tabs.GetCount();
	Tab &t 		= tabs[i];

	bool ac 	= i == active;
	bool hl 	= i == highlight;
		
	int lx 		= i > 0 ? style->extendleft : 0;
	int x 		= t.x - sc.GetPos() + style->margin - lx;

	int ndx = !IsEnabled() ? CTRL_DISABLED :
		       ac ? CTRL_PRESSED :
		       hl ? CTRL_HOT : CTRL_NORMAL;

	Image nimg;
	
	int cx = t.cx + lx + (ac ? style->sel.right + style->sel.left : 0);
	int cy = ac ? t.cy - style->sel.top + style->sel.bottom : t.cy - style->sel.top;
	
	int h = style->sel.top + style->sel.bottom;
	int hs = ac ? 0 : h;
	if(layout == LAYOUT_RIGHT || layout == LAYOUT_BOTTOM)
		hs = -hs;
	
	ImageDraw img(cx, cy);
				
	img.DrawRect(0, 0, cx, cy, SColorFace);

	const Value& sv = (cnt == 1 ? style->both : i == 0 ? style->first : i == cnt - 1 ? style->last : style->normal)[ndx];

	ChPaint(img, 0, 0, cx, cy, sv);

	if(crosses && tabs.GetCount() > 1)
		img.DrawImage(t.cx - isz.cx  - QT_MARGIN, (cy + h - isz.cy) / 2,
	            (ac || hl) ? (cross == i ? Img::CR2 : ac ? Img::CR1 : Img::CR0) : 
	            Img::CR0);
	
	if(file_icons)
	{
		Image icon = Img::ARH;
		//Image icon = IdeFileImage(t.name);
		img.DrawImage(QT_MARGIN, (cy + hs - icon.GetSize().cx) / 2, icon);
	}
	
	int shift = 0;
	switch(layout)
	{
		case LAYOUT_LEFT:
			nimg = MirrorVert(RotateAntiClockwise(img));
			break;
		case LAYOUT_TOP:
			nimg = img;			
			break;
		case LAYOUT_RIGHT:
			nimg = RotateClockwise(img);
			shift = QT_SBSEPARATOR;
			break;
		case LAYOUT_BOTTOM:
			nimg = MirrorVert(img);
			shift = QT_SBSEPARATOR;
			break;
	}

	if(ac) 
		w.DrawImage(horizontal ? x - style->sel.left : shift, 
		            horizontal ? shift : x - style->sel.left, 
		            nimg);
	else
		w.DrawImage(horizontal ? x : style->sel.left - shift, 
		            horizontal ? style->sel.top - shift : x,
		            nimg);

	DrawLabel(w, x, cy + hs, t); 
}

void QuickTabs::Paint(Draw &w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorFace/*LtRed*/);

	if(!horizontal)
		w.DrawRect(layout == LAYOUT_LEFT ? sz.cx - 1 : 0, 0, 
				1, sz.cy, Color(128, 128, 128));
	else
		w.DrawRect(0, layout == LAYOUT_TOP ? sz.cy - 1 : 0, 
				sz.cx, 1, Color(128, 128, 128));

	Fix(sz);

	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].visible && i != active)
			DrawTab(w, sz, i);

	if(active >= 0)
		DrawTab(w, sz, active);
	 
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
			if(horizontal)
			{
				w.DrawRect(x + 1, y, 2, cy, c);
				w.DrawRect(x, y, 4, 1, c);
				w.DrawRect(x, y + cy - 1, 4, 1, c);
			}
			else
			{
				Swap(x, y);
				w.DrawRect(x, y + 1, cy, 2, c);
				w.DrawRect(x, y, 1, 4, c);
				w.DrawRect(x + cy - 1, y, 1, 4, c);				
			}
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

	int cnt = tabs.GetCount() - 1;
	t.name = name;
	t.group = GetFileFolder(name);
	t.id = GetNextId();
	MakeGroups();
	Repos();
	SetCursor(tabs.GetCount() - 1);
}

int QuickTabs::GetWidth()
{
	int j = GetLast();
	return tabs[GetLast()].Right() + style->margin * 2;
}

int QuickTabs::GetHeight()
{
	return style->tabheight + style->sel.top + QT_SBHEIGHT + QT_SBSEPARATOR;
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
	Fix(p);
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
	Fix(p);
	if(HasCapture())
	{
		sc.AddPos(p.x - oldp.x, true);
		oldp = p;
		return;
	}

	p.x += sc.GetPos() - style->margin;
	Size sz = GetSize();
	Fix(sz);
	int h = highlight;
	bool iscross = false;
	bool istab = false;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		Point np(p.x + (i > 0 ? style->extendleft : 0), p.y);
		if(tabs[i].HasMouse(np))
		{
			istab = true;
			//int h = sz.cy + (active == i ? 0 : style->sel.top);
			int h = style->sel.top + style->sel.bottom;
			int hs = active == i ? style->sel.top : h;
			if(layout == LAYOUT_RIGHT || layout == LAYOUT_BOTTOM)
				hs = -hs;
			hs += sz.cy;
			int ws = active == i ? style->sel.left : 0;
			
			iscross = crosses ? tabs[i].HasMouseCross(np, ws, hs) : false;
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

QuickTabs& QuickTabs::SetLayout(int l)
{
	layout = l;
	switch(l)
	{
		case LAYOUT_LEFT:
			horizontal = sc.horizontal = false;
			sc.layout = LAYOUT_RIGHT;
			break;
		case LAYOUT_TOP:
			horizontal = sc.horizontal = true;
			sc.layout = LAYOUT_BOTTOM;
			break;
		case LAYOUT_RIGHT:
			horizontal = sc.horizontal = false;
			sc.layout = LAYOUT_LEFT;
			break;
		case LAYOUT_BOTTOM:
			horizontal = sc.horizontal = true;
			sc.layout = LAYOUT_TOP;
			break;
	}
	RefreshLayout();
	return *this;
}

