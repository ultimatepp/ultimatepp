#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)    // DLOG(rmsecs() << ' ' << x)
//_DBG_ #define LOG_EVENTS

BiVector<Ctrl::Event> Ctrl::Events;

Point         Ctrl::CurrentMousePos;
guint         Ctrl::CurrentState;
guint32       Ctrl::CurrentTime;
Ctrl::Event   Ctrl::CurrentEvent;

bool  GetShift() { return Ctrl::CurrentState & GDK_SHIFT_MASK; }
bool  GetCtrl() { return Ctrl::CurrentState & GDK_CONTROL_MASK; }
bool  GetAlt() { return Ctrl::CurrentState & GDK_MOD1_MASK; }
bool  GetCapsLock() { return Ctrl::CurrentState & GDK_LOCK_MASK; }
bool  GetMouseLeft() { return Ctrl::CurrentState & GDK_BUTTON1_MASK; }
bool  GetMouseRight() { return Ctrl::CurrentState & GDK_BUTTON3_MASK; }
bool  GetMouseMiddle() { return Ctrl::CurrentState & GDK_BUTTON2_MASK; }
Point GetMousePos() { return Ctrl::CurrentMousePos; }

#ifdef LOG_EVENTS
Tuple2<int, const char *> xEvent[] = {
	{ GDK_NOTHING, "GDK_NOTHING" },
	{ GDK_DELETE, "GDK_DELETE" },
	{ GDK_DESTROY, "GDK_DESTROY" },
	{ GDK_EXPOSE, "GDK_EXPOSE" },
	{ GDK_MOTION_NOTIFY, "GDK_MOTION_NOTIFY" },
	{ GDK_BUTTON_PRESS, "GDK_BUTTON_PRESS" },
	{ GDK_2BUTTON_PRESS, "GDK_2BUTTON_PRESS" },
	{ GDK_3BUTTON_PRESS, "GDK_3BUTTON_PRESS" },
	{ GDK_BUTTON_RELEASE, "GDK_BUTTON_RELEASE" },
	{ GDK_KEY_PRESS, "GDK_KEY_PRESS" },
	{ GDK_KEY_RELEASE, "GDK_KEY_RELEASE" },
	{ GDK_ENTER_NOTIFY, "GDK_ENTER_NOTIFY" },
	{ GDK_LEAVE_NOTIFY, "GDK_LEAVE_NOTIFY" },
	{ GDK_FOCUS_CHANGE, "GDK_FOCUS_CHANGE" },
	{ GDK_CONFIGURE, "GDK_CONFIGURE" },
	{ GDK_MAP, "GDK_MAP" },
	{ GDK_UNMAP, "GDK_UNMAP" },
	{ GDK_PROPERTY_NOTIFY, "GDK_PROPERTY_NOTIFY" },
	{ GDK_SELECTION_CLEAR, "GDK_SELECTION_CLEAR" },
	{ GDK_SELECTION_REQUEST, "GDK_SELECTION_REQUEST" },
	{ GDK_SELECTION_NOTIFY, "GDK_SELECTION_NOTIFY" },
	{ GDK_PROXIMITY_IN, "GDK_PROXIMITY_IN" },
	{ GDK_PROXIMITY_OUT, "GDK_PROXIMITY_OUT" },
	{ GDK_DRAG_ENTER, "GDK_DRAG_ENTER" },
	{ GDK_DRAG_LEAVE, "GDK_DRAG_LEAVE" },
	{ GDK_DRAG_MOTION, "GDK_DRAG_MOTION" },
	{ GDK_DRAG_STATUS, "GDK_DRAG_STATUS" },
	{ GDK_DROP_START, "GDK_DROP_START" },
	{ GDK_DROP_FINISHED, "GDK_DROP_FINISHED" },
	{ GDK_CLIENT_EVENT, "GDK_CLIENT_EVENT" },
	{ GDK_VISIBILITY_NOTIFY, "GDK_VISIBILITY_NOTIFY" },
	{ GDK_NO_EXPOSE, "GDK_NO_EXPOSE" },
	{ GDK_SCROLL, "GDK_SCROLL" },
	{ GDK_WINDOW_STATE, "GDK_WINDOW_STATE" },
	{ GDK_SETTING, "GDK_SETTING" },
	{ GDK_OWNER_CHANGE, "GDK_OWNER_CHANGE" },
	{ GDK_GRAB_BROKEN, "GDK_GRAB_BROKEN" },
	{ GDK_DAMAGE, "GDK_DAMAGE" },
	{ GDK_EVENT_LAST, "GDK_EVENT_LAST" },
};
#endif

