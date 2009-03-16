#include "DockTabBar.h"

#include "DockCont.h"
#define ANIM_SPEED 10
#define ANIM_FRAMES 10

// DockTabBar
int AutoHideBar::autohide_timeout = 1000;

void DockTabBar::FrameLayout(Rect& r)
{
	if (IsAutoHide()) return Hide();
	Show();
	TabBar::FrameLayout(r);	
}

void DockTabBar::FrameAddSize(Size& sz)
{
	if (!IsAutoHide()) 
		TabBar::FrameAddSize(sz);
}

void DockTabBar::PaintTabData(Draw& w, Point p, const Size& sz, const Value& q, const Font& font, Color ink, dword style)
{
	DockableCtrl *d;
	WString txt;
	ink = (style == CTRL_DISABLED) ? SColorDisabled : ink;
	
	if (IsTypeRaw<DockCont *>(q)) {
		DockCont *c = ValueTo<DockCont *>(q);
		d = &c->GetCurrent();
		txt = c->GetTitle();
	}
	else {
		ASSERT(IsTypeRaw<DockableCtrl *>(q));
		d = ValueTo<DockableCtrl *>(q);
		txt = d->GetTitle();
	}

	TabCenterText(p, sz, font);
	if(icons)
	{
		const Image& icon = (style == CTRL_DISABLED) ? DisabledImage(d->GetIcon()) : d->GetIcon();
		int al = GetAlign();
		Point ip = p;
		Size isz = icon.GetSize();
		if (al == RIGHT)
			ip.x -= isz.cx-1;
		else if (al == LEFT)
			ip.y -= isz.cy;
		if (IsVert()) {
			w.DrawImage(ip.x-1, ip.y, icon);
			p.y += (isz.cy + TB_SPACEICON) * ((al == LEFT) ? -1 : 1);
		}
		else {			
			w.DrawImage(ip.x, ip.y-2, icon);
			p.x += isz.cx + TB_SPACEICON;
		}	
	}
	if (showtext)
		w.DrawText(p.x, p.y, GetTextAngle(), txt, font, ink);
}

Size DockTabBar::GetStdSize(Value& q)
{
	DockableCtrl *d;
	Value v;
	if (IsTypeRaw<DockCont *>(q)) {
		DockCont *c = ValueTo<DockCont *>(q);
		d = &c->GetCurrent();
		v = c->GetTitle();
	}
	else {
		ASSERT(IsTypeRaw<DockableCtrl *>(q));
		d = ValueTo<DockableCtrl *>(q);
		v = d->GetTitle();
	}
	int isz = (IsVert() ? d->GetIcon().GetHeight() : d->GetIcon().GetWidth());
	return showtext ? TabBar::GetStdSize(v) + Size(isz + TB_SPACEICON, 0) : Size(isz, isz);
}

void DockTabBar::RightDown(Point p, dword keyflags)
{
	if (GetHighlight() >= 0) 
		WhenContext(GetHighlight());	
	else 
		TabBar::RightDown(p, keyflags);
}

void DockTabBar::LeftDrag(Point p, dword keyflags)
{
	if (keyflags & K_CTRL || keyflags & K_SHIFT)
		return TabBar::LeftDrag(p, keyflags &= ~K_SHIFT);
	if (GetHighlight() >= 0)
		WhenDrag(GetHighlight());	
}

DockTabBar::DockTabBar()
{
	autohide = -1; 
	icons = true;
	showtext = true;
	AutoScrollHide().InactiveDisabled();
}

// AutoHide bar
void AutoHideBar::MouseEnter(Point p, dword keyflags)
{
	if (ctrl && ctrl->IsOpen())
		KillTimeCallback(TIMEID_HIDE_TIMEOUT);			
	DockTabBar::MouseEnter(p, keyflags);
}

void AutoHideBar::MouseLeave()
{
	if (ctrl && ctrl->IsOpen())
		KillSetTimeCallback(autohide_timeout, THISBACK1(HideAnimate, ctrl), TIMEID_HIDE_TIMEOUT);				
	DockTabBar::MouseLeave();
}

void AutoHideBar::AddCtrl(DockCont& c, const String& group)
{ 
	TabBar::Add(RawToValue<DockCont *>(&c), group); 
	if (GetCount() == autohide+1)
		RefreshParentLayout();
}

void AutoHideBar::RemoveCtrl(DockCont& c, int ix)
{
	if (c.IsOpen()) c.Close();

	if (ctrl == &c) {
		if (popup.IsOpen())
			popup.Close();
		ctrl->Remove();
		//KillTimeCallback(TIMEID_ANIMATE);
		KillTimeCallback(TIMEID_HIDE_TIMEOUT);			
		ctrl = NULL;				
	}
	if (ix >= 0) DockTabBar::Close(ix);
	if (GetCount() == autohide)
		RefreshParentLayout();	
}

void AutoHideBar::RemoveCtrl(int ix)
{
	if (ix >= 0) {
		DockCont *c = GetCtrl(ix);
		ASSERT(c);
		RemoveCtrl(*c, ix);
	}		
}

