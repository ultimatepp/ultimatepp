#include "TabBar.h"

#define IMAGECLASS TabBarImg
#define IMAGEFILE <TabBar/TabBar.iml>
#include <Draw/iml_source.h>

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
	stacking = true;
	groupsort = true;
	neverempty = 1;
	
	style[0] = style[1] = style[2] = style[3] = NULL;
	SetAlign(TOP);
	SetFrameSize(GetHeight());
	BackPaint();
}

void TabBar::CloseAll()
{
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
		if(i != active)
			tabs.Remove(i);

	SyncScrollBar();
	MakeGroups();
	Repos();
	SetCursor(0);
	WhenCloseAll();
}

int TabBar::GetNextId()
{
	return ++id;
}

void TabBar::ContextMenu(Bar& bar)
{
	if (highlight >= 0) {
		bar.Add(tabs.GetCount() > 1, "Close", THISBACK1(Close, highlight));
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

	bar.Add("Close others", THISBACK(CloseAll));
}

void TabBar::GroupMenu(Bar &bar, int n)
{
	bar.Add("Set active", THISBACK1(DoGrouping, n));
	bar.Add("Close", THISBACK1(DoCloseGroup, n));
}

bool TabBar::Tab::HasMouse(const Point& p) const
{
	return visible && p.x >= real_pos.x && p.x < real_pos.x + real_size.cx &&
	                  p.y >= real_pos.y && p.y < real_pos.y + real_size.cy;
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
	for (int i = 0; i < tabs.GetCount(); i++) {
		Tab &base = tabs[i];
		for (int j = i+1; j < tabs.GetCount(); j++) {
			Tab &t = tabs[j];
			if ((!grouping || base.group == t.group) && stackfunc(base.data, t.data)) {
				base.stack.AddTail(t.data);
				tabs.Remove(j);
				j--;
			}
		}
	}
	highlight = -1;
	MakeGroups();	
	Repos();
	int c = active;
	SetData(v);
	if (active < 0 || c != active)
		Refresh();
}

void TabBar::DoUnstacking() 
{
	for (int i = 0; i < tabs.GetCount(); i++) {
		for (int j = tabs[i].stack.GetCount()-1; j >= 0; j--) {
			Tab &t = tabs.Insert(i+1);
			t.id = GetNextId();	
			t.data = tabs[i].stack[j];
			t.group = tabs[i].group;
		}
		tabs[i].stack.Clear();
	}
	highlight = -1;
	MakeGroups();	
	Repos();
	if (HasCursor())	
		SetCursor(-1);
	else
		Refresh();
}

void TabBar::StackRemove(BiVector<Value> &stack, int ix)
{
	int cnt = stack.GetCount() >> 1;
	if (ix <= cnt) {
		int i = ix-1;
		while (i >= 0) {
			Swap(stack[i], stack[ix]);
			ix = i;
			i--;
		}
		stack.DropHead();
	}
	else {
		int i = ix+1;
		while (i < stack.GetCount()) {
			Swap(stack[i], stack[ix]);
			ix = i;
			i++;
		}
		stack.DropTail();	
	}
}

void TabBar::MakeGroups()
{
	groups[0].count = tabs.GetCount();
	groups[0].first = 0;
	groups[0].last = tabs.GetCount() - 1;

	if (groupsort)
		Sort(tabs, TabGroupSort());

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
	if (cnt == n)
		--group;

	String group = groups[n].name;

	for(int i = tabs.GetCount() - 1; i >= 0; i--)
	{
		if(group == tabs[i].group && tabs.GetCount() > 1) {
			Value v = tabs[i].data;
			tabs.Remove(i);
			WhenClose(v);
		}
	}
	if(cnt > 1)
		groups.Remove(n);
	MakeGroups();
	Repos();
	SetCursor(-1);
}

void TabBar::NewGroup(const String &name, const Value &data)
{
	Group &g = groups.Add();
	g.name = name;
	g.data = data;
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

WString TabBar::ParseLabel(const WString& s)
{
	return s;
}

void TabBar::PaintTabData(Draw& w, const Rect& r, const Tab& tab, const Font &font, Color ink, dword style)
{
	WString txt;
	Font f = font;
	Color i = ink;
	const Value &q = tab.data;

	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		if(!IsNull(t.font))
			f = t.font;
		i = IsNull(t.ink) ? ink : t.ink;
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);
	
	Point p = GetTextPosition(r, GetTextSize(txt, font).cy, TB_MARGIN);
	w.DrawText(p.x, p.y, GetTextAngle(), ParseLabel(txt), f, i);	
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
		p.x = r.left + (r.GetWidth() - cy) / 2 + 1;
		p.y = side == LEFT ? r.bottom - space - cx : r.top + space;
	}
	else if (align == RIGHT)
	{
		p.x = r.right - (r.GetWidth() + cy) / 2 - 1;
		p.y = side == LEFT ? r.top + space : r.bottom - space - cx;
	}
	else if (align == TOP)
	{
		p.x = side == LEFT ? r.left + space : r.right - cx - space;
		p.y = r.top + (r.GetHeight() - cy) / 2 + 1;
	}
	else if (align == BOTTOM)
	{
		p.x = side == LEFT ? r.left + space : r.right - cx - space;
		p.y = r.bottom - (r.GetHeight() + cy) / 2 - 1;
	}
	return p;
}

