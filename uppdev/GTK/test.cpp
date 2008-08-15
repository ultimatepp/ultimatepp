#include <CtrlLib/CtrlLib.h>

//#include <gtk/gtk.h>
//#include <gdk/gdkx.h>
//#include <gdk/gdkprivate.h>

enum {
	GTK_BOX,
	GTK_CHECK,
	GTK_OPTION,
	GTK_ARROW,
	GTK_SLIDER,
	GTK_ICON,

	GTK_MARGIN1 = 0x0010,
	GTK_MARGIN2 = 0x0010,
	GTK_MARGIN3 = 0x0010,
	GTK_MARGIN4 = 0x0010,

	GTK_VAL1    = 0x0100,

	GTK_TOP     = 0x1000,
	GTK_VCENTER = 0x2000,
	GTK_BOTTOM  = 0x3000,
	GTK_LEFT    = 0x4000,
	GTK_HCENTER = 0x5000,
	GTK_RIGHT   = 0x6000,
};

#ifdef CPU_64
#define GTK__ALLOCATION_OFFSET 64
#define GTK__COLOR_OFFSET      24
#else
#define GTK__ALLOCATION_OFFSET 36
#define GTK__COLOR_OFFSET      12
#endif


G_obj *gtk__parent();
G_obj *Setup(G_obj *widget);
Image GetGTK(G_obj *widget, int state, int shadow, const char *detail, int type, int cx, int cy);

G_obj *Get(G_obj *(*widget)())
{
	static VectorMap<uintptr_t, G_obj *> wmap;
	G_obj *w = wmap.Get((uintptr_t)widget, NULL);
	if(!w) {
		w = (*widget)();
		if(GTK().gtk_widget_get_parent(w) == NULL)
			Setup(w);
		wmap.Add((uintptr_t)widget, w);
	}
	return w;
}

Image GetGTK(G_obj *(*widget)(), int state, int shadow, const char *detail, int type, int cx, int cy)
{
	Image m = GetGTK(Get(widget), state, shadow, detail, type, cx, cy);
	return m;
}

G_obj *gtk__vscrollbar()
{
	G_obj *adj = GTK().gtk_adjustment_new(250, 0, 1000, 1, 1, 500);
	G_obj *w = GTK().gtk_vscrollbar_new(adj);
	GOBJ().g_object_unref(adj);
	return w;
}

G_obj *gtk__defaultbutton()
{
	G_obj *w = GTK().gtk_button_new();
	Setup(w);
	GTK().gtk_widget_set(w, "can-default", true, NULL);
	GTK().gtk_window_set_default(gtk__parent(), w);
	return w;
}

int  GTKInt(G_obj *widget, const char *id);

G_obj *gtk__radiobutton() {
	return GTK().gtk_radio_button_new(NULL);
}

struct Test : TopWindow {
	EditIntSpin n;
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		int isz = 16;
		w.DrawRect(GetSize(), Cyan());
		//w.DrawRect(GetSize(), White());
		G_color *c = (G_color *)(24 + (byte *)GTK().gtk_widget_get_style(gtk__parent()));
     	for(int b = 0; b < 8; b++)
    		for(int a = 0; a < 8; a++) {
    			Image m;
    			if(b < 5)
	    			switch((int)~n) {
//					case 0: m = GetGTK(gtk__radiobutton, a, b, "radiobutton", GTK_OPTION|GTK_MARGIN1, isz, isz); break;
/*					case 1: m = GetGTK(GTK().gtk_check_button_new, a, b, "checkbutton", GTK_CHECK|GTK_MARGIN1, isz, isz); break;
					case 2: m = GetGTK(GTK().gtk_button_new, a, b, "button", GTK_BOX|GTK_MARGIN1, 64, 24); break;
					case 3: m = GetGTK(gtk__defaultbutton, a, b, "button", GTK_BOX|GTK_MARGIN1, 64, 24); break;
					case 4: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_BOX|GTK_TOP|GTK_MARGIN1, 14, 14); break;
					case 5: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_BOX|GTK_BOTTOM|GTK_MARGIN1, 14, 14); break;
					case 6: m = GetGTK(gtk__vscrollbar, a, b, "trough", GTK_BOX, 16, 30); break;
					case 7: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_ARROW|GTK_MARGIN1, 14, 14); break;
					case 8: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_ARROW|GTK_VAL1|GTK_MARGIN1, 14, 14); break;
					case 9: m = GetGTK(gtk__vscrollbar, a, b, "slider", GTK_SLIDER, 16, 30); break;*/
					case 0: m = GetGTK(GTK().gtk_image_new, a, b, "gtk-ok", GTK_ICON, 32, 32); break;

	    			}
				w.DrawRect(a * 80 + 60, b * 40 + 30, 10, 10, Color(c->r >> 8, c->g >> 8, c->b >> 8));
				w.DrawImage(a * 80, b * 40, m);
				w.DrawText(a * 80 + 60, b * 40 + 20, Format("%d,%d", b, a));
				c++;
    		}
	}

	virtual void Paint2(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(GetSize(), White());

		static GdkColormap* cm = gdk_x11_colormap_foreign_new(
		                              gdkx_visual_get(XVisualIDFromVisual(Xvisual)), Xcolormap);
		GdkWindow *gw = gdk_window_foreign_new(w.GetDrawable());
		GtkStyle *style = gtk_widget_get_style((GtkWidget *)gtk__parent());
		GdkRectangle r;
		r.x = r.y = 0;
		r.width = r.height = 64;
		gtk_paint_box(style, gw,
		              (GtkStateType)0, (GtkShadowType)0, &r,
		              (GtkWidget*)GTK().gtk_button_new(), "button",
		              0, 0, 64, 64);
	}

	void DoRefresh() { Refresh(); }

	typedef Test CLASSNAME;

	Test() {
		n <<= 0;
		n <<= THISBACK(DoRefresh);
		Add(n.RightPos(0, 50).BottomPos(0, 20));
		Sizeable();
	}
};

void SetX11ErrorHandler();

GUI_APP_MAIN
{
	DUMP(OFFSETOF(GtkWidget, state));
	DUMP(sizeof(GtkButton));
	Test().Run();
}
