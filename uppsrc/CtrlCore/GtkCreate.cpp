#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

#define LLOG(x)    //  DLOG(x)

void Ctrl::Create(Ctrl *owner, bool popup)
{
	MemoryIgnoreLeaksBlock ___;
	GuiLock __;
	ASSERT_(IsMainThread(), "Only the main thread can open a window");
	LLOG("Create " << Name() << " " << GetRect());
	ASSERT(!IsChild() && !IsOpen());
	LLOG("Ungrab1");

	Top *top = new Top;
	SetTop(top);
	top->window = gtk_window_new(popup && owner ? GTK_WINDOW_POPUP : GTK_WINDOW_TOPLEVEL);
	top->owner = owner;

	static int id;
	top->id = ++id;

	Win& w = wins.Add();
	w.id = top->id;
	w.ctrl = this;
	w.gtk = top->window;
	w.gdk = nullptr;
	w.drawing_area = nullptr;

	TopWindow *tw = dynamic_cast<TopWindow *>(this);
	GdkWindowTypeHint type_hint;
	if(popup && !owner) {
		gtk_window_set_decorated(gtk(), FALSE);
		type_hint = GDK_WINDOW_TYPE_HINT_POPUP_MENU;
	}
	else {
		type_hint = popup ? GDK_WINDOW_TYPE_HINT_COMBO
		                  : tw && tw->tool ? GDK_WINDOW_TYPE_HINT_UTILITY
		                  : owner ? GDK_WINDOW_TYPE_HINT_DIALOG
		                  : GDK_WINDOW_TYPE_HINT_NORMAL;
	}
	gtk_window_set_type_hint(gtk(), type_hint);

	Rect r = GetRect();
	
	top->csd.Create(type_hint);
	if(top->csd.IsEnabled()) {
		top->header = gtk_header_bar_new();
		if (findarg(type_hint, GDK_WINDOW_TYPE_HINT_POPUP_MENU) >= 0)
			gtk_widget_set_size_request(top->header, 1, 1);
		else
			gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(top->header), TRUE);

		gtk_window_set_titlebar(gtk(), top->header);
		gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(top->header), false);
		
		top->drawing_area = gtk_drawing_area_new();
		gtk_widget_set_can_focus(top->drawing_area, TRUE);

		r.left -= top->csd.LeftMargin();
		r.right += top->csd.RightMargin();
		r.top -= top->csd.TopMargin();
		r.bottom += top->csd.BottomMargin();

		g_signal_connect(top->window, "delete-event", G_CALLBACK(GtkEvent), (gpointer)(uintptr_t)top->id);
	}
	else {
		top->drawing_area = top->window;
	}

	w.drawing_area = top->drawing_area;
	
	top->cursor_id = -1;

	gtk_widget_set_events(top->drawing_area, GDK_ALL_EVENTS_MASK & ~GDK_POINTER_MOTION_HINT_MASK & ~GDK_SMOOTH_SCROLL_MASK);
	g_signal_connect(top->drawing_area, "event", G_CALLBACK(GtkEvent), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->drawing_area, "draw", G_CALLBACK(GtkDraw), (gpointer)(uintptr_t)top->id);

	GdkWindowTypeHint hint = gtk_window_get_type_hint(gtk());
	if(tw && findarg(hint, GDK_WINDOW_TYPE_HINT_NORMAL, GDK_WINDOW_TYPE_HINT_DIALOG, GDK_WINDOW_TYPE_HINT_UTILITY) >= 0)
		tw->SyncSizeHints();

	gtk_window_set_default_size(gtk(), LSC(r.GetWidth()), LSC(r.GetHeight()));
	gtk_window_move(gtk(), LSC(r.left), LSC(r.top));
	gtk_window_resize(gtk(), LSC(r.GetWidth()) - top->csd.ExtraWidth(),
	                  LSC(r.GetHeight()) - top->csd.ExtraHeight());
		
	if (top->header) {
		gtk_container_add(GTK_CONTAINER(top->window), top->drawing_area);
		gtk_widget_show_all(top->window);
	} else {
		gtk_widget_realize(top->window);
	}
	
	w.gdk = gtk_widget_get_window(top->window);
	
	if(owner && owner->top)
		gtk_window_set_transient_for(gtk(), owner->gtk());
	gtk_widget_set_app_paintable(top->window, TRUE);
	gtk_widget_set_can_focus(top->window, TRUE);
	isopen = true;

	top->im_context = gtk_im_multicontext_new();
	gtk_im_context_set_client_window(top->im_context, gdk());
	gtk_im_context_set_use_preedit(top->im_context, true);
	g_signal_connect(top->im_context, "preedit-changed", G_CALLBACK(IMPreedit), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->im_context, "preedit-start", G_CALLBACK(IMPreedit), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->im_context, "preedit-end", G_CALLBACK(IMPreeditEnd), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->im_context, "commit", G_CALLBACK(IMCommit), (gpointer)(uintptr_t)top->id);

	WndShow(IsShown());

	SweepConfigure(true);
	FocusSync();
	if(!popup)
		SetWndFocus();

	activeCtrl = this;

	DndInit();

	StateH(OPEN);

	GdkModifierType mod;
	Point m = GetMouseInfo(gdk(), mod);
	r = GetWndScreenRect().GetSize();
	if(r.Contains(m))
		DispatchMouse(MOUSEMOVE, m);

	RefreshLayoutDeep();
}

