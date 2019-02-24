#include "Docking.h"

namespace Upp {

#define ALIGN_ASSERT(al)	ASSERT(al >= 0 && al < 4)
#define FRAME_MOVE_DIV 		5 // Outside fraction of the highlight that the mouse must be in to trigger dockpane reordering
#define VERSION				5 // Serialisation version

/*
 * Public interface
*/ 
void DockWindow::State(int reason)
{
	if (reason == Ctrl::OPEN) {
		if (!hideframe[0].GetParent())
			DockLayout();
		if (!init) {
			DockInit();
			init = true;
			StopHighlight(false);
		}
	}
	TopWindow::State(reason);
}

bool DockWindow::Key(dword key, int count)
{
	DoHotKeys(key);
	return TopWindow::Key(key, count);
}

void DockWindow::DoHotKeys(dword key)
{
	if (!HasCloseButtons() || IsLocked()) return;
	for (int i = 0; i < dockers.GetCount(); i++) {
		if (dockers[i]->IsHotKey(key))
			HideRestoreDocker(*dockers[i]);
	}
}

void DockWindow::Dock(int align, DockableCtrl& dc, int pos)
{
	ALIGN_ASSERT(align);
	Register(dc);
	DockContainer(align, *GetReleasedContainer(dc), pos);
}

void DockWindow::Tabify(DockableCtrl& target, DockableCtrl& dc)
{
	ASSERT(GetContainer(target));
	Register(dc);
	DockAsTab(*GetContainer(target), dc);
}

void DockWindow::Float(DockableCtrl& dc, Point p)
{
	if (dc.GetParent() && p.IsNullInstance())
		p = GetScreenRect().TopLeft();
	else
		Register(dc);
	FloatContainer(*GetReleasedContainer(dc), p);
}

void DockWindow::Float(DockableCtrl& dc, const char *title, Point p)
{
	dc.Title(title);
	Float(dc, p);
}

void DockWindow::AutoHide(DockableCtrl& dc)
{
	if (dc.IsAutoHide()) return;
	int align = GetDockAlign(dc);
	AutoHide(align == DOCK_NONE ? DOCK_TOP : align,  dc);
}

void DockWindow::AutoHide(int align, DockableCtrl& dc)
{
	ALIGN_ASSERT(align);
	Register(dc);
	DockCont *c = GetReleasedContainer(dc);
	c->StateAutoHide(*this);
	hideframe[align].AddCtrl(*c, dc.GetGroup());
}

int DockWindow::FindDocker(const Ctrl *dc)
{
	for (int i = 0; i < dockers.GetCount(); i++)
		if (dc == (Ctrl *) dockers[i])
			return i;
	return -1;
}

DockableCtrl& DockWindow::Register(DockableCtrl& dc)
{
	int ix = FindDocker(&dc);
	if (ix < 0) {
		ix = dockers.GetCount();
		dockers.Add(&dc);
		dockerpos.Add();
	}
	return *dockers[ix];
}

void DockWindow::Deregister(const DockableCtrl& dc)
{
	int ix = FindDocker(&dc);
	if (ix >= 0) {
		DockableCtrl &dc = *dockers[ix];
		dockers.Remove(ix);
		Close(dc);
		dockerpos.Remove(ix);
	}
	for (int i = 0; i < ctrls.GetCount(); i++) {
		if (&dc == &ctrls[i]) {
			ctrls.Remove(i);
			break;
		}
	}
}

void DockWindow::Close(DockableCtrl& dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetCount() > 1) {
		SaveDockerPos(dc);
		dc.Remove();
		c->Layout();
		return;
	}
	if (c) CloseContainer(*c);
}

void DockWindow::ActivateDockable(Ctrl& c)
{
	int ix = FindDocker(&c);
	if (ix >= 0)
		Activate(*dockers[ix]);
}

void DockWindow::ActivateDockableChild(Ctrl& c)
{
	Ctrl *p = c.GetParent();
	int ix = -1;
	while (p && (ix = FindDocker(p)) < 0)
		p = p->GetParent();
	if (ix >= 0)
		Activate(*dockers[ix]);
}

void DockWindow::Activate(DockableCtrl& dc)
{
	if (dc.IsVisible() && dc.IsOpen())
		return dc.TimedHighlight(200);
	DockCont *c = GetContainer(dc);
	if (!c)
		c = CreateContainer(dc);
	if (c->IsHidden())
		RestoreDockerPos(dc);
	else if (c->IsAutoHide()) {
		for (int i = 0; i <= DOCK_BOTTOM; i++)
			if (hideframe[i].HasCtrl(*c))
				hideframe[i].ShowAnimate(c);
	}
	else
		c->SetCursor(dc);
}

void DockWindow::SaveDockerPos(DockableCtrl& dc, PosInfo& pi)
{
	// Get the container
	DockCont *cont = GetContainer(dc);
	if (!cont) {
		// Ctrl must be hidden
		pi = PosInfo();
		return;	
	}
	// Are we tabbed?
	pi.tabcont = (cont->GetCount() > 1) ? cont : NULL;
	// Find top DockCont in case of nesting
	DockCont *parent = GetContainer(*cont);
	while (parent) {
		cont = parent;
		parent = GetContainer(*cont);
	}
	// Save state
	pi.state = cont->GetDockState();
	// determine context info
	StringStream s;
	switch (pi.state) {
		case DockCont::STATE_DOCKED: {
			int align = GetDockAlign(*cont);
			ALIGN_ASSERT(align);
			int ps = dockpane[align].GetChildIndex(cont);
			ASSERT(ps >= 0);
			Size sz = cont->GetSize();
			s % align % ps % sz;
			break;
		}
		case DockCont::STATE_FLOATING:
			cont->SerializePlacement(s);
			break;
		case DockCont::STATE_AUTOHIDE:
			for (int i = 0; i < 4; i++) {
				int ix = hideframe[i].FindCtrl(*cont);
				if (ix >= 0) {
					s.Put(ix);
					break;
				}
				ASSERT(i != 3); // No alignment found!
			}
			break;
		default:
			return;
	}
	pi.data = s;	
}

void DockWindow::SetDockerPosInfo(DockableCtrl& dc, const PosInfo& pi)
{
	// Find PosInfo record for the ctrl
	int ix = FindDocker(&dc);
	if (ix < 0) return;
	dockerpos[ix] = pi;
}

void DockWindow::SaveDockerPos(DockableCtrl& dc)
{
	// Find PosInfo record for the ctrl
	int ix = FindDocker(&dc);
	if (ix < 0) return;
	SaveDockerPos(dc, dockerpos[ix]);
}

