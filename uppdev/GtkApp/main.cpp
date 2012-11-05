#include "GtkApp.h"

static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GtkDraw w;
	w.cr = gdk_cairo_create(widget->window);

	w.DrawRect(0, 0, 100, 100, LtBlue());

	w.Clipoff(50, 50, 30, 30);
	w.DrawImage(0, 0, CtrlImg::exclamation());
	w.End();

	w.DrawImage(50, 50 + 32, CtrlImg::exclamation(), Rect(24, 24, 10, 10));

	w.DrawImage(150, 50, CtrlImg::exclamation(), Red());
	w.DrawImage(150, 50 + 32, CtrlImg::exclamation(), Rect(24, 24, 10, 10), Red());
	
	w.DrawText(0, 0, "Hello GTK!", Roman(30));
	
	cairo_destroy(w.cr);
	
	return FALSE;
}

int main( int argc, char *argv[])
{
	GtkWidget *window;
	
	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	GtkWidget *darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER (window), darea);

	g_signal_connect(darea, "expose-event", G_CALLBACK(on_expose_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);
	
	gtk_main();

	return 0;
}