void TabBar::PaintTab(Draw &w, const Style &s, const Size &sz, int n, bool enable, bool dragsample)
{
	TabBar::Tab &t = tabs[n];
	int cnt = dragsample ? 1 : tabs.GetCount();
	int sep = TB_SBSEPARATOR * sc.IsVisible();
	
	Size tsz;
	Point p;
	
	int align = GetAlign();
	bool ac = n == active;
	bool hl = n == highlight;

	int ndx = !enable ? CTRL_DISABLED :
		       ac ? CTRL_PRESSED :
		       hl ? CTRL_HOT : CTRL_NORMAL;

	int c = align == LEFT ? cnt - n : n;	
	const Value& sv = (cnt == 1 ? s.both : c == 0 ? s.first : c == cnt - 1 ? s.last : s.normal)[ndx];

	int lx = n > 0 ? s.extendleft : 0;
	int x = t.pos.x - sc.GetPos() + s.margin - lx;
	
	p = Point(x - s.sel.left, 0);		
	tsz = Size(t.size.cx + lx + s.sel.right + s.sel.left, t.size.cy + s.sel.bottom);

	if (align == BOTTOM || align == RIGHT)
		p.y -= s.sel.bottom - sep;
			
	Rect ra(Fixed(p), Fixed(tsz));

	p = Point(x, s.sel.top);
	tsz = Size(t.size.cx + lx, t.size.cy - s.sel.top);

	int dy = -s.sel.top * ac;
	
	if (align == BOTTOM || align == RIGHT)
	{
		p.y -= s.sel.top - sep;
		dy = -dy;
	}
	
	Rect rn(Fixed(p), Fixed(tsz));

	t.real_pos = (ac ? ra : rn).TopLeft();
	t.real_size = (ac ? ra : rn).GetSize();
		
	ChPaint(w, Rect(t.real_pos, t.real_size), sv);
	
	rn = Rect(Fixed(Point(p.x, p.y + dy)), Fixed(tsz));
	
	#ifdef TABBAR_DEBUG
	DrawFrame(w, rn, Green);
	#endif
	
	if(crosses && (cnt > neverempty || t.stack.GetCount())) {

		Size isz = TabBarImg::CR0().GetSize();
		Point p = GetImagePosition(rn, isz.cx, isz.cy, TB_MARGIN, RIGHT);

		t.cross_pos = p;
		t.cross_size = isz;
		w.DrawImage(p.x, p.y, (ac || hl) ? (cross == n ? TabBarImg::CR2 : ac ? TabBarImg::CR1 : TabBarImg::CR0) : TabBarImg::CR0);
	}
		
	if (display)
		display->Paint(w, rn, t.data, s.text_color[ndx], SColorDisabled(), ndx);
	else
		PaintTabData(w, rn, t, s.font, s.text_color[ndx], ndx);	
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
	// Clear real_size for non-visible tabs to prevent mouse handling bugs
	for (int i = 0; i < first; i++)
		tabs[i].real_size = Size(0, 0);	
	for (int i = last+1; i < tabs.GetCount(); i++)
		tabs[i].real_size = Size(0, 0);		
	// Draw inactive groups
	if (inactivedisabled)
		for (int i = first; i <= last; i++) {
			if(!tabs[i].visible && i != active)
				PaintTab(w, st, sz, i, !IsEnabled());
		}
			
	// Draw selected tab
	if(active >= first && active <= last)
		PaintTab(w, st, sz, active, true);
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
			int y = st.sel.top;
			int cy = sz.cy - y;
			if (IsHorz()) {
				w.DrawRect(x + 1, y, 2, cy, SRed());
				w.DrawRect(x, y, 4, 1, SRed());
				w.DrawRect(x, y + cy - 1, 4, 1, SRed());			
			}
			else{
				w.DrawRect(y, x + 1, cy, 2, SRed());
				w.DrawRect(y, x, 1, 4, SRed());
				w.DrawRect(y + cy - 1, x, 1, 4, SRed());			
			}
		}
		// Draw transparent drag image
		Point mouse = GetMousePos() - GetScreenRect().TopLeft();
		Size isz 	= dragtab.GetSize();
		if (IsHorz())
			w.DrawImage(mouse.x - isz.cx/2, 0, isz.cx, isz.cy, dragtab);
		else
			w.DrawImage(0, mouse.y - isz.cy/2, isz.cx, isz.cy, dragtab);		
	}
}