Ctrl *Ctrl::GetTopCtrlFromId(int id)
{
	int q = FindId(id);
	if(q >= 0) {
		Ctrl *p = wins[q].ctrl;
		if(p && p->top)
			return p;
	}
	return NULL;
}

gboolean Ctrl::GtkEvent(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	GuiLock __;
	GdkEventKey *key;
	bool pressed = false;
	bool  retval = true;
	Value value;
	Ctrl *p = GetTopCtrlFromId(user_data);
#ifdef LOG_EVENTS
	String ev = "?";
	Tuple2<int, const char *> *f = FindTuple(xEvent, __countof(xEvent), event->type);
	if(f)
		ev = f->b;
	LOG(rmsecs() << " FETCH EVENT " << ev << " ctrl: " << Name(p));
#endif

	switch(event->type) {
	case GDK_EXPOSE:
	case GDK_DAMAGE:
		if(p) {
#ifdef LOG_EVENTS
			TimeStop tm;
#endif
			p->fullrefresh = false;
			GdkEventExpose *e = (GdkEventExpose *)event;
			SystemDraw w(gdk_cairo_create(p->gdk()), p->gdk());
			painting = true;
			Rect r = RectC(e->area.x, e->area.y, e->area.width, e->area.height);
			w.Clip(r);
			p->UpdateArea(w, r);
			w.End();
			cairo_destroy(w);
			if(p->top->dr)
				DrawDragRect(*p, *p->top->dr);
			painting = false;
#ifdef LOG_EVENTS
			LOG("* " << ev << " elapsed " << tm);
#endif			
		}
		return true;
	case GDK_DELETE:
		break;
	case GDK_FOCUS_CHANGE:
		if(p) {
			if(((GdkEventFocus *)event)->in)
				gtk_im_context_focus_in(p->top->im_context);
			else
				gtk_im_context_focus_out(p->top->im_context);
			AddEvent(user_data, EVENT_NONE, value, event);
		}
		return false;
	case GDK_LEAVE_NOTIFY:
	case GDK_MOTION_NOTIFY:
		break;
	case GDK_BUTTON_PRESS:
		value = DoButtonEvent(event, true);
		if(IsNull(value))
			return false;
		break;
	case GDK_2BUTTON_PRESS:
		value = DoButtonEvent(event, true);
		if(IsNull(value))
			return false;
		break;
	case GDK_BUTTON_RELEASE:
		value = DoButtonEvent(event, false);
		if(IsNull(value))
			return false;
		break;
	case GDK_SCROLL: {
		GdkEventScroll *e = (GdkEventScroll *)event;
		value = findarg(e->direction, GDK_SCROLL_UP, GDK_SCROLL_LEFT) < 0 ? -120 : 120;
		break;
	}
	case GDK_KEY_PRESS:
		pressed = true;
	case GDK_KEY_RELEASE:
		key = (GdkEventKey *)event;
		value = (int) key->keyval;
		if(pressed) {
			p = GetTopCtrlFromId(user_data);
			if(p && gtk_im_context_filter_keypress(p->top->im_context, key))
				return true;
		}
		break;
	case GDK_CONFIGURE: {
		retval = false;
		GdkEventConfigure *e = (GdkEventConfigure *)event;
		value = RectC(e->x, e->y, e->width, e->height);
		LLOG("GDK_CONFIGURE " << value);
		break;
	}
	default:
		return false;
	}
	AddEvent(user_data, event->type, value, event);
	return retval;
}

int Ctrl::DoButtonEvent(GdkEvent *event, bool press)
{
	GdkEventButton *e = (GdkEventButton *)event;
	static int mask[] = { GDK_BUTTON1_MASK, GDK_BUTTON2_MASK, GDK_BUTTON3_MASK };
	if(e->button >= 1 && e->button <= 3) {
		int m = mask[e->button - 1];
		int state = e->state;
		if(press) // gtk seems to provide state "before" the event, so buttons are wrong
			state |= m;
		else
			state &= ~m;
		return e->button;
	}
	return Null;
}

