#include "DockCtrl/DockCtrl.h"

#define IMAGECLASS DockCtrlImgs
#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#include <Draw/iml_source.h>


void DockCtrlCustomFrame::FrameLayout(Rect& r)
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
}

void DockCtrlCustomFrame::FrameAddSize(Size& sz)
{
	switch(layout)
	{
		case LAYOUT_LEFT:
			sz.cx += size;
			break;
		case LAYOUT_TOP:
			sz.cy += size;
			break;
		case LAYOUT_RIGHT:
			sz.cx += size;
			break;
		case LAYOUT_BOTTOM:
			sz.cy += size;
			break;		
	}	
}

DockCtrlCustomFrame& DockCtrlCustomFrame::SetStyle(const DockCtrlChStyle::Style& s)
{ 
	style = &s;
	return *this;
}

const DockCtrlChStyle::Style* DockCtrlCustomFrame::GetStyle()
{
 	return style ? style : &DockCtrlChStyle::StyleDefault(); 	
}


//----------------------------------------------------------------------------------------------

void TabInterface::Add(String name, bool activate)
{
	Tab &t = tabs.Add();
	if(IsNull(name) || name == t_("")) name = t_("Untitled");
	else t.name 	= name;
	t.id 			= GetNextId();
	t.active 		= activate;
	t.visible 		= true;
	t.highlighted 	= false;
	t.icon			= DockCtrlImgs::DefaultImage();
	ReposTabs();
	SetActiveTab(tabs.GetCount() - 1);
}

void TabInterface::Add(DockableCtrl& dock, bool activate)
{
	Tab &t = tabs.Add();
	if(dock.GetLabel().IsVoid() || dock.GetLabel() == t_("")) t.name = t_("Untitled");
	else t.name 	= dock.GetLabel();
	t.id 			= GetNextId();
	t.active 		= activate;
	t.visible 		= true;
	t.highlighted 	= false;
	t.icon			= dock.GetIcon();
	t.dock			= &dock;
	ReposTabs();
	SetActiveTab(tabs.GetCount() - 1);
}

void TabInterface::Remove(DockableCtrl& dock)
{
}

void TabInterface::Remove(int n)
{
}

void TabInterface::Close(int n)
{
	if(tabs.GetCount() < 1 || n >= tabs.GetCount())			// !!!
			return;
	int c 	= Find(tabs[n].id);
	int nc	= GetNext(c);
	if(nc < 0)
		nc = max(0, GetPrev(c));

	if(hasscrollbar) scrollbar.AddTotal(-tabs[n].cx);
	tabs.Remove(n);
	ReposTabs();
	SetActiveTab(nc);
}

void TabInterface::CloseAll()
{
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
	{
			tabs.At(i).dock = NULL;
			tabs.At(i).id	= -1;
			tabs.Remove(i);
	}
	if(hasscrollbar) scrollbar.SetTotal(0);//scrollbar.SetTotal(tabs[0].cx);
	ReposTabs();
	SetActiveTab(-1);	
	id = -1;
}

void TabInterface::SetActiveTab(int n)
{
	int tabcount = tabs.GetCount();

	if(tabcount == 0) 
		return;

	if(n < 0 || n >= tabcount)
		n = max(0, tabcount - 1);
	
	Tab& t = tabs[active = n];
	int cx = t.x - (int(hasscrollbar) * scrollbar.GetPos());
	if(hasscrollbar)
	{
		if(cx < 0)
			scrollbar.AddPos(cx - 10);
		else
		{
			cx = t.x + t.cx - GetSize().cx - scrollbar.GetPos();
			if(cx > 0)
				scrollbar.AddPos(cx + 10);
		}
	}
	if(HasMouse())MouseMove(GetMouseViewPos(), 0);
	UpdateActionRefresh();
	WhenSelect();
}

void TabInterface::SetActiveTab(DockableCtrl& dock)
{
	SetActiveTab(Find(dock));	
}

void TabInterface::SetActiveTabTitle(String name, Image icon)
{
	if(active < 0) return;
	Tab& t = tabs.At(active);
	t.name = name;
	t.icon = icon;
	ReposTabs();
}

