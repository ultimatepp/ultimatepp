#include "GtkApp.h"

#define IMAGECLASS TestImg
#define IMAGEFILE <GtkApp/Test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <GtkApp/Test.iml>
#include <Draw/iml_source.h>

static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GtkDraw w;
	w.cr = gdk_cairo_create(widget->window);

	w.DrawRect(0, 0, 100, 100, Yellow());

	w.Clipoff(50, 50, 30, 30);
	w.DrawImage(0, 0, TestImg::Test());
	w.End();

	w.DrawImage(50, 50 + 32, TestImg::Test(), Rect(24, 24, 10, 10));

	w.DrawImage(150, 50, TestImg::Test(), Red());
	w.DrawImage(150, 50 + 32, TestImg::Test(), Rect(24, 24, 10, 10), Red());
	
//	w.DrawText(20, 20, "Hello GTK!", Roman(50));


		w.Offset(150, 50);
		const char *text = "Programming is fun";
		Font fnt(Roman(60));
		FontInfo fi = fnt.Info();
		int x = 0;
		Vector<int> dx;
		for(const char *s = text; *s; s++) {
			int width = fi[*s];
			w.DrawRect(x, 0, width - 1, fi.GetAscent(), Color(255, 255, 200));
			w.DrawRect(x, fi.GetAscent(), width - 1, fi.GetDescent(), Color(255, 200, 255));
			w.DrawRect(x + width - 1, 0, 1, fi.GetHeight(), Black());
			dx.Add(width + 4);
			x += width;
		}
		w.DrawRect(0, 0, 4, 4, Black());
		w.DrawText(0, 0, text, fnt);
		w.DrawText(0, 70, text, fnt, Blue(), dx.GetCount(), dx.Begin());

	cairo_destroy(w.cr);
	
	return FALSE;
}

int main( int argc, char *argv[])
{
	GtkWidget *window;
	
	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_default_size((GtkWindow *)window, 1000, 600);

	GtkWidget *darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER (window), darea);

	g_signal_connect(darea, "expose-event", G_CALLBACK(on_expose_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);
	
	gtk_main();

	return 0;
}