Ctrl::Event::Event()
{
	event = NULL;
}

void Ctrl::Event::Free()
{
	if(event) {
		gdk_event_free(event);
		event = NULL;
	}
}

void Ctrl::Event::Set(const Event& e)
{
	*(Event0 *)this = e;
	event = e.event ? gdk_event_copy(e.event) : NULL;
}

Ctrl::Event::~Event()
{
	Free();
}

Ctrl::Event::Event(const Event& e)
{
	Set(e);
}

void Ctrl::Event::operator=(const Event& e)
{
	if(this == &e)
		return;
	Free();
	Set(e);
}

void Ctrl::AddEvent(gpointer user_data, int type, const Value& value, GdkEvent *event)
{
	if(Events.GetCount() > 50000)
		return;
	Event& e = Events.AddTail();
	e.windowid = (uint32)(uintptr_t)user_data;
	e.type = type;
	e.value = value;
	gint x, y;
	GdkModifierType mod;
	gdk_window_get_pointer(gdk_get_default_root_window(), &x, &y, &mod);
	e.mousepos = Point(x, y);
	e.state = mod;
	e.count = 1;
	e.event = NULL;
	if(event) {
		e.time = gdk_event_get_time(event);
		e.event = gdk_event_copy(event);
	}
	else {
		e.time = gtk_get_current_event_time();
		e.event = gtk_get_current_event();
	}
}

void Ctrl::IMCommit(GtkIMContext *context, gchar *str, gpointer user_data)
{
	GuiLock __;
	AddEvent(user_data, EVENT_TEXT, FromUtf8(str), NULL);
}

void Ctrl::FetchEvents(bool may_block)
{
	LLOG("FetchEvents " << may_block);
	int level = LeaveGuiMutexAll();
	while(g_main_context_iteration(NULL, may_block))
		may_block = false;
	EnterGuiMutex(level);
}

bool Ctrl::IsWaitingEvent0(bool fetch)
{
	if(fetch)
		FetchEvents(FALSE);
	return Events.GetCount();
}

bool Ctrl::IsWaitingEvent()
{
	ASSERT_(IsMainThread(), "IsWaitingEvent can only run in the main thread");
	return IsWaitingEvent0(true);
}

struct ProcStop {
	TimeStop tm;
	String   ev;
	
	~ProcStop() { LOG("* " << ev << " elapsed " << tm); }
};

bool Ctrl::DispatchMouseIn(int act, int zd)
{
	Point p = GetMousePos();
	Rect r = GetScreenRect();
	if(r.Contains(p)) {
		p -= r.TopLeft();
		DispatchMouse(act, p, zd);
		return true;
	}
	return false;
}

void Ctrl::GtkMouseEvent(int action, int act, int zd)
{
	if(grabpopup && activePopup.GetCount()) {
		for(int i = activePopup.GetCount(); --i >= 0;)
			if(activePopup[i] && activePopup[i]->DispatchMouseIn(act, zd))
				return;
		if(action == DOWN) { // Deactivate active popup(s) if clicked outside of active popups
			IgnoreMouseUp();
			if(activePopup.Top())
				activePopup.Top()->GetTopWindow()->ActivateWnd();
		}
		else
		if(activePopup[0]) { // Redirect other events to TopWindow that spawned first popup
			Ptr<TopWindow> w = activePopup[0]->GetTopWindow();
			if(w)
				w->DispatchMouseIn(act, zd);
		}
		return;
	}
	DispatchMouse(act, GetMousePos() - GetScreenRect().TopLeft(), zd);
}

void Ctrl::GtkButtonEvent(int action)
{
	int act = action;
	int button = CurrentEvent.value;
	if(action != MOUSEMOVE)
		act |= button == 2 ? MIDDLE : button == 3 ? RIGHT : LEFT;
	GtkMouseEvent(action, act, 0);
}

