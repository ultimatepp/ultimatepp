#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

bool GtkCSD::IsSSDSupported()
{
	if (Ctrl::IsX11()) {
		return false;
	}
	
	// TODO: Rewrite to negotiate with window manager once XDG decoration protocol will be
	// stable. (https://wayland.app/protocols/xdg-decoration-unstable-v1)
	
	// NOTE: Server side decoration are optional. It might be supported by Window manager or
	// not. Let's have a list of desktop environments on which we tested it works correctly.
	auto desktop = GetEnv("XDG_SESSION_DESKTOP");
	if (desktop == "KDE") {
		return true;
		//return true;
	}
	
	return false;
}

void GtkCSD::Create(GdkWindowTypeHint hint)
{
	enabled = false;
	left = right = top = bottom = 0;
	if(!Ctrl::IsWayland())
		return;
	if(IsSSDSupported() && hint != GDK_WINDOW_TYPE_HINT_POPUP_MENU ||
	   hint == GDK_WINDOW_TYPE_HINT_COMBO)
		return;
	
	enabled = true;
	GtkWidget* win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* header;
	if (findarg(hint, GDK_WINDOW_TYPE_HINT_POPUP_MENU) >= 0) {
		header = gtk_drawing_area_new();
		gtk_widget_set_size_request(header, 1, 1);
		gtk_window_set_titlebar((GtkWindow *)win, header);
	} else {
		header = gtk_header_bar_new();
		gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
		gtk_window_set_titlebar((GtkWindow *)win, header);
	}
	GtkWidget* drawing_area = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), drawing_area);
	gtk_widget_show_all(win);
	
	gdk_window_get_origin(gtk_widget_get_window(drawing_area), &left, &top);
	
	gint drawing_area_width = gtk_widget_get_allocated_width(drawing_area);
	gint drawing_area_height = gtk_widget_get_allocated_height(drawing_area);
	
	gint win_width = gtk_widget_get_allocated_width(win);
	gint win_height = gtk_widget_get_allocated_height(win);
	
	right = win_width - left - drawing_area_width;
	bottom = win_height - top - drawing_area_height;
	
	left = Ctrl::SCL(left);
	right = Ctrl::SCL(right);
	bottom = Ctrl::SCL(bottom);
	top = Ctrl::SCL(top);
	
	gtk_widget_destroy(drawing_area);
	gtk_widget_destroy(header);
	gtk_widget_destroy(win);
}

};

#endif
