#include "CtrlCore.h"

#ifdef PLATFORM_X11
#include <X11/Xlocale.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_X11

#ifdef _DEBUG


bool Ctrl::LogMessages
// = true
;
#endif

#define LLOG(x)      // DLOG(x)
#define LTIMING(x)   // RTIMING(x)
#define LDUMP(x)     // DDUMP(x)
#define LDUMPC(x)    // DDUMPC(x)

// #define SYNCHRONIZE

#define x_Event(x) { x, #x },

static
struct XEventMap {
	int         ID;
	const char *name;
}
sXevent[] = {
#include "X11Event.i"
	{ 0, NULL }
};

ArrayMap<Window, Ctrl::XWindow>& Ctrl::Xwindow()
{
	return Single< ArrayMap<Window, XWindow> >();
}

int       Ctrl::WndCaretTime;
bool      Ctrl::WndCaretVisible;
int       Ctrl::Xbuttons;
Window    Ctrl::grabWindow, Ctrl::focusWindow;
int       Ctrl::Xeventtime;

int       Ctrl::PopupGrab;
Ptr<Ctrl> Ctrl::popupWnd;

Point     Ctrl::mousePos;

static int s_starttime;

void Ctrl::DoPaint(const Vector<Rect>& invalid)
{
	if(IsVisible()) {
		LTIMING("DoPaint");
		fullrefresh = false;
//		if(GLX) return;
		GC gc = XCreateGC(Xdisplay, (Drawable)top->window, 0, 0);
		XftDraw *xftdraw = XftDrawCreate(Xdisplay, (Drawable) top->window,
		                                 DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
		Draw draw(top->window, gc, xftdraw, invalid);
		UpdateArea(draw, draw.GetClip());
		XftDrawDestroy(xftdraw);
		XFreeGC(Xdisplay, gc);
	}
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	if(r.IsEmpty() || !GetWindow()) return;
	int cx = r.Width() - abs(dx);
	int cy = r.Height() - abs(dy);
	GC gc = XCreateGC(Xdisplay, GetWindow(), 0, 0);
	XCopyArea(Xdisplay, GetWindow(), GetWindow(), gc,
	          r.left + max(-dx, 0), r.top + max(-dy, 0), cx, cy,
	          r.left + max(dx, 0), r.top + max(dy, 0));
	XFreeGC(Xdisplay, gc);
	XWindow *xw = GetXWindow();
	Vector<Rect> ur;
	if(xw)
		for(int i = 0; i < xw->invalid.GetCount(); i++)
			if(xw->invalid[i].Intersects(r))
				ur.Add(xw->invalid[i]);
	for(int i = 0; i < ur.GetCount(); i++)
		Invalidate(*xw, ur[i].Offseted(dx, dy));
}

bool Ctrl::IsWaitingEvent()
{
	return XPending(Xdisplay);
}

Ctrl *Ctrl::CtrlFromWindow(Window w)
{
	int q = Xwindow().Find(w);
	return q >= 0 ? Xwindow()[q].ctrl : NULL;
}

Ctrl::XWindow *Ctrl::GetXWindow()
{
	if(!top) return NULL;
	int q = Xwindow().Find(top->window);
	return q >= 0 ? &Xwindow()[q] : NULL;
}
// 01/12/2007 - mdelfede
// added support for windowed controls

// Gets handle of window containing control
Window Ctrl::GetParentWindow(void) const
{
	Ctrl const *q = GetParentWindowCtrl();
	if(q)
		return q->top->window;
	else
		return 0;

} // END Ctrl::GetParentWindow()

// Get control with parentwindow as handle
Ctrl *Ctrl::GetParentWindowCtrl(void) const
{
	Ctrl *q = parent;
	while(q && !q->top)
		q = q->parent;
	return q;

} // END Ctrl::GetParentWindowCtrl()

// Gets the rect inside the parent window
Rect Ctrl::GetRectInParentWindow(void) const
{
	Rect r = GetRect();
	Ctrl *q = parent;
	while(q)
  	{
		if(q->top)
			break;
		r += q->GetRect().TopLeft() + q->GetView().TopLeft();
		q = q->parent;
	}
	if(q)
		r += q->GetView().TopLeft();
	return r;
}
// 01/12/2007 - END

bool Ctrl::HookProc(XEvent *event) { return false; }

void DnDRequest(XSelectionRequestEvent *se);
void DnDClear();

void Ctrl::ProcessEvent(XEvent *event)
{
	if(xim && XFilterEvent(event, None))
		return;
	ArrayMap<Window, Ctrl::XWindow>& xmap = Xwindow();
	for(int i = 0; i < xmap.GetCount(); i++)
		if(xmap[i].ctrl && xmap[i].ctrl->HookProc(event))
			return;
	FocusSync();
	if(event->type == PropertyNotify &&
	   (event->xproperty.atom == XAtom("_QT_SETTINGS_TIMESTAMP_") ||
	    event->xproperty.atom == XAtom("_KDE_NET_USER_TIME"))) {
		LLOG("Property notify " << XAtomName(event->xproperty.atom)
		     << " " << (void *)event->xany.window);
		for(int i = 0; i < Xwindow().GetCount(); i++) {
			if(Xwindow().GetKey(i)) {
				Ctrl *q = Xwindow()[i].ctrl;
				if(q) q->Refresh();
			}
		}
	}
	if(event->type == SelectionRequest &&
	   event->xselectionrequest.owner == xclipboard().win) {
		if(event->xselectionrequest.selection == XAtom("XdndSelection"))
			DnDRequest(&event->xselectionrequest);
		else
			xclipboard().Request(&event->xselectionrequest);
		return;
	}
	if(event->type == SelectionClear &&
	   event->xselectionclear.window == xclipboard().win) {
		if(event->xselectionclear.selection == XAtom("PRIMARY")) {
			sel_formats.Clear();
			sel_ctrl = NULL;
		}
		if(event->xselectionclear.selection == XAtom("CLIPBOARD"))
			xclipboard().Clear();
		if(event->xselectionrequest.selection == XAtom("XdndSelection"))
			DnDClear();
	   	return;
	}
	int q = xmap.Find(event->xany.window);
#ifdef _DEBUG
	bool eo = false;
	if(LogMessages && event->type != NoExpose && event->type != PropertyNotify
	               && event->type != MotionNotify)
		for(XEventMap *m = sXevent; m->ID; m++)
			if(m->ID == event->type) {
				int t = GetTickCount() - s_starttime;
				VppLog() << Format("%d.%03d", t / 1000, t % 1000);
				VppLog() << " EVENT " << Format("%-20.20s", m->name);
				VppLog() << "[window: " << event->xany.window << "] ";
				if(q >= 0)
					VppLog() << '<' << UPP::Name(Xwindow()[q].ctrl) << '>';
				else
					VppLog() << "<unknown ctrl> ";
				if(event->type == ClientMessage)
					VppLog() << ": " << XAtomName(event->xclient.message_type);
				VppLog() << '\n';
				eo = true;
				break;
			}
#endif
	DropStatusEvent(event);
	if(q < 0) {
		if(event->type == ButtonRelease && popupWnd)
			popupWnd->SetFocus();
		return;
	}
	XWindow& w = xmap[q];
	if(w.ctrl) {
		w.ctrl->EventProc(w, event);
		if(w.ctrl)
			w.ctrl->SyncMoves();
	}
	else
		xmap.SetKey(q, None);
	DefferedFocusSync();
#ifdef _DEBUG
#ifdef UPP_HEAP
	if(MemoryCheck)
		UPP::MemoryCheck();
#endif
	if(eo)
		LLOG("------ end of event processing ----------");
#endif
}

void Ctrl::TimerAndPaint() {
	LTIMING("TimerAndPaint");
	TimerProc(GetTickCount());
	for(int i = 0; i < Xwindow().GetCount(); i++) {
		XWindow& xw = Xwindow()[i];
		if(Xwindow().GetKey(i) && xw.exposed && xw.invalid.GetCount()) {
			if(xw.ctrl) {
				LLOG("..and paint " << UPP::Name(xw.ctrl));
				FocusSync();
				xw.ctrl->SyncScroll();
				Vector<Rect> x = xw.invalid;
				xw.invalid.Clear();
				xw.ctrl->DoPaint(x);
			}
			else
				Xwindow().SetKey(i, None);
		}
	}
	SyncCaret();
	AnimateCaret();
	XSync(Xdisplay, false);
}

bool Ctrl::ProcessEvent(bool *)
{
	if(!IsWaitingEvent()) return false;
	XEvent event;
	XNextEvent(Xdisplay, &event);
	ProcessEvent(&event);
	return true;
}

void SweepMkImageCache();

bool Ctrl::ProcessEvents(bool *)
{
	if(ProcessEvent()) {
		while(ProcessEvent() && (!LoopCtrl || LoopCtrl->InLoop())); // LoopCtrl-MF 071008
		TimerAndPaint();
		SweepMkImageCache();
		return true;
	}
	TimerAndPaint();
	SweepMkImageCache();
	return false;
}

void Ctrl::GuiSleep(int ms)
{
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(Xconnection, &fdset);
	timeval timeout;
	timeout.tv_sec = ms / 1000;
	timeout.tv_usec = ms % 1000 * 1000;
	XFlush(Xdisplay);
	select(Xconnection + 1, &fdset, NULL, NULL, &timeout);
}

static int granularity = 10;

void Ctrl::SetTimerGranularity(int ms)
{
	granularity = ms;
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << BeginIndent);
	Ctrl *ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	while(ctrl ? ctrl->InLoop() && ctrl->IsOpen() : GetTopCtrls().GetCount()) {
		XEvent event;
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(Xconnection, &fdset);
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * granularity;
		XFlush(Xdisplay);
		select(Xconnection + 1, &fdset, NULL, NULL, &timeout);
//		GuiSleep()(granularity);
		SyncMousePos();
		while(IsWaitingEvent()) {
			LTIMING("XNextEvent");
			XNextEvent(Xdisplay, &event);
			ProcessEvent(&event);
		}
		TimerAndPaint();
	}

	if(ctrl)
		LoopCtrl = ploop;

	LoopLevel--;
	LLOG(EndIndent << "Leaving event loop ");
}

