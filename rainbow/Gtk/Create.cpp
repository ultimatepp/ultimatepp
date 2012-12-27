#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)      DLOG(x)

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

	TopWindow *tw = dynamic_cast<TopWindow *>(this);
	gtk_window_set_type_hint(gtk(), popup ? GDK_WINDOW_TYPE_HINT_COMBO
	                                : tw && tw->tool ? GDK_WINDOW_TYPE_HINT_UTILITY
	                                : owner ? GDK_WINDOW_TYPE_HINT_DIALOG
	                                : GDK_WINDOW_TYPE_HINT_NORMAL);

	top->cursor_id = -1;

	gtk_widget_set_events(top->window, 0xffffffff);
	g_signal_connect(top->window, "event", G_CALLBACK(GtkEvent), this);
	
	gtk_widget_realize(top->window);

	WndSetPos0(GetRect());

	if(owner && owner->top)
		gtk_window_set_transient_for(gtk(), owner->gtk());
	gtk_widget_set_app_paintable(top->window, TRUE);
	gtk_widget_set_can_focus(top->window, TRUE);
	isopen = true;

	top->im_context = gtk_im_multicontext_new();
	gtk_im_context_set_client_window(top->im_context, gdk());
 	gtk_im_context_set_use_preedit(top->im_context, false);
	g_signal_connect(top->im_context, "commit", G_CALLBACK(IMCommit), this);

	WndShow(IsShown());

	Win& w = wins.Add();
	w.ctrl = this;
	w.gtk = top->window;
	w.gdk = top->window->window;
	FocusSync();
	if(!popup)
		SetWndFocus();
}

void Ctrl::WndDestroy0()
{
	GuiLock __;
	LLOG("WndDestroy " << Name());
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

Vector<Ctrl *> Ctrl::activePopup;

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
		SetFocus();
		activePopup.Add(this);
		StartGrabPopup();
		CheckMouseCtrl();
	}
	DDUMP(Upp::Name(owner));
	DDUMP(Upp::Name(GetOwner()));
}

END_UPP_NAMESPACE

#endif