TabInterface& TabInterface::HasScrollBar(bool b)
{
	if(b)
	{
		AddFrame(scrollbar); 
		scrollbar.WhenScroll = THISBACK(OnScrollBar);
	}
	else 
		RemoveFrame(scrollbar);
	Refresh();	 
	hasscrollbar = b;
	return *this;
}

int TabInterface::GetActiveTab()
{
	return active;
}

int TabInterface::Find(int itemid)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].id == itemid) return i;
	return -1;
}

int TabInterface::Find(DockableCtrl& dock)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].dock == &dock) return i;
	return -1;
}

int TabInterface::GetNext(int n)
{
	for(int i = n + 1; i < tabs.GetCount(); i++)
		if(tabs[i].visible) return i;
	return -1;
}

int TabInterface::GetPrev(int n)
{
	for(int i = n - 1; i >= 0; i--)
		if(tabs[i].visible) return i;
	return -1;
}


int TabInterface::GetWidth(int n)
{
	Tab &t = tabs[n];
	t.textsize  	= GetTextSize(t.name, style->font);
	return TAB_MARGIN * 2 + t.textsize.cx + ((hasfileicon ? TAB_SPACE : TAB_SPACE / 2) + DockCtrlImgs::DClose().GetSize().cx) * hastabbutton + (TAB_FILEICON + TAB_SPACEICON) * hasfileicon;
}

int TabInterface::GetWidth()
{
	return tabs[tabs.GetCount() - 1].Right() + style->margin * 2;
}

int TabInterface::GetHeight()
{
	return style->tabheight + style->sel.bottom + (int(hasscrollbar) * (TAB_SBHEIGHT + TAB_SBSEPARATOR));
}

void TabInterface::ReposTabs()
{
	if(!tabs.GetCount())
		return;

	int j = 0;
	bool first = true;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		tabs[i].x = first ? 0 : tabs[j].Right();
		j = i;
		first = false;
		tabs[i].visible = true;
		tabs[i].y = 0;
		tabs[i].cx = GetWidth(i);
		tabs[i].cy = style->tabheight + style->sel.top;
	}
	if(hasscrollbar) scrollbar.SetTotal(GetWidth());
}

void TabInterface::DrawLabel(Draw& w, int x, int y, Tab& t, bool isactive)
{
	String fn = GetFileName(t.name);

	if(horizontal)
		DrawFileName(w, x + TAB_MARGIN + (TAB_FILEICON + TAB_SPACEICON) * int(hasfileicon), (y - t.textsize.cy) / 2,
	             t.textsize.cx, t.textsize.cy, fn.ToWString(),
	            false, StdFont(), Black, LtBlue, 
	            Null, Null, false);
	else
		w.DrawText((y + t.textsize.cy) / 2 + 1, 
				x + TAB_MARGIN + (TAB_FILEICON + TAB_SPACEICON) * int(hasfileicon),
				2700, fn.ToWString(), 
				style->font);
}

void TabInterface::DrawTab(Draw& w, Size &sz, int i)
{
	Size isz 	= DockCtrlImgs::DClose().GetSize();
	int cnt 	= tabs.GetCount();
	Tab &t 		= tabs[i];

	bool ac 	= i == active;
	bool hl 	= i == highlight;
		
	int lx   = i > 0 ? style->extendleft : 0;
	int x    = t.x - (hasscrollbar ? scrollbar.GetPos() : 0) + style->margin - lx;

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

	if(hastabbutton && tabs.GetCount())
		img.DrawImage(t.cx - isz.cx - TAB_MARGIN, 
				(cy + h - isz.cy) / 2,
				(ac || hl) ? (tabbutton == i ? DockCtrlImgs::DCloseh : 
				ac ? DockCtrlImgs::DCloses : DockCtrlImgs::DClose) : 
				DockCtrlImgs::DClose);

	if(hasfileicon)
		img.DrawImage(TAB_MARGIN, (cy + h - t.icon.GetSize().cx) / 2, t.icon);
	
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
			shift = TAB_SBSEPARATOR;
			break;
		case LAYOUT_BOTTOM:
			nimg = MirrorVert(img);
			shift = TAB_SBSEPARATOR;
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

	DrawLabel(w, x, cy + hs, t, ac); 	
}