void Ctrl::Proc()
{
#ifdef LOG_EVENTS
	String ev = "?";
	Tuple2<int, const char *> *f = FindTuple(xEvent, __countof(xEvent), CurrentEvent.type);
	if(f)
		ev = f->b;
	LOG(rmsecs() << " PROCESS EVENT " << Upp::Name(this) << " " << ev);
	ProcStop tm;
	tm.ev = ev;
#endif
	if(!IsOpen())
		return;
	Ptr<Ctrl> _this = this;
	bool pressed = false;
	int  kv;
	static int clicktime = msecs() - 100000;
	switch(CurrentEvent.type) {
	case GDK_MOTION_NOTIFY: {
		GtkMouseEvent(MOUSEMOVE, MOUSEMOVE, 0);
		break;
	}
	case GDK_BUTTON_PRESS:
		if(!HasWndFocus() && !popup)
			SetWndFocus();
		ClickActivateWnd();
		if(ignoremouseup) {
			KillRepeat();
			ignoreclick = false;
			ignoremouseup = false;
		}
		if(!ignoreclick)
			GtkButtonEvent(msecs(clicktime) < 250 ? DOUBLE : DOWN);
		clicktime = msecs();
		break;
/*	case GDK_2BUTTON_PRESS:
		if(!ignoreclick)
			GtkButtonEvent(DOUBLE);
		break;
*/	case GDK_BUTTON_RELEASE:
		if(ignoreclick)
			EndIgnore();
		else
			GtkButtonEvent(UP);
		break;
	case GDK_SCROLL: {
		GtkMouseEvent(MOUSEWHEEL, MOUSEWHEEL, CurrentEvent.value);
		break;
	}
	case GDK_KEY_PRESS:
		pressed = true;
	case GDK_KEY_RELEASE:
		kv = CurrentEvent.value;
		if(kv >= 0 && kv < 65536) {
			LLOG("keyval " << FormatIntHex(kv) << ' ' << (char)kv);
			if(kv >= 'a' && kv <= 'z')
				kv = kv - 'a' + 'A';
			static Tuple2<int, int> cv[] = {
				{ GDKEY(BackSpace), K_BACKSPACE },
				{ GDKEY(Tab), K_TAB },
				{ GDKEY(ISO_Left_Tab), K_TAB },
				{ GDKEY(Return), K_ENTER },
				{ GDKEY(Escape), K_ESCAPE },
				{ GDKEY(space), K_SPACE },
				{ GDKEY(Control_L), K_CTRL_KEY },
				{ GDKEY(Control_R), K_CTRL_KEY },
				{ GDKEY(Shift_L), K_SHIFT_KEY },
				{ GDKEY(Shift_R), K_SHIFT_KEY },
				{ GDKEY(Alt_L), K_ALT_KEY },
				{ GDKEY(Alt_R), K_ALT_KEY },

				{ GDKEY(KP_Space), K_SPACE },
				{ GDKEY(KP_Tab), K_TAB },
				{ GDKEY(KP_Enter), K_ENTER },
				{ GDKEY(KP_F1), K_F1 },
				{ GDKEY(KP_F2), K_F2 },
				{ GDKEY(KP_F3), K_F3 },
				{ GDKEY(KP_F4), K_F4 },
				{ GDKEY(KP_Home), K_HOME },
				{ GDKEY(KP_Left), K_LEFT },
				{ GDKEY(KP_Up), K_UP },
				{ GDKEY(KP_Right), K_RIGHT },
				{ GDKEY(KP_Down), K_DOWN },
				{ GDKEY(KP_Page_Up), K_PAGEUP },
				{ GDKEY(KP_Page_Down), K_PAGEDOWN },
				{ GDKEY(KP_End), K_END },
				{ GDKEY(KP_Begin), K_HOME },
				{ GDKEY(KP_Insert), K_INSERT },
				{ GDKEY(KP_Delete), K_DELETE },
			};
			Tuple2<int, int> *x = FindTuple(cv, __countof(cv), kv);
			if(x)
				kv = x->b;
			else
				kv += K_DELTA;
			if(GetShift() && kv != K_SHIFT_KEY)
				kv |= K_SHIFT;
			if(GetCtrl() && kv != K_CTRL_KEY)
				kv |= K_CTRL;
			if(GetAlt() && kv != K_ALT_KEY)
				kv |= K_ALT;
			LLOG(GetKeyDesc(kv) << ", pressed: " << pressed << ", count: " << CurrentEvent.count);
#ifdef GDK_WINDOWING_X11
			if(pressed)
				for(int i = 0; i < hotkey.GetCount(); i++) {
					if(hotkey[i] && keyhot[i] == (dword)kv) {
						hotkey[i]();
						return;
					}
				}
#endif
			DispatchKey(!pressed * K_KEYUP + kv, CurrentEvent.count);
		}
		break;
	case EVENT_TEXT: {
		WString h = CurrentEvent.value;
		for(int i = 0; i < h.GetCount(); i++) // TODO: Add compression
			DispatchKey(h[i], 1);
		break;
	}
	case GDK_DELETE: {
		TopWindow *w = dynamic_cast<TopWindow *>(this);
		if(w) {
			if(IsEnabled()) {
				IgnoreMouseUp();
				w->WhenClose();
			}
		}
		return;
	}
	case GDK_CONFIGURE: {
			Rect rect = CurrentEvent.value;
			if(GetRect() != rect)
				SetWndRect(rect);
		}
		break;
	default:
		return;
	}
	if(_this)
		_this->PostInput();
}