Image TabBar::GetDragSample()
{
	return GetDragSample(highlight);
}

Image TabBar::GetDragSample(int n)
{
	if (n < 0) return Image();
	Tab &tab = tabs[n];
	const Style& st = *style[GetAlign()];

	Size tsz(tab.real_size);
	ImageDraw iw(tsz);
	iw.DrawRect(tsz, SColorFace()); //this need to be fixed - if inactive tab is dragged gray edges are visible
	
	Point temp = tab.pos;
	tab.pos.x = sc.GetPos();
	tab.pos.y = 0;
	PaintTab(iw, st, GetSize(), n, true, true);
	tab.pos.x = temp.x; 
	tab.pos.y = temp.y;
	
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
	Tab &t = tabs[n];
	Size tsz = GetStdSize(t);
	return TB_MARGIN * 2 + tsz.cx + (TB_SPACE + TabBarImg::CR0().GetSize().cx) * crosses;
}

Size TabBar::GetStdSize(const Value &q)
{
	if (display)
		return display->GetStdSize(q);
	else if (q.GetType() == STRING_V || q.GetType() == WSTRING_V)
		return GetTextSize(ParseLabel(WString(q)), StdFont());
	else
		return GetTextSize("A Tab", StdFont());	
}

Size TabBar::GetStdSize(const Tab &t)
{
	return GetStdSize(t.data);
}

TabBar& TabBar::Add(const Value &data, String group, bool make_active)
{
	return Insert(tabs.GetCount(), data, group, make_active);
}

TabBar& TabBar::Insert(int ix, const Value &data, String group, bool make_active)
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
	bool stacked = false;
	if (stacking) { 
		for (int i = 0; i < tabs.GetCount(); i++) {
			Tab &t = tabs[i];
			if (t.group == group && stackfunc(t.data, data)) {
				if (make_active) {
					t.stack.AddHead(t.data);
					t.data = data;		
				}
				else
					t.stack.AddTail(data);
				stacked = true;
			}
		}
	}
	if (!stacked) {
		Tab &t = tabs.Insert(ix);	
		t.data = data;
		t.id = GetNextId();
		t.group = group;
		
		MakeGroups();	
	}
	Repos();
	active = -1;
	if (make_active) 
		SetCursor(minmax(ix, 0, tabs.GetCount() - 1));		
	return *this;	
}
int TabBar::GetWidth() const
{
	if (!tabs.GetCount()) return 0;
	return tabs[GetLast()].Right() + style[GetAlign()]->margin * 2;
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
	bool v = IsNull(g) ? true : g == tabs[i].group;
	if(v)
	{
		tabs[i].pos.x = first ? 0 : tabs[j].Right();
		/* Separators
		if (showseps && !first && tabs[i].group != tabs[j].group) {
			seperators.Add(tabs[i].pos.x);
			tabs[i].pos.x += TB_SEPARATOR;
		}	*/
		j = i;
		first = false;
	}
	else {
		tabs[i].pos.x = 0;
		if (inactive) {
			tabs[i].pos.x = tabs[j].Right();
			return (j = i);
		}
	}

	tabs[i].visible = v;
	tabs[i].pos.y = 0;
	tabs[i].size.cx = GetWidth(i);
	tabs[i].size.cy = GetStyleHeight(*style[1]);

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
	id = -1;	
	tabs.Clear();
	groups.Clear();
	NewGroup("All");
	group = 0;
	Refresh();
}

TabBar& TabBar::Crosses(bool b)
{
	crosses = b;
	Repos();
	return *this;
}

TabBar& TabBar::Grouping(bool b)
{
	grouping = b;
	return *this;
}

TabBar& TabBar::GroupSort(bool b)
{
	groupsort = b;
	if (b)
		MakeGroups();
	return *this;
}
/* Separators
TabBar& TabBar::Seperators(bool b)
{
	showseps = b;
	Repos();
	return *this;
}
*/
TabBar& TabBar::AutoScrollHide(bool b)
{
	autoscrollhide = b;
	sc.Hide();
	SetFrameSize(GetHeight(), false);
	SyncScrollBar(GetWidth());
	return *this;
}

