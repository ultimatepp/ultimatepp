#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture

#include <limits.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#undef Picture
#undef Time
#undef Font
#undef Display

enum {
	GTK_BOX,
	GTK_CHECK,
	GTK_OPTION,
	GTK_ARROW,
	GTK_SLIDER,
	GTK_ICON,
	GTK_EXT,
	GTK_SHADOW,
	GTK_FOCUS,
	GTK_FLATBOX,
	GTK_BGBOX,
	GTK_THEMEICON,

	GTK_MARGIN1 = 0x0010,
	GTK_MARGIN2 = 0x0020,
	GTK_MARGIN3 = 0x0030,
	GTK_MARGIN4 = 0x0040,

	GTK_VAL1    = 0x0100,
	GTK_VAL2    = 0x0200,
	GTK_VAL3    = 0x0300,
	GTK_VAL4    = 0x0400,

	GTK_TOP     = 0x1000,
	GTK_VCENTER = 0x2000,
	GTK_BOTTOM  = 0x3000,
	GTK_LEFT    = 0x4000,
	GTK_HCENTER = 0x5000,
	GTK_RIGHT   = 0x6000,

	GTK_DEFLATE1 = 0x10000,
	GTK_DEFLATE2 = 0x20000,
	GTK_DEFLATE3 = 0x30000,
	GTK_DEFLATE4 = 0x40000,

	GTK_RANGEA =  0x100000,
	GTK_RANGEB =  0x200000,
	GTK_RANGEC =  0x400000,
	GTK_RANGED =  0x800000,
};

struct G_rect  { int x, y, cx, cy; };
struct G_color { dword _; word r, g, b; };

#ifdef CPU_64
#define GTK__ALLOCATION_OFFSET 64
#define GTK__COLOR_OFFSET      24
#else
#define GTK__ALLOCATION_OFFSET 36
#define GTK__COLOR_OFFSET      12
#endif

namespace Upp {
GtkWidget *gtk__parent();
GtkWidget *Setup(GtkWidget *widget);
void ChGtkNew(GtkWidget *widget, const char *detail, int type);
Value GtkMakeCh(int shadow, int state);
Image GetGTK(GtkWidget *widget, int state, int shadow, const char *detail, int type, int cx, int cy);
int  GtkInt(GtkWidget *widget, const char *id);
Color ChGtkColor(int ii, GtkWidget *widget);
Image GtkImage(const char *id, int size, int maxh = INT_MAX);
};

struct Test : TopWindow {
//	EditIntSpin n;
	virtual void Paint(Draw& draw) {
		Size sz = GetSize();
		int isz = 16;
		draw.DrawRect(GetSize(), SColorFace());
		draw.DrawImage(100, 100, UPP::GtkImage("gtk-ok", 16));
		draw.DrawImage(200, 100, UPP::GtkImage("gtk-yes", 16));
		draw.DrawImage(220, 100, UPP::GtkImage("gtk-yes", 4));
		return;
	}

	void DoRefresh() { Refresh(); }

	typedef Test CLASSNAME;

	Test() {
		Sizeable();
	}
};

GUI_APP_MAIN {
	PromptYesNo("This is prompt dialog");
	PromptRetryCancel("This is prompt dialog");
	PromptAbortRetryIgnore("This is prompt dialog");
	PromptOK("This is prompt dialog");
	PromptOKCancel("This is prompt dialog");
	PromptYesNoCancel("This is prompt dialog");
	PromptAbortRetry("This is prompt dialog");
	PromptAbortRetryIgnore("This is prompt dialog");

/*
	if(GNOME()) {
		Exclamation("Warning!");
		GNOME().gnome_triggers_vdo("", "question", NULL);
		FileSel fs;
		fs.ExecuteOpen();
	}
*/
	PromptYesNo("Yes or no ?");
	Exclamation("Warning!");
	PromptOK("Info...");

//	DeleteFolderDeep(GetHomeDirFile(".gtkskin"));
//	RealizeDirectory(GetHomeDirFile(".gtkskin"));

	char *theme;
	g_object_get(gtk_settings_get_default(), "gtk-theme-name", &theme, NULL);
	DUMP(theme);
	Test().Run();
}