void DockWindow::RestoreDockerPos(DockableCtrl& dc, bool savefirst)
{
	// Find PosInfo record for the ctrl
	int ix = FindDocker(&dc);
	if (ix < 0) return;
	PosInfo pi = dockerpos[ix];
	if (savefirst)
		SaveDockerPos(dc);
	if (pi.state == DockCont::STATE_NONE) {
		Float(dc);
		return;
	}	
	
	// Read position based on state
	StringStream s(pi.data);
	switch (pi.state) {
		case DockCont::STATE_DOCKED: {
			int align, ps;
			Size sz;
			s % align % ps % sz;
			ALIGN_ASSERT(align);
			if (pi.tabcont && pi.tabcont->GetDockAlign() == align)
				DockAsTab(*~pi.tabcont, dc);
			else {
				DockCont *cont = GetReleasedContainer(dc);
				cont->StateDocked(*this);
				Dock0(align, *cont, ps, sz);
			}
			break;
		}
		case DockCont::STATE_FLOATING: {
			DockCont *cont = GetReleasedContainer(dc);
			cont->SerializePlacement(s);
			if (pi.tabcont && pi.tabcont->IsFloating())
				DockAsTab(*~pi.tabcont, dc);			
			else
				FloatContainer(*cont, Null, false);
			break;
		}
		case DockCont::STATE_AUTOHIDE: {
			AutoHide(s.Get(), dc);
			break;
		}
		default:
			NEVER();
	}
}

void DockWindow::HideRestoreDocker(DockableCtrl& dc)
{
	if (dc.IsHidden())
		DockWindow::RestoreDockerPos(dc);
	else
		DockWindow::Close(dc);		
}

void DockWindow::DockGroup(int align, String group, int pos)
{
	ALIGN_ASSERT(align);
	bool all = group == "All"; 
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group && IsDockAllowed(align, *dockers[i]))
			Dock(align, *dockers[i], pos);
}

void DockWindow::ForceDockGroup(int align, String group, int pos)
{
	ALIGN_ASSERT(align);
	bool all = group == "All"; 
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group)
			Dock(align, *dockers[i], pos);
}

void DockWindow::FloatGroup(String group)
{
	bool all = group == "All"; 	
	Point p = GetScreenRect().TopLeft();
	Point inc(20, 20);	
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group) {
			p += inc;
			Float(*dockers[i], p);
		}
}

void DockWindow::AutoHideGroup(int align, String group)
{
	ALIGN_ASSERT(align);
	bool all = group == "All"; 	
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group)
			AutoHide(align, *dockers[i]);
}

void DockWindow::AutoHideGroup(String group)
{
	bool all = group == "All"; 	
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group)
			AutoHide(*dockers[i]);	
}

void DockWindow::TabDockGroup(int align, String group, int pos)
{
	if (DockCont *c = TabifyGroup(group)) {
		if (c->IsDockAllowed(align))
			DockContainer(align, *c, pos);
		else
			FloatContainer(*c);
	}
}

void DockWindow::ForceTabDockGroup(int align, String group, int pos)
{
	if (DockCont *c = TabifyGroup(group))
		DockContainer(align, *c, pos);
}

void DockWindow::TabFloatGroup(String group)
{
	if (DockCont *c = TabifyGroup(group)) 
		FloatContainer(*c);
}

DockCont * DockWindow::TabifyGroup(String group)
{
	DockCont *c = NULL;	
	bool all = group == "All"; 
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group) {
			if (c)
				DockAsTab(*c, *dockers[i]);
			else
				c = GetReleasedContainer(*dockers[i]);	
		}
	return c;
}

void DockWindow::CloseGroup(String group)
{
	bool all = group == "All"; 	
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group)
			Close(*dockers[i]);
	for (int i = 0; i < conts.GetCount(); i++)
	conts[i].Layout();
}

/*
 * Container management
*/ 
DockCont * DockWindow::CreateContainer()
{
	DockCont *dc = &conts.Add();
	dc->StateNotDocked(this);
	SyncContainer(*dc);
	return dc;
}

DockCont * DockWindow::CreateContainer(DockableCtrl& dc)
{
	DockCont *c = CreateContainer();
	c->Add(dc);
	return c;
}

void DockWindow::DestroyContainer(DockCont& c)
{
	for (int i = 0; i < conts.GetCount(); i++)
		if (&conts[i] == &c) 
			return conts.Remove(i);
}

DockCont *DockWindow::GetReleasedContainer(DockableCtrl& dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetCount() > 1) {
		SaveDockerPos(dc);
		dc.Remove();
		c->RefreshLayout();
		c = NULL;
	}
	if (c) 
		Detach(*c);
	else
		c = CreateContainer(dc);
	return c;
}

void DockWindow::SyncContainer(DockCont& c)
{
//	c.ToolWindow(childtoolwindows);	
	c.Grouping(grouping);
	c.WindowButtons(menubtn, hidebtn, closebtn);
	c.SyncButtons();
	c.Lock(IsLocked());
	if (!tabalign)
		c.SyncTabs(TabBar::BOTTOM, tabtext);
	else {
		int align;
		if ((align = GetDockAlign(c)) == DOCK_NONE && c.IsAutoHide())
			for (int i = 0; i < DOCK_BOTTOM; i++)
				if (hideframe[i].HasCtrl(c)) {
					align = i;
					break; 	
				}
		align = IsTB(align) ? TabBar::RIGHT : TabBar::BOTTOM;
		c.SyncTabs(align, tabtext);
	}
	
}

void DockWindow::SyncAll()
{
	for (int i = 0; i < conts.GetCount(); i++)
		SyncContainer(conts[i]);
}

/*
 * Docking/Undocking helpers
*/ 
void DockWindow::Undock0(Ctrl& c, bool do_animatehl, int fsz, bool ishighlight)
{
	int al = GetDockAlign(c);
	Ctrl *p = c.GetParent();
	if (p != &dockpane[al]) {
		c.Remove();
		if (p) {
			p->Layout();
			p->RefreshLayout();
		}
	}
	else {
		if (dockpane[al].GetFirstChild() == dockpane[al].GetLastChild())
			fsz = 0;
		dockpane[al].Undock(c, do_animatehl, ishighlight); // TODO: fix nasty hack
		if (fsz >= 0)
			DoFrameSize(do_animatehl, al, fsz);
	}
}

void DockWindow::DoFrameSize(bool animate, int align, int targetsize)
{
	if (!animate || !IsAnimatedFrames()) {
		dockframe[align].SetSize(targetsize);
		if (targetsize <= 0)
			dockframe[align].Hide();
	}
	else {
		FrameAnim& a = frameanim[align];
		if (a.inc)
			dockframe[align].SetSize(a.target);
		a.target = max(targetsize, 0);
		a.inc = (targetsize - dockframe[align].GetSize()) / dockpane[0].GetAnimMaxTicks();
		if (!a.inc) 
			a.inc = (targetsize > dockframe[align].GetSize()) ? 1 : -1;
		KillSetTimeCallback(-dockpane[0].GetAnimInterval(), THISBACK(FrameAnimateTick), TIMEID_ANIMATE);
	}			
}