void Ctrl::SyncExpose()
{
	if(!top) return;
	XEvent event;
	while(XCheckTypedWindowEvent(Xdisplay, top->window, Expose, &event))
		ProcessEvent(&event);
	while(XCheckTypedWindowEvent(Xdisplay, top->window, GraphicsExpose, &event))
		ProcessEvent(&event);
}

void Ctrl::Create(Ctrl *owner, bool redirect, bool savebits)
{
	LLOG("Create " << Name() << " " << GetRect());
	ASSERT(!IsChild() && !IsOpen());
	LLOG("Ungrab1");
	ReleaseGrab();
	XSetWindowAttributes swa;
	swa.bit_gravity = ForgetGravity;
	swa.background_pixmap = None;
	swa.override_redirect = redirect;
	swa.save_under = savebits;
	Color c = SColorPaper();
	swa.background_pixel = GetXPixel(c.GetR(), c.GetG(), c.GetB());
	Rect r = GetRect();
	isopen = true;
	Window w = XCreateWindow(Xdisplay, RootWindow(Xdisplay, Xscreenno),
	                         r.left, r.top, r.Width(), r.Height(),
	                         0, CopyFromParent, InputOutput, CopyFromParent,
	                         CWBitGravity|CWSaveUnder|CWOverrideRedirect|
	                         (IsCompositedGui() ? CWBackPixel : CWBackPixmap),
	                         &swa);
	if(!w) XError("XCreateWindow failed !");
	int i = Xwindow().Find(None);
	if(i >= 0) Xwindow().SetKey(i, w);
	XWindow& cw = i >= 0 ? Xwindow()[i] : Xwindow().Add(w);
	cw.ctrl = this;
	cw.exposed = false;
	cw.owner = owner;
	cw.xic = xim ? XCreateIC(xim,
	                         XNInputStyle, XIMPreeditNothing|XIMStatusNothing,
	                         XNClientWindow, w,
	                         XNFocusWindow, w,
	                         NULL)
	             : NULL;
	top = new Top;
	top->window = w;
	long im_event_mask = 0;
	if(cw.xic)
		XGetICValues(cw.xic, XNFilterEvents, &im_event_mask, NULL);
	XSelectInput(Xdisplay, w, ExposureMask|StructureNotifyMask|KeyPressMask|
	             FocusChangeMask|KeyPressMask|KeyReleaseMask|PointerMotionMask|
	             ButtonPressMask|ButtonReleaseMask|PropertyChangeMask|
	             VisibilityChangeMask|im_event_mask);
	int version = 5;
	XChangeProperty(Xdisplay, w, XAtom("XdndAware"), XA_ATOM, 32,
					0, (byte *)&version, 1);
	CancelMode();
	if(r.Contains(GetMousePos()))
		DispatchMouse(MOUSEMOVE, GetMousePos() - r.TopLeft());
}

