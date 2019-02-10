#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

DHCtrl::DHCtrl()
{
	fixed = widget = NULL;
}

DHCtrl::~DHCtrl()
{
	CloseGtk();
}

void DHCtrl::OpenGtk()
{
	Ctrl *top = GetTopCtrl();
	if(!top)
		return;
	DDUMP(top->gdk());
	fixed = gtk_fixed_new();
	gtk_fixed_set_has_window((GtkFixed *)fixed, true);
	gtk_container_add(GTK_CONTAINER(top->gtk()), fixed);
	gtk_widget_realize(fixed);
	gtk_widget_show(fixed);
	
	SetupWidget(fixed, NULL);
	DDUMP(fixed);
	DDUMP(gtk());
	DDUMP(gdk());
	
	widget = gtk_button_new_with_label("TEST");
	gtk_fixed_put((GtkFixed *)fixed, widget, 0, 0);
	gtk_widget_realize(widget);
	gtk_widget_show(widget);
}

void DHCtrl::SyncGtk()
{
	// optimize...
	if(fixed && widget) {
		Rect r = GetRect();
		gtk_widget_set_size_request(widget, r.GetWidth(), r.GetHeight());
		gtk_fixed_move((GtkFixed *)fixed, widget, r.left, r.top);
	}
}

void DHCtrl::CloseGtk()
{
	if(widget)
		gtk_widget_destroy(widget);
	if(fixed)
		WndDestroy();
	fixed = widget = NULL;
}

void DHCtrl::State(int reason)
{
	switch(reason) {
	case OPEN:
		OpenGtk();
	default:
		SyncGtk();
		break;
	case CLOSE:
		CloseGtk();
	}
}

};

#endif