void DockWindow::FrameAnimateTick()
{
	bool done = true;
	for (int i = 0; i < 4; i++) {
		FrameAnim& a = frameanim[i];
		if (a.inc) {
			int sz = dockframe[i].GetSize() + a.inc;
			if (a.inc > 0 && sz >= a.target || a.inc < 0 && sz <= a.target) {
				sz = a.target;
				a.inc = 0;
				if (sz == 0)
					dockframe[i].Hide();
			}
			else
				done = false;			
			dockframe[i].SetSize(sz);
		}
	}
	if (done)
		KillTimeCallback(TIMEID_ANIMATE);
}
void DockWindow::Undock(DockCont& c)
{
	if (!c.IsFloating() && !c.IsHidden()) {
		if (c.IsAutoHide()) {
			for (int i = 0; i < 4; i++) {
				int ix = hideframe[i].FindCtrl(c);
				if (ix >= 0) {
					hideframe[i].RemoveCtrl(ix);
					hideframe[i].RefreshFrame();
					break;
				}
			}				
		}
		else {
			int dock = GetDockAlign(c);
			if (dock >= 0 && dock < 4) {
				Ctrl *p = &c;
				while (p && p->GetParent() != &dockpane[dock]) p = p->GetParent();
				ASSERT(p);
				bool v = !IsTB(dock);		
				c.SyncUserSize(v, !v);
			}
			Undock0(c);		
		}
		c.StateNotDocked();
	}		
}

void DockWindow::Unfloat(DockCont& c)
{
	if (c.IsFloating()) {
		c.SyncUserSize(true, true);
		if (c.IsOpen() || c.IsPopUp()) 
			c.Close();
		c.StateNotDocked();
	}
}

void DockWindow::DockAsTab(DockCont& target, DockableCtrl& dc)
{
	DockCont *c = GetContainer(dc);
	if (c) {
		if (c->GetCount() == 1)
			CloseContainer(*c);
		else {
			Ctrl *c = dc.GetParent();
			dc.Remove();
			c->Layout();
		}
	}
	target.Add(dc);
}

void DockWindow::Dock0(int align, Ctrl& c, int pos, bool do_animatehl, bool ishighlight)
{
	Dock0(align, c, pos, CtrlBestSize(c, align), do_animatehl, ishighlight);
}

void DockWindow::Dock0(int align, Ctrl& c, int pos, Size sz, bool do_animatehl, bool ishighlight)
{
	int fsz = IsTB(align) ? sz.cy : sz.cx;
	if (!dockframe[align].IsShown())
		dockframe[align].Show();
	if (fsz > dockframe[align].GetSize())
		DoFrameSize(do_animatehl, align, fsz);
	dockpane[align].Dock(c, sz, pos, do_animatehl, ishighlight);
}

/*
 * Container docking
*/ 
void DockWindow::DockContainer(int align, DockCont& c, int pos)
{
	Detach(c);
	c.StateDocked(*this);
	Dock0(align, c, pos);
}

void DockWindow::DockContainerAsTab(DockCont& target, DockCont& c, bool do_nested)
{
	Detach(c);	
	if (do_nested) {
		c.StateTabbed(*this);
		target << c;
	}
	else {
		target.AddFrom(c);
		DestroyContainer(c);	
	}
}

void DockWindow::FloatContainer(DockCont& c, Point p, bool move)
{
	ASSERT(IsOpen());
	if (c.IsFloating()) return;
	Detach(c);	
	c.StateFloating(*this);
	if (move) {
		Size best = CtrlBestSize(c, false);
		if (p.IsNullInstance()) 
			p = GetScreenRect().CenterPoint() - best/2;
		c.SetClientRect(Rect(p, best));
	}
	c.Open(this);
}

void DockWindow::FloatFromTab(DockCont& c, DockCont& tab)
{
	Rect r = c.GetScreenRect();
	tab.SetClientRect(r);
	tab.StateNotDocked(this);
	c.RefreshLayout();
	tab.MoveBegin();
}

void DockWindow::AutoHideContainer(int align, DockCont& c)
{
	while (c.GetCount() && !c.IsAutoHide())
		AutoHide(align, c.Get(0));	
}

void DockWindow::CloseContainer(DockCont& c)
{
	c.SetAllDockerPos();
	c.Clear(); 
	Detach(c);
	DestroyContainer(c);	
}

/*
 * Docking Interface helpers
*/ 
DockCont *DockWindow::GetMouseDockTarget()
{
	Point mp = GetMousePos();
	for (int i = 0; i < 4; i++) {
		if (dockpane[i].IsShown()) {
			Rect r = dockpane[i].GetScreenRect();
			if (r.Contains(mp))
				return dynamic_cast<DockCont *>(dockpane[i].ChildFromPoint(mp -= r.TopLeft()));
		}
	}
	return NULL;
}

int DockWindow::GetDockAlign(const Ctrl& c) const
{
	Ctrl *p = c.GetParent();
	while (p && p->GetParent() != this) p = p->GetParent();
	for (int i = 0; i < 4; i++) 
		if (dockpane[i].GetParent() == p) return i;
	return DOCK_NONE;
}

int DockWindow::GetDockAlign(const Point& p) const
{
	for (int i = 0; i < 4; i++) 
		if (dockpane[i].IsVisible() && dockpane[i].GetScreenRect().Contains(p)) 
			return i; 
	return DOCK_NONE;
}

int DockWindow::GetAutoHideAlign(const DockCont& c) const
{
	for (int i = 0; i < 4; i++) {
		if (hideframe[i].HasCtrl(c))
			return i;
	}
	return DOCK_NONE;
}

Size DockWindow::CtrlBestSize(const Ctrl& c, int align, bool restrict) const
{
	Size mn = c.GetMinSize();
	Size mx = c.GetMaxSize();
	Size std = c.GetStdSize();
	if (restrict) {
		if (IsTB(align))
			mx.cy = minmax(GetSize().cy/2, mn.cy, mx.cy);
		else
			mx.cx = minmax(GetSize().cx/2, mn.cx, mx.cx);
	}
	return minmax(std, mn, mx);
}

