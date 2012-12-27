#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

//#include <winnls.h>

//#include "imm.h"

NAMESPACE_UPP

#define LLOG(x)     LOG(x)
#define LOG_EVENTS

static Point            sMousepos;
static GdkModifierType  sModmask;
static bool             sMouseposValid;

void syncMousePos()
{
	if(sMouseposValid)
		return;
	gint x, y;
	gdk_window_get_pointer(gdk_get_default_root_window(), &x, &y, &sModmask);
	sMousepos = Point(x, y);
}

void Ctrl::InvalidateMousePos()
{
	sMouseposValid = false;
}

bool  GetShift() { syncMousePos(); return sModmask & GDK_SHIFT_MASK; }
bool  GetCtrl() { syncMousePos(); return sModmask & GDK_CONTROL_MASK ; }
bool  GetAlt() { syncMousePos(); return sModmask & GDK_MOD1_MASK; }
bool  GetCapsLock() { syncMousePos(); return sModmask & GDK_LOCK_MASK; }
bool  GetMouseLeft() { syncMousePos(); return sModmask & GDK_BUTTON1_MASK; }
bool  GetMouseRight() { syncMousePos(); return sModmask & GDK_BUTTON3_MASK; }
bool  GetMouseMiddle() { syncMousePos(); return sModmask & GDK_BUTTON2_MASK; }
Point GetMousePos() { syncMousePos(); return sMousepos; }

void Ctrl::GtkMouseEvent(int action, GdkEvent *event)
{
	GdkEventButton *e = (GdkEventButton *)event;
	int act = action;
	if(action != MOUSEMOVE)
		act |= e->button == 2 ? MIDDLE : e->button == 3 ? RIGHT : LEFT;
	if(grabpopup && activePopup.GetCount()) {
		Point p = GetMousePos();
		for(int i = activePopup.GetCount() - 1; i >= 0; i--) {
			Ctrl *w = activePopup[i];
			Rect r = w->GetScreenRect();
			if(r.Contains(p)) {
				p -= r.TopLeft();
				w->DispatchMouse(act, p);
				return;
			}
		}
		if(action == DOWN) { // Deactivate active popup(s) if clicked outside of active popups
			IgnoreMouseUp();
			Ptr<TopWindow> w = activePopup.Top()->GetTopWindow();
			if(w)
				w->ActivateWnd();
			if(w) {
				Rect r = w->GetScreenRect();
				if(r.Contains(p)) {
					p -= r.TopLeft();
					w->DispatchMouse(MOUSEMOVE, p);
				}
			}
		}
		return;
	}
	DispatchMouse(act, Point((int)e->x, (int)e->y));
}

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