/*
//		ChPaint(w, 0, 300, 14, 14, ChGet("ScrollBarUp"));
		//w.DrawRect(GetSize(), White());
		G_color *c = (G_color *)(24 + (byte *)GTK().gtk_widget_get_style(gtk__parent()));
     	for(int b = 0; b < 8; b++)
    		for(int a = 0; a < 5; a++) {
//				Image m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_BOX|GTK_TOP, 14, 14);
				static G_obj *menu = Gtk_menu();
    			Image m = GetGTK(menu, a, b, "menu", GTK_BOX, 64, 18);
//    			Image m = GetGTK(GTK().gtk_notebook_new, a, b, "notebook", GTK_BOX, 64, 24);

    			if(b < 5)
	    			switch((int)~n) {
					case 0: m = GetGTK(gtk__radiobutton, a, b, "radiobutton", GTK_OPTION|GTK_MARGIN1, isz, isz); break;
					case 1: m = GetGTK(GTK().gtk_check_button_new, a, b, "checkbutton", GTK_CHECK|GTK_MARGIN1, isz, isz); break;
					case 2: m = GetGTK(GTK().gtk_button_new, a, b, "button", GTK_BOX|GTK_MARGIN1, 64, 24); break;
					case 3: m = GetGTK(gtk__defaultbutton, a, b, "button", GTK_BOX|GTK_MARGIN1, 64, 24); break;
					case 4: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_BOX|GTK_TOP|GTK_MARGIN1, 14, 14); break;
					case 5: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_BOX|GTK_BOTTOM|GTK_MARGIN1, 14, 14); break;
					case 6: m = GetGTK(gtk__vscrollbar, a, b, "trough", GTK_BOX, 16, 30); break;
					case 7: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_ARROW|GTK_MARGIN1, 14, 14); break;
					case 8: m = GetGTK(gtk__vscrollbar, a, b, "vscrollbar", GTK_ARROW|GTK_VAL1|GTK_MARGIN1, 14, 14); break;
					case 9: m = GetGTK(gtk__vscrollbar, a, b, "slider", GTK_SLIDER, 16, 30); break;
//					case 0: m = GetGTK(gtk__parent(), a, b, "gtk-dialog-info", GTK_ICON, 0, 0); break;

	    			}
				w.DrawRect(a * 80 + 60, b * 40 + 30, 30, 30, Color(c->r >> 8, c->g >> 8, c->b >> 8));
				w.DrawImage(a * 80, b * 40, m);
				w.DrawText(a * 80 + 60, b * 40 + 20, Format("%d,%d", b, a));
				c++;
*/


/*
MIST:
GtkInt(w, "inner-border") = 0
GtkInt(w, "interior-focus") = 1
GtkInt(w, "focus-line-width") = 1


*/

/*
//		draw.DrawRect(GetSize(), White());
		{
			GtkWidget *tooltip = gtk_window_new(GTK_WINDOW_POPUP);
			gtk_widget_realize(tooltip);
		    GtkStyle* style = gtk_rc_get_style_by_paths(gtk_settings_get_default(),
                                      "gtk-tooltips", "GtkWindow",
                                      GTK_TYPE_WINDOW);

		    gtk_style_attach(style, tooltip->window);



			static GtkWidget *toolbar;
			if(!toolbar) {
				GtkWidget *handle = Setup(gtk_handle_box_new());
				toolbar = gtk_toolbar_new();
				gtk_container_add(GTK_CONTAINER(handle), toolbar);
		        gtk_widget_realize(toolbar);
		        gtk_widget_show(toolbar);
			}

	    	GtkWidget *w = Setup(gtk_text_view_new());
		//	w = Setup(gtk_window_new(GTK_WINDOW_POPUP));

			for(int x = 0; x < 5; x++)
				for(int y = 0; y < 5; y++) {
//					draw.DrawText(x * 100, y * 30, AsString(10 * x + y) + "Progam", Arial(0), ChGtkColor(5 * x + y, menu_item));
//					menu_item->state = x;
					draw.DrawImage(x * 100, y * 30,
//						GetGTK(toolbar, x, y, "toolbar", GTK_BGBOX, 90, 25));
						GetGTK(w, 32, 32, "gnome-fs-home", GTK_THEMEICON, 32, 32));
				}
		}
*/