DockCont *DockWindow::FindDockTarget(DockCont& dc, int& dock)
{
	Point p = GetMousePos();
	Rect r = GetScreenView(); // IODO: This is not technically correct if the user has added additional frames. 
							  // Need to manually FrameAddSize for all frames up tolast dockpane
	DockCont *target = NULL;
	int align = DOCK_NONE;
	
	dock = DOCK_NONE;
	
	if (r.Contains(p)) {
		// Check for mouse near hidden dockpanes
		dock = GetPointAlign(p, r, true, true, true);
		if (dock != DOCK_NONE && dockpane[dock].IsVisible())
			dock = DOCK_NONE;
	}
	else {
		// Check for mouse over a docked container
		target = GetMouseDockTarget();		
		if (target) {
			r = target->GetScreenRect();
			dock = GetDockAlign(*target);
			align = GetPointAlign(p, r, IsTabbing(), IsTB(dock), !IsTB(dock));
		}
		else 
			return NULL;
	}
	
	if (dock != DOCK_NONE && (!dc.IsDockAllowed(dock) 
		 || IsPaneAnimating(dock) || IsFrameAnimating(dock))
		 || (dock == DOCK_NONE && !target)) {
		dock = DOCK_NONE;
		return NULL;
	}	
	
	// Prepare for highlight
	if (target) {
		GetHighlightCtrl().bounds = GetAlignBounds(align, r, IsTabbing(), IsTB(dock), !IsTB(dock));	
		if (align == DOCK_NONE) 
			dock = DOCK_NONE; // Tabbing
		// The following code handles the case of an insertion between two docked controls. In this case we must set 
		//  the highlight bounds to be a union of the bounds from each control. Very ugly.
		if (dock != DOCK_NONE) {
			Ctrl *c = IsTL(align) ? target->GetPrev() : target->GetNext();
			if (c) {
				int opal = align > 1 ? align-2 : align+2;
				GetHighlightCtrl().bounds.Union(GetAlignBounds(opal, c->GetScreenRect(), IsTabbing()));
			}
			target = IsTL(align) ? target : dynamic_cast<DockCont*>(target->GetNext());
		}
		
	}
	else if (dock != DOCK_NONE)
		GetHighlightCtrl().bounds = GetAlignBounds(dock, r, true);	
		
	return target;	
}

Rect DockWindow::GetAlignBounds(int al, Rect r, bool center, bool allow_lr, bool allow_tb)
{
	Size border = r.GetSize()/4;
	switch (al) {
	case DOCK_LEFT:
		r.right = r.left + (center ? border.cx : (GHalf_(r.left + r.right)));
		return r;
	case DOCK_TOP:
		r.bottom = r.top + (center ? border.cy : (GHalf_(r.top + r.bottom)));
		return r;
	case DOCK_RIGHT:
		r.left = r.right - (center ? border.cx : (GHalf_(r.left + r.right)));
		return r;
	case DOCK_BOTTOM:
		r.top = r.bottom - (center ? border.cy : (GHalf_(r.top + r.bottom)));
		return r;
	}
	if (allow_lr)
		r.DeflateHorz(border.cx);
	if (allow_tb)
		r.DeflateVert(border.cy);
	return r;
}	

int DockWindow::GetPointAlign(const Point p, Rect r, bool center, bool allow_lr, bool allow_tb)
{
	Size border = r.GetSize();
	border.cx = allow_lr ? border.cx/4 : 0;
	border.cy = allow_tb ? border.cy/4 : 0;
	if (center && r.Deflated(border).Contains(p))
		return DOCK_NONE;
	int q = GetQuad(p, r);
	int al = DOCK_NONE;
	if (q == 0 || q == 1)
		al = DOCK_TOP;
	else if (q == 2 || q == 3)
		al = DOCK_RIGHT;
	else if (q == 4 || q == 5)
		al = DOCK_BOTTOM;
	else if (q == 6 || q == 7)
		al = DOCK_LEFT;
	if (!allow_lr && (al == DOCK_LEFT || al == DOCK_RIGHT))
		al = (q == 7 || q == 2) ? DOCK_TOP : DOCK_BOTTOM;
	else if (!allow_tb && (al == DOCK_TOP || al == DOCK_BOTTOM))
		al = (q == 0 || q == 5) ? DOCK_LEFT : DOCK_RIGHT;
	return al;
}

int DockWindow::GetQuad(Point p, Rect r)
/* Finds the quadrant in the rectangle for point p:
	-------
	|\0|1/|
	|7\|/2|
	|-----| 
	|6/|\3|
	|/5|4\|
	-------
*/
{
	Point cp = r.CenterPoint();
	p -= cp;
	cp -= r.TopLeft();
	if (p.x <= 0) { // Left
		if (p.y <= 0) // Top-Left
			return ((p.y * cp.x)/cp.y > p.x) ? 7 : 0;
		else {// Bottom-Left
			p.x = -p.x;
			return ((p.y * cp.x)/cp.y > p.x) ? 5 : 6;
		}
	}
	else { // Right
		if (p.y <= 0) { // Top-Right
			p.x = -p.x;
			return ((p.y * cp.x)/cp.y > p.x) ? 2 : 1;
		}
		else // Bottom-Right
			return ((p.y * cp.x)/cp.y > p.x) ? 4 : 3;		
	}
}

Rect DockWindow::GetFinalAnimRect(int align, Ctrl& c)
{
	if (c.GetParent() != &dockpane[align])
		// c is docked as a tab
		return c.GetScreenRect();
	
	Rect r = dockpane[align].GetFinalAnimRect(c);
	r.Offset(dockpane[align].GetScreenRect().TopLeft());
	if (IsFrameAnimating(align)) {
		switch (align) {
		case DOCK_LEFT:
			r.right = r.left + frameanim[align].target - 4;
			break;
		case DOCK_TOP:
			r.bottom = r.top + frameanim[align].target - 4;
			break;
		case DOCK_RIGHT:
			r.left = r.right - frameanim[align].target - 4;
			break;
		case DOCK_BOTTOM:
			r.top = r.bottom - frameanim[align].target - 4;
			break;
		}		
	}
	return r;	
}

// HighlightCtrl
void DockWindow::HighlightCtrl::Paint(Draw& w)
{	
	if (buffer.IsEmpty())
		ChPaint(w, GetSize(), *highlight);
	else
		w.DrawImage(0, 0, buffer);
}

void DockWindow::HighlightCtrl::SetHighlight(const Value& hl, bool _isnested, bool _cannest, Image bg)
{
	highlight = &hl;
	img = bg;	
	buffer.Clear();
	cannest = _cannest;
	isnested = cannest && _isnested;
	CreateBuffer();
}

void DockWindow::HighlightCtrl::SetNested(bool _isnested)
{
	bool nest = cannest && _isnested;
	if (nest != isnested) {
		isnested = nest;		
		CreateBuffer();
	}
}