void Ctrl::WndDestroy()
{
	LLOG("WndDestroy " << Name());
	if(!top || !isopen) return;
	AddGlobalRepaint();
	bool revertfocus = HasWndFocus();
	for(int i = 0; i < Xwindow().GetCount(); i++) {
		LOGBEGIN();
		XWindow& w = Xwindow()[i];
		if(Xwindow().GetKey(i) != None && w.owner == this && w.ctrl->IsOpen())
			w.ctrl->WndDestroy();
		LOGEND();
	}
	Ptr<Ctrl> owner;
	int i = Xwindow().Find(top->window);
	if(i >= 0) {
		XWindow& w = Xwindow()[i];
		owner = w.owner;
		w.invalid.Clear();
		if(w.xic)
			XDestroyIC(w.xic);
	}
	isopen = false;
	if(focusWindow == top->window)
		focusWindow = None;
	if(grabWindow == top->window)
		grabWindow = None;
	XDestroyWindow(Xdisplay, top->window);
	if(i >= 0) {
		Xwindow().SetKey(i, None);
		top->window = None;
		Xwindow()[i].ctrl = NULL;
	}
	if(revertfocus && owner)
		owner->TakeFocus();
	delete top;
	top = NULL;
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> v;
	const ArrayMap<Window, Ctrl::XWindow>& w = Xwindow();
	for(int i = 0; i < w.GetCount(); i++)
		if(w.GetKey(i) && w[i].ctrl && !w[i].ctrl->parent)
			v.Add(w[i].ctrl);
	return v;
}