bool Ctrl::ProcessEvent0(bool *quit, bool fetch)
{
	ASSERT(IsMainThread());
	bool r = false;
	if(IsWaitingEvent0(fetch)) {
		while(Events.GetCount() > 1) { // Event compression (coalesce autorepeat, mouse moves/wheel, configure)
			Event& a = Events[0];
			Event& b = Events[1];
			if(b.type == a.type && a.windowid == b.windowid && a.state == b.state) {
				if(a.type == GDK_KEY_PRESS && a.value == b.value)
					b.count += a.count;
			    else
			    if(a.type == GDK_SCROLL)
			        b.value = (int)b.value + (int)a.value;
			    else
			    if(findarg(a.type, GDK_MOTION_NOTIFY, GDK_CONFIGURE) < 0)
			        break;
				Events.DropHead();
			}
			else
				break;
			FocusSync();
		}
		Event& e = Events.Head();
		CurrentTime = e.time;
		CurrentMousePos = e.mousepos;
		CurrentState = e.state;
		CurrentEvent = e;
		Value val = e.value;
		Events.DropHead();
		Ctrl *w = GetTopCtrlFromId(e.windowid);
		FocusSync();
		CaptureSync();
		if(w)
			w->Proc();
		r = true;
	}
	if(quit)
		*quit = IsEndSession();
	FocusSync();
	SyncCaret();
	return r;
}

bool Ctrl::ProcessEvent(bool *quit)
{
	return ProcessEvent0(quit, true);
}

gboolean Ctrl::TimeHandler(GtkWidget *)
{ // we only need timer to periodically break blocking g_main_context_iteration
	return true;
}

void SweepMkImageCache();

bool Ctrl::ProcessEvents0(bool *quit, bool fetch)
{
	bool r = false;
	while(IsWaitingEvent0(fetch) && (!LoopCtrl || LoopCtrl->InLoop()))
		r = ProcessEvent0(quit, fetch) || r;
	TimerProc(GetTickCount());
	AnimateCaret();
	if(quit)
		*quit = IsEndSession();
	for(int i = 0; i < wins.GetCount(); i++)
		if(wins[i].ctrl)
			wins[i].ctrl->SyncScroll();
	gdk_window_process_all_updates();
	FetchEvents(FALSE); // To perform any pending GDK_EXPOSE
	gdk_flush();
	return r;
}

bool Ctrl::ProcessEvents(bool *quit)
{
	ASSERT_(IsMainThread(), "ProcessEvents can only run in the main thread");
	return ProcessEvents0(quit, true);
}

void Ctrl::SysEndLoop()
{
}

void WakeUpGuiThread()
{
	g_main_context_wakeup(g_main_context_default());
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT_(IsMainThread(), "EventLoop can only run in the main thread");
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	if(!GetMouseRight() && !GetMouseMiddle() && !GetMouseLeft())
		ReleaseCtrlCapture();
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	while(!IsEndSession() &&
	      (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount())) {
		FetchEvents(TRUE);
		ProcessEvents();
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

gboolean sOnce(GtkWidget *)
{
	return false;
}

void Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	ASSERT_(IsMainThread(), "Only the main thread can perform GuiSleep");
	if(ms < 20) // Periodic timer is each 20ms, so that is the longest possible wait
		g_timeout_add(ms, (GSourceFunc) sOnce, NULL); // otherwise setup shorter timer
	FetchEvents(TRUE);
}

END_UPP_NAMESPACE

#endif