void DockWindow::HighlightCtrl::CreateBuffer()
{
	if (!img.IsEmpty()) {
		Size sz = img.GetSize();
		ImageDraw w(sz);
		w.DrawImage(0, 0, img);
		if (isnested) {
			Rect r = sz;
			Rect t = r;
			const TabCtrl::Style& s = TabBar::StyleDefault();
			t.bottom -= s.tabheight + s.sel.top + 1;	// Nasty bodge! See TabBar::GetStyleHeight
			r.top = t.bottom-1;
			r.right = max(min(150, r.GetWidth()/3), 20);
			r.left += 10;
			ChPaint(w, r, *highlight);
			ChPaint(w, t, *highlight);
		}
		else
			ChPaint(w, sz, *highlight);			
		buffer = w;
		Refresh();
	}	
}

// Drag and Drop interface
void DockWindow::StartHighlight(DockCont *dc)
{
	int align = DOCK_NONE;
	DockCont *target = FindDockTarget(*dc, align);
	if (align != DOCK_NONE || target) {
		dc->SyncUserSize(true, true);
		Highlight(align, *dc, target);
	}
	else 
		StopHighlight(IsAnimatedHighlight());
}

void DockWindow::Highlight(int align, DockCont& cont, DockCont *target)
{
	HighlightCtrl& hl = GetHighlightCtrl();
	DockableCtrl& dc = cont.GetCurrent();
	ASSERT(&hl != (Ctrl *)&cont);

	hl.SizeHint(cont.GetMinSize(), cont.GetMaxSize(), cont.GetStdSize()).Show();

	prehighlightframepos = -1;
	if (align != DOCK_NONE) {
		if (NeedFrameReorder(align))
			DoFrameReorder(align);
		// Do highlight
		hl.SetHighlight(dc.GetStyle().highlight[0], false, 0);
		hl.oldframesize = dockframe[align].GetSize();	
		int pos = target ? dockpane[align].GetChildIndex(target) : -1;
		Dock0(align, hl, pos, IsAnimatedHighlight(), true);
	}
	else if (target && IsTabbing()) {
		hl.Title(cont.GetTitle(true)).Icon(dc.GetIcon());
		hl.SetHighlight(dc.GetStyle().highlight[1], CheckNesting(), cont.GetCount() > 1, 
		             target->GetHighlightImage());
		if (false && frameorder) {
			align = GetDockAlign(*target);
			ALIGN_ASSERT(align);
			if (NeedFrameReorder(align))
				DoFrameReorder(align);			
		}		             
		DockAsTab(*target, hl);	
	}
}

void DockWindow::StopHighlight(bool do_animatehl)
{
	HighlightCtrl& hl = GetHighlightCtrl();
	if (hl.GetParent()) {
		UndoFrameReorder();
		Undock0(hl, do_animatehl, hl.oldframesize, true);
		hl.ClearHighlight();
	}
}

void DockWindow::DoFrameReorder(int align)
{
	// Reorder dockpanes
	int ix = FindFrame(dockframe[align]);
	if (ix != dockframepos) {
		prehighlightframepos = ix;
		RemoveFrame(ix);
		InsertFrame(dockframepos, dockframe[align]);
	}	
}

void DockWindow::UndoFrameReorder()
{
	if (IsReorderingFrames()) {
		CtrlFrame& c = GetFrame(dockframepos);
		RemoveFrame(dockframepos);
		InsertFrame(prehighlightframepos, c);
		prehighlightframepos = -1;
	}	
}

bool DockWindow::NeedFrameReorder(int align)
{
	// Determine if we need to re-order the dockpanes to set layout precedence
	if (!frameorder) return false;
	switch (align) {
	case DOCK_LEFT:
		return GetMousePos().x < (GetHighlightCtrl().bounds.left + GetHighlightCtrl().bounds.Width() / FRAME_MOVE_DIV);
	case DOCK_RIGHT:
		return GetMousePos().x > (GetHighlightCtrl().bounds.right - GetHighlightCtrl().bounds.Width() / FRAME_MOVE_DIV);
	case DOCK_TOP:
		return GetMousePos().y < (GetHighlightCtrl().bounds.top + GetHighlightCtrl().bounds.Height() / FRAME_MOVE_DIV);	
	case DOCK_BOTTOM:
		return GetMousePos().y > (GetHighlightCtrl().bounds.bottom - GetHighlightCtrl().bounds.Height() / FRAME_MOVE_DIV);		
	}	
	return false;
}

void DockWindow::ContainerDragStart(DockCont& dc)
{
	if (!dc.IsFloating()) {
		// Reposition if not under the mouse
		Detach(dc);	
		dc.StateFloating(*this);
		Rect r = dc.GetScreenView();
		r.SetSize(CtrlBestSize(dc, false));
		dc.SyncUserSize(true, true);
		if (IsAnimatedHighlight() && dc.IsDocked() && dc.GetParent()) {
			Undock0(dc, true);
			dc.StateNotDocked();
		}
		dc.PlaceClientRect(r);
		if(!dc.IsOpen())
			dc.Open(this);
		dc.StartMouseDrag();
	}
}

void DockWindow::ContainerDragMove(DockCont& dc)
{
	HighlightCtrl& hl = GetHighlightCtrl();
	Point p = GetMousePos();
	if (hl.GetParent()) {
		if (!hl.bounds.Contains(p))
			StopHighlight(IsAnimatedHighlight());
		else {
			if (frameorder) {
				int align = GetDockAlign(hl);
				bool needed = (hl.GetParent() == &dockpane[align]) &&  NeedFrameReorder(align);
				if (!needed && IsReorderingFrames())
					UndoFrameReorder();
				else if (needed && !IsReorderingFrames())
					DoFrameReorder(align);
			}
			hl.SetNested(CheckNesting());
		}
		return KillTimeCallback(TIMEID_ANIMATE_DELAY);
	}	
	animdelay ? 
		KillSetTimeCallback(animdelay, THISBACK1(StartHighlight, &dc), TIMEID_ANIMATE_DELAY) :
		StartHighlight(&dc);
}
	
void DockWindow::ContainerDragEnd(DockCont& dc)
{
	KillTimeCallback(TIMEID_ANIMATE_DELAY);
	if (!dc.IsFloating()) return;
	HighlightCtrl& hl = GetHighlightCtrl();

	Ctrl *p = hl.GetParent();
	int align = DOCK_NONE; 

	if (p) 
		for (int i = 0; i < 4; i++)
			if (p == &dockpane[i]) align = i;

	if (animatewnd && (p || align != DOCK_NONE))
		dc.Animate(GetFinalAnimRect(align, hl), dockpane[0].GetAnimMaxTicks(), 5);
		
	if (align != DOCK_NONE) {
		Unfloat(dc);
		dc.StateDocked(*this);
		dockpane[align].Swap(hl, dc);
		dc.SetFocus();		
	}
	else if (DockCont *target = dynamic_cast<DockCont *>(p)) {
		StopHighlight(false);
		DockContainerAsTab(*target, dc, CheckNesting() && dc.GetCount() > 1);
	}
	else
		StopHighlight(false);
}

