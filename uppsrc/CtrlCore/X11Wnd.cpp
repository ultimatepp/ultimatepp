#include "CtrlCore.h"

#ifdef GUI_X11

#include <X11/Xlocale.h>

namespace Upp {

#ifdef _DEBUG

bool Ctrl::LogMessages

// = true _DBG_
;
bool __X11_Grabbing = false;
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

int       Ctrl::Xbuttons;
Window    Ctrl::grabWindow, Ctrl::focusWindow;
int       Ctrl::Xeventtime;

int       Ctrl::PopupGrab;
Ptr<Ctrl> Ctrl::popupWnd;

Point     Ctrl::mousePos;

static int s_starttime;

void Ctrl::DoPaint(const Vector<Rect>& invalid)
{
	GuiLock __;
	Window xwin = GetWindow();
	if(utop && IsVisible()) {
		LTIMING("DoPaint");
		fullrefresh = false;
//		if(GLX) return;
		GC gc = XCreateGC(Xdisplay, (Drawable)xwin, 0, 0);
		XftDraw *xftdraw = XftDrawCreate(Xdisplay, (Drawable)xwin,
		                                 DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
		SystemDraw draw(xwin, gc, xftdraw, invalid);
		painting = true;
		UpdateArea(draw, draw.GetClip());
		painting = false;
		XftDrawDestroy(xftdraw);
		XFreeGC(Xdisplay, gc);
	}
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	GuiLock __;
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
	ASSERT_(IsMainThread(), "IsWaitingEvent can only run in the main thread");
	GuiLock __;
	return XPending(Xdisplay);
}

Ctrl *Ctrl::CtrlFromWindow(Window w)
{
	GuiLock __;
	int q = Xwindow().Find(w);
	return q >= 0 ? Xwindow()[q].ctrl : NULL;
}

Ctrl::XWindow *Ctrl::GetXWindow()
{
	GuiLock __;
	Window xwin = GetWindow();
	if(!xwin) return NULL;
	int q = Xwindow().Find(xwin);
	return q >= 0 ? &Xwindow()[q] : NULL;
}
// 01/12/2007 - mdelfede
// added support for windowed controls

// Gets handle of window containing control
Window Ctrl::GetParentWindow(void) const
{
	GuiLock __;
	Ctrl const *q = GetParentWindowCtrl();
	if(q && utop)
	{
		return q->utop->window;
	}
	else
		return 0;

} // END Ctrl::GetParentWindow()

// Get control with parentwindow as handle
Ctrl *Ctrl::GetParentWindowCtrl(void) const
{
	GuiLock __;
	Ctrl *q = GetParent();
	while(q && !q->utop)
		q = q->GetParent();
	return q;

} // END Ctrl::GetParentWindowCtrl()

// Gets the rect inside the parent window
Rect Ctrl::GetRectInParentWindow(void) const
{
	GuiLock __;
	Rect r = GetScreenRect();
	Ctrl *q = GetParent();
	while(q && !q->utop)
		q = q->GetParent();
	if(q)
	{
		Rect pr = q->GetScreenRect();
		r -= pr.TopLeft();
	}
	return r;
}
// 01/12/2007 - END

bool Ctrl::HookProc(XEvent *event) { return false; }

void DnDRequest(XSelectionRequestEvent *se);
void DnDClear();

dword X11mods(dword key)
{
	dword mod = 0;
	if(key & K_ALT)
		mod |= Mod1Mask;
	if(key & K_SHIFT)
		mod |= ShiftMask;
	if(key & K_CTRL)
		mod |= ControlMask;
	return mod;
}

Vector<Event<> > Ctrl::hotkey;
Vector<dword> Ctrl::keyhot;
Vector<dword> Ctrl::modhot;

int Ctrl::RegisterSystemHotKey(dword key, Function<void ()> cb)
{
	GuiLock __;
	ASSERT(key >= K_DELTA);
	bool b = TrapX11Errors();
	KeyCode k = XKeysymToKeycode(Xdisplay, key & 0xffff);
	dword mod = X11mods(key);
	bool r = false;
	for(dword nlock = 0; nlock < 2; nlock++)
		for(dword clock = 0; clock < 2; clock++)
			for(dword slock = 0; slock < 2; slock++)
				r = XGrabKey(Xdisplay, k,
				             mod | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
				             Xroot, true, GrabModeAsync, GrabModeAsync) || r;
	UntrapX11Errors(b);
	if(!r) return -1;
	int q = hotkey.GetCount();
	for(int i = 0; i < hotkey.GetCount(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.At(q) << cb;
	keyhot.At(q) = k;
	modhot.At(q) = mod;
	return q;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
	GuiLock __;
	if(id >= 0 && id < hotkey.GetCount() && hotkey[id]) {
		bool b = TrapX11Errors();
		for(dword nlock = 0; nlock < 2; nlock++)
			for(dword clock = 0; clock < 2; clock++)
				for(dword slock = 0; slock < 2; slock++)
					XUngrabKey(Xdisplay, keyhot[id],
					           modhot[id] | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
					           Xroot);
		UntrapX11Errors(b);
		hotkey[id].Clear();
	}
}

void Ctrl::ProcessEvent(XEvent *event)
{
	GuiLock __;
	if(xim && XFilterEvent(event, None))
		return;
	if(event->type == KeyPress)
		for(int i = 0; i < hotkey.GetCount(); i++)
			if(hotkey[i] && keyhot[i] == event->xkey.keycode
			   && modhot[i] == (event->xkey.state & (Mod1Mask|ShiftMask|ControlMask))) {
				hotkey[i]();
				return;
			}
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
				if(!s_starttime)
					s_starttime = msecs();
				int t = msecs() - s_starttime;
				VppLog() << Format("%d.%01d", t / 1000, t % 1000);
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
	GuiLock __;
	LTIMING("TimerAndPaint");
	TimerProc(msecs());
	for(int i = 0; i < Xwindow().GetCount(); i++) {
		XWindow& xw = Xwindow()[i];
		if(Xwindow().GetKey(i) && xw.exposed && xw.invalid.GetCount()) {
			if(xw.ctrl) {
				LLOG("..and paint " << UPP::Name(xw.ctrl));
				xw.ctrl->SyncScroll();
				Vector<Rect> x = pick(xw.invalid);
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
	SyncIMPosition();
}

bool Ctrl::ProcessEvent(bool *)
{
	GuiLock __;
	if(!IsWaitingEvent()) return false;
	XEvent event;
	XNextEvent(Xdisplay, &event);
	ProcessEvent(&event);
	return true;
}

void SweepMkImageCache();

bool Ctrl::ProcessEvents(bool *)
{
	ASSERT_(IsMainThread(), "ProcessEvents can only run in the main thread");
	GuiLock __;
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

static bool WakePipeOK;
static int  WakePipe[2];

INITBLOCK {
	WakePipeOK = pipe(WakePipe) == 0;
	fcntl(WakePipe[0], F_SETFL, O_NONBLOCK);
	fcntl(WakePipe[1], F_SETFL, O_NONBLOCK);
}

void WakeUpGuiThread()
{
	if(WakePipeOK)
		IGNORE_RESULT(write(WakePipe[1], "\1", 1));
}

void Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	LLOG(msecs() << " GUISLEEP " << ms);
	ASSERT_(IsMainThread(), "Only main thread can perform GuiSleep");
	timeval timeout;
	timeout.tv_sec = ms / 1000;
	timeout.tv_usec = ms % 1000 * 1000;
	XFlush(Xdisplay);
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(Xconnection, &fdset);
	if(WakePipeOK)
		FD_SET(WakePipe[0], &fdset);
	int level = LeaveGuiMutexAll(); // Give  a chance to nonmain threads to touch GUI things
	select((WakePipeOK ? max(WakePipe[0], Xconnection) : Xconnection) + 1, &fdset, NULL, NULL, &timeout);
	char h;
	while(WakePipeOK && read(WakePipe[0], &h, 1) > 0) // There might be relatively harmless race condition here causing delay in ICall
		LLOG(msecs() << " WakeUpGuiThread detected!"); // or "void" passes through timer & paint
	EnterGuiMutex(level);
}

static int granularity = 10;

void Ctrl::SetTimerGranularity(int ms)
{
	GuiLock __;
	granularity = ms;
}

void Ctrl::SysEndLoop()
{
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT_(IsMainThread(), "EventLoop can only run in the main thread");
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	int64 loopno = ++EventLoopNo;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ctrl *ploop = NULL;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	while(loopno > EndSessionLoopNo && (ctrl ? ctrl->InLoop() && ctrl->IsOpen() : GetTopCtrls().GetCount())) {
		XEvent event;
		GuiSleep(granularity);
		SyncMousePos();
		while(IsWaitingEvent()) {
			LTIMING("XNextEvent");
			XNextEvent(Xdisplay, &event);
			LDUMP(event.type);
			ProcessEvent(&event);
		}
		TimerAndPaint();
		SweepMkImageCache();
	}

	if(ctrl)
		LoopCtrl = ploop;

	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ctrl::SyncExpose()
{
	GuiLock __;
	Window xwin = GetWindow();
	if(!xwin) return;
	XEvent event;
	while(top && XCheckTypedWindowEvent(Xdisplay, xwin, Expose, &event))
		ProcessEvent(&event);
	while(top && XCheckTypedWindowEvent(Xdisplay, xwin, GraphicsExpose, &event))
		ProcessEvent(&event);
}

void Ctrl::Create(Ctrl *owner, bool redirect, bool savebits)
{
	GuiLock __;
	ASSERT_(IsMainThread(), "Only main thread can create windows");
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
	                         CWBitGravity|CWSaveUnder|CWOverrideRedirect|CWBackPixmap,
	                         &swa);
	if(!w) XError("XCreateWindow failed !");
	int i = Xwindow().Find(None);
	if(i >= 0) Xwindow().SetKey(i, w);
	XWindow& cw = i >= 0 ? Xwindow()[i] : Xwindow().Add(w);
	cw.ctrl = this;
	cw.exposed = false;
	cw.owner = owner;
	
	cw.xic = NULL;
	
	if(xim) {
		cw.xic = XCreateIC(xim,
		                   XNInputStyle, XIMPreeditNothing|XIMStatusNothing,
		                   XNClientWindow, w,
		                   NULL);
	}
	Top *top = new Top;
	top->window = w;
	SetTop(top);
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

	if(redirect) {
		int windowType = XInternAtom(Xdisplay, "_NET_WM_WINDOW_TYPE_POPUP_MENU", false);
		XChangeProperty(Xdisplay, w, XInternAtom(Xdisplay, "_NET_WM_WINDOW_TYPE", false), XA_ATOM, 32, 
		                PropModeReplace, (byte *)&windowType, 1);
	}
	
	RefreshLayoutDeep();
	
	SyncIMPosition();
}

void Ctrl::WndDestroy()
{
	GuiLock __;
	LLOG("WndDestroy " << Name());
	Window xwin = GetWindow();
	if(!xwin || !isopen) return;
	AddGlobalRepaint();
	bool revertfocus = HasWndFocus() || !GetFocusCtrl();
	for(int i = 0; i < Xwindow().GetCount(); i++) {
		LOGBEGIN();
		XWindow& w = Xwindow()[i];
		if(Xwindow().GetKey(i) != None && w.owner == this && w.ctrl->IsOpen())
			w.ctrl->WndDestroy();
		LOGEND();
	}
	Ptr<Ctrl> owner;
	int i = Xwindow().Find(xwin);
	if(i >= 0) {
		XWindow& w = Xwindow()[i];
		owner = w.owner;
		w.invalid.Clear();
		if(w.xic)
			XDestroyIC(w.xic);
	}
	isopen = false;
	if(focusWindow == xwin)
		focusWindow = None;
	if(grabWindow == xwin)
		grabWindow = None;
	XDestroyWindow(Xdisplay, xwin);
	if(i >= 0) {
		Xwindow().SetKey(i, None);
		xwin = None;
		Xwindow()[i].ctrl = NULL;
	}

	if(revertfocus && owner)
		owner->TakeFocus();

	if(focusWindow) {
		int q = Xwindow().Find(focusWindow);
		if(q >= 0) {
			XIC xic = Xwindow()[q].xic;
			if(xic) {
				XSetICFocus(xic);
				SyncIMPosition();
			}
		}
	}

	DeleteTop();
	FocusSync();
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	GuiLock __;
	Vector<Ctrl *> v;
	const ArrayMap<Window, Ctrl::XWindow>& w = Xwindow();
	for(int i = 0; i < w.GetCount(); i++)
		if(w.GetKey(i) && w[i].ctrl && !w[i].ctrl->GetParent())
			v.Add(w[i].ctrl);
	return v;
}

void Ctrl::StartPopupGrab()
{
	GuiLock __;
	if(PopupGrab == 0) {
		Window xwin = GetWindow();
		if(!xwin) return;
		if(XGrabPointer(
		   Xdisplay, xwin, true,
		   ButtonPressMask|ButtonReleaseMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask,
		   GrabModeAsync, GrabModeAsync, None, None, CurrentTime) == GrabSuccess) {
				PopupGrab++;
				popupWnd = GetTopWindow();
#ifdef _DEBUG
				__X11_Grabbing = true;
#endif
			}
	}
}

void Ctrl::EndPopupGrab()
{
	GuiLock __;
	if(PopupGrab == 0) return;
	if(--PopupGrab == 0) {
		XUngrabPointer(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
#ifdef _DEBUG
		__X11_Grabbing = false;
#endif
	}
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool, bool)
{
	GuiLock __;
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
	if(utop) utop->owner = owner;
	StateH(OPEN);
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	return focusCtrl ? focusCtrl->GetTopCtrl() : NULL;
}

bool  Ctrl::IsAlphaSupported()
{
	return IsCompositedGui();
}

void  Ctrl::SetAlpha(byte alpha)
{
	GuiLock __;
	Window hwnd = GetWindow();
	if (!IsAlphaSupported() || GetParent() || !GetTop() || !hwnd)
		return;
	unsigned int opacity = (unsigned int) 16843009 * alpha;
	Atom aw_opacity = XInternAtom(Xdisplay, "_NET_WM_WINDOW_OPACITY", XFalse);
	XChangeProperty(Xdisplay, hwnd, aw_opacity, XA_CARDINAL, 32, PropModeReplace,
					(unsigned char *) &opacity, 1);
}

bool Ctrl::IsCompositedGui()
{
	GuiLock __;
	static bool b = XGetSelectionOwner(Xdisplay, XAtom(String().Cat() << "_NET_WM_CM_S" << Xscreenno)) != None;
	return b;
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	if(!IsOpen()) return NULL;
	int q = Xwindow().Find(GetWindow());
	return q >= 0 ? Xwindow()[q].owner : NULL;
}

void Ctrl::WndShow(bool b)
{
	GuiLock __;
	LLOG("WndShow " << b);
	Window xwin = GetWindow();
	if(xwin) {
		XWindowAttributes xwa;
		XGetWindowAttributes(Xdisplay, xwin, &xwa);
		bool v = xwa.map_state == IsViewable;
		if(b == v) return;
		if(b)
			XMapWindow(Xdisplay, xwin);
		else
			XUnmapWindow(Xdisplay, xwin);
		visible = b;
		StateH(SHOW);
	}
}

void Ctrl::WndUpdate()
{
	GuiLock __;
	LTIMING("WndUpdate");
	LLOG("WNDUPDATE");
	if(!top) return;  //aris002 when to update there is a question of (is)top concept
	SyncExpose();
	Window xwin = GetWindow();
	if(!xwin) return;
	XWindow& xw = Xwindow().Get(xwin);
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
	GuiLock __;
	LTIMING("WndUpdate Rect");
	LLOG("WNDUPDATE " << r);
	if(!top) return;
	SyncExpose();
	Window xwin = GetWindow();
	if(!xwin) return;
	XWindow& xw = Xwindow().Get(xwin);
	bool dummy;
	SyncScroll();
	DoPaint(Intersect(xw.invalid, r, dummy));
	LTIMING("WndUpdate XSync");
	XSync(Xdisplay, false);
	xw.invalid = Subtract(xw.invalid, r, dummy);
}

void Ctrl::WndSetPos(const Rect& r)
{
	GuiLock __;
	Window xwin = GetWindow();
	if(!xwin) return;
	LLOG("WndSetPos0 " << Name() << r);
	AddGlobalRepaint();
	XMoveResizeWindow(Xdisplay, xwin, r.left, r.top, r.Width(), r.Height());
	rect = r;
	SetWndRect(r);
}

bool Ctrl::IsWndOpen() const
{
	return top;
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	Window xwin = GetWindow();
	if(!IsEnabled() || !xwin || !IsVisible()) return false;
	if(xwin == grabWindow) return true;
	int status = XGrabPointer(
		Xdisplay, xwin, false,
		ButtonPressMask|ButtonReleaseMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask,
		GrabModeAsync, GrabModeAsync, None, None, CurrentTime
	);
	if(status) return false;
#ifdef _DEBUG
		__X11_Grabbing = true;
#endif
	LLOG("Capture set ok");
	grabWindow = xwin;
	return true;
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	Window xwin = GetWindow();
	return top && xwin == grabWindow;
}

void Ctrl::ReleaseGrab()
{
	GuiLock __;
	if(grabWindow) {
		LLOG("RELEASE GRAB");
		XUngrabPointer(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
		grabWindow = None;
#ifdef _DEBUG
		__X11_Grabbing = false;
#endif
	}
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	LLOG("Releasing capture");
	Window xwin = GetWindow();
	if(top && xwin == grabWindow) {
		LLOG("Ungrab3");
		ReleaseGrab();
		return true;
	}
	return false;
}

void Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	static Image img;
	static Cursor shc;
	if(img.GetSerialId() != image.GetSerialId()) {
		img = image;
		Cursor hc = (Cursor)CursorX11(IsNull(img) ? Image::Arrow() : img);
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
	LLOG("TAKE_FOCUS0 " << Name());
	GuiLock __;
	if(IsChild()) {
		Ctrl *w = GetTopCtrl();
		if(w) w->TakeFocus();
		return;
	}
	XWindow *w = GetXWindow();
	if(!w)
		return;
	Window xwin = GetWindow();
	if(ignoretakefocus) {
		LLOG("IGNORED TAKE_FOCUS (caused by CreateWindow)");
		if(focusWindow != xwin && focusWindow != None)
			XSetInputFocus(Xdisplay, focusWindow, RevertToParent, CurrentTime);
		return;
	}
	if(w->owner) {
		LLOG("IGNORED TAKE_FOCUS (window is owned)");
		return;
	}
	LLOG("TAKE_FOCUS " << Name());
	if(IsEnabled() && IsVisible() && xwin != GetXServerFocusWindow()) {
		ClearKbdState_();
		SetWndFocus();
	}
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
	GuiLock __;
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
	GuiLock __;
	LLOG("SetWndFocus " << Name());
	Window xwin = GetWindow();
	if(top && xwin != focusWindow && IsEnabled() && IsVisible()) {
		LLOG("Setting focus... ");
		LTIMING("XSetInfputFocus");
		Ptr<Ctrl> _this = this;
		KillFocus(focusWindow);
		if(_this && xwin) {
			XSetInputFocus(Xdisplay, xwin, RevertToParent, CurrentTime);
			focusWindow = xwin;
			SetFocusWnd();
		}
		return true;
	}
	return false;
}

bool Ctrl::HasWndFocus() const
{
	GuiLock __;
	Window xwin = GetWindow();
	return top && xwin == focusWindow;
}

Window Ctrl::GetXServerFocusWindow()
{
	GuiLock __;
	Window w;
	int    dummy;
	LLOG("XGetInputFocus");
	XGetInputFocus(Xdisplay, &w, &dummy);
	return w;
}

void Ctrl::FocusSync()
{
	LLOG("FocusSync");
	GuiLock __;
	Window fw = GetXServerFocusWindow();
	if(fw != focusWindow) {
		LLOG("FocusSync to " << FormatIntHex(fw));
		if(fw) {
			int q = Xwindow().Find(fw);
			if(q >= 0) {
				LLOG("Focus to app window");
				Window fwo = focusWindow;
				focusWindow = fw;
				XWindow& w = Xwindow()[q];
				if(w.ctrl) {
					if(w.ctrl->IsPopUp()) {
						AnimateCaret();
						return;
					}
					KillFocus(fwo);
//					focusWindow = None; // 1010-10-13 cxl (SetWndFocus does not set this)
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

void  Ctrl::SyncIMPosition()
{
/*
	if(!focusWindow)
		return;
	int q = Xwindow().Find(focusWindow);
	if(q < 0)
		return;
	XWindow& xw = Xwindow()[q];
	XIC xic = xw.xic;
	if(xw.xic && xw.ctrl) {
		XVaNestedList   preedit_attr;
		XPoint          spot;
		spot.x = xw.ctrl->caretx + xw.ctrl->caretcx;
		spot.y = xw.ctrl->carety + xw.ctrl->caretcy;
		preedit_attr = XVaCreateNestedList(0, XNSpotLocation, &spot, NULL);
		XSetICValues(xw.xic, XNPreeditAttributes, preedit_attr, NULL);
		XFree(preedit_attr);
	}
*/
}

void Ctrl::Invalidate(XWindow& xw, const Rect& _r)
{
	GuiLock __;
	LTIMING("Invalidate");
	LLOG("Invalidate " << UPP::Name(xw.ctrl) << " " << _r);
	AddRefreshRect(xw.invalid, _r);
}

void Ctrl::AddGlobalRepaint()
{
	GuiLock __;
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
	GuiLock __;
	Window xwin = GetWindow();
	if(!xwin) return;
	LLOG("WndInvalidateRect0 " << r);
	Invalidate(Xwindow().Get(xwin), r);
}

void Ctrl::SetWndForeground()
{
	GuiLock __;
	LLOG("SetWndForeground " << Name());
	Window xwin = GetWindow();
	if(!xwin || !IsVisible()) return;
	if(!IsEnabled()) {
		LLOG("Not enabled");
		XWindow *w = GetXWindow();
		if(w && w->last_active && w->last_active != this &&
		   w->last_active->IsOpen() && w->last_active->IsEnabled())
			w->last_active->SetWndForeground();
	}
	else {
		Ptr<Ctrl> _this = this;
		SetWndFocus();
		if(_this && xwin)
			XRaiseWindow(Xdisplay, xwin);
	}
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	const Ctrl *q = GetTopWindow();
	return ~focusCtrlWnd == (q ? q : GetTopCtrl());
}

void Ctrl::WndEnable(bool b)
{
	GuiLock __;
	LLOG("WndEnable");
	if(!top) return;
	if(!b) {
		ReleaseCapture();
		if(HasWndFocus())
			XSetInputFocus(Xdisplay, None, RevertToPointerRoot, CurrentTime);
	}
}

// 01/12/2007 - mdelfede
// added support for windowed controls
Ctrl::XWindow *Ctrl::AddXWindow(Window &w)
{
	GuiLock __;
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
	GuiLock __;
	int i = Xwindow().Find(w);
	if(i >= 0) {
		Xwindow().SetKey(i, None);
		Xwindow()[i].ctrl = NULL;
	}

}
Ctrl::XWindow *Ctrl::XWindowFromWindow(Window &w)
{
	GuiLock __;
	int i = Xwindow().Find(w);
	if(i >= 0)
		return &Xwindow()[i];
	else
		return NULL;
}

// Synchronizes the native windows inside ctrls
void Ctrl::SyncNativeWindows(void)
{
	GuiLock __;
	ArrayMap<Window, Ctrl::XWindow>& xwindows = Xwindow();
	for(int i = 0; i < xwindows.GetCount(); i++)
	{
		XWindow &xw = xwindows[i];
		Window w = xwindows.GetKey(i);
		if(xw.ctrl && xw.ctrl->GetParent() && w)
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

TopFrameDraw::TopFrameDraw(Ctrl *ctrl, const Rect& r)
{
	EnterGuiMutex();
	Ctrl *top = ctrl->GetTopCtrl();
	Vector<Rect> clip;
	clip.Add(r);
	dw = top->GetWindow();
	gc = XCreateGC(Xdisplay, dw, 0, 0);
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw,
	                        DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
	Init(clip, r.TopLeft());
}

TopFrameDraw::~TopFrameDraw()
{
	XFreeGC(Xdisplay, gc);
	LeaveGuiMutex();
}

}

#endif
