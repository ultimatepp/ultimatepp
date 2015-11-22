#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)   //   DLOG(x)

void Ctrl::Create(Ctrl *owner, bool popup)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("Create " << Name() << " " << GetRect());
	ASSERT(!IsChild() && !IsOpen());
	LLOG("Ungrab1");

	top = new Top;
	top->window = gtk_window_new(popup ? GTK_WINDOW_POPUP : GTK_WINDOW_TOPLEVEL);
	top->owner = owner;
	
	static int id;
	top->id = ++id;

	Win& w = wins.Add();
	w.id = top->id;
	w.ctrl = this;
	w.gtk = top->window;
	w.gdk = top->window->window;

	TopWindow *tw = dynamic_cast<TopWindow *>(this);
	gtk_window_set_type_hint(gtk(), popup ? GDK_WINDOW_TYPE_HINT_COMBO
	                                : tw && tw->tool ? GDK_WINDOW_TYPE_HINT_UTILITY
	                                : owner ? GDK_WINDOW_TYPE_HINT_DIALOG
	                                : GDK_WINDOW_TYPE_HINT_NORMAL);

	top->cursor_id = -1;

	gtk_widget_set_events(top->window, 0xffffffff);
	g_signal_connect(top->window, "event", G_CALLBACK(GtkEvent), (gpointer)(uintptr_t)top->id);
	
	gtk_widget_realize(top->window);

	Rect r = GetRect();
	gtk_window_move(gtk(), r.left, r.top);
	gtk_window_resize(gtk(), r.GetWidth(), r.GetHeight());

	if(owner && owner->top)
		gtk_window_set_transient_for(gtk(), owner->gtk());
	gtk_widget_set_app_paintable(top->window, TRUE);
	gtk_widget_set_can_focus(top->window, TRUE);
	isopen = true;

	top->im_context = gtk_im_multicontext_new();
	gtk_im_context_set_client_window(top->im_context, gdk());
 	gtk_im_context_set_use_preedit(top->im_context, false);
	g_signal_connect(top->im_context, "commit", G_CALLBACK(IMCommit), (gpointer)(uintptr_t)top->id);

	WndShow(IsShown());

	FocusSync();
	if(!popup)
		SetWndFocus();
	
	DndInit();
	
	StateH(OPEN);
}

void Ctrl::WndDestroy0()
{
	GuiLock __;
	LLOG("WndDestroy " << Name());
	DndExit();
	gtk_widget_destroy(top->window);
	g_object_unref(top->im_context);
	isopen = false;
	popup = false;
	delete top;
	top = NULL;
	int q = FindCtrl(this);
	if(q >= 0)
		wins.Remove(q);
}

Vector< Ptr<Ctrl> > Ctrl::activePopup;

void Ctrl::GuiPlatformRemove()
{
	for(int i = 0; i < activePopup.GetCount();)
		if(activePopup[i] == this) {
			if(this == grabpopup && gdk_pointer_is_grabbed())
				StopGrabPopup();
			activePopup.Remove(i);
			StartGrabPopup();
		}
		else
			i++;
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	GuiLock __;
	LLOG("POPUP " << Name() << ", " << GetRect() << ", activate " << activate);
	Create(owner, true);
	popup = true;
	if(activate) {
		Ptr<Ctrl> _this = this;
		SetFocus();
		if(_this) {
			activePopup.Add(this);
			StartGrabPopup();
			CheckMouseCtrl();
		}
	}
}

END_UPP_NAMESPACE

#endif
