#include "TabBar.h"

#define TFILE <TabBar/TabBar.t>
#include <Core/t.h>

#define IMAGECLASS TabBarImg
#define IMAGEFILE <TabBar/TabBar.iml>
#include <Draw/iml_source.h>

NAMESPACE_UPP

// AlignedFrame
void AlignedFrame::FrameLayout(Rect &r)
{
	switch(layout)
	{
		case LEFT:
			LayoutFrameLeft(r, this, framesize);
			break;
		case TOP:
			LayoutFrameTop(r, this, framesize);
			break;
		case RIGHT:
			LayoutFrameRight(r, this, framesize);
			break;
		case BOTTOM:
			LayoutFrameBottom(r, this, framesize);
			break;
	}
	r.top += border;
	r.left += border;
	r.right -= border;
	r.bottom -= border;
}

void AlignedFrame::FrameAddSize(Size& sz)
{
	sz += border * 2;
	IsVert() ? sz.cx += framesize : sz.cy += framesize;
}

void AlignedFrame::FramePaint(Draw& w, const Rect& r)
{
	if(border > 0)
	{
		Rect n = r;
		switch(layout)
		{
			case LEFT:
				n.left += framesize;
				break;
			case TOP:
				n.top += framesize;
				break;
			case RIGHT:
				n.right -= framesize;
				break;
			case BOTTOM:
				n.bottom -= framesize;
				break;		
		}
		ViewFrame().FramePaint(w, n);
	}
	else
		FrameCtrl<Ctrl>::FramePaint(w, r);		
}

AlignedFrame& AlignedFrame::SetFrameSize(int sz, bool refresh)
{
	framesize = sz; 
	if (refresh) RefreshParentLayout(); 
	return *this;	
}

void AlignedFrame::Fix(Size& sz)
{
	if(IsVert())
		Swap(sz.cx, sz.cy);
}

void AlignedFrame::Fix(Point& p)
{
	if(IsVert())
		Swap(p.x, p.y);
}

Size AlignedFrame::Fixed(const Size& sz)
{
	return IsVert() ? Size(sz.cy, sz.cx) : Size(sz.cx, sz.cy);
}

Point AlignedFrame::Fixed(const Point& p)
{
	return IsVert() ? Point(p.y, p.x) : Point(p.x, p.y);
}

// TabScrollBar
TabScrollBar::TabScrollBar()
{
	Clear();
}

void TabScrollBar::Clear()
{
	total = 0;
	pos = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	sz.Clear();
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
}

void TabScrollBar::Paint(Draw &w)
{
	if(!ready)
	{
		UpdatePos();
		ready = true;
	}
	Size rsz = GetSize();
	#ifdef TABBAR_DEBUG
	w.DrawRect(rsz, Red);
	#else
	w.DrawRect(rsz, White);
	#endif
	Point p;
	
	if(total > sz.cx) {
		p = Point(ffloor(pos), 1);
		rsz = Size(fceil(size), 2);
	}
	else {
		p = Point(0, 1);
		rsz = Size(sz.cx, 2);
	}
	Fix(p);
	Fix(rsz);
	w.DrawRect(p.x, p.y, rsz.cx, rsz.cy, Blue);
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
	UpdateActionRefresh();	
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
	UpdateActionRefresh();
}

void TabScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	AddPos(-zdelta / 4, true);
	UpdateActionRefresh();
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
	sz = GetSize();
	Fix(sz);
	total += t;
	if(total <= 0 || sz.cx <= 0)
		cs = ics = 0;
	else
		cs = sz.cx / ((double) total + 0.5);
	size = sz.cx * cs;
	ps = min(ps, (double)(total-sz.cx));
	pos = (int)(ps * cs);		
	old_pos = new_pos = (int)(pos - start_pos);
	
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

bool TabScrollBar::IsScrollable() const
{
	// Note: sz already 'fixed'
	return total > sz.cx && sz.cx > 0;
}

// TabBar
TabBar::Style TabBar::leftstyle;
TabBar::Style TabBar::rightstyle;
TabBar::Style TabBar::bottomstyle;

TabBar::TabBar()
{
	Clear();
	display = NULL;
	crosses = true;
	grouping = true;
	isctrl = false;
	isdrag = false;
	inactivedisabled = false;
	autoscrollhide = true;
	stacking = false;
	groupsort = false;
	groupseps = false;
	allownullcursor = false;
	icons = true;
	mintabcount = 1;
	style[0] = style[1] = style[2] = style[3] = NULL;
	SetAlign(TOP);
	SetFrameSize(GetHeight(false));
	BackPaint();
}

void TabBar::CloseAll(int exception)
{
	Vector<Value>vv;
	for(int i = 0; i < tabs.GetCount(); i++)
		if(i != exception)
			vv.Add(tabs[i].key);
	if (CancelCloseAll() || CancelCloseSome(Vector<Value>(vv,0))) return;
	WhenCloseAll();
	WhenCloseSome(vv);
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
		if(i != exception)
			tabs.Remove(i);

	MakeGroups();
	Repos();
	SetCursor(0);
	Refresh();
}

int TabBar::GetNextId()
{
	return ++id;
}

void TabBar::ContextMenu(Bar& bar)
{
	if (highlight >= 0) {
		bar.Add(tabs.GetCount() > 1, t_("Close"), THISBACK1(Close, highlight));
		bar.Separator();
	}
	int cnt = groups.GetCount();
	for(int i = 0; i < cnt; i++)
	{
		String name = Format("%s (%d)", groups[i].name, groups[i].count);
		Bar::Item &it = i > 0 ? bar.Add(name, THISBACK1(GroupMenu, i))
		                      : bar.Add(name, THISBACK1(DoGrouping, i));
		if(i == group)
			it.Image(TabBarImg::CHK);
		if(i == 0 && cnt > 1)
			bar.Separator();
	}
	bar.Separator();

	bar.Add(t_("Close others"), THISBACK1(CloseAll, highlight));
}

void TabBar::GroupMenu(Bar &bar, int n)
{
	bar.Add(t_("Set active"), THISBACK1(DoGrouping, n));
	bar.Add(t_("Close"), THISBACK1(DoCloseGroup, n));
}

bool TabBar::Tab::HasMouse(const Point& p) const
{
	return visible && p.x >= tab_pos.x && p.x < tab_pos.x + tab_size.cx &&
	                  p.y >= tab_pos.y && p.y < tab_pos.y + tab_size.cy;
}

