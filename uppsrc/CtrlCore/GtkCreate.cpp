#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

#define LLOG(x)    //  DLOG(x)

void Ctrl::SetCustomBarColor(Color c)
{
	GuiLock __;

	Top *top = GetTop();
	if(!top || !top->header)
		return;

	String new_css_class;

	static VectorMap<Color, GtkCssProvider *> providers;

	int q = providers.Find(c);
	auto ClassName = [&] { return "upp_custom_titlebar_" + AsString(q); };
	if(q < 0) {
		if(providers.GetCount() > 64)
			Panic("Too many custom bar colors !");
		q = providers.GetCount();
		String css;
		css << "." << ClassName() << " { background-image: none; background-color: "
		    << Format("#%02x%02x%02x;", c.GetR(), c.GetG(), c.GetB()) << "; }";
		GtkCssProvider *provider = gtk_css_provider_new();
		gtk_css_provider_load_from_data (provider, css, -1, NULL);
		gtk_style_context_add_provider_for_screen(gtk_widget_get_screen(top->window),
		                                          GTK_STYLE_PROVIDER (provider),
		                                          GTK_STYLE_PROVIDER_PRIORITY_USER);
		providers.Add(c, provider);
	}
	new_css_class = ClassName();

	if(top->bar_css_class == new_css_class)
		return;

	if(top->bar_css_class.GetCount())
		gtk_style_context_remove_class (gtk_widget_get_style_context (top->header), ~top->bar_css_class);

	top->bar_css_class = new_css_class;
	gtk_style_context_add_class (gtk_widget_get_style_context (top->header), new_css_class);
}

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
	
	TopWindow *tw = dynamic_cast<TopWindow *>(this);
	if(popup) {
		gtk_window_set_decorated(gtk(), FALSE);
		if(IsWayland() && !owner) {
			gtk_window_set_titlebar(gtk(), gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
		}
	}
	gtk_window_set_type_hint(gtk(),
	            popup ? /*owner ? GDK_WINDOW_TYPE_HINT_COMBO : */GDK_WINDOW_TYPE_HINT_POPUP_MENU
	                  : tw && tw->tool ? GDK_WINDOW_TYPE_HINT_UTILITY
	                  : owner ? GDK_WINDOW_TYPE_HINT_DIALOG
	                  : GDK_WINDOW_TYPE_HINT_NORMAL);

	Rect r = GetRect();
	bool custom_bar = tw && tw->custom_bar;
	static bool need_csd = IsWayland() && GetEnv("XDG_SESSION_DESKTOP") != "KDE";
	top->csd = !popup && (need_csd || custom_bar);
#ifndef flagFORCE_CSD // Force using client side decorations even when server side is available
	if(tw && tw->force_csd)
#endif
		top->csd = !popup;
	if(top->csd) {
		ONCELOCK {
			UpdateWindowDecorationsGeometry();
		}
		top->client = gtk_drawing_area_new();
		if(custom_bar) {
		    top->header = gtk_event_box_new ();
			top->header_area = gtk_drawing_area_new();
			gtk_widget_set_size_request(top->header_area, -1, LSCH(tw->GetCustomTitleBarMetrics().height));
			gtk_container_add (GTK_CONTAINER (top->header), top->header_area);
		    gtk_widget_show_all(top->header);
		    
		    SetCustomBarColor(tw->custom_titlebar_bk);
		}
		else {
			top->header = gtk_header_bar_new();
			gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(top->header), TRUE);
			gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(top->header), false);
		}

		gtk_window_set_titlebar(gtk(), top->header);

		gtk_widget_set_can_focus(top->client, TRUE);

		gtk_container_add(GTK_CONTAINER(top->window), top->client);

		g_signal_connect(top->window, "delete-event", G_CALLBACK(GtkEvent), (gpointer)(uintptr_t)top->id);
	}
	else {
		ONCELOCK {
			UpdateWindowFrameMargins();
		}
		top->client = top->window;
	}

	top->cursor_id = -1;

	auto SetupEvents = [&](GtkWidget *w) {
		gtk_widget_set_events(w, GDK_ALL_EVENTS_MASK & ~GDK_POINTER_MOTION_HINT_MASK & ~GDK_SMOOTH_SCROLL_MASK);
	};
	SetupEvents(GTK_WIDGET(gtk()));
	g_signal_connect(gtk(), "event", G_CALLBACK(TopGtkEvent), (gpointer)(uintptr_t)top->id);
	
	auto SetupEvents2 = [&](GtkWidget *w) {
		SetupEvents(w);
		g_signal_connect(w, "event", G_CALLBACK(GtkEvent), (gpointer)(uintptr_t)top->id);
		g_signal_connect(w, "draw", G_CALLBACK(GtkDraw), (gpointer)(uintptr_t)top->id);
	};

	SetupEvents2(top->client);

	if(top->header_area)
		SetupEvents2(top->header_area);

	if(tw)
		if(findarg(gtk_window_get_type_hint(gtk()), GDK_WINDOW_TYPE_HINT_NORMAL, GDK_WINDOW_TYPE_HINT_DIALOG, GDK_WINDOW_TYPE_HINT_UTILITY) >= 0)
			tw->SyncSizeHints();

	if(IsWayland() || popup) {
		gtk_window_set_default_size(gtk(), LSC(r.GetWidth()), LSC(r.GetHeight()));
		gtk_window_move(gtk(), LSC(r.left), LSC(r.top));
		gtk_window_resize(gtk(), LSC(r.GetWidth()), LSC(r.GetHeight()));
	}
	
	if(IsWayland() && popup)
		gtk_widget_realize(top->window);
	else
		gtk_widget_show_all(top->window);

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

	WndSetPos(r);
	WndShow(IsShown());

	SweepConfigure(true);
	FocusSync();
	if(!popup)
		SetWndFocus();

	activeCtrl = this;

	DndInit();

	StateH(OPEN);

	Point m = prev_mouse_pos;
	r = GetWndScreenRect();
	if(r.Contains(m))
		DispatchMouse(MOUSEMOVE, m);

	top->sync_rect = true;
	WndRectsSync();
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
		if(owner->top && owner->utop && owner->utop->csd) {
			// TODO: This fix the problem with keyboard when going back to original window, but
			// the previous control is not being focused like it should be.
			gtk_window_set_focus(owner->gtk(), owner->utop->client);
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

