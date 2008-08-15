#include "DockCont.h"
#include "Docking.h"

// DockCont (Dockable Container)
#if	defined(PLATFORM_WIN32)
LRESULT DockCont::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCRBUTTONDOWN) {
		WindowMenu();
		return 1L;
	}
	if (message == WM_NCLBUTTONDOWN && IsFloating() && base)
		dragging = 1;
	if (message == WM_NCLBUTTONUP)
		dragging = 0;
	if (message == WM_MOVE && IsFloating() && base && GetMouseLeft() && dragging) {
		if (dragging == 1)	{
			MoveBegin();
			dragging++;
		}
		Moving();
		dragging = 2;
	}
	else if (message == WM_EXITSIZEMOVE && IsFloating() && base && !GetMouseLeft() && dragging) {
		MoveEnd();
		dragging = 0;
	}

	return TopWindow::WindowProc(message, wParam, lParam);
}

void DockCont::StartMouseDrag(const Point &p)
{
	SendMessage(GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(p.x, p.y));
}
#elif defined(PLATFORM_X11)
void DockCont::EventProc(XWindow& w, XEvent *event)
{

	if (IsOpen()) {
		switch(event->type) {
		case ConfigureNotify:{
				XConfigureEvent& e = event->xconfigure;
				if (Point(e.x, e.y) != GetScreenRect().TopLeft()) {
					if (!dragging)
						MoveBegin();
					Moving();
					SetFocus();
					dragging = true;
				}
			}
			break;
		case FocusIn: {
				XFocusChangeEvent &e = event->xfocus;
				if (e.mode == NotifyUngrab && dragging) {
					dragging = false;
					MoveEnd();
	//				SetFocus();
					return;
				}
				break;
			}
		}
	}
	TopWindow::EventProc(w, event);
}

void DockCont::StartMouseDrag(const Point &p)
{
	Atom xwndDrag = XAtom("_NET_WM_MOVERESIZE");
	XEvent e;
	Zero(e);
	e.xclient.type = ClientMessage;
	e.xclient.message_type = xwndDrag;
	e.xclient.window = GetWindow();
	e.xclient.format = 32;
	e.xclient.display = Xdisplay;
	e.xclient.send_event = XTrue;
	e.xclient.data.l[0] = p.x;
	e.xclient.data.l[1] = p.y;
	e.xclient.data.l[2] = 8;
	e.xclient.data.l[3] = 1;
	e.xclient.data.l[4] = 0;

	XUngrabPointer( Xdisplay, CurrentTime );
	XSendEvent(Xdisplay, RootWindow(Xdisplay, Xscreenno), XFalse, SubstructureNotifyMask, &e);
	XFlush(Xdisplay);
}
#endif

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

// Handle frame
int DockCont::Handle::GetHandleSize(const DockableCtrl::Style &s) const
{
	return 	(s.title_font.IsNull() ? 12 : s.title_font.GetHeight()+4)
			 + (s.handle_vert ? s.handle_margins.GetWidth() : s.handle_margins.GetHeight());
}

void DockCont::Handle::RefreshFocus(bool _focus)
{
	if (focus != _focus)
		{ focus = _focus; Refresh(); }
}

void DockCont::Handle::FrameLayout(Rect& r)
{
	if (!dc || !IsShown()) return;
	const DockableCtrl::Style &s = dc->GetStyle();
	if (s.handle_vert)
		LayoutFrameLeft(r, this, GetHandleSize(s));
	else
		LayoutFrameTop(r, this, GetHandleSize(s));
}

void DockCont::Handle::FrameAddSize(Size& sz)
{
	if (!dc || !IsShown()) return;
	const DockableCtrl::Style &s = dc->GetStyle();
	if (s.handle_vert)
		sz.cx += GetHandleSize(s);
	else
		sz.cy += GetHandleSize(s);
}

void DockCont::Handle::Paint(Draw& w)
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
		ChPaint(w, r, s.handle[focus]);

		Image img = dc->GetIcon();
		if (!img.IsEmpty()) {
			if (s.handle_vert) {
				int isz = r.GetWidth();
				p.y -= isz;
				isz -= (m.left + m.right);
				ChPaint(w, max(p.x+m.left, r.left), p.y, isz, isz, img);
				p.y -= 2;
			}
			else {
				int isz = r.GetHeight();
				isz -= (m.top + m.bottom);
				ChPaint(w, p.x, max(p.y, r.top), isz, isz, img);
				p.x += isz + 2;
			}
		}
		if (!s.title_font.IsNull()) {
			Ctrl *c = GetLastChild();
			while (c && !c->IsShown() && c->GetParent())
				c = c->GetNext();
			if (s.handle_vert)
				r.top = c ? c->GetRect().bottom + m.top : m.top;
			else
				r.right = c ? c->GetRect().left + m.right : m.right;
			w.Clip(r);
			WString text = IsNull(dc->GetGroup()) ? dc->GetTitle() : (WString)Format("%s (%s)", dc->GetTitle(), dc->GetGroup());
			w.DrawText(p.x, p.y, s.handle_vert ? 900 : 0, text, s.title_font, s.title_ink[focus]);
			w.End();
		}
	}
}