bool TabBar::Tab::HasMouseCross(const Point& p) const
{
	if(!visible)
		return false;

	return p.x >= cross_pos.x && p.x < cross_pos.x + cross_size.cx &&
	       p.y >= cross_pos.y && p.y < cross_pos.y + cross_size.cy;
}

int TabBar::FindGroup(const String& g) const
{
	for(int i = 0; i < groups.GetCount(); i++)
		if(groups[i].name == g)
			return i;
	return -1;
}

void TabBar::DoStacking()
{
	Value v = GetData();
		
	// Reset stack info
	for (int i = 0; i < tabs.GetCount(); i++) {
		Tab &t = tabs[i];
		t.stack = -1;
		t.stackid = GetStackId(t);
		t.sort_order = GetStackSortOrder(t);
	}
	// Create stacks
	Vector<Vector<Tab> > tstack;
	for (int i = 0; i < tabs.GetCount(); i++) {
		Tab &ti = tabs[i];
		if (ti.stack < 0) {
			ti.stack = tstack.GetCount();
			Vector<Tab> &ttabs = tstack.Add();
			ttabs.Add(ti);
			for (int j = i+1; j < tabs.GetCount(); j++)	{
				Tab &tj = tabs[j];
				if (tj.stack < 0 && tj.stackid == ti.stackid && (!grouping || tj.group == ti.group)) {
					tj.stack = ti.stack;
					ttabs.Add(tj);
				}
			}
		}
	}
	stackcount = tstack.GetCount();
	// Recombine
	tabs.SetCount(0);
	for (int i = 0; i < tstack.GetCount(); i++) {
		Sort(tstack[i], TabStackSort());
		tabs.AppendPick(tstack[i]);
	}
	highlight = -1;
	SetData(v);
	MakeGroups();	
	Repos();
}

void TabBar::DoUnstacking() 
{
	stackcount = 0;
	for (int i = 0; i < tabs.GetCount(); i++)
		tabs[i].stack = -1;
	highlight = -1;
	MakeGroups();	
	Repos();
	if (HasCursor())	
		SetCursor(-1);
	else
		Refresh();
}