/*
 * Misc
*/ 
void DockWindow::LockLayout(bool lock)
{
	locked = lock;
	SyncAll();
}

DockWindow& DockWindow::TabAutoAlign(bool al)
{
	tabalign = al;
	SyncAll(); 
	return *this;		
}

DockWindow& DockWindow::TabShowText(bool text)
{
	tabtext = text; 
	SyncAll(); 
	return *this;	
}

void DockWindow::SetFrameSize(int align, int size)
{
	ALIGN_ASSERT(align);
	dockframe[align].SetSize(size);
}

DockWindow&  DockWindow::Animate(bool highlight, bool frame, bool window, int ticks, int interval)
{
	animatehl = highlight;
	animatewnd = window;
	animatefrm = frame;
	ticks = max(ticks, 1);
	interval = max(interval, 1);
	for (int i = 0; i < 4; i++) 
		dockpane[i].SetAnimateRate(ticks, interval);
	return *this;
}

DockWindow&  DockWindow::Grouping(bool _grouping)
{
	if (_grouping != grouping) {
		grouping = _grouping;
		SyncAll();
	}
	return *this;
}

DockWindow&  DockWindow::SetFrameOrder(int first, int second, int third, int fourth)
{
	if (fourth != DOCK_NONE)
		DoFrameReorder(fourth);
	if (third != DOCK_NONE)
		DoFrameReorder(third);
	if (second != DOCK_NONE)
		DoFrameReorder(second);
	if (first != DOCK_NONE)
		DoFrameReorder(first);
	RefreshLayout();
	return *this;
}

DockWindow&  DockWindow::AllowDockAll()
{
	return AllowDockLeft().AllowDockRight().AllowDockTop().AllowDockBottom();	
}

DockWindow&  DockWindow::AllowDockNone()
{
	return AllowDockLeft(false).AllowDockRight(false).AllowDockTop(false).AllowDockBottom(false);
}

bool DockWindow::IsDockAllowed(int align, DockableCtrl& dc) const
{
	ALIGN_ASSERT(align);
	return dockable[align] && dc.IsDockAllowed(align);
}

DockWindow&  DockWindow::AutoHide(bool v)
{
	if (v != autohide) {
		autohide = v;
		SyncAll();
	}
	return *this;
}

DockWindow&  DockWindow::WindowButtons(bool menu, bool hide, bool close)
{
	if (menu == menubtn && close == closebtn && hide == hidebtn) return *this;;
	menubtn = menu;
	closebtn = close;
	hidebtn = hide;
	SyncAll();
 	return *this;	
}

void DockWindow::DockLayout(bool tb_precedence)
{
	if (hideframe[0].GetParent())
		for (int i = 0; i < 4; i++) {
			RemoveFrame(hideframe[i]);
			RemoveFrame(dockframe[i]);
		}
	int i = tb_precedence ? 0 : 1;
	// Top, Bottom, Left, Right
	AddFrame(hideframe[1-i]);
	AddFrame(hideframe[3-i]);
	AddFrame(hideframe[0+i]);
	AddFrame(hideframe[2+i]);
			
	dockframepos = GetFrameCount();
	AddFrame(dockframe[1-i]);
	AddFrame(dockframe[3-i]);
	AddFrame(dockframe[0+i]);
	AddFrame(dockframe[2+i]);
}

DockableCtrl&  DockWindow::Dockable(Ctrl& ctrl, WString title)
{
	DockableCtrl& dc = Register(ctrls.Add().SizeHint(ctrl.GetMinSize()));
	if (!IsNull(title)) dc.Title(title);
	dc << ctrl.SizePos();
	return dc;
}

void DockWindow::DockManager()
{
	DockConfigDlg(*this).Run();
}

void DockWindow::DockWindowMenu(Bar& bar)
{
	if (IsGrouping())
		menu.GroupListMenu(bar);
	else
		menu.WindowListMenu(bar, t_("All"));
	if (dockers.GetCount())
		bar.Separator();
	if (layouts.GetCount()) {
		bar.Add(t_("Layouts"), callback(&menu, &DockMenu::LayoutListMenu));
		bar.Separator();
	}
	bar.Add(t_("Manage Windows..."), THISBACK(DockManager));
}

void DockWindow::SerializeWindow(Stream& s)
{
	int version = VERSION;
	int v = version;
	s / v;
	
	SerializeLayout(s, true);
	
	s % tabbing % autohide % animatehl % nestedtabs 
	  % grouping % menubtn % closebtn % hidebtn % nesttoggle;
	if (version >= 4)
		s % locked;
	if (version >= 5)
		s % frameorder;
	if (s.IsLoading()) {
		SyncAll();
		init = true;
	}
}

void DockWindow::ClearLayout()
{
	// Close everything
	for (int i = 0; i < conts.GetCount(); i++)
		CloseContainer(conts[i]);
	for (int i = 0; i < 4; i++) {
		dockpane[i].Clear(); 
		hideframe[i].Clear();
		frameanim[i].inc = 0;
	}
	conts.Clear();	
}

