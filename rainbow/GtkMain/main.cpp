#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

void TestDraw(Draw& w)
{
	w.DrawRect(0, 0, 10000, 10000, WhiteGray());
	w.DrawRect(0, 0, 100, 100, Yellow());
	
	w.DrawLine(10, 40, 400, 20, 3, Blue());
	
	w.DrawImage(0, 0, CtrlImg::exclamation());

	w.Clipoff(50, 50, 30, 30);
	w.DrawImage(0, 0, CtrlImg::exclamation());
	w.End();

	w.DrawImage(50, 50 + 32, CtrlImg::exclamation(), RectC(24, 24, 10, 10));

	w.DrawImage(150, 50, CtrlImg::exclamation(), Red());
	w.DrawImage(150, 50 + 32, CtrlImg::exclamation(), RectC(24, 24, 10, 10), Red());
	
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
	
	w.DrawRect(50, 400, 2, 2, Black());
	w.DrawText(50, 400, 200, "Angled text", Arial(50).Underline(), Red());

	w.DrawRect(600, 400, 2, 2, Black());
	w.DrawText(600, 400, 1000, "Angled text", Arial(50).Underline(), Black());
	w.End();
}

static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	SystemDraw w(gdk_cairo_create(widget->window));

	TestDraw(w);

	cairo_destroy(w);
	
	return FALSE;
}

gboolean CtrlEvent(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
	Ctrl *ctrl = (Ctrl *)user_data;
	DDUMP((int)event->type);
	switch(event->type) {
	case GDK_MOTION_NOTIFY: {
		GdkEventMotion *e = (GdkEventMotion *)event;
		DLOG("Motion");
		DDUMP(e->x);
		DDUMP(e->y);
		DDUMP(e->state);
		return true;
	}
	case GDK_BUTTON_PRESS:
		DLOG("EndLoop!");
		ctrl->EndLoop();
		return true;
	default:;
	}
	return false;
}

struct MyApp : TopWindow {
	Point pos;

	virtual void MouseMove(Point p, dword keyflags)
	{
		pos = p;
		DDUMP(pos);
		DDUMP(IsVisible());
		DDUMP(IsOpen());
		Refresh();
	}
	
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawText(10, 10, AsString(pos));
	}
};

CONSOLE_APP_MAIN
{
	Ctrl myapp;

	{
		ImageDraw w(1000, 1000);
		TestDraw(w);
		PNGEncoder().SaveFile("/home/cxl/test.png", w);
	}

	{
		ImageDraw w(1000, 1000);
		TestDraw(w);
		for(int i = 0; i < 100; i++)
			w.Alpha().DrawRect(i * 10, 0, 10, 1000, GrayColor(255 - i));
		PNGEncoder().SaveFile("/home/cxl/test_alpha.png", w);
	}
	
//	gtk_init(&argc, &argv);
	gtk_init(0, NULL);
	Ctrl::GlobalBackBuffer();

#if 1
//	MyApp().Run();
//	return;

	PromptOK("Hello world!");
	return;
	
	RichEditWithToolBar edit;
	edit.SetQTF("[A500 Hello World!");
	TopWindow win;
	win.Add(edit.SizePos());
	win.Run();

//	MyApp().Run();
#else
	GtkWidget *window;
	
//	window = gtk_window_new(GTK_WINDOW_POPUP);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_default_size((GtkWindow *)window, 1000, 600);

	GtkWidget *darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER (window), darea);


	gtk_widget_set_events(darea, 0xffffffff);
	g_signal_connect(darea, "expose-event", G_CALLBACK(on_expose_event), &myapp);
	g_signal_connect(darea, "event", G_CALLBACK(CtrlEvent), &myapp);
//	g_signal_connect(window, "expose-event", G_CALLBACK(on_expose_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);

	Ctrl::EventLoop(&myapp);	
//	gtk_main();
#endif
}