void TabBar::MakeGroups()
{
	groups[0].count = tabs.GetCount();
	groups[0].first = 0;
	groups[0].last = tabs.GetCount() - 1;

	if (groupsort)
		StableSort(tabs, TabGroupSort());

	for(int i = 1; i < groups.GetCount(); i++)
	{
		groups[i].count = 0;
		groups[i].first = 10000000;
		groups[i].last = 0;
	}

	for(int i = 0; i < tabs.GetCount(); i++)
	{
		Tab &tab = tabs[i];
		int n = FindGroup(tab.group);
		ASSERT(n >= 0);
		if (n > 0) {
			if(groups[n].active < 0)
				groups[n].active = tab.id;
			groups[n].count++;
			groups[n].last = i;
	
			if(i < groups[n].first)
				groups[n].first = i;
			if(i > groups[n].last)
				groups[n].last = i;
		}
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

void TabBar::DoGrouping(int n)
{
	group = n;
	Repos();
	SyncScrollBar();
	SetCursor(-1);
}

void TabBar::DoCloseGroup(int n)
{
	int cnt = groups.GetCount();
	if(cnt <= 0)
		return;

	String groupName = groups[n].name;
	
	/*
		do WhenCloseSome()/CancelCloseSome() checking
		before WhenClose()/CancelClose() stuff
		(that code must be reviewed anyways...)
		In order to leave existing code as it is, following
		changes have effect *ONLY* if WhenCloseSome()/CancelCloseSome()
		callbacks are used, otherwise previous path is taken.
		I think we should anyways review some parts of it later
	*/
	
	if(WhenCloseSome || CancelCloseSome)
	{
		Vector<Value>vv;
		int nTabs = 0;
		for(int i = 0; i < tabs.GetCount(); i++)
			if(groupName == tabs[i].group) {
				vv.Add(tabs[i].key);
				nTabs++;
			}
		// at first, we check for CancelCloseSome()
		if(vv.GetCount() && !CancelCloseSome(Vector<Value>(vv,0))) {
			// we didn't cancel globally, now we check CancelClose()
			// for each tab -- group gets removed ONLY if ALL of
			// group tabs are closed
			for(int i = tabs.GetCount() - 1; i >= 0; i--) {
				if(groupName == tabs[i].group && tabs.GetCount() > 1) {
					Value v = tabs[i].key;
					if(!CancelClose(v))
					{
						nTabs--;
						WhenClose(v);
						tabs.Remove(i);
					}
				}
				// remove group if all of its tabs get closed
				if(!nTabs) {
					if(cnt == n)
						group--;
					if(cnt > 1)
						groups.Remove(n);
				}
				MakeGroups();
				Repos();
				SetCursor(-1);
			}
		}
		return;
	}

	// previous code path, taken if WhenCancelSome()/WhenCloseSome()
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
	{
		if(groupName == tabs[i].group && tabs.GetCount() > 1) {
			Value v = tabs[i].value; // should be key ??
			if (!CancelClose(v)) {
				WhenClose(v);
				tabs.Remove(i);
			}
		}
	}

	if (cnt == n)
		group--;

	if(cnt > 1) // what if CancelClose suppressed some tab closing ?
		groups.Remove(n);
	MakeGroups();
	Repos();
	SetCursor(-1);
}

void TabBar::NewGroup(const String &name)
{
	Group &g = groups.Add();
	g.name = name;
	g.count = 0;
	g.first = 10000000;
	g.last = 0;
	g.active = -1;
}

Value TabBar::AlignValue(int align, const Value &v, const Size &isz)
{
	if (align == TOP) return v;
	if (IsTypeRaw<Image>(v)) {
		switch(align) {
			case AlignedFrame::LEFT:   return RotateAntiClockwise((Image)v);
			case AlignedFrame::RIGHT:  return RotateClockwise((Image)v);
			case AlignedFrame::BOTTOM: return MirrorVert((Image)v);
		}
	}
	else if (!IsTypeRaw<Color>(v)) {
		ImageDraw w(isz.cx, isz.cy);
		w.DrawRect(isz, SColorFace());
		ChPaint(w, isz, v);
		ImageBuffer img;
		Image temp = (Image)w;
		switch(align) {
			case AlignedFrame::LEFT: 
				temp = RotateAntiClockwise(temp);
				img = temp; // GCC
				img.SetHotSpot(Point(10, 10));
				img.Set2ndSpot(Point(isz.cy / 2, isz.cx / 2));
				break;
			case AlignedFrame::RIGHT: 
				temp = RotateClockwise(temp);
				img = temp;
				img.SetHotSpot(Point(isz.cy - 10, isz.cx - 10));
				img.Set2ndSpot(Point(isz.cy / 2, isz.cx / 2));
				break;
			case AlignedFrame::BOTTOM:
				temp = MirrorVert(temp);
				img = temp;
				img.SetHotSpot(Point(10, 10));
				img.Set2ndSpot(Point(isz.cx / 2, isz.cy / 2));
				break;

		}
		return (Image)img;
	}	
	return v;
}

void TabBar::PaintStackedTab(Draw& w, const Rect &r, const Tab& tab, const Font &font, Color ink, dword style)
{
	if (!IsNull(tab.img))
		w.DrawImage(r.left, r.top, tab.img);
	else
		w.DrawText(r.left, r.top, "...", font, ink);	
}

void TabBar::PaintTab(Draw& w, const Rect& r, const Tab& tab, const Font &font, Color ink, dword style)
{
	WString txt;
	Font f = font;
	Color i = ink;
	const Value &q = tab.value;

	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		if(!IsNull(t.font))
			f = t.font;
		i = IsNull(t.ink) ? ink : t.ink;
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);

	Point p;
	int text_offset = TB_MARGIN;
	if (PaintIcons() && tab.HasIcon()) {
		Size isz = min(tab.img.GetSize(), Size(TB_ICON, TB_ICON));
		p = GetImagePosition(r, isz.cx, isz.cy, text_offset, LEFT);
		w.DrawImage(p.x, p.y, isz.cx, isz.cy, tab.img);
		text_offset += TB_ICON + 2;
	}
	p = GetTextPosition(r, GetTextSize(txt, f).cy, text_offset);
	w.DrawText(p.x, p.y, GetTextAngle(), txt, f, i);
}

Point TabBar::GetTextPosition(const Rect& r, int cy, int space) const
{
	Point 	p;
	int align = GetAlign();
	
	if(align == LEFT)
	{
		p.y = r.bottom - space;
		p.x = r.left + (r.GetWidth() - cy) / 2;
	}
	else if(align == RIGHT)
	{
		p.y = r.top + space;
		p.x = r.right - (r.GetWidth() - cy) / 2;
	}
	else
	{
		p.x = r.left + space;
		p.y = r.top + (r.GetHeight() - cy) / 2;
	}
	return p;
}

Point TabBar::GetImagePosition(const Rect& r, int cx, int cy, int space, int side) const
{
	Point p;
	int align = GetAlign();

	if (align == LEFT)
	{
		p.x = r.left + (r.GetWidth() - cy) / 2 + 2;
		p.y = side == LEFT ? r.bottom - space - cx : r.top + space;
	}
	else if (align == RIGHT)
	{
		p.x = r.right - (r.GetWidth() + cy) / 2 - 2;
		p.y = side == LEFT ? r.top + space : r.bottom - space - cx;
	}
	else if (align == TOP)
	{
		p.x = side == LEFT ? r.left + space : r.right - cx - space;
		p.y = r.top + (r.GetHeight() - cy) / 2 + 2;
	}
	else if (align == BOTTOM)
	{
		p.x = side == LEFT ? r.left + space : r.right - cx - space;
		p.y = r.bottom - (r.GetHeight() + cy) / 2 - 2;
	}
	return p;
}

void TabBar::PaintTab(Draw &w, const Style &s, const Size &sz, int n, bool enable, bool dragsample)
{
	TabBar::Tab &t = tabs[n];
	
	int align = GetAlign();
	int cnt = dragsample ? 1 : tabs.GetCount();
	int sep = TB_SBSEPARATOR * sc.IsVisible();
	
	bool ac = n == active;
	bool hl = n == highlight || (stacking && highlight >= 0 && tabs[highlight].stack == t.stack);

	int ndx = !enable ? CTRL_DISABLED :
		       ac ? CTRL_PRESSED :
		       hl ? CTRL_HOT : CTRL_NORMAL;

	int c = align == LEFT ? cnt - n : n;	
	const Value& sv = (cnt == 1 ? s.both : c == 0 ? s.first : c == cnt - 1 ? s.last : s.normal)[ndx];

	int lx = n > 0 ? s.extendleft : 0;
	int x = t.pos.x - sc.GetPos() - lx + s.margin;
	
	Point pa = Point(x - s.sel.left, 0);
	Size  sa = Size(t.size.cx + lx + s.sel.right + s.sel.left, t.size.cy + s.sel.bottom);

	Point pn = Point(x, s.sel.top);
	Size  sn = Size(t.size.cx + lx, t.size.cy - s.sel.top);

	int dy = (-s.sel.top) * ac;
	int sel = s.sel.top;

	if (align == BOTTOM || align == RIGHT)
	{
		pa.y -= s.sel.bottom - sep;
		pn.y -= s.sel.top - sep;
		dy = -dy;
		sel = s.sel.bottom;
	}
			
	Rect ra(Fixed(pa), Fixed(sa));
	Rect rn(Fixed(pn), Fixed(sn));

	t.tab_pos = (ac ? ra : rn).TopLeft();
	t.tab_size = (ac ? ra : rn).GetSize();
		
	if(dragsample)
	{
		ChPaint(w, Rect(Point(0, 0), t.tab_size), sv);							
		rn = Rect(Fixed(Point(s.sel.left * ac, sel * ac + dy)), Fixed(sn));
	}
	else
	{
		ChPaint(w, Rect(t.tab_pos, t.tab_size), sv);		
		rn = Rect(Fixed(Point(pn.x, pn.y + dy)), Fixed(sn));
	}
	
	#ifdef TABBAR_DEBUG
	DrawFrame(w, rn, Green);
	#endif
	
	if(crosses && cnt > mintabcount && !dragsample) {
		t.cross_size = TabBarImg::CR0().GetSize();
		t.cross_pos = GetImagePosition(rn, t.cross_size.cx, t.cross_size.cy, TB_MARGIN, RIGHT);
		w.DrawImage(t.cross_pos.x, t.cross_pos.y, (ac || hl) 
			? (cross == n ? TabBarImg::CR2 : ac ? TabBarImg::CR1 : TabBarImg::CR0)
			: TabBarImg::CR0);
	}
		
	if (display)
		display->Paint(w, rn, t.value, s.text_color[ndx], SColorDisabled(), ndx);
	else {
		PaintTab(w, rn, t, s.font, s.text_color[ndx], ndx);	
		if (stacking) {
			int ix = n+1;
			int lastcx = t.stdcx + TB_MARGIN;
			while (ix < tabs.GetCount() && tabs[ix].stack == t.stack) {
				Tab &q = tabs[ix];
				
				int ndx = !enable ? CTRL_DISABLED : 
						   highlight == ix ? CTRL_HOT : CTRL_NORMAL;
				
				rn = (align == LEFT)  ? Rect(rn.BottomLeft() - Point(0, lastcx + q.size.cx), Size(sn.cy, q.size.cx)) :
					 (align == RIGHT) ? Rect(rn.TopLeft() + Point(0, lastcx), Size(sn.cy, q.size.cx)) 
					 				  : Rect(rn.TopLeft() + Point(lastcx, 0), Size(q.size.cx, sn.cy)); 
				
				PaintStackedTab(w, rn, q, s.font, s.text_color[ndx], ndx);

				q.tab_pos = rn.TopLeft();
				q.tab_size = rn.GetSize();
	
				lastcx = q.size.cx;
				ix++;					
			}
		}
	}
}

void TabBar::PaintSeparator(Draw &w, const Rect r)
{
	if (IsVert()) {
		int cy = TabBarImg::SEP().GetSize().cy;
		ChPaint(w, 0, r.top + (r.GetHeight() - cy)/2, r.GetWidth(), cy, TabBarImg::SEPV());			
	}
	else {
		int cx = TabBarImg::SEP().GetSize().cx;
		ChPaint(w, r.left + (r.GetWidth() - cx)/2, 0, cx, r.GetHeight(), TabBarImg::SEP());			
	}
}

void TabBar::Paint(Draw &w)
{
	int align = GetAlign();
	const Style &st = *style[align];
	Size sz = GetSize();
	
	#ifdef TABBAR_DEBUG
	w.DrawRect(sz, Yellow);
	#else
	w.DrawRect(sz, SColorFace());
	#endif
	
	if(sc.IsShown())
	{
		IsVert() ? 
			w.DrawRect(align == LEFT ? sz.cx - 1 : 0, 0, 1, sz.cy, Color(128, 128, 128)):
			w.DrawRect(0, align == TOP ? sz.cy - 1 : 0, sz.cx, 1, Color(128, 128, 128));	
	}

	if (!tabs.GetCount()) return;
	
	int limt = sc.GetPos() + (IsVert() ? sz.cy : sz.cx);	
	int first = 0;
	int last = tabs.GetCount()-1;
	// Find first visible tab
	for(int i = 0; i < tabs.GetCount(); i++) {
		Tab &tab = tabs[i]; 
		if (tab.pos.x + tab.size.cx > sc.GetPos()) {
			first = i;
			break;
		}
	}
	// Find last visible tab
	for(int i = first + 1; i < tabs.GetCount(); i++) { 
		if (tabs[i].visible && tabs[i].pos.x > limt) {
			last = i;
			break;
		}
	}
	// Draw active group
	for (int i = first; i <= last; i++) {
		if(tabs[i].visible && i != active)
			PaintTab(w, st, sz, i, IsEnabled());
	}
	// Clear tab_size for non-visible tabs to prevent mouse handling bugs
	for (int i = 0; i < first; i++)
		tabs[i].tab_size = Size(0, 0);	
	for (int i = last+1; i < tabs.GetCount(); i++)
		tabs[i].tab_size = Size(0, 0);		
	// Draw inactive groups
	if (inactivedisabled)
		for (int i = first; i <= last; i++) {
			if(!tabs[i].visible && i != active && (!stacking || IsStackHead(i)))
				PaintTab(w, st, sz, i, !IsEnabled());
		}
			
	// Draw selected tab
	if(active >= first && active <= last)
		PaintTab(w, st, sz, active, true);
	
	// Separators
	if (groupseps) {
		int cy = IsVert() ? sz.cx : sz.cy;
		for (int i = 0; i < separators.GetCount(); i++) {
			int x = separators[i];
			if (x > sc.GetPos() && x < limt)
				PaintSeparator(w, Rect(
					Fixed(Point(x - sc.GetPos() + GetStyle().sel.left, 0)), 
					Fixed(Size(TB_SPACE - GetStyle().sel.left, cy-1))));
		}
	}
	
	// Draw drag highlights
	if(target >= 0)
	{
		// Draw target marker
		int drag = isctrl ? highlight : active;
		if(target != drag && target != drag + 1)
		{
			last = GetLast();
			first = GetFirst();
			int x = (target == last + 1 ? tabs[last].Right() : tabs[target].pos.x)
			        - sc.GetPos() - (target <= first ? 1 : 2)
			        + st.margin - (target > 0 ? st.extendleft : 0);

			if (IsHorz())
				DrawVertDrop(w, x + 1, 0, sz.cy);
			else
				DrawHorzDrop(w, 0, x + 1, sz.cx);
		}
		// Draw transparent drag image
		Point mouse = GetMousePos() - GetScreenRect().TopLeft();
		Size isz = dragtab.GetSize();
		int p = 0;
		int sep = TB_SBSEPARATOR * sc.IsVisible();
		
		int top = drag == active ? st.sel.bottom : st.sel.top;
		if (align == BOTTOM || align == RIGHT)
			p = int(drag == active) * -top + sep;
		else
			p = int(drag != active) * top;
		
		if (IsHorz())
			w.DrawImage(mouse.x - isz.cx / 2, p, isz.cx, isz.cy, dragtab);
		else
			w.DrawImage(p, mouse.y - isz.cy / 2, isz.cx, isz.cy, dragtab);		
	}
}

Image TabBar::GetDragSample()
{
	return GetDragSample(highlight);
}

Image TabBar::GetDragSample(int n)
{
	if (n < 0) return Image();
	Tab &t = tabs[n];

	Size tsz(t.tab_size);
	ImageDraw iw(tsz);
	iw.DrawRect(tsz, SColorFace()); //this need to be fixed - if inactive tab is dragged gray edges are visible
	
	PaintTab(iw, *style[GetAlign()], tsz, n, true, true);
	
	Image img = iw;
	ImageBuffer ib(img);
	Unmultiply(ib);
	RGBA *s = ~ib;
	RGBA *e = s + ib.GetLength();
	while(s < e) {
		s->a = 180;
		s++;
	}
	Premultiply(ib);
	return ib;
}

void TabBar::Scroll()
{
	Refresh();
}

int TabBar::GetWidth(int n)
{
	return GetStdSize(tabs[n]).cx + GetExtraWidth();
}

int TabBar::GetExtraWidth()
{
	return TB_MARGIN * 2 + (TB_SPACE + TabBarImg::CR0().GetSize().cx) * crosses;	
}

Size TabBar::GetStdSize(const Value &q)
{
	if (display)
		return display->GetStdSize(q);
	else if (q.GetType() == STRING_V || q.GetType() == WSTRING_V)
		return GetTextSize(WString(q), GetStyle().font);
	else
		return GetTextSize("A Tab", GetStyle().font);	
}

Size TabBar::GetStackedSize(const Tab &t)
{
	if (!IsNull(t.img))
		return t.img.GetSize();
	return GetTextSize("...", GetStyle().font, 3);
}

Size TabBar::GetStdSize(const Tab &t)
{
	return (PaintIcons() && t.HasIcon()) ? (GetStdSize(t.value) + Size(TB_ICON+2, 0)) : GetStdSize(t.value);
}

TabBar& TabBar::Add(const Value &value, Image icon, String group, bool make_active)
{
	return InsertKey(tabs.GetCount(), value, value, icon, group, make_active);
}

TabBar& TabBar::Insert(int ix, const Value &value, Image icon, String group, bool make_active)
{
	return InsertKey(tabs.GetCount(), value, value, icon, group, make_active);
}

TabBar& TabBar::AddKey(const Value &key, const Value &value, Image icon, String group, bool make_active)
{
	return InsertKey(tabs.GetCount(), key, value, icon, group, make_active);
}

TabBar& TabBar::InsertKey(int ix, const Value &key, const Value &value, Image icon, String group, bool make_active)
{
	int id = InsertKey0(ix, key, value, icon, group);
	
	MakeGroups();	
	Repos();
	active = -1;
	if (make_active || (!allownullcursor && active < 0)) 
		SetCursor((groupsort || stacking) ? FindId(id) : ( minmax(ix, 0, tabs.GetCount() - 1)));		
	return *this;	
}

int TabBar::InsertKey0(int ix, const Value &key, const Value &value, Image icon, String group)
{
	ASSERT(ix >= 0);
	int g = 0;
	if (!group.IsEmpty()) {
		g = FindGroup(group);
		if (g < 0) {
			NewGroup(group);
			g = groups.GetCount() - 1;
		}
	}	
	
	group = groups[g].name;
	Tab t;
	t.value = value;
	t.key = key;
	t.img = icon;
	t.id = GetNextId();
	t.group = group;	
	if (stacking) {
		t.stackid = GetStackId(t);
		t.sort_order = GetStackSortOrder(t);
		
		// Override index
		int tail = -1;
		for (int i = 0; i < tabs.GetCount(); i++)
			if (tabs[i].stackid == t.stackid && (!grouping || tabs[i].group == t.group))
				tail = FindStackTail(tabs[i].stack);
		if (tail >= 0) {
			ix = tail+1;
			t.stack = tabs[tail].stack;
			tail++;
		}
		else {
			ix = (ix < tabs.GetCount()) ? FindStackHead(tabs[ix].stack) : ix;
			t.stack = stackcount++;
		}
		tabs.Insert(ix, t);
		if (tail >= 0) {
			int head = FindStackHead(t.stack);
			int headid = tabs[head].id;			
			Sort(tabs.GetIter(head), tabs.GetIter(tail+1), TabStackSort());
			while (tabs[head].id != headid)
				CycleTabStack(head, t.stack);
		}
	}
	else
		tabs.Insert(ix, t);
	return t.id;
}

int TabBar::GetWidth() const
{
	if (!tabs.GetCount()) return 0;
	int ix = GetLast();
	if (IsStackHead(ix)) 
		return tabs[ix].Right() + style[GetAlign()]->margin * 2;
	int stack = tabs[ix].stack;
	ix--;
	while (ix >= 0 && tabs[ix].stack == stack)
		ix--;
	return tabs[ix+1].Right() + style[GetAlign()]->margin * 2;
	
}

int TabBar::GetHeight(bool scrollbar) const
{
	return TabBar::GetStyleHeight(*style[GetAlign()]) + TB_SBSEPARATOR * int(scrollbar);
}

int TabBar::GetStyleHeight(const Style &s)
{
	return s.tabheight + s.sel.top;
}

void TabBar::Repos()
{
	if(!tabs.GetCount())
		return;

	String g = GetGroupName();

	int j;
	bool first = true;
	j = 0;
	separators.Clear();
	for(int i = 0; i < tabs.GetCount(); i++)
		j = TabPos(g, first, i, j, false);
	if (inactivedisabled)
		for(int i = 0; i < tabs.GetCount(); i++)
			if (!tabs[i].visible)
				j = TabPos(g, first, i, j, true);
	SyncScrollBar();
}

int TabBar::TabPos(const String &g, bool &first, int i, int j, bool inactive)
{
	bool ishead = IsStackHead(i);
	bool v = IsNull(g) ? true : g == tabs[i].group;
	Tab& t = tabs[i];

	if(ishead && (v || inactive))
	{
		t.visible = v;
		t.pos.y = 0;
		t.size.cy = GetStyleHeight(*style[1]);
				
		// Normal visible or inactive but greyed out tabs
		t.pos.x = first ? 0 : tabs[j].Right();
		// Separators
		if (groupseps && !first && t.group != tabs[j].group) {
			separators.Add(t.pos.x);
			t.pos.x += TB_SPACE;
		}
		
		
		int cx = GetStdSize(t).cx;
		t.stdcx = cx;
		// Stacked/shortened tabs
		if (stacking) {
			int n = i+1;
			while (n < tabs.GetCount() && tabs[n].stack == t.stack)	{
				Tab &q = tabs[n];
				q.visible = false;
				q.pos.x = t.pos.x + cx;
				q.pos.y = 0;
				q.size.cx = GetStackedSize(q).cx;
				q.size.cy = t.size.cy;
				cx += q.size.cx;	
				n++;			
			}
			
		}		
		t.size.cx = cx + GetExtraWidth();
		
		j = i;
		first = false;
	}
	else if (!(v || inactive)) {
		t.visible = false;
		t.pos.x = sc.GetTotal() + GetSize().cx;
	}
	return j;	
}

void TabBar::SyncScrollBar(bool synctotal)
{
	if (synctotal)
		sc.SetTotal(GetWidth());			
	if (autoscrollhide) {
		bool v = sc.IsScrollable();
		if (sc.IsShown() != v) {
			SetFrameSize((v ? sc.GetFrameSize() : 0) + GetHeight(v), false);
			sc.Show(v);	
			PostCallback(THISBACK(RefreshParentLayout));
		}
	}
	else {
		SetFrameSize(sc.GetFrameSize() + GetHeight(true), false);
		sc.Show();
	}
}

int TabBar::FindId(int id) const
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].id == id)
			return i;
	return -1;
}