void TabInterface::Paint(Draw& d)
{
	Size sz = GetSize();
	d.DrawRect(sz, SColorFace());

	if(!horizontal)
		d.DrawRect(layout == LAYOUT_LEFT ? sz.cx - 1: 0, 0, 
				1, sz.cy, LtGray());
	else
		d.DrawRect(0, layout == LAYOUT_TOP ? sz.cy  - 1 : 0, 
				sz.cx, 1, LtGray());

	Fix(sz);
	
	int tabcount = tabs.GetCount();
	
	for(int i = 0; i < tabcount; i++)
		if(tabs[i].IsVisible() && active != i && i < tabcount)
			DrawTab(d, sz, i);
	
	if(active >= 0 && active < tabcount)
		DrawTab(d, sz, active);

	if(target >= 0)
	{
		int last = tabs.GetCount() - 1;
		int first = 0;

		if(target != active && target != active + 1)
		{

			int x = (target == last + 1 ? tabs[last].Right() : tabs[target].x)
			        - (hasscrollbar ? scrollbar.GetPos() : 0) - (target <= first ? 1 : 2)
			        + style->margin - (target > 0 ? style->extendleft : 0);

			int y = style->sel.top;
			
			int cy = sz.cy - y;
			
			Color c(255, 0, 0);

			if(horizontal)
			{
				d.DrawRect(x + 1, y, 2, cy, c);
				d.DrawRect(x, y, 4, 1, c);
				d.DrawRect(x, y + cy - 1, 4, 1, c);
			}
			else
			{
				Swap(x, y);
				d.DrawRect(x, y + 1, cy, 2, c);
				d.DrawRect(x, y, 1, 4, c);
				d.DrawRect(x + cy - 1, y, 1, 4, c);				
			}
		}
	}
}

void TabInterface::LeftDown(Point p, dword keyflags)
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

	if(tabbutton >= 0)
	{
		int i = Find(tabbutton);
		if(i == -1 && active >= 0)
			i = active;
		
		if(i >= 0)
		{
			DockableCtrl* dock = tabs[i].dock;
			if(dock) 
				WhenClose(tabbutton, *dock);			
		}
	}
	
	if(highlight >= 0)
	{
		SetActiveTab(highlight);
		if(active >= 0)
			WhenActive(active, *tabs[active].dock);
	}
}

void TabInterface::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
}

void TabInterface::MiddleDrag(Point p, dword keyflags)
{
	if(!isdraggable) return;
	isdragged = true;
	DoDragAndDrop(InternalClip(*this, Format(internalname, this)));
}

void TabInterface::LeftDrag(Point p, dword keyflags)
{
	if(keyflags & K_SHIFT)
		return;
	if(highlight < 0)
		return;

	if(isctrl)
	{
		isdragged = true;
		DoDragAndDrop(InternalClip(*this, Format(internalname, this)));
		return;
	}
	
	if(highlight >= 0)
	{
		Fix(p);
		SetActiveTab(highlight);
		p.x += (int(hasscrollbar) * scrollbar.GetPos());
		if(tabs[active].HasMouse(p))
			WhenDrag(active, *tabs[active].dock);
	}

}


void TabInterface::RightDown(Point p, dword keyflags)
{
	if(highlight >= 0) 
		SetActiveTab(highlight);
	WhenContext(active, *tabs[active].dock);
}

void TabInterface::RightUp(Point p, dword keyflags)
{
}