void Ctrl::WndDestroy()
{
	GuiLock __;
	LLOG("WndDestroy " << Name());
	DndExit();
	Ctrl *owner = GetOwner();
	if(owner && owner->top) {
		for(int i = 0; i < wins.GetCount(); i++)
			if(wins[i].ctrl && wins[i].ctrl->GetOwner() == this && wins[i].gtk) {
				LLOG("Changing owner");
				gtk_window_set_transient_for((GtkWindow *)wins[i].gtk, owner->gtk());
			}
		if(HasFocusDeep() || !GetFocusCtrl())
			activeCtrl = owner;
	}
	Top *top = GetTop();
	if(top->im_context) {
		g_object_unref(top->im_context);
		top->im_context = nullptr;
	}
	gtk_widget_destroy(top->window);
	top->window = nullptr;
	isopen = false;
	popup = false;
	DeleteTop();
	top = NULL;
	int q = FindCtrl(this);
	if(q >= 0)
		wins.Remove(q);
	if(owner) {
		if(owner->top && owner->utop && owner->utop->csd.IsEnabled()) {
			// TODO: This fix the problem with keyboard when going back to original window, but
			// the previous control is not being focused like it should be.
			gtk_window_set_focus(owner->gtk(), owner->utop->drawing_area);
		}
		owner->WndUpdate();
	}
	TopWindow *w = dynamic_cast<TopWindow *>(this);
	if(w && w->overlapped.GetWidth() && w->overlapped.GetHeight())
		SetRect(w->overlapped);
}

Vector< Ptr<Ctrl> > Ctrl::activePopup;
Vector< Ptr<Ctrl> > Ctrl::visiblePopup;

void Ctrl::GuiPlatformRemove()
{
	for(int i = 0; i < visiblePopup.GetCount();)
		if(visiblePopup[i] == this)
			visiblePopup.Remove(i);
		else
			i++;
	for(int i = 0; i < activePopup.GetCount();)
		if(activePopup[i] == this) {
			if(this == grabpopup && MouseIsGrabbed())
				StopGrabPopup();
			activePopup.Remove(i);
			StartGrabPopup();
		}
		else
			i++;
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool, bool)
{
	GuiLock __;
	LLOG("POPUP " << Name() << ", " << GetRect() << ", activate " << activate);
	Create(owner ? owner->GetTopCtrl() : GetActiveCtrl(), true);
	popup = true;
	Ptr<Ctrl> _this = this;
	if(activate) {
		SetFocus();
		if(_this) {
			activePopup.Add(this);
			StartGrabPopup();
			CheckMouseCtrl();
		}
	}
	if(_this)
		visiblePopup.Add(this);
}


}

#endif