int TabBar::GetNext(int n) const
{
	for(int i = n + 1; i < tabs.GetCount(); i++)
		if(tabs[i].visible)
			return i;
	return -1;
}

int TabBar::GetPrev(int n) const
{
	for(int i = n - 1; i >= 0; i--)
		if(tabs[i].visible)
			return i;
	return -1;
}

void TabBar::Clear()
{
	highlight = -1;
	active = -1;
	target = -1;
	cross = -1;
	id = -1;	
	stackcount = 0;
	tabs.Clear();
	groups.Clear();
	NewGroup(t_("TabBarGroupAll\aAll"));
	group = 0;
	Refresh();
}

TabBar& TabBar::Crosses(bool b)
{
	crosses = b;
	Repos();
	Refresh();
	return *this;
}

TabBar& TabBar::Grouping(bool b)
{
	grouping = b;
	return *this;
}

TabBar& TabBar::GroupSort(bool b)
{
	Value v;
	if (b && HasCursor())
		v = GetData();
	groupsort = b;
	MakeGroups();
	Repos();
	if (b && HasCursor())
		SetData(v);
	Refresh();
	return *this;
}

TabBar& TabBar::GroupSeparators(bool b)
{
	groupseps = b;
	Repos();
	Refresh();
	return *this;
}