void TabInterface::MouseMove(Point p, dword keyflags)
{
	Fix(p);
	if(HasCapture() && hasscrollbar)
	{
		scrollbar.AddPos(p.x - oldp.x, true);
		oldp = p;
		return;
	}
	
	p.x += (int(hasscrollbar) * scrollbar.GetPos()) - style->margin + style->extendleft;
	Size sz = GetSize();
	Fix(sz);	
	int h = highlight;
	bool istabbutton = false;
	bool istab = false;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		if(tabs[i].HasMouse(p))
		{
			istab = true;
			//int h = sz.cy + (active == i ? 0 : style->sel.top);
			int h = style->sel.top + style->sel.bottom;
			int hs = active == i ? style->sel.top : h;
			if(layout == LAYOUT_RIGHT || layout == LAYOUT_BOTTOM)
				hs = -hs;
			hs += sz.cy;
			int ws = active == i ? style->sel.left : 0;			
			istabbutton = hastabbutton ? tabs[i].HasMouseButton(p, ws, hs) : false;
			if(highlight != i || (istabbutton && tabbutton != i))
			{
				tabbutton = istabbutton ? i : -1;
				highlight = i;
				WhenHighlight();
				Refresh();
				return;
			}
		}
	}

	if(!istab && h >= 0)
	{
		highlight = -1;
		WhenHighlight();
		Refresh();
	}

	if(!istabbutton && tabbutton >= 0)
	{
		tabbutton = -1;
		Refresh();
	}
}

void TabInterface::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(!hasscrollbar) return;
	scrollbar.AddPos(-zdelta / 4, true);
	MouseMove(p, 0);
}


void TabInterface::MouseLeave()
{
	if(isdragged)
		return;
	highlight 	= -1;
	tabbutton 	= -1;
	Refresh();
}

int TabInterface::GetTargetTab(Point p)
{
	Fix(p);
	p.x += (hasscrollbar ? scrollbar.GetPos() : 0);

	int f = 0;
	int l = tabs.GetCount() - 1;

	if(tabs[f].visible && p.x < tabs[f].x + tabs[f].cx / 2)
		return f;

	for(int i = l; i >= f; i--)
		if(tabs[i].visible && p.x >= tabs[i].x + tabs[i].cx / 2)
			return i == l ? i + 1 : GetNext(i);
	return -1;
}

int TabInterface::GetSourceTab(Point p)
{
	int cnt = tabs.GetCount();
	if(((active >= 0 && active < cnt) || (highlight >= 0 && highlight < cnt))) 
	{
		for(int i = tabs.GetCount() - 1; i >= 0; i--)
			if(tabs[i].visible && tabs[i].HasMouse(p))
				return (source = i);
	}
	return (source = highlight);	
}

