#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <gtk/gtk.h>



static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;
	
	cr = gdk_cairo_create(widget->window);
	
	cairo_move_to(cr, 30, 30);
	cairo_show_text(cr, "Hello world!");
	
	cairo_destroy(cr);
	
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