TabBar& TabBar::AutoScrollHide(bool b)
{
	autoscrollhide = b;
	sc.Hide();
	SetFrameSize(GetHeight(false), false);
	SyncScrollBar(GetWidth());
	return *this;
}

TabBar& TabBar::InactiveDisabled(bool b)
{
	inactivedisabled = b; 
	Repos(); 
	Refresh();	
	return *this;
}

TabBar& TabBar::AllowNullCursor(bool b)
{
	allownullcursor = b;
	return *this;
}

TabBar& TabBar::Icons(bool v)
{
	icons = v;
	Repos();
	Refresh();
	return *this;
}

TabBar& TabBar::Stacking(bool b)
{
	stacking = b;	
	if (b)
		DoStacking();
	else
		DoUnstacking();
	Refresh();
	return *this;
}

void TabBar::FrameSet()
{
	int al = GetAlign();
	Ctrl::ClearFrames();
	sc.Clear();
	sc.SetFrameSize(TB_SBHEIGHT).SetAlign((al >= 2) ? al - 2 : al + 2);
	sc <<= THISBACK(Scroll);
	sc.Hide();
	if (!sc.IsChild())
		AddFrame(sc);

	style[0] = &StyleLeft(); 
	style[1] = &StyleDefault(); 
	style[2] = &StyleRight(); 
	style[3] = &StyleBottom(); 

	SyncScrollBar(GetWidth());
}