void DockWindow::SerializeLayout(Stream& s, bool withsavedlayouts)
{
	StopHighlight(false);
	int cnt = 0;
	int version = VERSION;
	
	s.Magic();
	s % version;
	
	// Groups
	ArrayMap<String, Vector<int> > groups;
	if (s.IsStoring())
		for (int i = 0; i < dockers.GetCount(); i++) {
			String g = dockers[i]->GetGroup();
			if (!g.IsEmpty()) {
				int ix = groups.Find(g);
				if (ix < 0) {
					groups.Add(dockers[i]->GetGroup(), Vector<int>());
					ix = groups.GetCount() - 1;
				}	
				groups[ix].Add(i);
			}
		}
	s % groups;
	if (s.IsLoading()) {
		ClearLayout();		
		
		for (int i = 0; i < dockers.GetCount(); i++)
			dockers[i]->SetGroup(Null);
		for (int i = 0; i < groups.GetCount(); i++) {
			Vector<int>& v = groups[i];
			const String& g = groups.GetKey(i);
			for (int j = 0; j < v.GetCount(); j++) {
				int ix = v[j];
				if (ix >= 0 && ix < dockers.GetCount())
					dockers[ix]->SetGroup(g);
			}
		}
	}
			    
	if (s.IsStoring()) {
		// Write frame order
		for (int i = 0; i < 4; i++) {
			int ps = max(FindFrame(dockframe[i]) - dockframepos, 0);
			s % ps;
		}
		// Write docked
		for (int i = 0; i < 4; i++) {
			DockPane& pane = dockpane[i];
			int fsz = dockframe[i].IsShown() ? dockframe[i].GetSize() : 0;
			
			s / fsz % pane;
			DockCont *dc = dynamic_cast<DockCont *>(pane.GetFirstChild());
			for (int j = 0; dc && j < pane.GetCount(); j++) {
				s % *dc;
				dc = dynamic_cast<DockCont *>(dc->GetNext());
			}
		}
		cnt = 0;
		// Count Floating
		for (int i = 0; i < conts.GetCount(); i++)
			if (conts[i].IsFloating()) cnt++;
		// Write Floating
		s / cnt;
		for (int i = 0; i < conts.GetCount(); i++) {
			if (conts[i].IsFloating()) {
				conts[i].Serialize(s);
				conts[i].SerializePlacement(s, false);
			}
		}
		// Write Autohidden
		for (int i = 0; i < 4; i++) {
			cnt = hideframe[i].GetCount();
			s / cnt;
			for (int j = 0; j < hideframe[i].GetCount(); j++) {
				int ix = FindDocker(&hideframe[i].GetCtrl(j)->Get(0));
				if (ix >= 0)
					s / ix;
			}
		}		
	}
	else {
		// Read frame order
		if (version >= 5) {
			int ps[4];
			for (int i = 0; i < 4; i++) {
				s % ps[i];
				SetFrame(dockframepos + i, InsetFrame());
			}
			for (int i= 0; i < 4; i++)
				SetFrame(ps[i] + dockframepos, dockframe[i]);
		}
		// Read docked
		for (int i = 0; i < 4; i++) {
			DockPane& pane = dockpane[i];
			dockframe[i].Hide();
			int fsz;
			s / fsz % pane;

			for (int j = 0; j < pane.GetCount(); j++) {
				DockCont *dc = CreateContainer();
				s % *dc;
				dc->StateDocked(*this);
				pane << *dc;
			}
			if (fsz && pane.GetCount()) {
				dockframe[i].SetSize(fsz);
				dockframe[i].Show();
			} 
			else
				dockframe[i].SetSize(0);			
		}	
		// Read floating
		s / cnt;
		for (int i = 0; i < cnt; i++) {
			DockCont *dc = CreateContainer();
			dc->Serialize(s);
			FloatContainer(*dc);
			dc->SerializePlacement(s, false);	
		}
		// Read Autohidden
		for (int i = 0; i < 4; i++) {
			s / cnt;
			for (int j = 0; j < cnt; j++) {
				int ix;
				s / ix;
				if (ix >= 0 && ix < dockers.GetCount())
					AutoHide(i, *dockers[ix]);
			}		
		}
		// Clear empty containers
		for (int i = conts.GetCount()-1; i >= 0; i--) {
			if (!conts.GetCount()) CloseContainer(conts[i]);
		}
		RefreshLayout();
	}
	bool haslay = withsavedlayouts;
	s % haslay;
	if (withsavedlayouts && (s.IsStoring() || haslay))
		s % layouts;
	s.Magic();
}

void DockWindow::BackupLayout()
{
	StringStream s;
	SerializeLayout(s);
	layoutbackup = s;
}

void DockWindow::RestoreLayout()
{
	if (!layoutbackup.GetCount()) return;
	StringStream s(layoutbackup);
	SerializeLayout(s);	
}

int DockWindow::SaveLayout(String name)
{
	ASSERT(!name.IsEmpty());
	StringStream s;
	SerializeLayout(s, false);
	int ix = layouts.FindAdd(name, String());
	layouts[ix] = (String)s;
	return ix;
}

void DockWindow::LoadLayout(String name)
{
	int ix = layouts.Find(name); 
	if (ix >= 0) 
		LoadLayout(ix);	
}

void DockWindow::LoadLayout(int ix)
{
	ASSERT(ix >= 0 && ix < layouts.GetCount());
	StringStream s((String)layouts[ix]);
	SerializeLayout(s, false);
	RefreshLayout();
}

void DockWindow::EnableFloating(bool enable)
{
	for (int i = 0; i < conts.GetCount(); i++)
		if (conts[i].IsFloating())
			conts[i].Enable(enable);
}

DockWindow::DockWindow()
{
	menu.Set(*this);

#ifdef PLATFORM_WIN32
	childtoolwindows = true;
#endif

	init= false;
	tabbing = true;
	nestedtabs = false;
	grouping = true;
	menubtn = true;
	nesttoggle = (K_CTRL | K_SHIFT);
	closebtn = true;
	hidebtn = true;
	locked = false;
	tabtext = true;
	tabalign = false;
	autohide = true;
	frameorder = true;
	childtoolwindows = false;
	showlockedhandles = false;

	for (int i = 0; i < 4; i++) {
		dockframe[i].Set(dockpane[i], 0, i);
		IsTB(i) ? dockpane[i].Horz() : dockpane[i].Vert();
		hideframe[i].SetAlign(i);
		dockframe[i].Hide();
	}
	AllowDockAll().Animate().AnimateDelay(30);
}

// PopUpDockWindow
#define POPUP_SPACING 0

void PopUpDockWindow::ContainerDragStart(DockCont& dc)
{
	DockWindow::ContainerDragStart(dc);
	last_target = NULL;
	last_popup = NULL;	
}

void PopUpDockWindow::ContainerDragMove(DockCont& dc)
{
	int align = DOCK_NONE;

	// Is the highlight the same as last time? (Quick escape)
	if (last_popup && last_popup->IsPopUp() && last_popup->GetRect().Contains(GetMousePos()))
		return;
	
	DockCont *target = GetMouseDockTarget();
	int dock = DOCK_NONE;
	if (target) {
		dock = GetDockAlign(*target);
		if (!dc.IsDockAllowed(dock)) 
			target = NULL;
	}
	bool target_changed =  (target != last_target) 
						&& !GetHighlightCtrl().GetParent() 
						&& (!target || !IsPaneAnimating(dock));
	
	// Hide show inner popups as necessary
	if (!target && last_target != NULL)
		HidePopUps(true, false);
	else if (target_changed)
		ShowInnerPopUps(dc, target);
	ShowOuterPopUps(dc);		
	last_target = target;
	
	// Get potential alignment
	align = PopUpHighlight(inner, 5);
	if (align == DOCK_NONE) {
		target = NULL;
		last_target = NULL;
		align = PopUpHighlight(outer, 4);
	}
	else if (align == 4)
		align = DOCK_NONE;
	else if (target) {
		target = IsTL(align) ? target : dynamic_cast<DockCont*>(target->GetNext());
		align = dock;
	}
	
	// Do highlight
	if (align != DOCK_NONE || target) {
		if (align == DOCK_NONE) StopHighlight(false);
		dc.SyncUserSize(true, true);
		Highlight(align, dc, target);
	}
	else  {
		PopUpHighlight(hide, 4);
		StopHighlight(IsAnimatedHighlight());
		last_popup = NULL;
	}
}