void AutoHideBar::TabHighlight()
{
	Ctrl *c = NULL;
	int ix = TabBar::GetHighlight();
	if (ix >= 0)
		c = GetCtrl(ix);
	if (!c || ctrl == c) 
		return;
	else if (ctrl) {
		if (c) {
			if (popup.IsPopUp())
				popup.Close();
			ctrl->Remove();
			ctrl = NULL;
		}
		else 
			HideAnimate(ctrl);
	}
	if (c) {
		ASSERT(ix >= 0 && ix < GetCount());
		// Clear WhenHighlight ot prevent stack overflow. Perhaps a better solution should be found...
		Callback cb = WhenHighlight;
		WhenHighlight = Callback();
		SetCursor(ix);
		ShowAnimate(c);
		WhenHighlight = cb;
	}
}

void AutoHideBar::TabClose(Value v)
{
	RemoveCtrl(*(ValueTo<DockCont *>(v)), -1);
	if (GetCount() == autohide-1)
		RefreshParentLayout();	
}

void AutoHideBar::ShowAnimate(Ctrl *c)
{
	if (c == ctrl) return;
	if (popup.IsPopUp()) popup.Close();
	
	Rect target = Ctrl::GetScreenRect();
	Size sz = c->GetStdSize();
	switch (GetAlign()) {
	 	case DockTabBar::LEFT: 
	 		sz.cy = target.Height();
	 		target.left = target.right;
	 		break;
	 	case DockTabBar::TOP:
	 		sz.cx = target.Width();
	 		target.top = target.bottom;
	 		break;
	 	case DockTabBar::RIGHT: 
	 		sz.cy = target.Height();
	 		target.right = target.left;
	 		break;
	 	case DockTabBar::BOTTOM:
	 		sz.cx = target.Width();
	 		target.bottom = target.top;
	 		break;
	};
	if (IsVert())
		sz.cx = min(sz.cx, GetParent()->GetSize().cx / 2);
	else
		sz.cy = min(sz.cy, GetParent()->GetSize().cy / 2);

	Rect init = target;
	AdjustSize(init, Size(5, 5));
	AdjustSize(target, sz);

	c->SetRect(sz);
	c->SizePos();
	popup << *(ctrl = c);
	c->Show();

	popup.SetRect(target);
	popup.PopUp(GetParent(), false, true, false, false);
	Animate(popup, target, GUIEFFECT_SLIDE);
}

void AutoHideBar::HideAnimate(Ctrl *c)
{
	ASSERT(ctrl);
	// If the popup has a child popup active then reset timer and keep the popup visible
	Vector<Ctrl *> wins = Ctrl::GetTopCtrls();
	for(int i = 0; i < wins.GetCount(); i++) {
		if (wins[i]->IsPopUp() && wins[i]->GetOwner() == &popup) {
			KillSetTimeCallback(autohide_timeout, THISBACK1(HideAnimate, ctrl), TIMEID_HIDE_TIMEOUT);
			return;
		}
	}
	
#ifdef PLATFORM_WIN32
	Rect r = popup.GetRect();
	AdjustSize(r, -r.GetSize());
	Animate(popup, r, GUIEFFECT_SLIDE);
#endif
	popup.Close();
	ctrl->Remove();
	ctrl = NULL;
	DockTabBar::SetCursor(-1);
}

void AutoHideBar::AdjustSize(Rect& r, const Size& sz)
{
	switch (DockTabBar::GetAlign()) {
	 	case DockTabBar::LEFT: 
	 		r.right += sz.cx;
	 		break;
	 	case DockTabBar::TOP: 
	 		r.bottom += sz.cy;
	 		break;
	 	case DockTabBar::RIGHT:
	 		r.left -= sz.cx;
	 		break;
	 	case DockTabBar::BOTTOM:
	 		r.top -= sz.cy;
	 		break;
	};		
}

int AutoHideBar::FindCtrl(const DockCont& c) const
{
	if (&c == ctrl) return GetCursor();
	for (int i = 0; i < GetCount(); i++) {
		DockCont *v = ValueTo<DockCont *>(Get(i));
		if (v == &c) return i;
	}
	return -1;
}

void AutoHideBar::TabDrag(int ix)
{
	GetCtrl(ix)->MoveBegin(); 
}

AutoHideBar::AutoHideBar()
{
	ctrl = NULL;
	AutoHideMin(0).InactiveDisabled().CanEmpty();
	popup.WhenEnter = THISBACK2(MouseEnter, Point(0, 0), 0);
	popup.WhenLeave = THISBACK(MouseLeave);
	DockTabBar::WhenHighlight = THISBACK(TabHighlight);
	WhenClose = THISBACK(TabClose);
	WhenDrag = THISBACK(TabDrag);
}

void AutoHideBar::HidePopup::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if (event == MOUSELEAVE)
		WhenLeave();
	else if (event != MOUSEMOVE) 
		WhenEnter();
}