void TabBar::ResetStyles()
{
	StyleLeft();
	StyleRight();
	StyleBottom();
}

void TabBar::FrameLayout(Rect& r)
{
	AlignedFrame::FrameLayout(r);	
}

void TabBar::Layout()
{
	if (autoscrollhide && tabs.GetCount()) 
		SyncScrollBar(false); 
}

int TabBar::FindValue(const Value &v) const
{
	for (int i = 0; i < tabs.GetCount(); i++)
		if (tabs[i].value == v)
			return i;
	return -1;
}

int TabBar::FindKey(const Value &v) const
{
	for (int i = 0; i < tabs.GetCount(); i++)
		if (tabs[i].key == v)
			return i;
	return -1;
}

bool TabBar::IsStackHead(int n) const
{
	return tabs[n].stack < 0 || n == 0 || (n > 0 && tabs[n-1].stack != tabs[n].stack);
}

bool TabBar::IsStackTail(int n) const
{
	return tabs[n].stack < 0 || n >= tabs.GetCount()-1 || (n < tabs.GetCount() && tabs[n+1].stack != tabs[n].stack);
}

int TabBar::FindStackHead(int stackix) const
{
	int i = 0;
	while (tabs[i].stack != stackix) 
		i++;
	return i;
}

int TabBar::FindStackTail(int stackix) const
{
	int i = tabs.GetCount()-1;
	while (tabs[i].stack != stackix) 
		i--;
	return i;
}

int TabBar::SetStackHead(Tab &t)
// Returns index of stack head
{
	ASSERT(stacking);
	int id = t.id;
	int stack = t.stack;
	int head = FindStackHead(stack);
	while (tabs[head].id != id)
		CycleTabStack(head, stack);
	return head;
}

int TabBar::CycleTabStack(int n)
// Returns index of stack head
{
	int head = FindStackHead(n);
	CycleTabStack(head, n);
	return head;
}

void TabBar::CycleTabStack(int head, int n)
{
	// Swap tab to end of stack
	int ix = head;
	while (!IsStackTail(ix)) {
		tabs.Swap(ix, ix+1);
		++ix;
	}
}

void TabBar::SetData(const Value &key)
{
	int n = FindKey(key); 
	if (n >= 0) {
		if (stacking && tabs[n].stack >= 0)
			n = SetStackHead(tabs[n]);
		SetCursor(n);
	}
}