void Ctrl::StartPopupGrab()
{
	if(PopupGrab == 0) {
		if(!top) return;
		if(XGrabPointer(
		   Xdisplay, top->window, true,
		   ButtonPressMask|ButtonReleaseMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask,
		   GrabModeAsync, GrabModeAsync, None, None, CurrentTime) == GrabSuccess) {
				PopupGrab++;
				popupWnd = GetTopWindow();
			}
	}
}

void Ctrl::EndPopupGrab()
{
	if(PopupGrab == 0) return;
	if(--PopupGrab == 0) {
		XUngrabPointer(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
	}
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool, bool)
{
	LLOG("POPUP: " << UPP::Name(this));
	Ctrl *q = owner ? owner->GetTopCtrl() : GetActiveCtrl();
	ignoretakefocus = true;
	Create(q, true, savebits);
	if(activate) {
		q->StartPopupGrab();
		popupgrab = true;
	}
	if(top) popup = true;
	WndShow(visible);
	if(activate && IsEnabled())
		SetFocus();
	if(top) top->owner = owner;
	StateH(OPEN);
}

Ctrl *Ctrl::GetActiveCtrl()
{
	return focusCtrl ? focusCtrl->GetTopCtrl() : NULL;
}

bool  Ctrl::IsAlphaSupported() { return false; }
void  Ctrl::SetAlpha(byte alpha) {}

bool Ctrl::IsCompositedGui()
{
	static bool b = XGetSelectionOwner(Xdisplay, XAtom(String().Cat() << "_NET_WM_CM_S" << Xscreenno)) != None;
	return b;
}

Ctrl *Ctrl::GetOwner()
{
	if(!IsOpen()) return NULL;
	int q = Xwindow().Find(GetWindow());
	return q >= 0 ? Xwindow()[q].owner : NULL;
}

void Ctrl::WndShow(bool b)
{
	LLOG("WndShow " << b);
	if(top) {
		XWindowAttributes xwa;
		XGetWindowAttributes(Xdisplay, top->window, &xwa);
		bool v = xwa.map_state == IsViewable;
		if(b == v) return;
		if(b)
			XMapWindow(Xdisplay, top->window);
		else
			XUnmapWindow(Xdisplay, top->window);
		visible = b;
		StateH(SHOW);
	}
}

void Ctrl::WndUpdate()
{
	LTIMING("WndUpdate");
	LLOG("WNDUPDATE");
	if(!top) return;
	SyncExpose();
	XWindow& xw = Xwindow().Get(top->window);
	if(xw.exposed && xw.invalid.GetCount()) {
		SyncScroll();
		DoPaint(xw.invalid);
		xw.invalid.Clear();
		LTIMING("WndUpdate XSync");
		XSync(Xdisplay, false);
	}
}

void Ctrl::WndUpdate(const Rect& r)
{
	LTIMING("WndUpdate Rect");
	LLOG("WNDUPDATE " << r);
	if(!top) return;
	SyncExpose();
	XWindow& xw = Xwindow().Get(top->window);
	bool dummy;
	SyncScroll();
	DoPaint(Intersect(xw.invalid, r, dummy));
	LTIMING("WndUpdate XSync");
	XSync(Xdisplay, false);
	xw.invalid = Subtract(xw.invalid, r, dummy);
}

void Ctrl::WndSetPos(const Rect& r)
{
	if(!top) return;
	LLOG("WndSetPos " << Name() << r);
	AddGlobalRepaint();
	XMoveResizeWindow(Xdisplay, top->window, r.left, r.top, r.Width(), r.Height());
	rect = r;
	SetWndRect(r);
}

bool Ctrl::IsWndOpen() const
{
	return top;
}

bool Ctrl::SetWndCapture()
{
	if(!IsEnabled() || !top || !IsVisible()) return false;
	if(top->window == grabWindow) return true;
	int status = XGrabPointer(
		Xdisplay, top->window, false,
		ButtonPressMask|ButtonReleaseMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask,
		GrabModeAsync, GrabModeAsync, None, None, CurrentTime
	);
	if(status) return false;
	LLOG("Capture set ok");
	grabWindow = top->window;
	return true;
}

bool Ctrl::HasWndCapture() const
{
	return top && top->window == grabWindow;
}

void Ctrl::ReleaseGrab()
{
	if(grabWindow) {
		LLOG("RELEASE GRAB");
		XUngrabPointer(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
		grabWindow = None;
	}
}

bool Ctrl::ReleaseWndCapture()
{
	LLOG("Releasing capture");
	if(top && top->window == grabWindow) {
		LLOG("Ungrab3");
		ReleaseGrab();
		return true;
	}
	return false;
}

/*
void Ctrl::SetMouseCursor(const Image& image)
{
	if(!top) return;
	Window w = GetWindow();
	if(IsPopUp()) {
		Ctrl *tw = GetTopWindow();
		if(tw)
			w = tw->GetWindow();
	}
	static Image img;
	static Cursor shc;
	static Window wnd;
	if(img.GetSerialId() != image.GetSerialId() || w != wnd) {
		img = image;
		Cursor hc = X11Cursor(img);
		wnd = w;
		XDefineCursor(Xdisplay, wnd, hc);
		if(shc)
			XFreeCursor(Xdisplay, shc);
		shc = hc;
		XFlush(Xdisplay);
	}
}
*/

void Ctrl::SetMouseCursor(const Image& image)
{
	static Image img;
	static Cursor shc;
	if(img.GetSerialId() != image.GetSerialId()) {
		img = image;
		Cursor hc = X11Cursor(IsNull(img) ? Image::Arrow() : img);
		for(int i = 0; i < Xwindow().GetCount(); i++) {
			Window wnd = Xwindow().GetKey(i);
			if(wnd)
				XDefineCursor(Xdisplay, wnd, hc);
		}
		if(shc)
			XFreeCursor(Xdisplay, shc);
		shc = hc;
		XFlush(Xdisplay);
	}
}

void ClearKbdState_();

void Ctrl::TakeFocus()
{
	if(IsChild()) {
		Ctrl *w = GetTopCtrl();
		if(w) w->TakeFocus();
		return;
	}
	XWindow *w = GetXWindow();
	if(!w)
		return;
	if(ignoretakefocus) {
		LLOG("IGNORED TAKE_FOCUS (caused by CreateWindow)");
		if(focusWindow != top->window && focusWindow != None)
			XSetInputFocus(Xdisplay, focusWindow, RevertToParent, CurrentTime);
		return;
	}
	if(w->owner) {
		LLOG("IGNORED TAKE_FOCUS (window is owned)");
		return;
	}
	LLOG("TAKE_FOCUS " << Name());
	ClearKbdState_();
	if(IsEnabled() && IsVisible() && top->window != GetXServerFocusWindow())
		SetWndFocus();
	if(this != focusCtrlWnd) {
		if(IsEnabled()) {
			SetLastActive(w, this);
			LLOG("Activate " << Name());
			SetFocusWnd();
		}
		else {
			LLOG("Activate of disabled window" << Name());
			if(w->last_active && w->last_active->IsOpen() && w->last_active->IsEnabled() &&
			   w->last_active != this) {
				LLOG("    activating last active: " << UPP::Name(w->last_active));
				w->last_active->TakeFocus();
			}
		}
	}

	return;
}

void Ctrl::KillFocus(Window window)
{
	if(!window)
		return;
	int q = Xwindow().Find(window);
	if(q < 0)
		return;
	XWindow& w = Xwindow()[q];
	if(w.ctrl)
		w.ctrl->KillFocusWnd();
}

bool Ctrl::SetWndFocus()
{
	LLOG("SetWndFocus " << Name());
	if(top && top->window != focusWindow && IsEnabled() && IsVisible()) {
		LLOG("Setting focus... ");
		LTIMING("XSetInfputFocus");
		KillFocus(focusWindow);
		XSetInputFocus(Xdisplay, top->window, RevertToParent, CurrentTime);
		focusWindow = top->window;
		SetFocusWnd();
		return true;
	}
	return false;
}

bool Ctrl::HasWndFocus() const
{
	return top && top->window == focusWindow;
}

Window Ctrl::GetXServerFocusWindow()
{
	Window w;
	int    dummy;
	XGetInputFocus(Xdisplay, &w, &dummy);
	return w;
}

void Ctrl::FocusSync()
{
	Window fw = GetXServerFocusWindow();
	if(fw != focusWindow) {
		LLOG("FocusSync to " << FormatIntHex(fw));
		if(fw) {
			int q = Xwindow().Find(fw);
			if(q >= 0) {
				LLOG("Focus to app window");
				focusWindow = fw;
				XWindow& w = Xwindow()[q];
				if(w.ctrl) {
					if(w.ctrl->IsPopUp()) {
						AnimateCaret();
						return;
					}
					KillFocus(focusWindow);
					focusWindow = None;
					w.ctrl->SetFocusWnd();
					AnimateCaret();
					return;
				}
			}
		}
		KillFocus(focusWindow);
		focusWindow = None;
		AnimateCaret();
	}
}

void  Ctrl::AnimateCaret()
{
	int v = !(((GetTickCount() - WndCaretTime) / 500) & 1);
	if(v != WndCaretVisible) {
		RefreshCaret();
		WndCaretVisible = v;
	}
}

void Ctrl::Invalidate(XWindow& xw, const Rect& _r)
{
	LTIMING("Invalidate");
	LLOG("Invalidate " << UPP::Name(xw.ctrl) << " " << _r);
	Vector<Rect> inv;
	Rect r = _r;
	int ra = r.Width() * r.Height();
	for(int i = 0; i < xw.invalid.GetCount(); i++) {
		const Rect& ir = xw.invalid[i];
		Rect ur = r | ir;
		if(ur.Width() * ur.Height() < 2 * (ir.Width() * ir.Height() + ra))
			r = ur;
		else
		if(!r.Contains(xw.invalid[i]))
			inv.Add(xw.invalid[i]);
	}
	Vector<Rect> rs;
	rs.Add(r);
	for(int i = 0; i < inv.GetCount(); i++) {
		bool ch = false;
		Vector<Rect> rs1 = Subtract(rs, inv[i], ch);
		if(ch) rs = rs1;
	}
	inv.AppendPick(rs);
	xw.invalid = inv;
}

void Ctrl::AddGlobalRepaint()
{
	Rect rect = GetRect();
	rect.Inflate(32, 32);//TODO !!! Not correct !!! should read frame extent instead... - or shift GraphicsExposes /  NoExposes...
	ArrayMap<Window, Ctrl::XWindow>& w = Xwindow();
	for(int i = 0; i < w.GetCount(); i++)
		if(w.GetKey(i) && w[i].ctrl) {
			const Rect& r = w[i].ctrl->rect;
			if(rect.Intersects(r))
				Invalidate(w[i], Rect(rect.Offseted(-r.TopLeft())));
		}
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	if(!top) return;
	LLOG("WndInvalidateRect " << r);
	Invalidate(Xwindow().Get(top->window), r);
}

void Ctrl::SetWndForeground()
{
	LLOG("SetWndForeground " << Name());
	if(!top || !IsVisible()) return;
	if(!IsEnabled()) {
		LLOG("Not enabled");
		XWindow *w = GetXWindow();
		if(w && w->last_active && w->last_active != this &&
		   w->last_active->IsOpen() && w->last_active->IsEnabled())
			w->last_active->SetWndForeground();
	}
	else {
		SetWndFocus();
		XRaiseWindow(Xdisplay, top->window);
	}
}

bool Ctrl::IsWndForeground() const
{
	const Ctrl *q = GetTopWindow();
	return ~focusCtrlWnd == (q ? q : GetTopCtrl());
}

bool Ctrl::WndEnable(bool b)
{
	LLOG("WndEnable");
	if(!top) return false;
	if(!b) {
		ReleaseCapture();
		if(HasWndFocus())
			XSetInputFocus(Xdisplay, None, RevertToPointerRoot, CurrentTime);
	}
	return true;
}

// 01/12/2007 - mdelfede
// added support for windowed controls
Ctrl::XWindow *Ctrl::AddXWindow(Window &w)
{
	int i = Xwindow().Find(None);
	if(i >= 0)
		Xwindow().SetKey(i, w);
	XWindow& cw = i >= 0 ? Xwindow()[i] : Xwindow().Add(w);
	cw.ctrl    = this;
	cw.exposed = true;
	cw.owner   = GetParent();
	cw.xic     = NULL;
	return &cw;
}

void Ctrl::RemoveXWindow(Window &w)
{
	int i = Xwindow().Find(w);
	if(i >= 0) {
		Xwindow().SetKey(i, None);
		Xwindow()[i].ctrl = NULL;
	}

}
Ctrl::XWindow *Ctrl::XWindowFromWindow(Window &w)
{
	int i = Xwindow().Find(w);
	if(i >= 0)
		return &Xwindow()[i];
	else
		return NULL;
}

// Synchronizes the native windows inside ctrls
void Ctrl::SyncNativeWindows(void)
{
	ArrayMap<Window, Ctrl::XWindow>& xwindows = Xwindow();
	for(int i = 0; i < xwindows.GetCount(); i++)
	{
		XWindow &xw = xwindows[i];
		Window w = xwindows.GetKey(i);
		if(xw.ctrl && xw.ctrl->parent && w)
		{
			Window dummy;
			int x, y;
			unsigned int width, height, border, depth;
			XGetGeometry(Xdisplay, w, &dummy, &x, &y, &width, &height, &border, &depth);
			Rect r = xw.ctrl->GetRectInParentWindow();
			if( (x != r.left || y != r.top) && ((int)width == r.Width() && (int)height == r.Height()))
				XMoveWindow(Xdisplay, w, r.left, r.top);
			else if( (x == r.left && y == r.top) && ((int)width != r.Width() || (int)height != r.Height()))
				XResizeWindow(Xdisplay, w, r.Width(), r.Height());
			else if( x != r.left || y != r.top || (int)width != r.Width() || (int)height != r.Height())
				XMoveResizeWindow(Xdisplay, w, r.left, r.top, r.Width(), r.Height());
		}
	}

} // END Ctrl::SyncNativeWindows()

// 01/12/2007 - END

ViewDraw::ViewDraw(Ctrl *ctrl)
{
	Ctrl *top = ctrl->GetTopCtrl();
	Rect r = ctrl->GetScreenView() - top->GetScreenRect().TopLeft();
	Vector<Rect> clip;
	clip.Add(r);
	dw = top->GetWindow();
	gc = XCreateGC(Xdisplay, dw, 0, 0);
#ifdef PLATFORM_XFT
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw,
	                        DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
#endif
	Init(clip, r.TopLeft());
}

ViewDraw::~ViewDraw()
{
	XFreeGC(Xdisplay, gc);
}

#endif

END_UPP_NAMESPACE