void DockCont::Layout()
{
	if (waitsync) {
		waitsync = false;
		if (GetCount() == 1) {
			Value v = tabbar.Get(0);
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
		tabbar.Insert(0, v, dc->GetGroup(), true);
	}
	else if (DockCont *dc = dynamic_cast<DockCont *>(child)) {
		Value v = ValueCast(dc);
		tabbar.Insert(0, v, Null, true);
	}
	else
		return;
	child->SizePos();
	TabSelected();
	if (GetCount() >= 2) RefreshLayout();
}

void 	DockCont::MoveBegin()		{ base->ContainerDragStart(*this); }
void 	DockCont::Moving()			{ base->ContainerDragMove(*this); }
void 	DockCont::MoveEnd()			{ base->ContainerDragEnd(*this); }

void DockCont::WindowMenu()
{
	MenuBar bar;
	DockContMenu menu(base);
	menu.ContainerMenu(bar, this, true);
	bar.Execute();
}

void DockCont::TabSelected()
{
	int ix = tabbar.GetCursor();
	if (ix >= 0) {
		DockableCtrl *dc = Get0(ix);
		if (!dc) return;
		Ctrl *ctrl = GetCtrl(ix);
		Ctrl *first = &handle;

		for (Ctrl *c = first->GetNext(); c; c = c->GetNext())
			if (c != ctrl) c->Hide();
		ctrl->Show();
		Icon(dc->GetIcon()).Title(dc->GetTitle());

		handle.dc = dc;
		SyncButtons(*dc);

		if (IsTabbed()) {
			DockCont *c = static_cast<DockCont *>(GetParent());
			c->tabbar.SyncRepos();
			c->TabSelected();
			c->RefreshFrame();
		}
		else
			RefreshLayout();
	}
}

void DockCont::TabDragged(int ix)
{
	if (ix >= 0) {
		// Special code needed
		Value v = tabbar.Get(ix);
		if (IsDockCont(v)) {
			DockCont *c = ContCast(v);
			c->Remove();
			base->FloatFromTab(*this, *c);
		}
		else {
			DockableCtrl *c = DockCast(v);
			c->Remove();
			base->FloatFromTab(*this, *c);
		}
		if (tabbar.IsAutoHide())
			RefreshLayout();
	}
}

void DockCont::TabContext(int ix)
{
	MenuBar bar;
	DockContMenu 	menu(base);
	DockMenu 		tabmenu(base);
	Value v = tabbar.Get(ix);
	if (IsDockCont(v))
		menu.ContainerMenu(bar, ContCast(v), false);
	else
		tabmenu.WindowMenuNoClose(bar, DockCast(v));
	bar.Separator();
	tabbar.ContextMenu(bar);
	bar.Execute();
}

void DockCont::TabClosed(Value v)
{
	CtrlCast(v)->Remove();
	if (IsDockCont(v)) base->CloseContainer(*ContCast(v));
	waitsync = true;
	Layout();
	if (tabbar.GetCount() == 1)
		RefreshLayout();
}

void DockCont::CloseAll()
{
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

void DockCont::AutoHideAlign(int align)
{
	base->AutoHideContainer((align == DockWindow::DOCK_NONE) ? DockWindow::DOCK_TOP : align, *this);
}

void DockCont::AddFrom(DockCont &cont, int except)
{
	bool all = except < 0 || except >= cont.GetCount();
	for (int i = cont.GetCount() - 1; i >= 0; i--)
		if (i != except) {
			Ctrl *c = cont.GetCtrl(i);
			c->Remove();
			Add(*c);
		}
	if (all)
		cont.tabbar.Clear();
}

void DockCont::Clear()
{
	for (int i = 0; i < tabbar.GetCount(); i++)
		CtrlCast(tabbar.Get(i))->Close();
	tabbar.Clear();
	handle.dc = NULL;
}

void DockCont::StateDocked(DockWindow &dock)
{
	base = &dock;
	dockstate = STATE_DOCKED;
	handle.Show();
	Show();
}

void DockCont::StateFloating(DockWindow &dock)
{
	base = &dock;
	dockstate = STATE_FLOATING;
	handle.Hide();
	Show();
}

void DockCont::SyncButtons(DockableCtrl &dc)
{
	if (base) {
		Ctrl::LogPos lp;
		const DockableCtrl::Style &s = dc.GetStyle();
		int btnsize = handle.GetHandleSize(s) - 3;

		Logc &inc = s.handle_vert ? lp.y : lp.x;
		lp.x = s.handle_vert ? Ctrl::Logc(ALIGN_LEFT, 1, btnsize) : Ctrl::Logc(ALIGN_RIGHT, 1, btnsize);
		lp.y = Ctrl::Logc(ALIGN_TOP, 1, btnsize);

		if (close.GetParent()) {
			close.SetLook(s.close).SetPos(lp).Show();
			inc.SetA(inc.GetA() + btnsize + 1);
		}
		bool ah = base->IsAutoHide();
		autohide.Show(ah);
		if (ah && autohide.GetParent()) {
			autohide.SetLook(s.autohide).SetPos(lp);
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

void DockCont::GroupRefresh()
{
	for (int i = 0; i < tabbar.GetCount(); i++)
		if (!IsDockCont(tabbar.Get(i)))
			tabbar.SetTabGroup(i, DockCast(tabbar.Get(i))->GetGroup());
	Refresh();
}

void DockCont::GetGroups(Vector<String> &groups)
{
	for (int i = 0; i < tabbar.GetCount(); i++) {
		Value v = tabbar.Get(i);
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

void DockCont::WindowButtons(bool menu, bool hide, bool _close)
{
	AddRemoveButton(windowpos, menu);
	AddRemoveButton(autohide, hide);
	AddRemoveButton(close, _close);
	SyncButtons();
}

void DockCont::AddRemoveButton(Ctrl &c, bool state)
{
	if (state && !c.GetParent())
		Add(c);
	else if (!state)
		c.Remove();
}

void DockCont::Highlight()
{
	if (!GetCount() || (!IsOpen() && !IsPopUp() && !GetParent())) return;
	ViewDraw v(this);
	ChPaint(v, GetSize(), GetCurrent().GetStyle().highlight);
}

Image DockCont::GetHighlightImage()
{
	Ctrl *ctrl = GetCtrl(GetCursor());
	ImageDraw img(ctrl->GetRect().GetSize());
	ctrl->DrawCtrlWithParent(img);
	return img;
}

Size DockCont::GetMinSize() const
{
	if (ignoreminsize) return Size(0, 0);
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

bool DockCont::IsDockAllowed(int align, int dc_ix) const
{
	if (dc_ix >= 0) return IsDockAllowed0(align, tabbar.Get(dc_ix));
	else if (!base->IsDockAllowed(align)) return false;

	for (int i = 0; i < tabbar.GetCount(); i++)
		if (!IsDockAllowed0(align, tabbar.Get(i))) return false;
	return true;
}

bool DockCont::IsDockAllowed0(int align, const Value &v) const
{
	return IsDockCont(v) ? ContCast(v)->IsDockAllowed(align, -1) : base->IsDockAllowed(align, *DockCast(v));
}

DockableCtrl * DockCont::Get0(int ix) const
{
	if (ix < 0 || ix > tabbar.GetCount()) return NULL;
	Value v = tabbar.Get(ix);
	return IsDockCont(v) ? ContCast(v)->GetCurrent0() : DockCast(v);
}

WString DockCont::GetTitle(bool force_count) const
{
	if ((IsTabbed() || force_count) && tabbar.GetCount() > 1)
		return (WString)Format("%s (%d/%d)", GetCurrent().GetTitle(), tabbar.GetCursor()+1, tabbar.GetCount());
	return GetCurrent().GetTitle();
}

void DockCont::Serialize(Stream& s)
{
	int ctrl = 'D';
	int cont = 'C';
	const Vector<DockableCtrl *> &dcs = base->GetDockableCtrls();

	if (s.IsStoring()) {
		if (GetCount() == 1 && IsDockCont(tabbar.Get(0)))
			return ContCast(tabbar.Get(0))->Serialize(s);

		int cnt = GetCount();
		s / cont / cnt;
		for (int i = GetCount() - 1; i >= 0; i--) {
			if (IsDockCont(tabbar.Get(i)))
				ContCast(tabbar.Get(i))->Serialize(s);
			else {
				DockableCtrl *dc = DockCast(tabbar.Get(i));
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
				if (ix >= 0 && ix <= dcs.GetCount())
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

void DockCont::DockContMenu::ContainerMenu(Bar &bar, DockCont *c, bool withgroups)
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

DockCont::DockCont()
{
	dragging = false;
	dockstate = STATE_NONE;
	base = NULL;
	waitsync = false;
	ignoreminsize = false;
	usersize.cx = usersize.cy = Null;
	BackPaint();
#ifdef PLATFORM_WIN32
	ToolWindow();
#endif
	NoCenter().Sizeable(true).MaximizeBox(false).MinimizeBox(false);

	AddFrame(FieldFrame());
	AddFrame(tabbar);
	AddFrame(handle);
	tabbar.AutoHideMin(1);
	tabbar.WhenCursor 		= THISBACK(TabSelected);
	tabbar.WhenDrag 		= THISBACK(TabDragged);
	tabbar.WhenContext 		= THISBACK(TabContext);
	tabbar.WhenClose 		= THISBACK(TabClosed);
	tabbar.WhenCloseAll		= THISBACK(RefreshLayout);
	tabbar.SetBottom();

	handle << close << autohide << windowpos;
	handle.WhenContext = THISBACK(WindowMenu);
	handle.WhenLeftDrag = THISBACK(MoveBegin);
	close.Tip(t_("Close")) 				<<= THISBACK(CloseAll);
	autohide.Tip(t_("Auto-Hide")) 		<<= THISBACK(AutoHide);
	windowpos.Tip(t_("Window Menu")) 	<<= THISBACK(WindowMenu);
	WhenClose 							= THISBACK(CloseAll);
}