void TabBar::Set(int n, const Value &newkey, const Value &newvalue, Image icon)
{
	ASSERT(n >= 0 && n < tabs.GetCount());
	tabs[n].key = newkey;
	tabs[n].value = newvalue;
	if (IsNull(icon))
		tabs[n].img = icon;
	if (stacking) {
		Tab t = tabs[n];
		tabs[n].stackid = GetStackId(tabs[n]);
		tabs[n].sort_order = GetStackSortOrder(tabs[n]);
		if (t.stackid != tabs[n].stackid || t.sort_order != tabs[n].sort_order) {
			tabs.Remove(n);
			InsertKey0(GetCount(), t.key, t.value, t.img, t.group);
		}
	}
	Repos();
	Refresh();
}

void TabBar::Set(const Value &key, const Value &newvalue, Image icon)
{
	Set(FindKey(key), key, newvalue);
}

void TabBar::Set(int n, const Value &newvalue, Image icon)
{
	Set(n, tabs[n].key, newvalue);
}

void TabBar::SetIcon(int n, Image icon)
{
	ASSERT(n >= 0 && n < tabs.GetCount());
	tabs[n].img = icon;
	Repos();
	Refresh();
}

void TabBar::LeftDown(Point p, dword keyflags)
{
	if(keyflags & K_SHIFT)
	{
		highlight = -1;
		Refresh();
		SetCapture();
		Fix(p);
		oldp = p;
		return;
	}

	isctrl = keyflags & K_CTRL;
	if(isctrl)
		return;

	if(cross != -1) {
		Value v = tabs[cross].key;
		Vector<Value>vv;
		vv.Add(v);
		int ix = cross;
		if (!CancelClose(v) && !CancelCloseSome(Vector<Value>(vv, 0))) {
			Close(ix);
			WhenClose(v);
			WhenCloseSome(vv);
		}
	}
	else if(highlight >= 0) {
		if (stacking && highlight == active) {
			CycleTabStack(tabs[active].stack);
			Repos();
			UpdateActionRefresh();
		}
		else
			SetCursor(highlight);
	}
}

void TabBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
}

void TabBar::LeftDouble(Point p, dword keysflags)
{
	WhenLeftDouble();
}

void TabBar::RightDown(Point p, dword keyflags)
{
	MenuBar::Execute(THISBACK(ContextMenu), GetMousePos());
}

void TabBar::MiddleDown(Point p, dword keyflags)
{
	if (highlight >= 0)
	{
		Value v = tabs[highlight].key;
		Vector<Value>vv;
		vv.Add(v);
		if (!CancelClose(v) && ! CancelCloseSome(Vector<Value>(vv, 0))) {
			Value v = tabs[highlight].key;
			Close(highlight);
			WhenClose(v);
			WhenCloseSome(vv);
		}
	}
}

void TabBar::MiddleUp(Point p, dword keyflags)
{
}

int TabBar::GetTargetTab(Point p)
{
	p.x += sc.GetPos();

	int f = GetFirst();
	int l = GetLast();

	if(tabs[f].visible && p.x < tabs[f].pos.x + tabs[f].size.cx / 2)
		return f;

	for(int i = l; i >= f; i--)
		if(tabs[i].visible && p.x >= tabs[i].pos.x + tabs[i].size.cx / 2)
			return i == l ? i + 1 : GetNext(i);
//		if(tabs[i].visible && p.x >= tabs[i].x && p.x <= tabs[i].x + tabs[i].cx)
//			return i;
	return -1;
}

void TabBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sc.AddPos(-zdelta / 4, true);
	Scroll();
	MouseMove(p, 0);
}

bool TabBar::ProcessMouse(int i, const Point& p)
{
	if(i >= 0 && i < tabs.GetCount() && tabs[i].HasMouse(p))
	{
		if (stacking && ProcessStackMouse(i, p))
			return true;
		bool iscross = crosses ? tabs[i].HasMouseCross(p) : false;
		if(highlight != i || (iscross && cross != i || !iscross && cross == i))
		{
			cross = iscross ? i : -1;
			SetHighlight(i);
		}
		return true;
	}
	return false;
}

bool TabBar::ProcessStackMouse(int i, const Point& p)
{
	int j = i+1;
	while (j < tabs.GetCount() && tabs[j].stack == tabs[i].stack) {
		if (Rect(tabs[j].tab_pos, tabs[j].tab_size).Contains(p)) {
			cross = -1;
			if (highlight != j)
				SetHighlight(j);
			return true;
		}
		j++;
	}
	return false;	
}

void TabBar::SetHighlight(int n)
{
	highlight = n;
	WhenHighlight();
	Refresh();	
}

void TabBar::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
	{
		Fix(p);
		sc.AddPos(p.x - oldp.x, true);
		oldp = p;
		Refresh();
		return;
	}
	
	if(ProcessMouse(active, p))
		return;
		
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		if(i == active)
			continue;
		
		if(ProcessMouse(i, p))
			return;
	}

	if(highlight >= 0 || cross >= 0)
	{
		highlight = cross = -1;
		WhenHighlight();
		Refresh();
	}
}

void TabBar::MouseLeave()
{
	if(isdrag)
		return;
	highlight = cross = -1;
	WhenHighlight();
	Refresh();
}

void TabBar::DragAndDrop(Point p, PasteClip& d)
{
	Fix(p);
	int c = GetTargetTab(p);
	int tab = isctrl ? highlight : active;

	if (&GetInternal<TabBar>(d) != this) return;

	if (stacking) {
		tab = FindStackHead(tabs[tab].stack);
		c = FindStackHead(tabs[c].stack);
	}

	bool sametab = c == tab || c == tab + 1;
	bool internal = AcceptInternal<TabBar>(d, "tabs");

	if(!sametab && internal && d.IsAccepted())
	{
		int id = active >= 0 ? tabs[active].id : -1;
		
		// Count stack
		int count = 1;
		if (stacking) {
			int ix = tab+1;
			int stack = tabs[tab].stack;
			while (ix < tabs.GetCount() && tabs[ix].stack == stack)
				ix++;
			count = ix - tab;
		}
		// Copy tabs
		Vector<Tab> stacktemp;
		stacktemp.SetCount(count);
		Copy(&stacktemp[0], &tabs[tab], count);
		// Remove
		tabs.Remove(tab, count);
		if (tab < c)
			c -= count;
		// Re-insert
		tabs.InsertPick(c, stacktemp);
		
		active = id >= 0 ? FindId(id) : -1;
		isdrag = false;
		target = -1;
		MakeGroups();
		Repos();
		Refresh();
		Sync();
		MouseMove(p, 0);
	}
	else if(isdrag)
	{
		if(internal)
		{
			target = -1;
			isdrag = false;
		}
		else
			target = c;
		Refresh();
	}
}