void TabInterface::DragAndDrop(Point p, PasteClip& d)
{
	int c = GetTargetTab(p);
	int tab =  isctrl ? highlight : active;
	if(tab == -1) return;
		
	bool sametab = c == tab || c == tab + 1;
	bool internal = AcceptInternal<TabInterface>(d, internalname);

	if(!sametab && d.IsAccepted())
	{
		if(internal)
		{
			int id = tabs[active].id;
			Tab t = tabs[tab];
			tabs.Insert(c, t);
			tabs.Remove(tab + int(c < tab));
			active = Find(id);
			isdragged = false;
			ReposTabs();
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

void TabInterface::DragEnter()
{
}

void TabInterface::DragLeave()
{
	target = -1;
	Refresh();
}

void TabInterface::DragRepeat(Point p)
{
	{
		int dx = GetDragScroll(this, p, 16).x;
		if(dx != 0 && hasscrollbar)
			scrollbar.AddPos(dx);
	}
}

void TabInterface::CancelMode()
{
	isdragged = false;
	target = -1;
	Refresh();
}


TabInterface& TabInterface::SetLayout(int l)
{
	DockCtrlCustomFrame::SetLayout(l);
	switch(l)
	{
		case LAYOUT_LEFT:
			horizontal = scrollbar.horizontal = false;
			scrollbar.SetLayout(LAYOUT_RIGHT);
			break;
		case LAYOUT_TOP:
			horizontal = scrollbar.horizontal = true;
			scrollbar.SetLayout(LAYOUT_BOTTOM);
			break;
		case LAYOUT_RIGHT:
			horizontal = scrollbar.horizontal = false;
			scrollbar.SetLayout(LAYOUT_LEFT);
			break;
		case LAYOUT_BOTTOM:
			horizontal = scrollbar.horizontal = true;
			scrollbar.SetLayout(LAYOUT_TOP);
			break;
	}
	RefreshLayout();
	return *this;	
}

TabInterface::TabInterface()
{
	internalname = "tabs";
	
	id			= -1;
	active		= -1;
	highlight	= -1;
	visible  	= -1;
	fileicon	= -1;
	tabbutton	= -1;
	target 		= -1;	// drag target.
	source		= -1;	// drag source.
	
	hasfileicon		= false;
	hastabbutton 	= true;
	isdraggable		= true;
	isctrl 			= false;
	isdragged		= false;
	
	SetStyle(TabCtrl::StyleDefault());
	DockCtrlCustomFrame::layout = LAYOUT_BOTTOM;
	HasScrollBar(true);
	SetSize(GetHeight() + 2);	
	BackPaint();
}

TabInterface::~TabInterface()
{
	for(int i = 0; i < tabs.GetCount(); i++)
		tabs.Remove(i);
}

//----------------------------------------------------------------------------------------------

TabInterface::TabScrollBar::TabScrollBar()
{
	DockCtrlCustomFrame::layout = LAYOUT_TOP;
	DockCtrlCustomFrame::SetSize(TAB_SBHEIGHT);

	total = 0;
	pos = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	ready = false;
}

void TabInterface::TabScrollBar::Paint(Draw &w)
{
	if(!ready)
	{
		UpdatePos();
		ready = true;
	}
	Size sz = GetSize();
	
	w.DrawRect(sz, SColorFace());
	
	if(horizontal)
	{
		if(total > sz.cx)
			w.DrawRect(ffloor(pos), 1, fceil(size), 2, Blue);
		else
			w.DrawRect(0, sz.cy - 1, sz.cx, 1,  White);
	}
	else
	{
		if(total > sz.cy)
			w.DrawRect(1, ffloor(pos), 2, fceil(size), Blue);
		else
			w.DrawRect(sz.cx - 1, 0, 1, sz.cy, White);
	}
}

void TabInterface::TabScrollBar::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	Fix(p);
	old_pos = new_pos = p.x;
	if(p.x < pos || p.x > pos + size)
		start_pos = size / 2;
	else
		start_pos = UPP::tabs(p.x - pos);
	UpdatePos();
	Refresh();
}

void TabInterface::TabScrollBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	Fix(p);
	old_pos = p.x;
}

void TabInterface::TabScrollBar::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;
	Fix(p);
	new_pos = p.x;
	UpdatePos();
	Refresh();
}

void TabInterface::TabScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	AddPos(-zdelta / 4, true);
}

void TabInterface::TabScrollBar::Layout()
{
	UpdatePos(false);
}

int TabInterface::TabScrollBar::GetPos() const
{
	return ffloor(ps);
}

void TabInterface::TabScrollBar::SetPos(int p, bool dontscale)
{
	Fix(sz);
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

void TabInterface::TabScrollBar::AddPos(int p, bool dontscale)
{
	Fix(sz);
	pos += total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

int TabInterface::TabScrollBar::GetTotal() const
{
	return total;
}

void TabInterface::TabScrollBar::AddTotal(int t)
{
	total += t;
	UpdatePos();
	Refresh();
}

void TabInterface::TabScrollBar::SetTotal(int t)
{
	bool upd = total < t;
	total = t;
	UpdatePos(upd);
	Refresh();
}

void TabInterface::TabScrollBar::GoEnd()
{
	pos = total;
	UpdatePos(false);
	Refresh();
}

void TabInterface::TabScrollBar::GoBegin()
{
	pos = 0;
	UpdatePos(false);
	Refresh();
}

void TabInterface::TabScrollBar::Set(const TabScrollBar& t)
{
	total = t.total;
	pos = t.pos;
	ps = t.ps;
	Refresh();
}

void TabInterface::TabScrollBar::UpdatePos(bool update)
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

TabInterface::TabScrollBar& TabInterface::TabScrollBar::SetLayout(int l)
{
	layout = l;
	RefreshLayout();
	return *this;	
}
