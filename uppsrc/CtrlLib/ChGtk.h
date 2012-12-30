#ifdef PLATFORM_X11

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

	GTKELEMENT_TABFLAG = 0x40,
	GTK_BOTTOMLINE = 0x100
};


/*
enum
{
  GTK_SHADOW_NONE,
  GTK_SHADOW_IN,
  GTK_SHADOW_OUT,
  GTK_SHADOW_ETCHED_IN,
  GTK_SHADOW_ETCHED_OUT
};

enum
{
  GTK_STATE_NORMAL,
  GTK_STATE_ACTIVE,
  GTK_STATE_PRELIGHT,
  GTK_STATE_SELECTED,
  GTK_STATE_INSENSITIVE
};
*/
struct GtkRangeLayout_ {
  GdkRectangle stepper_a;
  GdkRectangle stepper_b;
  GdkRectangle stepper_c;
  GdkRectangle stepper_d;
};

GtkWidget *gtk__parent();
GtkWidget *Setup(GtkWidget *widget);

void SetChGtkSpy_(void (*spy)(const char *name, int state, int shadow, const char *detail, int type, int cx, int cy, const Value& look));

Image GetGTK(GtkWidget *widget, int state, int shadow, const char *detail, int type, int cx, int cy, Rect rect = Null);
Image GtkGetLastImage();

struct ChGtkI : Moveable<ChGtkI> {
	GtkWidget  *widget;
	const char *detail;
	int         type;
};

Vector<ChGtkI>& ChGtkIs();
GtkWidget      *ChGtkLast();
const char     *ChGtkLastDetail();

int  ChGtkLastType();
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
void GtkIml(int uii, GtkWidget *w, int shadow, int state, const char *detail, int type, int cx, int cy, const Rect& rect = Null);
void GtkIml(int uii, GtkWidget *w, int shadow, const char *detail, int type, int cx, int cy, const Rect& rect = Null);
Color ChGtkColor(int ii, GtkWidget *widget);
void ChGtkColor(Color& c, int ii);
void ChGtkColor(Color *c, int ii);
Image GtkImage(const char *id, int size, int maxh = INT_MAX);
void ChCtrlImg(int ii, const char *id, int size, int maxh = INT_MAX);
Image GtkChImgLook(int shadow, int state, int kind);
void GtkChImgWith(Value& look, int shadow, int state, const Image& img, Color c, int kind, Point offset = Point(0, 0));
void GtkChImgWith(Value *look, const Image& img, int kind, Point offset = Point(0, 0));
bool IsEmptyImage(const Image& m);

Image  GtkThemeIcon(const char *name, bool large);

int    GtkStyleInt(const char *name);
String GtkStyleString(const char *name);

void GtkChScrollBar(Value *lbutton, Value *lbutton2,
                    Value *lower, Value *thumb, Value *upper,
                    Value *ubutton2, Value *ubutton, 
                    int i_larrow, int i_uarrow, bool horz);

};

#endif
