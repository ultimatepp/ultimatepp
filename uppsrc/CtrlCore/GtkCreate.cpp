#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)    //  DLOG(x)

void Ctrl::Create(Ctrl *owner, bool popup)
{
	MemoryIgnoreLeaksBlock ___;
	GuiLock __;
	ASSERT_(IsMainThread(), "Only the main thread can open a window");
	LLOG("Create " << Name() << " " << GetRect());
	ASSERT(!IsChild() && !IsOpen());
	LLOG("Ungrab1");

	top = new Top;
	top->window = gtk_window_new(popup && owner ? GTK_WINDOW_POPUP : GTK_WINDOW_TOPLEVEL);
	top->owner = owner;
	
	static int id;
	top->id = ++id;

	Win& w = wins.Add();
	w.id = top->id;
	w.ctrl = this;
	w.gtk = top->window;
	w.gdk = top->window->window;

	TopWindow *tw = dynamic_cast<TopWindow *>(this);
	if(popup && !owner) {
		gtk_window_set_decorated(gtk(), FALSE);
		gtk_window_set_has_frame(gtk(), FALSE);
		gtk_window_set_type_hint(gtk(), GDK_WINDOW_TYPE_HINT_POPUP_MENU);
	}
	else
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
	
	SweepConfigure(true);
	
	DndInit();
	
	StateH(OPEN);

	activeCtrl = this;

	int mousex, mousey;
	gdk_window_get_pointer(gdk(), &mousex, &mousey, NULL);
	Point m(mousex, mousey);
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
		if(HasFocusDeep())
			activeCtrl = owner;
	}
	g_object_unref(top->im_context);
	gtk_widget_destroy(top->window);
	isopen = false;
	popup = false;
	delete top;
	top = NULL;
	int q = FindCtrl(this);
	if(q >= 0)
		wins.Remove(q);
	if(owner)
		owner->WndUpdate();
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

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool, bool)
{
	GuiLock __;
	LLOG("POPUP " << Name() << ", " << GetRect() << ", activate " << activate);
	Create(owner ? owner->GetTopCtrl() : GetActiveCtrl(), true);
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
