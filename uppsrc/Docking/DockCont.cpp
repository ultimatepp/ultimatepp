#include "Docking.h"

namespace Upp {

// ImgButton
void ImgButton::Paint(Draw &w)
{
	Size sz = GetSize();
	if (look)
		ChPaint(w, sz, look[Pusher::GetVisualState()]);
	int dx = IsPush() * -1;
	int dy = IsPush();
	if (!img.IsEmpty()) {
		Size isz = img.GetSize();
		w.DrawImage((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy, img);
	}
}

// DockContHandle frame
int DockCont::DockContHandle::GetHandleSize(const DockableCtrl::Style &s) const
{
	return 	(IsNull(s.title_font) ? DPI(12) : s.title_font.GetCy() + DPI(2))
			 + (s.handle_vert ? s.handle_margins.GetWidth() : s.handle_margins.GetHeight()); 
}
/*
void DockCont::DockContHandle::RefreshFocus(bool _focus)
{
	if (focus != _focus) 	
		{ focus = _focus; Refresh(); }
}
*/
void DockCont::DockContHandle::FrameLayout(Rect& r)
{
	if (!dc || !IsShown()) return;
	const DockableCtrl::Style &s = dc->GetStyle();
	if (s.handle_vert)
		LayoutFrameLeft(r, this, GetHandleSize(s));
	else
		LayoutFrameTop(r, this, GetHandleSize(s));
}

void DockCont::DockContHandle::FrameAddSize(Size& sz)
{
	if (!dc || !IsShown()) return;
	const DockableCtrl::Style &s = dc->GetStyle();
	if (s.handle_vert)
		sz.cx += GetHandleSize(s);
	else
		sz.cy += GetHandleSize(s);
}

void DockCont::DockContHandle::Paint(Draw& w)
{
	if (IsShown() && dc) {
		const DockableCtrl::Style &s = dc->GetStyle();
		Rect r = GetSize();
		const Rect &m = s.handle_margins;
		Point p;
		
		if (s.handle_vert)
			p = Point(r.left-1 + m.left, r.bottom - m.bottom);
		else
			p = Point(r.left + m.left, r.top + m.top);
		ChPaint(w, r, s.handle[0]);
		
		Image img = dc->GetIcon();
		if (!img.IsEmpty()) {
			if (s.handle_vert) {
				int isz = r.GetWidth();
				p.y -= isz;
				isz -= 2;
				img = CachedRescale(img, Size(isz, isz));
				w.DrawImage(1, max(p.y, r.top), img);
				p.y -= 2;
			}
			else {
				int isz = r.GetHeight() - 2;
				img = CachedRescale(img, Size(isz, isz));
				w.DrawImage(p.x, 1, img);
				p.x += isz + 2;
			}
		}
		if (!IsNull(s.title_font)) {		
			Ctrl *c = GetLastChild();
			while (c && !c->IsShown() && c->GetParent())
				c = c->GetPrev();
			if (s.handle_vert)
				r.top = c ? c->GetRect().bottom - m.top : r.top - m.top;
			else
				r.right = c ? c->GetRect().left - m.right : r.right - m.right;
			w.Clip(r);
			WString text = IsNull(dc->GetGroup()) ? dc->GetTitle() : (WString)Format("%s (%s)", dc->GetTitle(), dc->GetGroup());
			w.DrawText(p.x, p.y, s.handle_vert ? 900 : 0, text, s.title_font, s.title_ink[0/*focus*/]);
			w.End();
		}
	}	
}

void DockCont::Layout()
{
	if (waitsync) {
		waitsync = false;
		if (GetCount() == 1) {
			Value v = tabbar.GetKey(0);
			if (IsDockCont(v)) {
				DockCont *dc = ContCast(v);
				AddFrom(*dc);
				base->CloseContainer(*dc);
				RefreshLayout();
			}
		}
		if (!tabbar.GetCount())
			base->CloseContainer(*this);
		TabSelected();
	}
}

void DockCont::ChildRemoved(Ctrl *child)
{
	if (child->GetParent() != this || !tabbar.GetCount()) return;
	Ctrl *c = dynamic_cast<DockableCtrl *>(child);
	if (!c) c = dynamic_cast<DockCont *>(child);
	if (c)
		for (int i = 0; i < GetCount(); i++)
			if (c == GetCtrl(i)) {
				tabbar.Close(i);
				waitsync = true;
				break;
			}
}

void DockCont::ChildAdded(Ctrl *child)
{
	if (child->GetParent() != this) 
		return;
	else if (DockableCtrl *dc = dynamic_cast<DockableCtrl *>(child)) {
		Value v = ValueCast(dc);
		tabbar.InsertKey(0, v, dc->GetTitle(), dc->GetIcon(), dc->GetGroup(), true);
	}	
	else if (DockCont *dc = dynamic_cast<DockCont *>(child)) {
		Value v = ValueCast(dc);
		tabbar.InsertKey(0, v, dc->GetTitle(), Null, Null, true);
	}	
	else 
		return;	
	child->SizePos();
	TabSelected();	
	if (GetCount() >= 2) RefreshLayout();
}

bool DockCont::Key(dword key, int count)
{
	if (!IsDocked())
		PostCallback(callback1(base, &DockWindow::DoHotKeys, key));
	return !IsDocked();
}

// The use of Single<> here is bad form, but I am unable to declare it a 
//  member in the header (dependencies) and I can't think of a better way
void DockCont::MoveBegin() 
{ 
	static bool in_move = false;
	if (!in_move && !base->IsLocked()) { 
		in_move = true;	
		base->SaveDockerPos(GetCurrent(), Single<DockWindow::PosInfo>()); 
		base->ContainerDragStart(*this); 
		in_move = false;
	} 
}

void DockCont::Moving() { 
	if (!base->IsLocked()) base->ContainerDragMove(*this); 
}

void DockCont::MoveEnd() { 
	if (!base->IsLocked()) { 
		SetAllDockerPos(); 
		base->ContainerDragEnd(*this); 
	}	
}

void DockCont::WindowMenu()
{
	if (!base->IsLocked()) {
		MenuBar bar;
		DockContMenu menu(base);
		menu.ContainerMenu(bar, this, true);
		bar.Execute();
	}
}

void DockCont::Animate(Rect target, int ticks, int interval)
{
	if (!IsOpen() || GetParent()) return;
	animating = true;	
	Rect dr = GetRect();
	target -= dr;
#ifdef PLATFORM_WIN32
	target.top += 16; // Fudge for titlebar. Should get from OS?
#endif
	target.left /= ticks;
	target.right /= ticks;
	target.top /= ticks;
	target.bottom /= ticks;
	for (int i = 0; i < ticks; i++) {
		dr += target;
		SetRect(dr);
		Sync();
		ProcessEvents();
		Sleep(interval);
	}	
	animating = false;	
}

void DockCont::TabSelected() 
{
	int ix = tabbar.GetCursor();
	if (ix >= 0) {
		DockableCtrl *dc = Get0(ix);
		if (!dc) return;
		
		Ctrl *ctrl = GetCtrl(ix);
		for (Ctrl *c = FindFirstChild(); c; c = c->GetNext())
			if (c != ctrl && !c->InFrame()) 
				c->Hide();
		ctrl->Show();

		handle.dc = dc;
		Icon(dc->GetIcon()).Title(dc->GetTitle());

		SyncButtons(*dc);

		if (IsTabbed()) {
			DockCont *c = static_cast<DockCont *>(GetParent());
			c->ChildTitleChanged(*this);
			c->TabSelected();
		}
		else {
			handle.Refresh();
			RefreshLayout();
		}
	}
}

void DockCont::TabDragged(int ix) 
{
	if (base->IsLocked())
		return;
	if (ix >= 0) {
		// Special code needed
		Value v = tabbar.GetKey(ix);
		if (IsDockCont(v)) {
			DockCont *c = ContCast(v);
			c->Remove();
			base->FloatFromTab(*this, *c);
		}
		else {
			DockableCtrl *c = DockCast(v);
			base->SaveDockerPos(*c);
			c->Remove();
			base->FloatFromTab(*this, *c);
		}
		if (tabbar.IsAutoHide()) 
			RefreshLayout();		
	}
}

void DockCont::TabContext(int ix)
{
	MenuBar 		bar;
	DockContMenu 	menu(base);
	DockMenu 		tabmenu(base);
	if (ix >= 0) {
		Value v = tabbar.GetKey(ix);
		if (IsDockCont(v))
			menu.ContainerMenu(bar, ContCast(v), false);
		else
			tabmenu.WindowMenuNoClose(bar, DockCast(v));
	}
	if(base->HasCloseButtons()) {
		if (!bar.IsEmpty())
			bar.Separator();
		tabbar.ContextMenu(bar);
	}
	bar.Execute();
}

void DockCont::TabClosed0(Value v)
{
	if (IsDockCont(v)) {
		DockCont *c = ContCast(v);
		c->Remove();
		base->CloseContainer(*c);
	}
	else {
		DockableCtrl *c = DockCast(v);
		base->SaveDockerPos(*c);
		c->Remove();
		c->WhenState();		
	}
	waitsync = true;
}

void DockCont::TabClosed(Value v)
{
	TabClosed0(v);
	Layout();
	if (tabbar.GetCount() == 1) 
		RefreshLayout();
}

void DockCont::TabsClosed(ValueArray vv)
{
	for (int i = vv.GetCount()-1; i >= 0 ; --i)
		TabClosed0(vv[i]);
	Layout();
	if (tabbar.GetCount() == 1) 
		RefreshLayout();	
}

void DockCont::SortTabs(bool b)
{
	tabbar.SortTabs(b);
}

void DockCont::SortTabs(ValueOrder &sorter)
{
	tabbar.SortTabValues(sorter);
}

void DockCont::SortTabsOnce()
{
	tabbar.SortTabsOnce();
}

void DockCont::SortTabsOnce(ValueOrder &sorter)
{
	tabbar.SortTabValuesOnce(sorter);
}

void DockCont::CloseAll()
{
	if(!base->HasCloseButtons()) return;
	base->CloseContainer(*this);
}

void DockCont::Float()
{
	base->FloatContainer(*this);
}

void DockCont::Dock(int align)
{
	base->DockContainer(align, *this);
}

void DockCont::AutoHide()
{
	AutoHideAlign(IsAutoHide() ? GetAutoHideAlign() : GetDockAlign());
}

void DockCont::AutoHideAlign(int align)
{
	ASSERT(base);
	if (IsAutoHide()) {
		if (align == DockWindow::DOCK_NONE)
			base->FloatContainer(*this, Null, false);
		else			
			base->DockContainer(align, *this);	
	}
	else	
		base->AutoHideContainer((align == DockWindow::DOCK_NONE) ? DockWindow::DOCK_TOP : align, *this);
}

void DockCont::RestoreCurrent()
{
	base->RestoreDockerPos(GetCurrent(), true);
}

void DockCont::AddFrom(DockCont& cont, int except)
{
	bool all = except < 0 || except >= cont.GetCount();
	int ix = GetCount();
	for (int i = cont.GetCount() - 1; i >= 0; i--)
		if (i != except) {
			Ctrl *c = cont.GetCtrl(i);
			c->Remove();
			Add(*c);				
		}		
	if (all)
		cont.tabbar.Clear();
	SignalStateChange(ix, GetCount()-1);
}

void DockCont::Clear()
{
	if (tabbar.GetCount()) {
		// Copy tabbar values and clear to prevent this being called multple times if child 
		//  if destroyed on closing. pfft
		Vector<Value> v;
		v.SetCount(tabbar.GetCount());
		for (int i = 0; i < tabbar.GetCount(); i++)
			v[i] = tabbar.GetKey(i);
		tabbar.Clear();
		// Remove ctrls and signal correct state change
		for (int i = 0; i < v.GetCount(); i++) {
			if (IsDockCont(v[i])) { 
				DockCont *dc = ContCast(v[i]);
				dc->Remove();
				dc->State(*base, STATE_NONE);
			}
			else {
				DockableCtrl *dc = DockCast(v[i]);
				dc->Remove();
				dc->WhenState();
			}
		}		

	}
	handle.dc = NULL;
}

void DockCont::State(DockWindow& dock, DockCont::DockState state)
{
	base = &dock;
	dockstate = state;	
	SyncFrames();
	SyncButtons();
	Show(); 
	SignalStateChange();
}

void DockCont::SignalStateChange()
{
	if (GetCount())
		SignalStateChange(GetCount()-1, 0);
}

void DockCont::SignalStateChange(int from, int to)
{
	for (int i = to; i >= from; i--) {
		if (IsDockCont(tabbar.GetKey(i)))
			ContCast(tabbar.GetKey(i))->SignalStateChange();
		else
			DockCast(tabbar.GetKey(i))->WhenState();
	}
}

void DockCont::SyncButtons(DockableCtrl& dc)
{
//	if (!handle.IsShown()) return;
	if (base) {
		Ctrl::LogPos lp;
		const DockableCtrl::Style& s = dc.GetStyle();
		int btnsize = handle.GetHandleSize(s) - 3;
		
		Logc& inc = s.handle_vert ? lp.y : lp.x;
		lp.x = s.handle_vert ? Ctrl::Logc(ALIGN_LEFT, 1, btnsize) : Ctrl::Logc(ALIGN_RIGHT, 1, btnsize);
		lp.y = Ctrl::Logc(ALIGN_TOP, 1, btnsize);		
		
		if (close.GetParent()) {
			close.SetLook(s.close).SetPos(lp).Show();
			inc.SetA(inc.GetA() + btnsize + 1);
		}
		bool ah = base->IsAutoHide();
		autohide.Show(ah);
		if (ah && autohide.GetParent()) {
			autohide.SetLook(IsAutoHide() ? s.pin : s.autohide).SetPos(lp);
			inc.SetA(inc.GetA() + btnsize + 1);		
		}
		if (windowpos.GetParent())
			windowpos.SetLook(s.windowpos).SetPos(lp).Show();
	}
	else {
		close.Hide();
		autohide.Hide();
		windowpos.Hide();
	}
}

void DockCont::SyncTabs(int align, bool text)
{
	tabbar.ShowText(text);
	if (align != tabbar.GetAlign())
		tabbar.SetAlign(align);
}

Ctrl * DockCont::FindFirstChild() const
{
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext())
		if (!c->InFrame()) return c;
	return NULL;
}

void DockCont::SetCursor(Ctrl& c)
{
	for (int i = 0; i < GetCount(); i++) 
		if (GetCtrl(i) == &c)
			return SetCursor(i);
}

void DockCont::GroupRefresh()
{
	for (int i = 0; i < tabbar.GetCount(); i++)
		if (!IsDockCont(tabbar.GetKey(i)))
			tabbar.SetTabGroup(i, DockCast(tabbar.GetKey(i))->GetGroup());
	Refresh();
}

void DockCont::GetGroups(Vector<String>& groups)
{
	for (int i = 0; i < tabbar.GetCount(); i++) {
		Value v = tabbar.GetKey(i);
		if (IsDockCont(v))
			ContCast(v)->GetGroups(groups);
		else {
			DockableCtrl *dc = DockCast(v);
			String g = dc->GetGroup();
			if (!g.IsEmpty()) {
				bool found = false;
				for (int j = 0;	j < groups.GetCount(); j++)
					if (groups[j] == g) {
						found = true;
						break;
					}
				if (!found)
					groups.Add(g);
			}				
		}
	}
}

void DockCont::SetAllDockerPos()
{
	DockWindow::PosInfo pi;
	bool posset = false;
	for (int i = 0; i < GetCount(); i++) {
		Value v = tabbar.GetKey(i);
		if (IsDockCont(v))
			ContCast(v)->SetAllDockerPos();
		else {
			DockableCtrl &dc = *DockCast(v);
			if (!posset)
				base->SaveDockerPos(dc, pi);
			base->SetDockerPosInfo(dc, pi);
		}
	}
}

void DockCont::WindowButtons(bool menu, bool hide, bool _close)
{
	bool locked = base->IsLocked();
	AddRemoveButton(windowpos, menu && !locked);
	AddRemoveButton(autohide, hide && !locked);
	AddRemoveButton(close, _close && !locked);
	tabbar.Crosses(_close && !locked);
	NoCloseBox((!_close) || locked);
	SyncButtons();
}

void DockCont::AddRemoveButton(Ctrl& c, bool state)
{
	if (state && !c.GetParent()) 
		handle.Add(c); 
	else if (!state) 
		c.Remove();	
}

void DockCont::Highlight()
{
	if (!GetCount() || (!IsOpen() && !IsPopUp() && !GetParent())) return;
	ViewDraw v(this); 
	ChPaint(v, GetSize(), GetCurrent().GetStyle().highlight[1]);
}

Image DockCont::GetHighlightImage()
{
	Ctrl *ctrl = GetCtrl(GetCursor());
	if (!ctrl) return Image();
	Size sz = ctrl->GetRect().GetSize();
	if (tabbar.IsAutoHide()) {
		if (tabbar.IsVert())
			sz.cx -= tabbar.GetFrameSize();
		else
			sz.cy -= tabbar.GetFrameSize();
	}
	ImageDraw img(sz);
	ctrl->DrawCtrlWithParent(img);
	return img;
}

Size DockCont::GetMinSize() const
{ 
	if (animating) return Size(0, 0);
	Size sz = tabbar.GetCount() ? GetCurrent().GetMinSize() : Size(0, 0); 
	sz = AddFrameSize(sz);
	return sz;
}

Size DockCont::GetMaxSize() const	
{ 
	Size sz = tabbar.GetCount() ? GetCurrent().GetMaxSize() : Size(0, 0);
	return AddFrameSize(sz);
}

Size DockCont::GetStdSize() const
{
	Size sz = usersize;
	if (IsNull(sz.cx) || IsNull(sz.cy)) {
		Size std = GetCurrent().GetStdSize();
		if (IsNull(sz.cx)) sz.cx = std.cx;
		if (IsNull(sz.cy)) sz.cy = std.cy;
	}
	return AddFrameSize(sz);
}

void DockCont::SyncUserSize(bool h, bool v)
{
	Size sz = GetSize();
	if (h) usersize.cx = sz.cx;
	if (v) usersize.cy = sz.cy;
}

int DockCont::GetDockAlign() const
{
	return base->GetDockAlign(*this); 	
}

int DockCont::GetAutoHideAlign() const
{
	return base->GetAutoHideAlign(*this); 		
}

bool DockCont::IsDockAllowed(int align, int dc_ix) const
{
	if (dc_ix >= 0) return IsDockAllowed0(align, tabbar.GetKey(dc_ix));
	else if (!base->IsDockAllowed(align)) return false;
	
	for (int i = 0; i < tabbar.GetCount(); i++)
		if (!IsDockAllowed0(align, tabbar.GetKey(i))) return false;
	return true;
}

bool DockCont::IsDockAllowed0(int align, const Value& v) const
{
	return IsDockCont(v) ? ContCast(v)->IsDockAllowed(align, -1) : base->IsDockAllowed(align, *DockCast(v));
}

DockableCtrl * DockCont::Get0(int ix) const
{ 
	if (ix < 0 || ix > tabbar.GetCount()) return NULL;
	Value v = tabbar.GetKey(ix); 
	return IsDockCont(v) ? ContCast(v)->GetCurrent0() : DockCast(v); 
}

WString DockCont::GetTitle(bool force_count) const
{
	if ((IsTabbed() || force_count) && tabbar.GetCount() > 1) 
		return (WString)Format("%s (%d/%d)", GetCurrent().GetTitle(), tabbar.GetCursor()+1, tabbar.GetCount()); 
	return GetCurrent().GetTitle();	
}

void DockCont::ChildTitleChanged(DockableCtrl &dc)
{
	ChildTitleChanged(&dc, dc.GetTitle(), dc.GetIcon());
}

void DockCont::ChildTitleChanged(DockCont &dc)
{
	ChildTitleChanged(&dc, dc.GetTitle(), Null);
}

void DockCont::ChildTitleChanged(Ctrl *child, WString title, Image icon)
{
	for (int i = 0; i < tabbar.GetCount(); i++)
		if (CtrlCast(tabbar.GetKey(i)) == child) {
		    tabbar.Set(i, tabbar.GetKey(i), title, icon);
		    break;
		}
	if (!GetParent()) 
		Title(GetTitle());
	RefreshFrame();
}

void DockCont::Serialize(Stream& s)
{
	int ctrl = 'D';
	int cont = 'C';
	const Vector<DockableCtrl *>& dcs = base->GetDockableCtrls();
	
	if (s.IsStoring()) {		
		if (GetCount() == 1 && IsDockCont(tabbar.GetKey(0)))
			return ContCast(tabbar.GetKey(0))->Serialize(s);

		int cnt = GetCount();
		s / cont / cnt;
		for (int i = GetCount() - 1; i >= 0; i--) {
			if (IsDockCont(tabbar.GetKey(i)))
				ContCast(tabbar.GetKey(i))->Serialize(s);
			else {
				DockableCtrl *dc = DockCast(tabbar.GetKey(i));
				int ix = base->FindDocker(dc);
				s / ctrl / ix;					
			}									
		}
		int cursor = tabbar.GetCursor();
		int groupix = tabbar.GetGroup();
		s / cursor / groupix;
	}
	else {
		int cnt;
		int type;
		
		s / type / cnt;
		ASSERT(type == cont);
		for (int i = 0; i < cnt; i++) {
			int64 pos = s.GetPos();
			s / type;
			if (type == cont) {
				s.Seek(pos);
				DockCont *dc = base->CreateContainer();
				dc->Serialize(s);
				base->DockContainerAsTab(*this, *dc, true);
			}
			else if (type == ctrl) {
				int ix;
				s / ix;
				if (ix >= 0 && ix < dcs.GetCount())
					Add(*dcs[ix]);
			}
			else
				ASSERT(false);								
		}
		int cursor, groupix;
		s / cursor / groupix;
		tabbar.SetGroup(groupix);
		tabbar.SetCursor(min(tabbar.GetCount()-1, cursor));
		TabSelected();
	}	
}
void DockCont::DockContMenu::ContainerMenu(Bar& bar, DockCont *c, bool withgroups)
{
	DockableCtrl *dc = &c->GetCurrent();
	cont = c;
		
	// TODO: Need correct group filtering
	withgroups = false;
	
	// If grouping, find all groups
	DockMenu::WindowMenu(bar, dc);	
	if (withgroups && dock->IsGrouping()) {
		Vector<String> groups;
		cont->GetGroups(groups);
		if (groups.GetCount()) {
			bar.Separator();
			Sort(groups);
			for (int i = 0; i < groups.GetCount(); i++)
				bar.Add(groups[i], THISBACK1(GroupWindowsMenu, groups[i]));
			bar.Add(t_("All"), THISBACK1(GroupWindowsMenu, String(Null)));	
		}
	}
}

void DockCont::DockContMenu::MenuDock(int align, DockableCtrl *dc)
{
	cont->Dock(align);
}

void DockCont::DockContMenu::MenuFloat(DockableCtrl *dc)
{
	cont->Float();
}

void DockCont::DockContMenu::MenuAutoHide(int align, DockableCtrl *dc)
{
	cont->AutoHideAlign(align);
}

void DockCont::DockContMenu::MenuClose(DockableCtrl *dc)
{
	cont->CloseAll();
}

void DockCont::Lock(bool lock)
{
	tabbar.Crosses(!lock && base && base->HasCloseButtons());
	tabbar.WhenDrag 		= lock ? Callback1<int>() : THISBACK(TabDragged);
	tabbar.WhenContext 		= lock ? Callback1<int>() : THISBACK(TabContext);	
	SyncFrames(lock && !base->IsShowingLockedHandles());
	RefreshLayout();
}

void DockCont::SyncFrames()
{
	SyncFrames(base->IsLocked() && !base->IsShowingLockedHandles());
}

void DockCont::SyncFrames(bool hidehandle)
{
	bool frames = !hidehandle && (IsDocked() || IsAutoHide());
	handle.Show(frames);
	if (frames)
		SetFrame(0, Single<DockCont::DockContFrame>());
	else 
		SetFrame(0, NullFrame());
}

DockCont::DockCont()
{
	dockstate = STATE_NONE;
	base = NULL;
	waitsync = false;	
	animating = false;
	usersize.cx = usersize.cy = Null;
	NoCenter();

	tabbar.SortTabValues(Single<StdValueOrder>());
	tabbar.SortTabs(false);

	tabbar.AutoHideMin(1);
	tabbar<<= THISBACK(TabSelected);
	tabbar.WhenClose = THISBACK(TabClosed);
	tabbar.WhenCloseSome = THISBACK(TabsClosed);
	tabbar.SetBottom();	

	WhenClose = THISBACK(CloseAll);
	
	handle << close << autohide << windowpos;
	handle.WhenContext = THISBACK(WindowMenu);
	handle.WhenLeftDrag = THISBACK(MoveBegin);
	handle.WhenLeftDouble = THISBACK(RestoreCurrent);
	close.Tip(t_("Close")) 				<<= THISBACK(CloseAll);
	autohide.Tip(t_("Auto-Hide")) 		<<= THISBACK(AutoHide);
	windowpos.Tip(t_("Window Menu")) 	<<= THISBACK(WindowMenu);		
	
	AddFrame(NullFrame());
	AddFrame(tabbar);
	AddFrame(handle);

	Lock(false);	
}

}
