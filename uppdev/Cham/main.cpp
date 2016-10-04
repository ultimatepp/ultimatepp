#include <CtrlLib/CtrlLib.h>

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


namespace Upp {
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
	GTK_XMARGIN = 0x0080,

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
	struct ChGtkI : Moveable<ChGtkI> {
		GtkWidget  *widget;
		const char *detail;
		int         type;
	};

	Image GetGTK(GtkWidget *widget, int state, int shadow, const char *detail, int type, int cx, int cy);
	Vector<ChGtkI>& ChGtkIs();
	GtkWidget *ChGtkLast();
	const char *ChGtkLastDetail();
	int ChGtkLastType();
	void ChGtkNew(GtkWidget *widget, const char *detail, int type);
	void ChGtkNew(const char *detail, int type);
	Value GtkLookFn(Draw& w, const Rect& rect, const Value& v, int op);
	Value GtkMakeCh(int shadow, int state, const Rect& r, const Rect& m);
	Value GtkMakeCh(int shadow, int state, const Rect& r);
	Value GtkMakeCh(int shadow, int state);
	void GtkCh(Value& look, int shadow, int state);
	void GtkCh(Value *look, const char *d);
	void GtkCh(Value *look);
	void GtkChButton(Value *look);
	void GtkChSlider(Value *look);
	void GtkChTrough(Value *look);
	void GtkChWith(Value& look, int shadow, int state, const Image& img, Color c, Point offset = Point(0, 0));
	void GtkChWith(Value *look, const char *d, const Image& img, Point offset = Point(0, 0));
	void GtkChButtonWith(Value *look, const Image& img);
	void GtkChArrow(Value *look, const Image& img, Point offset = Point(0, 0));
	int GtkInt(GtkWidget *widget, const char *id);
	int GtkInt(const char *id);
	void GtkIml(int uii, GtkWidget *w, int shadow, int state, const char *detail, int type, int cx, int cy);
	void GtkIml(int uii, GtkWidget *w, int shadow, const char *detail, int type, int cx, int cy);
	Color ChGtkColor(int ii, GtkWidget *widget);
	void ChGtkColor(Color& c, int ii);
	void ChGtkColor(Color *c, int ii);
	Image GtkImage(const char *id, int size, int maxh = INT_MAX);
	void ChCtrlImg(int ii, const char *id, int size, int maxh = INT_MAX);
	Image GtkChImgLook(int shadow, int state, int kind);
	void GtkChImgWith(Value& look, int shadow, int state, const Image& img, Color c, int kind, Point offset = Point(0, 0));
	void GtkChImgWith(Value *look, const Image& img, int kind, Point offset = Point(0, 0));
	bool IsEmptyImage(const Image& m);
	Image GtkThemeIcon(const char *name, int size);
	int GtkStyleInt(const char *name);
	String GtkStyleString(const char *name);
};

using namespace Upp;

struct LookDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, Cyan);
		Rect rr = r;
		rr.SetSize(16, 16);
		ChPaint(w, rr, q);
		rr.left += 20;
		rr.SetSize(16, 50);
		ChPaint(w, rr, q);
		rr.left += 20;
		rr.SetSize(50, 16);
		ChPaint(w, rr, q);
		rr.left += 55;
		rr.SetSize(50, 50);
		ChPaint(w, rr, q);
	}
};

struct MyApp : TopWindow {
	ArrayCtrl list;
	
	void PutLook(const char *s, Value look)
	{
		list.Add(s, look);
	}
	
	void PutLooks(const char *s, const Value *look, int count = 4)
	{
		for(int i = 0; i < count; i++)
			PutLook(String().Cat() << s << " " << i, look[i]);
	}
	
	MyApp() {
		Title(GtkStyleString("gtk-theme-name"));
		
		list.SetLineCy(50);
		list.AddColumn("Element");
		list.AddColumn("Image").SetDisplay(Single<LookDisplay>());


		PutLooks("ScrollBar::GetStyle().up", ScrollBar::StyleDefault().up.look);

		Value look[4];
		ChGtkNew("vscrollbar", GTK_BGBOX|GTK_TOP|GTK_RANGEA);
		GtkCh(look, "02141111");
		PutLooks("ChGtkNew(vscrollbar, GTK_ARROW)", look);

		Add(list.SizePos());
		list.NoCursor();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