bool Ctrl::Proc(GdkEvent *event)
{
	if(!top)
		return false;
	Ptr<Ctrl> _this = this;
#ifdef LOG_EVENTS
	String ev = "?";
	Tuple2<int, const char *> *f = FindTuple(xEvent, __countof(xEvent), event->type);
	if(f)
		ev = f->b;
	LOG(rmsecs() << " EVENT " << Upp::Name(this) << " " << ev);
#endif
	GdkEventKey *key;
	bool pressed = false;
	int kv;
	InvalidateMousePos();
	switch(event->type) {
	case GDK_DELETE: {
		TopWindow *w = dynamic_cast<TopWindow *>(this);
		if(w) {
			if(IsEnabled()) {
				IgnoreMouseUp();
				w->WhenClose();
			}
			return true;
		}
		return false;
	}
	case GDK_FOCUS_CHANGE:
		LLOG("FocusChange in: " << ((GdkEventFocus *)event)->in << ", focusCtrlWnd " << focusCtrlWnd);
		if(((GdkEventFocus *)event)->in) {
			gtk_im_context_focus_in(top->im_context);
			if(this != focusCtrlWnd) {
				LLOG("Activate " << Name());
				SetFocusWnd();
			}
		}
		else {
			gtk_im_context_focus_out(top->im_context);
			KillFocusWnd();
		}
		return true;
	case GDK_EXPOSE:
	case GDK_DAMAGE:
		if(top) {
			fullrefresh = false;
			GdkEventExpose *e = (GdkEventExpose *)event;
			SystemDraw w(gdk_cairo_create(gdk()));
			painting = true;
			DDUMP(RectC(e->area.x, e->area.y, e->area.width, e->area.height));
			UpdateArea(w, RectC(e->area.x, e->area.y, e->area.width, e->area.height));
			cairo_destroy(w);
			if(top->dr)
				DrawDragRect(*this, *top->dr);
			painting = false;
		}
		return true;
	case GDK_MOTION_NOTIFY:
		GtkMouseEvent(MOUSEMOVE, event);
		DoCursorShape();
		break;
	case GDK_BUTTON_PRESS:
		if(!HasWndFocus() && !popup)
			SetWndFocus();
		ClickActivateWnd();
		GtkMouseEvent(DOWN, event);
		break;
	case GDK_2BUTTON_PRESS:
		GtkMouseEvent(DOUBLE, event);
		break;
	case GDK_BUTTON_RELEASE:
		if(ignoreclick)
			EndIgnore();
		else
			GtkMouseEvent(UP, event);
		break;
	case GDK_KEY_PRESS:
		pressed = true;
		key = (GdkEventKey *)event;
	case GDK_KEY_RELEASE:
		key = (GdkEventKey *)event;
		kv = key->keyval;
		if(gtk_im_context_filter_keypress(top->im_context, key))
			break;
		if(kv >= 0 && kv < 65536) {
			DDUMP(pressed);
			LOG(FormatIntHex(kv) << ' ' << (char)kv);
			DDUMP(key->state & GDK_CONTROL_MASK);
			if(kv >= 'a' && kv <= 'z')
				kv = kv - 'a' + 'A';
			static Tuple2<int, int> cv[] = {
				{ GDK_KEY_BackSpace, K_BACKSPACE },
				{ GDK_KEY_Tab, K_TAB },
				{ GDK_KEY_Return, K_ENTER },
				{ GDK_KEY_Escape, K_ESCAPE },
			};
			Tuple2<int, int> *x = FindTuple(cv, __countof(cv), kv);
			if(x)
				kv = x->b;
			else
				kv += K_DELTA;
			if(key->state & GDK_SHIFT_MASK)
				kv |= K_SHIFT;
			if(key->state & GDK_CONTROL_MASK)
				kv |= K_CTRL;
			if(key->state & GDK_MOD1_MASK)
				kv |= K_ALT;
			LLOG(GetKeyDesc(kv) << ", pressed: " << pressed);
			DispatchKey(!pressed * K_KEYUP + kv, 1);
		}
		break;
	case GDK_CONFIGURE: {
		GdkEventConfigure *e = (GdkEventConfigure *)event;
		Rect rect = RectC(e->x, e->y, e->width, e->height);
		LLOG("ConfigureNotify " << rect << ", GetRect() " << GetRect());
		if(GetRect() != rect)
			SetWndRect(rect);
		// TODO: Add DHCtrl support
		}
		return false; // 'return true' leads to size problems
	default:
		return false;
	}
	if(_this)
		_this->PostInput();
	return true;
}

gboolean Ctrl::TimeHandler(GtkWidget *)
{
	InvalidateMousePos();
	TimerProc(GetTickCount());
	AnimateCaret();
	return true;
}

gboolean Ctrl::GtkEvent(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	return ((Ctrl *)(user_data))->Proc(event);
}

void Ctrl::IMCommit(GtkIMContext *context, gchar *str, gpointer user_data)
{
	WString s = FromUtf8(str);
	for(int i = 0; i < s.GetCount(); i++)
		((Ctrl *)(user_data))->DispatchKey(s[i], 1);
}

bool Ctrl::IsWaitingEvent()
{
	GDK_THREADS_LEAVE ();  
	bool result = g_main_context_pending(NULL);
	GDK_THREADS_ENTER ();
	return result;
}

bool Ctrl::ProcessEvent(bool *quit)
{
	ASSERT(IsMainThread());
	bool r = false;
	if(IsWaitingEvent()) {
		g_main_context_iteration(NULL, FALSE);
		if(quit)
			*quit = IsEndSession();
		r = true;
	}
	TimerProc(GetTickCount());
	SyncCaret();
	AnimateCaret();
	gdk_flush();
	return r;
}

void SweepMkImageCache();

bool Ctrl::ProcessEvents(bool *quit)
{
	bool r = false;
	while(IsWaitingEvent() && (!LoopCtrl || LoopCtrl->InLoop()))
		r = ProcessEvent(quit) || r;
	return r;
}

void Ctrl::SysEndLoop()
{
}

void WakeUpGuiThread()
{
	g_main_context_wakeup(g_main_context_default());
}

void Ctrl::EventLoop0(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
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

	GMainLoop *loop = g_main_loop_new(NULL, TRUE);

	while(!IsEndSession() && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
		GDK_THREADS_LEAVE();
		g_main_context_iteration(NULL, TRUE);
//		DLOG("g_main_context_iteration " << msecs());
		GDK_THREADS_ENTER();
		for(int i = 0; i < wins.GetCount(); i++) {
			Ctrl *q = wins[i].ctrl;
			if(q) q->SyncScroll();
		}
		TimerProc(GetTickCount());
		SyncCaret();
		AnimateCaret();
		CheckMouseCtrl();
		gdk_flush();
	}

	g_main_loop_unref(loop);

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ctrl::GuiSleep0(int ms)
{
	GuiLock __;
}

END_UPP_NAMESPACE

#endif