TabBar& TabBar::InactiveDisabled(bool b)
{
	inactivedisabled = b; 
	if (b) Repos(); 
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

int TabBar::Find(const Value &v) const
{
	for (int i = 0; i < tabs.GetCount(); i++)
		if (tabs[i].data == v)
			return i;
	return -1;
}

int TabBar::FindInStack(const Value &v, int &stackix) const
{
	stackix = -1;
	for (int i = 0; i < tabs.GetCount(); i++) {
		if (tabs[i].data == v)
			return i;
		for (int j = 0; j < tabs[i].stack.GetCount(); j++)
			if (tabs[i].stack[j] == v) {
				stackix = j;
				return i;
			}
	}
	return -1;	
}

void TabBar::CycleTabStack(int n)
{
	if (tabs[n].stack.GetCount()) {
		Tab &t = tabs[n];
		Value v = t.data;
		t.data = t.stack.Head();
		t.stack.DropHead();
		t.stack.AddTail(v);		
	}
}

void TabBar::SetData(const Value &data)
{
	int stackix = -1;
	int n = FindInStack(data, stackix); 
	if (n >= 0) {
		if (stackix >= 0) {
			Tab &t = tabs[n];
			Value v = t.stack[stackix];
			StackRemove(t.stack, stackix);
			t.stack.AddTail(t.data);
			t.data = v;
		}
		SetCursor(n);
	}
}

void TabBar::Set(int n, const Value &data, const String &group)
{
	ASSERT(n >= 0 && n < tabs.GetCount());
	tabs[n].data = data;
	if (!IsNull(data))
		SetTabGroup(n, group);
	else
		Repos();
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
		Value v = tabs[cross].data;
		Close(cross);
		WhenClose(v);
	}
	else if(highlight >= 0) {
		if (highlight == active) {
			CycleTabStack(active);
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
	Close(highlight);
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
	if(i >= 0 && tabs[i].HasMouse(p))
	{
		bool iscross = crosses ? tabs[i].HasMouseCross(p) : false;
		if(highlight != i || (iscross && cross != i || !iscross && cross == i))
		{
			cross = iscross ? i : -1;
			highlight = i;
			WhenHighlight();
			Refresh();
		}
		return true;
	}
	
	return false;
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

	bool sametab = c == tab || c == tab + 1;
	bool internal = AcceptInternal<TabBar>(d, "tabs");

	if(!sametab && d.IsAccepted())
	{
		if(internal)
		{
			int id = tabs[active].id;
			Tab t = tabs[tab];
			Tab &n = tabs.Insert(c);
			n = t;
			tabs.Remove(tab + int(c < tab));
			active = FindId(id);
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
	target = c;
	Refresh();
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

TabBar & TabBar::StackingFunc(Gate2<Value, Value> func)
{
	if (stacking) 
		DoUnstacking();
	
	stackfunc = func;
	DoStacking();
	stacking = true;	
	return *this;
}

TabBar & TabBar::NoStacking()
{
	if (stacking) {
		stacking = false;
		DoUnstacking();
	}
	return *this;
}

void TabBar::Close(int n)
{
	if (stacking && tabs[n].stack.GetCount()) {
		CycleTabStack(n);
		tabs[n].stack.DropTail();
		Repos();
		SetCursor(-1);
		return;	
	}

	if(tabs.GetCount() <= neverempty)
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
	SetCursor(-1);
}

void TabBar::CloseData(const Value &data)
{
	int stackix = -1;
	int tabix = FindInStack(data, stackix);
	if (tabix < 0) return;
	
	Tab &t = tabs[tabix];
	if (!stacking || t.stack.GetCount() == 0) return Close(tabix);
	
	if (stackix)
		StackRemove(t.stack, stackix);
	else {
		CycleTabStack(tabix);
		t.stack.DropTail();
	}
	Repos();
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

void TabBar::Serialize(Stream& s)
{
	// Note: doens't work with stacking
	int version = 0x00;
	int cnt;
	if (s.IsLoading()) Clear();
	
	s / version / group / highlight / active;		
	
	if (s.IsLoading()) {
		cnt = groups.GetCount();
		s / cnt;
		for (int i = 1; i < groups.GetCount(); i++) {
			Group &g = groups[i];
			s % g.data / g.active;
		}
		cnt = tabs.GetCount();
		s / cnt;
		for (int i = 0; i < tabs.GetCount(); i++) {
			Tab &t = tabs[i];
			int g = FindGroup(t.group);
			s % t.data / t.id / g;
		}
	}
	else {
		s / cnt;
		groups.SetCount(cnt);
		for (int i = 1; i < cnt; i++) {
			Group &g = groups[i];
			s % g.data / g.active;
		}
		s / cnt;
		tabs.SetCount(cnt);
		for (int i = 0; i < tabs.GetCount(); i++) {
			int g;
			Tab &t = tabs[i];
			s % t.data / t.id / g;
			t.group = groups[g].name;
		}
		MakeGroups();
		Repos();
	}
}