void TabBar::CancelMode()
{
	isdrag = false;
	target = -1;
	Refresh();
}

void TabBar::LeftDrag(Point p, dword keyflags)
{
	if(keyflags & K_SHIFT)
		return;
	if(highlight < 0)
		return;

	isdrag = true;
	dragtab = GetDragSample();
	DoDragAndDrop(InternalClip(*this, "tabs"));
}

void TabBar::DragEnter()
{
}

void TabBar::DragLeave()
{
	target = -1;
	Refresh();
}

void TabBar::DragRepeat(Point p)
{
	if(target >= 0)
	{
		Point dx = GetDragScroll(this, p, 16);
		Fix(dx);
		if(dx.x != 0)
			sc.AddPos(dx.x);
	}
}

void TabBar::SetCursor(int n)
{
	if(tabs.GetCount() == 0)
		return;

	if(n < 0)
	{
		n = max(0, FindId(GetGroupActive()));
		active = -1;
		if (allownullcursor)
			return;
	}

	bool is_all = IsGroupAll();
	bool same_group = tabs[n].group == GetGroupName();

	if((same_group || is_all) && active == n)
		return;


	bool repos = false;
	if (!IsStackHead(n)) 
	{
		n = SetStackHead(tabs[n]);
		repos = true;		
	}
	active = n;
	if(!is_all && !same_group) 
	{
		SetGroup(tabs[n].group);
		repos = true;
	}
	if (repos)
		Repos();

	SetGroupActive(tabs[n].id);

	int cx = tabs[n].pos.x - sc.GetPos();
	if(cx < 0)
		sc.AddPos(cx - 10);
	else
	{
		Size sz = Ctrl::GetSize();
		Fix(sz);
		cx = tabs[n].pos.x + tabs[n].size.cx - sz.cx - sc.GetPos();
		if(cx > 0)
			sc.AddPos(cx + 10);
	}

	Refresh();

	if(Ctrl::HasMouse())
	{
		Sync();
		MouseMove(GetMouseViewPos(), 0);
	}
	
	UpdateAction();
}

void TabBar::SetTabGroup(int n, const String &group)
{
	ASSERT(n >= 0 && n < tabs.GetCount());
	int g = FindGroup(group);
	if (g <= 0) 
		NewGroup(group);
	else if (groups[g].active == tabs[n].id)
		SetGroupActive(tabs[n].id);
	tabs[n].group = group;
	MakeGroups();
	Repos();
}

void TabBar::Close(int n)
{
	if(tabs.GetCount() <= mintabcount)
		return;

	if(n == active)
	{
		int c = FindId(tabs[n].id);
		int nc = GetNext(c);
		if(nc < 0)
			nc = max(0, GetPrev(c));
		SetGroupActive(tabs[nc].id);
	}
	sc.AddTotal(-tabs[n].size.cx);
	tabs.Remove(n);
	MakeGroups();
	Repos();
	
	highlight = min(highlight, tabs.GetCount()-1);
	if(n == active)
		SetCursor(-1);
	else {
		if (n < active)
			active--;
		Refresh();
		if (n == highlight && Ctrl::HasMouse()) {
			Sync();
			MouseMove(GetMouseViewPos(), 0);
		}	
	}
}

void TabBar::CloseData(const Value &key)
{
	int tabix = FindKey(key);
	if (tabix < 0) return;
	Close(tabix);
}

const TabBar::Style& TabBar::AlignStyle(int align, Style &s)
{
	Size sz(30, GetStyleHeight(s));
	for (int i = 0; i < 4; i++) s.first[i] =  	AlignValue(align, s.first[i], sz);
	for (int i = 0; i < 4; i++) s.last[i] =  	AlignValue(align, s.last[i], sz);
	for (int i = 0; i < 4; i++) s.normal[i] =  	AlignValue(align, s.normal[i], sz);
	for (int i = 0; i < 4; i++) s.both[i] =  	AlignValue(align, s.both[i], sz);
	return s;
}

CH_STYLE(TabBar, Style, StyleDefault)
{
	Assign(TabCtrl::StyleDefault());
	TabBar::ResetStyles();
}

const TabBar::Style& TabBar::StyleLeft()
{
	leftstyle = StyleDefault();
	return AlignStyle(AlignedFrame::LEFT, leftstyle);
}

const TabBar::Style& TabBar::StyleRight()
{
	rightstyle = StyleDefault();
	return AlignStyle(AlignedFrame::RIGHT, rightstyle);
}

const TabBar::Style& TabBar::StyleBottom()
{
	bottomstyle = StyleDefault();
	return AlignStyle(AlignedFrame::BOTTOM, bottomstyle);	
}

Vector<Value> TabBar::GetKeys() const
{
	Vector<Value> keys;
	keys.SetCount(tabs.GetCount());
	for (int i = 0; i < tabs.GetCount(); i++)
		keys[i] = tabs[i].key;
	return keys;
}

Vector<Image> TabBar::GetIcons() const
{
	Vector<Image> img;
	img.SetCount(tabs.GetCount());
	for (int i = 0; i < tabs.GetCount(); i++)
		img[i] = tabs[i].img;
	return img;
}

TabBar& TabBar::CopySettings(const TabBar &src)
{
	crosses = src.crosses;
	grouping = src.grouping;
	autoscrollhide = src.autoscrollhide;		
	nosel = src.nosel;
	nohl = src.nohl;
	inactivedisabled = src.inactivedisabled;
	stacking = src.stacking;
	groupsort = src.groupsort;
	groupseps = src.groupseps;
	allownullcursor = src.allownullcursor;
	icons = src.icons;
	mintabcount = src.mintabcount;
	
	if (stacking != src.stacking)
		Stacking(src.stacking);
	else {
		MakeGroups();
		Repos();
		Refresh();
	}
	return *this;
}

END_UPP_NAMESPACE