void PopUpDockWindow::ContainerDragEnd(DockCont& dc)
{
	int align = DOCK_NONE;
	if (IsAutoHide() && showhide)
		align = PopUpHighlight(hide, 4);
	HidePopUps(true, true);
	if (align != DOCK_NONE)
		AutoHideContainer(align, dc);	
	else
		DockWindow::ContainerDragEnd(dc);	
	last_target = NULL;
	last_popup = NULL;	
}

int PopUpDockWindow::PopUpHighlight(PopUpButton *pb, int cnt)
{
	int ix = -1;
	Point p = GetMousePos();
	for (int i = 0; i < cnt; i++) {
		if (pb[i].IsPopUp() && pb[i].GetRect().Contains(p)) {
			if (!pb[i].hlight) {
				pb[i].hlight = &(style->highlight);
				pb[i].Refresh();
			}
			ix = i;
			last_popup = &pb[i];
		}
		else if (pb[i].hlight) {
			pb[i].hlight = NULL;
			pb[i].Refresh();
		}
	}
	return ix;
}

void PopUpDockWindow::ShowOuterPopUps(DockCont& dc)
{
	Rect wrect = GetScreenRect();
	Point cp = wrect.CenterPoint();
	Size psz(style->outersize, style->outersize);
	Rect prect = Rect(psz);	
	psz /= 2;
	
	wrect.Deflate(12, 12);
	
	if (dc.IsDockAllowed(DOCK_LEFT)) 	ShowPopUp(outer[DOCK_LEFT], prect.Offseted(wrect.left + POPUP_SPACING, cp.y - psz.cy));	
	if (dc.IsDockAllowed(DOCK_TOP)) 	ShowPopUp(outer[DOCK_TOP], prect.Offseted(cp.x - psz.cx, wrect.top + POPUP_SPACING));	 
	if (dc.IsDockAllowed(DOCK_RIGHT)) 	ShowPopUp(outer[DOCK_RIGHT], prect.Offseted(wrect.right - POPUP_SPACING - psz.cx*2, cp.y - psz.cy));
	if (dc.IsDockAllowed(DOCK_BOTTOM)) 	ShowPopUp(outer[DOCK_BOTTOM], prect.Offseted(cp.x - psz.cx, wrect.bottom - POPUP_SPACING - psz.cy*2));
	
	if (IsAutoHide() && showhide) {
		ShowPopUp(hide[DOCK_LEFT], prect.Offseted(wrect.left + POPUP_SPACING + style->outersize, cp.y - psz.cy));	
		ShowPopUp(hide[DOCK_TOP], prect.Offseted(cp.x - psz.cx, wrect.top + POPUP_SPACING + style->outersize));	 
		ShowPopUp(hide[DOCK_RIGHT], prect.Offseted(wrect.right - POPUP_SPACING - psz.cx*2 - style->outersize, cp.y - psz.cy));
		ShowPopUp(hide[DOCK_BOTTOM], prect.Offseted(cp.x - psz.cx, wrect.bottom - POPUP_SPACING - psz.cy*2 - style->outersize));		
	}
}

void PopUpDockWindow::ShowInnerPopUps(DockCont& dc, DockCont *target)
{
	ASSERT(target);
	Rect wrect = GetScreenRect();
	Size psz(style->innersize, style->innersize);
	Rect prect = Rect(psz);	
	Point cp;
	psz /= 2;

	cp = target->GetScreenRect().CenterPoint();
	int d = psz.cy * 5;
	cp.x = minmax(cp.x, wrect.left + d, wrect.right - d);
	cp.y = minmax(cp.y, wrect.top + d, wrect.bottom - d);		

	int align = GetDockAlign(*target);
	if (IsTB(align)) { // Left/right docking allowed
		ShowPopUp(inner[DOCK_LEFT], prect.Offseted(cp.x - psz.cx*3, cp.y - psz.cy));	
		ShowPopUp(inner[DOCK_RIGHT], prect.Offseted(cp.x + psz.cx, cp.y - psz.cy));	
	}
	else {
		inner[DOCK_LEFT].Close();	
		inner[DOCK_RIGHT].Close();
	}
	if (!IsTB(align)) { // Top/bottom docking allowed
		ShowPopUp(inner[DOCK_TOP], prect.Offseted(cp.x - psz.cx, cp.y - psz.cy*3));
		ShowPopUp(inner[DOCK_BOTTOM], prect.Offseted(cp.x - psz.cx, cp.y + psz.cy));
	}
	else {
		inner[DOCK_TOP].Close();	
		inner[DOCK_BOTTOM].Close();
	}
	if (IsTabbing())
		ShowPopUp(inner[4], prect.Offseted(cp.x-psz.cx, cp.y-psz.cy));
}

void PopUpDockWindow::ShowPopUp(PopUpButton& pb, const Rect& r)
{
	pb.SetRect(r);
	if (!pb.IsPopUp())
		pb.PopUp(this, false, true, true);
}

void PopUpDockWindow::HidePopUps(bool _inner, bool _outer)
{
	if (_inner)
		for (int i = 0; i < 5; i++)
			inner[i].Close();	
	if (_outer) {
		for (int i = 0; i < 4; i++)
			outer[i].Close();
		for (int i = 0; i < 4; i++)
			hide[i].Close();
	}				
}

PopUpDockWindow&  PopUpDockWindow::SetStyle(const Style& s)
{
	style = &s;
	for (int i = 0; i < 5; i++)
		inner[i].icon = &s.inner[i];	
	for (int i = 0; i < 4; i++)
		outer[i].icon = &s.outer[i];
	for (int i = 0; i < 4; i++)
		hide[i].icon = &s.hide[i];
	return *this;	
}

PopUpDockWindow::PopUpDockWindow()
: showhide(true)
{
	SetStyle(StyleDefault());
	AnimateDelay(0);
}

CH_STYLE(PopUpDockWindow, Style, StyleDefault)
{
	inner[0] = DockingImg::DockLeft();
	inner[1] = DockingImg::DockTop();
	inner[2] = DockingImg::DockRight();
	inner[3] = DockingImg::DockBottom();
	inner[4] = DockingImg::DockTab();	
	
	outer[0] = DockingImg::DockLeft();
	outer[1] = DockingImg::DockTop();
	outer[2] = DockingImg::DockRight();
	outer[3] = DockingImg::DockBottom();

	hide[0] = DockingImg::HideLeft();
	hide[1] = DockingImg::HideTop();
	hide[2] = DockingImg::HideRight();
	hide[3] = DockingImg::HideBottom();

	highlight = DockingImg::DockHL();
	
	innersize = 25;
	outersize = 25;
}

}
