#include "CtrlLib.h"

#ifdef PLATFORM_X11

#ifdef flagNOGTK

NAMESPACE_UPP

void ChHostSkin() {}

END_UPP_NAMESPACE

#else

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

NAMESPACE_UPP

#if defined(_DEBUG)// && 0
#include <plugin/png/png.h>

void LOGPNG(const Image& m)
{
	static int i;
	PNGEncoder png;
	png.SaveFile(ConfigFile(AsString(i++) + ".png"), m);
}

#else

#define LOGPNG(a)

#endif

GtkWidget *gtk__parent()
{
	static GtkWidget *p;
	if(!p) {
		p = gtk_window_new(GTK_WINDOW_POPUP);
		gtk_widget_realize(p);
	}
	return p;
}

GtkWidget *Setup(GtkWidget *widget)
{
	static GtkWidget *fl;
	if (!fl) {
		fl = gtk_fixed_new();
		gtk_container_add(GTK_CONTAINER(gtk__parent()), fl);
		gtk_widget_realize(fl);
	}

	gtk_fixed_put(GTK_FIXED(fl), widget, 0, 0);
	gtk_widget_realize(widget);
	gtk_widget_show(widget);
	return widget;
}

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

static Image sLastImage;

struct GtkRangeLayout_ {
  GdkRectangle stepper_a;
  GdkRectangle stepper_b;
  GdkRectangle stepper_c;
  GdkRectangle stepper_d;
};

void (*chgtkspy__)(const char *name, int state, int shadow, const char *detail, int type, int cx, int cy, const Value& look);

void SetChGtkSpy_(void (*spy)(const char *name, int state, int shadow, const char *detail, int type, int cx, int cy, const Value& look))
{
	chgtkspy__ = spy;
}

Image GetGTK(GtkWidget *widget, int state, int shadow, const char *detail, int type, int cx, int cy)
{
	MemoryIgnoreLeaksBlock __;
	GdkPixbuf *icon;
	if(type == GTK_ICON || type == GTK_THEMEICON) {
		gtk_widget_set_sensitive(widget, 1);
		gtk_widget_set_state(widget, GTK_STATE_NORMAL);
		if(type == GTK_THEMEICON)
			icon = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), detail,
			                                (GtkIconSize)state, (GtkIconLookupFlags)0, NULL);
		else
			icon = gtk_widget_render_icon(widget, detail, (GtkIconSize)state, NULL);
		if(!icon) return Null;
		cx = gdk_pixbuf_get_width(icon);
		cy = gdk_pixbuf_get_height(icon);
	}
	else {
		gtk_widget_set_sensitive(widget, state != 4);
		gtk_widget_set_state(widget, (GtkStateType)state);
	}
	Rect rect;
	int ht = type & 0xf000;
	rect.top = ht == GTK_VCENTER ? cy : ht == GTK_BOTTOM ? 2 * cy : 0;
	rect.left = ht == GTK_HCENTER ? cx : ht == GTK_RIGHT ? 2 * cx : 0;
	rect.SetSize(cx, cy);
	if(ht >= GTK_LEFT && ht <= GTK_RIGHT)
		cx *= 3;
	if(ht >= GTK_TOP && ht <= GTK_BOTTOM)
		cy *= 3;
	type &= ~0xf000;
	if(GTK_IS_RANGE(widget)) {
		GtkRange *r = GTK_RANGE(widget);
		r->has_stepper_a = r->has_stepper_b = r->has_stepper_c = r->has_stepper_d = 1;
		GdkRectangle cr;
		cr.x = rect.left;
		cr.y = rect.top;
		cr.width = 0;
		cr.height = 0;
		GtkRangeLayout_ *rl = (GtkRangeLayout_ *)r->layout;
		rl->stepper_a = rl->stepper_b = rl->stepper_c = rl->stepper_d = cr;
		cr.width = rect.GetWidth();
		cr.height = rect.GetHeight();
		if(type & GTK_RANGEA) rl->stepper_a = cr;
		if(type & GTK_RANGEB) rl->stepper_b = cr;
		if(type & GTK_RANGEC) rl->stepper_c = cr;
		if(type & GTK_RANGED) rl->stepper_d = cr;
	}
	GdkRectangle& allocation = widget->allocation;
	allocation.x = 0;
	allocation.y = 0;
	allocation.width = cx;
	allocation.height = cy;
	Image m[2];
	Color back = White;
	int margin = (type >> 4) & 7;
	for(int i = 0; i < 2; i++) {
		ImageDraw iw(cx + 2 * margin, cy + 2 * margin);
		iw.DrawRect(0, 0, cx + 2 * margin, cy + 2 * margin, back);
		static GdkColormap *cm = gdk_x11_colormap_foreign_new(
			                        gdkx_visual_get(XVisualIDFromVisual(Xvisual)), Xcolormap);
		GdkPixmap *pixmap = gdk_pixmap_foreign_new(iw.GetDrawable());
		gdk_drawable_set_colormap(pixmap, cm);
		GdkRectangle cr;
		cr.x = 0;
		cr.y = 0;
		cr.width = cx;
		cr.height = cy;
		GtkStyle *style = gtk_widget_get_style(widget);
		int rcx = max(rect.GetWidth() - 2 * margin, 0);
		int rcy = max(rect.GetHeight() - 2 * margin, 0);
		int t1 = (type & 0xf00) >> 8;
		switch(type & 15) {
		case GTK_BGBOX:
			gtk_style_apply_default_background(style, pixmap, TRUE, (GtkStateType)state,
			                                   &cr, rect.left + margin, rect.top + margin, rcx, rcy);
		case GTK_BOX:
			gtk_paint_box(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                    widget, detail,
			                    rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_FLATBOX:
			gtk_paint_flat_box(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                    widget, detail,
			                    rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_CHECK:
			gtk_paint_check(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                      widget, detail,
			                      rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_OPTION:
			gtk_paint_option(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                       widget, detail,
			                       rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_ARROW:
			gtk_paint_arrow(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                      widget, detail, (GtkArrowType)t1, 1,
			                      rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_SLIDER:
			gtk_paint_slider(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                       widget, detail,
			                       rect.left + margin, rect.top + margin, rcx, rcy,
			                       (GtkOrientation)t1);
			break;
		case GTK_ICON:
		case GTK_THEMEICON:
			gdk_draw_pixbuf(pixmap, NULL, icon, 0, 0, 0, 0, -1, -1, (GdkRgbDither)0, 0, 0);
			break;
		case GTK_EXT:
			gtk_paint_extension(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                    widget, (gchar *)detail,
			                    rect.left + margin, rect.top + margin, rcx, rcy,
			                    (GtkPositionType)t1);
			break;
		case GTK_SHADOW:
		    gtk_paint_shadow(style, pixmap, (GtkStateType)state, (GtkShadowType)shadow, &cr,
			                 widget, (gchar *)detail,
			                 rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_FOCUS:
		    gtk_paint_focus(style, pixmap, (GtkStateType)state, &cr,
			                widget, (gchar *)detail,
			                rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		}
		g_object_unref(pixmap);
		m[i] = Crop(iw, rect.Inflated(margin));
		back = Black;
	}
	if(type == GTK_ICON)
		g_object_unref(icon);
	sLastImage = RecreateAlpha(m[0], m[1]);
	if(chgtkspy__)
		chgtkspy__(G_OBJECT_TYPE_NAME(widget), state, shadow, detail, type, cx, cy, sLastImage);
	return sLastImage;
}

struct ChGtkI : Moveable<ChGtkI> {
	GtkWidget  *widget;
	const char *detail;
	int         type;
};

Vector<ChGtkI>& ChGtkIs()
{
	static Vector<ChGtkI> x;
	return x;
}

GtkWidget *ChGtkLast()
{
	return ChGtkIs().Top().widget;
}

const char *ChGtkLastDetail()
{
	return ChGtkIs().Top().detail;
}

int ChGtkLastType()
{
	return ChGtkIs().Top().type;
}

void ChGtkNew(GtkWidget *widget, const char *detail, int type)
{
	if(gtk_widget_get_parent(widget) == NULL)
		Setup(widget);
	ChGtkI& q = ChGtkIs().Add();
	q.widget = widget;
	q.detail = detail;
	q.type = type;
}

void ChGtkNew(const char *detail, int type)
{
	ChGtkNew(ChGtkLast(), detail, type);
}

struct GtkElement {
	word  gtki;
	byte  state;
	byte  shadow;
	Rect  reduce;
	Rect  margins;
};

enum {
	GTKELEMENT_TABFLAG = 0x40
};

struct GtkImageMaker : ImageMaker {
	GtkElement e;
	ChGtkI     eg;
	Size       sz;

	virtual String Key() const {
		StringBuffer key;
		key.Cat((char *)&eg.widget, sizeof(eg.widget));
		key.Cat((char *)&eg.detail, sizeof(eg.detail));
		key.Cat((char *)&eg.type, sizeof(eg.type));
		key.Cat((char *)&e.gtki, sizeof(e.gtki));
		key.Cat((char *)&e.state, sizeof(e.state));
		key.Cat((char *)&e.shadow, sizeof(e.shadow));
		key.Cat((char *)&sz, sizeof(sz));
		return key;
	}
	virtual Image Make() const {
		return GetGTK(eg.widget, e.state, e.shadow & 0x3f, eg.detail, eg.type, sz.cx, sz.cy);
	}
};

Value GtkLookFn(Draw& w, const Rect& rect, const Value& v, int op)
{
	if(IsTypeRaw<GtkElement>(v)) {
		const GtkElement& e = ValueTo<GtkElement>(v);
		ChGtkI& eg = ChGtkIs()[e.gtki];
		switch(op) {
		case LOOK_MARGINS:
			return e.margins;
		case LOOK_ISOPAQUE:
			return false;
		}
		if(op == LOOK_PAINT || op == LOOK_PAINTEDGE) {
			Rect r = rect.Inflated(eg.type & GTK_XMARGIN ? 0 : (eg.type >> 4) & 7);
			r.Deflate((eg.type >> 16) & 15);
			if(r.IsEmpty()) return 1;
			GtkImageMaker gm;
			gm.e = e;
			gm.eg = eg;
			gm.sz = r.Size();
			Image m;
			if(e.shadow & 0x80) {
				m = MakeImage(gm);
				w.DrawImage(r.left, r.top,
				            m, RectC(0, 0, gm.sz.cx, gm.sz.cy - 7));
				w.DrawImage(r.left + 4, r.bottom - 7,
				            m, RectC(4, gm.sz.cy - 7, gm.sz.cx - 8, 5));
			}
			else {
				if(e.reduce.left || e.reduce.right || e.reduce.top || e.reduce.bottom) {
					Rect rr = e.reduce;
					rr.SetSize(gm.sz);
					gm.sz.cx += e.reduce.left + e.reduce.right;
					gm.sz.cy += e.reduce.top + e.reduce.bottom;
					w.DrawImage(r.left, r.top, MakeImagePaintOnly(gm), rr);
				}
				else
					w.DrawImage(r.left, r.top, MakeImagePaintOnly(gm));
				if((e.shadow & GTKELEMENT_TABFLAG) && r.GetWidth() > 0) {
					w.DrawRect(r.left, r.top + 3, 1, r.GetHeight() - 6, SColorLight());
					w.DrawRect(r.right - 1, r.top + 3, 1, r.GetHeight() - 6, SColorShadow());
				}
			}
			return 1;
		}
	}
	return Null;
}

Value GtkMakeCh(int shadow, int state, const Rect& r, const Rect& m)
{
	GtkElement e;
	e.gtki = ChGtkIs().GetCount() - 1;
	e.shadow = shadow;
	e.state = state;
	e.reduce = r;
	e.margins = m;
//	if(chgtkspy__) {
//		chgtkspy__
//	}
	return RawToValue(e);
}

Value GtkMakeCh(int shadow, int state, const Rect& r)
{
	return GtkMakeCh(shadow, state, r, Rect(0, 0, 0, 0));
}

Value GtkMakeCh(int shadow, int state)
{
	return GtkMakeCh(shadow, state, Rect(0, 0, 0, 0));
}

void GtkCh(Value& look, int shadow, int state)
{
	look = GtkMakeCh(shadow, state);
}

void GtkCh(Value *look, const char *d)
{
	GtkCh(look[CTRL_NORMAL], d[4] - '0', d[0] - '0');
	GtkCh(look[CTRL_HOT], d[5] - '0', d[1] - '0');
	GtkCh(look[CTRL_PRESSED], d[6] - '0', d[2] - '0');
	GtkCh(look[CTRL_DISABLED], d[7] - '0', d[3] - '0');
}

void GtkCh(Value *look)             { GtkCh(look, "02140000"); }
void GtkChButton(Value *look)       { GtkCh(look, "02142212"); }
void GtkChSlider(Value *look)       { GtkCh(look, "02242222"); }
void GtkChTrough(Value *look)       { GtkCh(look, "11141111"); }

void GtkChWith(Value& look, int shadow, int state, const Image& img, Color c, Point offset = Point(0, 0))
{
	GtkElement e;
	e.gtki = ChGtkIs().GetCount() - 1;
	e.shadow = shadow;
	e.state = state;
	look = ChLookWith(GtkMakeCh(shadow, state), img, c, offset);
}

void GtkChWith(Value *look, const char *d, const Image& img, Point offset = Point(0, 0))
{
	GtkChWith(look[CTRL_NORMAL], d[4] - '0', d[0] - '0', img, ButtonMonoColor(0));
	GtkChWith(look[CTRL_HOT], d[5] - '0', d[1] - '0', img, ButtonMonoColor(1));
	GtkChWith(look[CTRL_PRESSED], d[6] - '0', d[2] - '0', img, ButtonMonoColor(2), offset);
	GtkChWith(look[CTRL_DISABLED], d[7] - '0', d[3] - '0', img, ButtonMonoColor(3));
}

void GtkChButtonWith(Value *look, const Image& img) { GtkChWith(look, "02142222", img); }

void GtkChArrow(Value *look, const Image& img, Point offset = Point(0, 0))
{
	GtkChWith(look, "02142212", img, offset);
}

int  GtkInt(GtkWidget *widget, const char *id)
{
	int x = 0;
	gtk_widget_style_get(widget, id, &x, NULL);
	return x;
}

int  GtkInt(const char *id)
{
	return GtkInt(ChGtkLast(), id);
}

void GtkIml(int uii, GtkWidget *w, int shadow, int state, const char *detail, int type, int cx, int cy)
{
	CtrlsImg::Set(uii, GetGTK(w, state, shadow, detail, type, cx, cy));
}

void GtkIml(int uii, GtkWidget *w, int shadow, const char *detail, int type, int cx, int cy)
{
	GtkIml(uii + 0, w, shadow, 0, detail, type, cx, cy);
	GtkIml(uii + 1, w, shadow, 2, detail, type, cx, cy);
	GtkIml(uii + 2, w, shadow, 1, detail, type, cx, cy);
	GtkIml(uii + 3, w, shadow, 4, detail, type, cx, cy);
}

Color ChGtkColor(int ii, GtkWidget *widget)
{
	GdkColor cc = ((GtkStyle *)gtk_widget_get_style(widget))->fg[ii];
	return Color(cc.red >> 8, cc.green >> 8, cc.blue >> 8);
}

void ChGtkColor(Color& c, int ii)
{
	c = ChGtkColor(ii, ChGtkLast());
}

void ChGtkColor(Color *c, int ii)
{
	ChGtkColor(c[0], ii + 0);
	ChGtkColor(c[1], ii + 2);
	ChGtkColor(c[2], ii + 1);
	ChGtkColor(c[3], ii + 4);
}

Image GtkImage(const char *id, int size, int maxh = INT_MAX)
{
	if(maxh != INT_MAX)
		maxh = Ctrl::VertLayoutZoom(maxh);
	Image m = GetGTK(gtk__parent(), size, 0, id, GTK_ICON, 0, 0);
	if(!IsNull(m)) {
		Size sz = m.GetSize();
		if(sz.cy > maxh && sz.cy)
			m = Rescale(m, sz.cx * maxh / sz.cy, maxh);
	}
	return m;
}

void ChCtrlImg(int ii, const char *id, int size, int maxh = INT_MAX)
{
	Image m = GtkImage(id, size, maxh);
	if(!IsNull(m))
		CtrlImg::Set(ii, m);
}

enum {
	GTK_BOTTOMLINE = 0x100
};

Image GtkChImgLook(int shadow, int state, int kind)
{
	Image m = GetGTK(ChGtkLast(), state, shadow, ChGtkLastDetail(), ChGtkLastType(), 32, 32);
	int g = max(1, ImageMargin(m, 4, 10));
	if((kind & 7) == 1)
		m = WithHotSpots(Crop(m, 0, g, 32 - g, 32 - 2 * g), g, 0, 32 - 2 * g - 1, 0);
	if((kind & 7) == 2)
		m = WithHotSpots(Crop(m, g, g, 32 - g, 32 - 2 * g), 0, 0, 32 - 2 * g - g - 1, 0);
	else
		m = WithHotSpot(m, g, g);
	if((kind & GTK_BOTTOMLINE) && m.GetHeight() > 0) {
		ImageBuffer ib(m);
		RGBA c = SColorShadow();
		for(int i = 1; i < ib.GetWidth(); i++)
			ib[ib.GetHeight() - 1][i] = c;
		m = ib;
	}
	return m;
}

void GtkChImgWith(Value& look, int shadow, int state, const Image& img, Color c, int kind, Point offset = Point(0, 0))
{
	Value m = GtkChImgLook(shadow, state, kind);
	look = IsNull(img) ? m : ChLookWith(m, img, c, offset);
}

void GtkChImgWith(Value *look, const Image& img, int kind, Point offset = Point(0, 0))
{
	GtkChImgWith(look[CTRL_HOT], 2, 2, img, ButtonMonoColor(1), kind);
	GtkChImgWith(look[CTRL_PRESSED], 1, 1, img, ButtonMonoColor(2), kind, offset);
	GtkChImgWith(look[CTRL_DISABLED], 2, 4, img, ButtonMonoColor(3), kind);
	GtkChImgWith(look[CTRL_NORMAL], 2, 0, img, ButtonMonoColor(0), kind);
}

bool IsEmptyImage(const Image& m)
{
	const RGBA *s = ~m;
	const RGBA *e = ~m + m.GetLength();
	while(s < e) {
		if(s->a)
			return false;
		s++;
	}
	return true;
}

Image GtkThemeIcon(const char *name, int size)
{
	Image m = GetGTK(gtk__parent(), size, 0, name, GTK_THEMEICON, 0, 0);
	Size sz = m.GetSize();
	int rsz = size ? 48 : 16;
	if(sz.cx > rsz || sz.cy > rsz)
		m = Rescale(m, rsz, rsz);
	return m;
}

int GtkStyleInt(const char *name)
{
	int h = Null;
	g_object_get(gtk_settings_get_default(), name, &h, NULL);
	return h;
}

String GtkStyleString(const char *name)
{
	const char *h = "";
	g_object_get(gtk_settings_get_default(), name, &h, NULL);
	return h;
}

extern int    gtk_antialias;
extern int    gtk_hinting;
extern String gtk_hintstyle;
extern String gtk_rgba;

extern void ClearFtFaceCache();

void SetDefTrough(ScrollBar::Style& s)
{
	for(int i = 0; i < 4; i++) {
		Image m = CtrlsImg::Get(CtrlsImg::I_SBVU + i);
		ImageBuffer ib(m);
		Size sz = ib.GetSize();
		ib[0][sz.cx - 1] = ib[0][0] = SColorShadow();
		ib.SetHotSpot(Point(0, 0));
		ib.Set2ndSpot(Point(sz.cx - 1, 0));
		m = ib;
		s.vlower[i] = s.vupper[i] = m;
		m = CtrlsImg::Get(CtrlsImg::I_SBHU + i);
		ib = m;
		sz = ib.GetSize();
		ib[sz.cy - 1][0] = ib[0][0] = SColorShadow();
		ib.SetHotSpot(Point(0, 0));
		ib.Set2ndSpot(Point(0, sz.cy - 1));
		m = ib;
		s.hupper[i] = s.hlower[i] = m;
	}
}

void ChHostSkin()
{
	MemoryIgnoreLeaksBlock __;
	static struct { void (*set)(Color); int ii; } col[] = {
		{ SColorPaper_Write, 6*5 + 0 },
		{ SColorFace_Write, 1*5 + 0 },
		{ SColorText_Write, 5*5 + 0 },
		{ SColorHighlight_Write, 6*5 + 3 },
		{ SColorHighlightText_Write, 5*5 + 3 },
		{ SColorMenu_Write, 6*5 + 0 },
		{ SColorMenuText_Write, 5*5 + 0 },
		{ SColorDisabled_Write, 5*5 + 4 },
		{ SColorLight_Write, 2*5 + 0 },
		{ SColorShadow_Write, 3*5 + 0 },
	};
	for(int i = 0; i < __countof(col); i++)
		(*col[i].set)(ChGtkColor(col[i].ii, gtk__parent()));

	////////
		GtkWidget *label = gtk_label_new("Ch");
		Setup(label);
		Color ch_ink = ChGtkColor(0, label);
		gtk_widget_destroy(label);
		SColorLabel_Write(ch_ink);
		
	///////
	CtrlsImg::Reset();
	ColoredOverride(CtrlsImg::Iml(), ClassicCtrlsImg::Iml());

	ChLookFn(GtkLookFn);

	String engine = GtkStyleString("gtk-theme-name");
	
	if(chgtkspy__)
		engine.Clear();

	int fontname = Font::ARIAL;
	int fontheight = 13;
	bool bold = false;
	bool italic = false;

	String font_name = GtkStyleString("gtk-font-name");
	int xdpi = Nvl(GtkStyleInt("gtk-xft-dpi"), 72 * 1024);
	gtk_antialias = Nvl(GtkStyleInt("gtk-xft-antialias"), -1);
	gtk_hinting = Nvl(GtkStyleInt("gtk-xft-hinting"), -1);
//	gtk_hintstyle = GtkStyleString("gtk-xft-hintstyle");
	gtk_hintstyle = gtk_hinting? "hintfull" : "hintnone"; // Gtk does not seem to follow its own rules...
	gtk_rgba = GtkStyleString("gtk-xft-rgba");

	const char *q = strrchr(font_name, ' ');
	if(q) {
		int h = atoi(q);
		if(h)
			fontheight = h;
		String face(font_name, q);
		fontname = Font::FindFaceNameIndex(face);

		if(fontname == 0) {
			for(;;) {
				const char *q = strrchr(face, ' ');
				if(!q) break;
				const char *s = q + 1;
				if(stricmp(s, "Bold") == 0 || stricmp(s, "Heavy") == 0)
					bold = true;
				else
				if(stricmp(s, "Italic") == 0 || stricmp(s, "Oblique") == 0)
					italic = true;
				else
				if(stricmp(s, "Regular") == 0 || stricmp(s, "Light") || stricmp(s, "Medium"))
					;
				else
					continue;
				face = String(~face, q);
			}
			fontname = Font::FindFaceNameIndex(face);
			if(fontname == 0)
				if(ToUpper(face[0]) == 'M')
					fontname = Font::COURIER;
				else
				if(ToUpper(face[0]) == 'S' && ToUpper(face[1]) == 'e')
					fontname = Font::ROMAN;
				else
					fontname = Font::ARIAL;
		}
	}

	Draw::SetStdFont(Font(fontname, (fontheight * xdpi + 512*72) / (1024*72))
	                 .Bold(bold).Italic(italic));

	ClearFtFaceCache();

	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());

	Color fc = Blend(SColorHighlight, SColorShadow);

	ChGtkIs().Clear();
	GtkWidget *w = Setup(gtk_radio_button_new(NULL));
	int is = GtkInt(w, "indicator-size") + 2;
	GTK_TOGGLE_BUTTON(w)->active = false;
	GTK_TOGGLE_BUTTON(w)->inconsistent = false;
	GtkIml(CtrlsImg::I_S0, w, 2, "radiobutton", GTK_OPTION|GTK_MARGIN1, is, is);
	GTK_TOGGLE_BUTTON(w)->active = true;
	GtkIml(CtrlsImg::I_S1, w, 1, "radiobutton", GTK_OPTION|GTK_MARGIN1, is, is);
	gtk_widget_destroy(w);

	w = Setup(gtk_check_button_new());
	GTK_TOGGLE_BUTTON(w)->active = false;
	GTK_TOGGLE_BUTTON(w)->inconsistent = false;
	GtkIml(CtrlsImg::I_O0, w, 2, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	GTK_TOGGLE_BUTTON(w)->active = true;
	GtkIml(CtrlsImg::I_O1, w, 1, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	GTK_TOGGLE_BUTTON(w)->active = false;
	GTK_TOGGLE_BUTTON(w)->inconsistent = true;
	GtkIml(CtrlsImg::I_O2, w, 3, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	gtk_widget_destroy(w);

	if(engine == "Qt") {
		for(int i = 0; i < 4; i++) {
			Image m = CtrlsImg::Get(CtrlsImg::I_O2 + i);
			ImageBuffer ib(m);
			Size sz = ib.GetSize();
			if(sz.cx > 6)
				for(int q = -2; q <= 2; q++)
					ib[sz.cy / 2 - 1][sz.cx / 2 - q - 1] = i == CTRL_DISABLED ? SColorDisabled() : SColorText();
			m = ib;
			CtrlsImg::Set(CtrlsImg::I_O2 + i, m);
		}
	}

	Point po(0, 0);

	int classiq = engine == "Redmond" || engine == "Raleigh" || engine == "Glider" || engine == "Simple";

	{
		Button::Style& s = Button::StyleNormal().Write();
		s.overpaint = 3;
		static GtkWidget *button = gtk_button_new();
		ChGtkNew(button, "button", GTK_BOX|GTK_MARGIN3);
		GtkChButton(s.look);

		po.x = GtkInt("child-displacement-x");
		po.y = GtkInt("child-displacement-y");

		s.ok = GtkImage("gtk-ok", 4, 16);
		s.cancel = GtkImage("gtk-cancel", 4, 16);
		s.exit = GtkImage("gtk-quit", 4, 16);

		ChGtkColor(s.textcolor, 0 * 5);
		s.pressoffset = po;

		Color c = SColorFace();
		for(int i = 0; i < 4; i++)
			s.monocolor[i] = c.GetR() + c.GetG() + c.GetB() < 3 * 128 ? White() : Black();
		s.monocolor[3] = Gray();

		ToolBar::Style& ts = ToolBar::StyleDefault().Write();
		GtkChButton(ts.buttonstyle.look);
		ts.buttonstyle.look[CTRL_NORMAL] = Null;
		ts.buttonstyle.look[CTRL_DISABLED] = Null;
		GtkCh(ts.buttonstyle.look[CTRL_CHECKED], 1, 1);
		GtkCh(ts.buttonstyle.look[CTRL_HOTCHECKED], 1, 1);

		{
			HeaderCtrl::Style& hs = HeaderCtrl::StyleDefault().Write();
			if(classiq)
				for(int i = 0; i < 4; i++)
					hs.look[i] = s.look[i];
			else {
				ChGtkNew(button, "button", GTK_BOX);
				hs.look[0] = GtkMakeCh(2|GTKELEMENT_TABFLAG, /*0*/4, Rect(6, 3, 6, 0));
				hs.look[1] = GtkMakeCh(2|GTKELEMENT_TABFLAG, 2, Rect(6, 3, 6, 0));
				hs.look[2] = GtkMakeCh(1|GTKELEMENT_TABFLAG, 1, Rect(6, 3, 6, 0));
				hs.look[3] = GtkMakeCh(2|GTKELEMENT_TABFLAG, 4, Rect(6, 3, 6, 0));
				hs.pressoffset = po.x || po.y;
			}
		}
	}

	{
		Button::Style& s = Button::StyleOk().Write();
		static GtkWidget *def_button;
		if(!def_button) {
			def_button = gtk_button_new();
			Setup(def_button);
			gtk_widget_set(def_button, "can-default", true, NULL);
			gtk_window_set_default(GTK_WINDOW(gtk__parent()), def_button);
			ChGtkNew(def_button, "button", GTK_BOX|GTK_MARGIN3);
		}
		GtkChButton(s.look);
	}

	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		SetDefTrough(s);
		s.through = true;
		GtkObject *adj = gtk_adjustment_new(250, 0, 1000, 1, 1, 500);
		static GtkWidget *vscrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(adj));
		ChGtkNew(vscrollbar, "slider", GTK_SLIDER|GTK_VAL1);
		Image m = GetGTK(ChGtkLast(), 0, 0, "slider", GTK_SLIDER|GTK_VAL1, 16, 32);
		s.thumbmin = GtkInt("min-slider-length");
		s.barsize = GtkInt("slider-width");
		s.arrowsize = GtkInt("stepper-size");

		s.isright2 = s.isdown2 = GtkInt("has-secondary-forward-stepper");
		s.isleft2 = s.isup2 = GtkInt("has-secondary-backward-stepper");

		for(int i = 0; i < 6; i++)
			CtrlsImg::Set(CtrlsImg::I_DA + i, CtrlsImg::Get(CtrlsImg::I_kDA + i));

		if(engine == "Qt") {
			int r = Null;
			for(int i = 0; i < 4; i++) {
				ImageDraw iw(64, 64);
				iw.DrawRect(0, 0, 64, 64, SColorFace());
				ChPaint(iw, Size(64, 64), Button::StyleNormal().look[i == CTRL_HOT ? CTRL_NORMAL : i]);
				Image m = iw;
				if(IsNull(r))
					r = minmax(ImageMargin(m, 4, 50), 0, 2);
				m = Rescale(Crop(m, 4, 4, 56, 56), 16, 16);
				ChPartMaker pm(m);
				pm.tr = pm.tl = r;
				pm.bl = pm.br = 0;
				s.up.look[i] = ChLookWith(pm.Make(), CtrlsImg::UA(), ButtonMonoColor(i));
				pm.tr = pm.tl = 0;
				pm.t = false;
				s.down2.look[i] = ChLookWith(pm.Make(), CtrlsImg::DA(), ButtonMonoColor(i));
				pm.t = true;
				pm.bl = pm.br = r;
				s.down.look[i] = ChLookWith(pm.Make(), CtrlsImg::DA(), ButtonMonoColor(i));
				pm.br = pm.bl = 0;
				pm.b = false;
				s.up2.look[i] = ChLookWith(pm.Make(), CtrlsImg::UA(), ButtonMonoColor(i));
				pm.br = pm.bl = pm.tl = pm.tr = r;
				pm.b = true;
				Image bm = pm.Make();
				Button::StyleScroll().Write().look[i] = bm;
				pm.ResetShape();
				pm.t = pm.b = pm.l = false;
				Image lm = pm.Make();
				Button::StyleLeftEdge().Write().look[i] = lm;
				pm.r = false;
				pm.l = true;
				Image rm = pm.Make();
				Button::StyleEdge().Write().look[i] = rm;
				{
					DropList::Style& s = DropList::StyleFrame().Write();
					s.look[i] = s.trivial[i] = s.right[i] = ChLookWith(rm, CtrlsImg::DA());
					s.left[i] = ChLookWith(lm, CtrlsImg::DA());
					s.pressoffset = po;
				}
				{
					SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
					s.dec.look[i] = ChLookWith(rm, CtrlImg::spindown2());
					pm.b = true;
					s.inc.look[i] = ChLookWith(pm.Make(), CtrlImg::spinup2());
				}
				pm.ResetShape();
				pm.tl = pm.bl = r;
				pm.tr = pm.br = 0;
				s.left.look[i] = ChLookWith(pm.Make(), CtrlsImg::LA(), ButtonMonoColor(i));
				pm.tl = pm.bl = 0;
				pm.l = false;
				s.right2.look[i] = ChLookWith(pm.Make(), CtrlsImg::RA(), ButtonMonoColor(i));
				pm.l = true;
				pm.tl = pm.bl = 0;
				pm.tr = pm.br = r;
				s.right.look[i] = ChLookWith(pm.Make(), CtrlsImg::RA(), ButtonMonoColor(i));
				pm.tr = pm.br = 0;
				pm.r = false;
				s.left2.look[i] = ChLookWith(pm.Make(), CtrlsImg::LA(), ButtonMonoColor(i));
			}
			ChGtkNew(vscrollbar, "slider", GTK_SLIDER|GTK_VAL1|GTK_MARGIN1|GTK_XMARGIN);
			GtkChSlider(s.vthumb);
			s.barsize += 2;
			s.arrowsize++;
			SetDefTrough(s);
			static GtkWidget *hscrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(adj));
			ChGtkNew(hscrollbar, "slider", GTK_SLIDER|GTK_MARGIN1|GTK_XMARGIN);
			GtkChSlider(s.hthumb);
			s.overthumb = true;
		}
		else {
			GtkChSlider(s.vthumb);
			if(engine != "Nodoka") {
				ChGtkNew("trough", GTK_BGBOX);
				GtkChTrough(s.vupper);
				GtkChTrough(s.vlower);
			}
			bool atp = IsEmptyImage(GetGTK(ChGtkLast(), 2, 2, "vscrollbar", GTK_BOX|GTK_TOP|GTK_RANGEA, 16, 16));
			Size asz(s.barsize / 2, s.arrowsize / 2);
			if(engine == "Qt")
				atp = false;
			if(atp) {
				ChGtkNew("vscrollbar", GTK_ARROW);
				if(engine != "Human") {
					GtkCh(s.up.look, "02141111");
					GtkCh(s.up2.look, "02141111");
				}
				ChGtkNew("vscrollbar", GTK_ARROW|GTK_VAL1);
				if(engine != "Human") {
					GtkCh(s.down.look, "02141111");
					GtkCh(s.down2.look, "02141111");
				}

				static GtkWidget *btn = gtk_button_new();
				ChGtkNew(btn, "button", GTK_BOX);

				GtkChButton(Button::StyleScroll().Write().look);
				GtkChButton(Button::StyleEdge().Write().look);
				GtkChButton(Button::StyleLeftEdge().Write().look);

				{
					DropList::Style& s = DropList::StyleFrame().Write();
					GtkChButtonWith(s.look, CtrlsImg::DA());
					GtkChButtonWith(s.trivial, CtrlsImg::DA());
				}
				{
					SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
					GtkChButtonWith(s.inc.look, CtrlImg::spinup2());
					GtkChButtonWith(s.dec.look, CtrlImg::spindown2());
				}
			}
			else {
				if(engine != "Nodoka") {
					GtkIml(CtrlsImg::I_UA, ChGtkLast(), 0, 0, "vscrollbar", GTK_ARROW|GTK_TOP|GTK_RANGEA, asz.cx, asz.cy);
					GtkIml(CtrlsImg::I_DA, ChGtkLast(), 0, 0, "vscrollbar", GTK_ARROW|GTK_VAL1|GTK_BOTTOM|GTK_RANGED, asz.cx, asz.cy);
				}

				ChGtkNew("vscrollbar", GTK_BGBOX|GTK_TOP|GTK_RANGEA);
				GtkChArrow(s.up.look, CtrlsImg::UA(), po);
				ChGtkNew("vscrollbar", GTK_BGBOX|GTK_BOTTOM|GTK_RANGED);
				GtkChArrow(s.down.look, CtrlsImg::DA(), po);
				ChGtkNew("vscrollbar", GTK_BGBOX|GTK_VCENTER|GTK_RANGEC);
				GtkChArrow(s.up2.look, CtrlsImg::UA(), po);
				ChGtkNew("vscrollbar", GTK_BGBOX|GTK_VCENTER|GTK_RANGEB);
				GtkChArrow(s.down2.look, CtrlsImg::DA(), po);

				ChGtkNew("vscrollbar", GTK_BOX|GTK_VCENTER|GTK_RANGEB);
				GtkCh(Button::StyleScroll().Write().look, "02142222");

				int q = !classiq;

				GtkChImgWith(Button::StyleEdge().Write().look, Null, 1 * q);
				GtkChImgWith(Button::StyleLeftEdge().Write().look, Null, 2 * q);

				{
					DropList::Style& s = DropList::StyleFrame().Write();
					GtkChImgWith(s.look, CtrlsImg::DA(), 1 * q, po);
					GtkChImgWith(s.trivial, CtrlsImg::DA(), 1 * q, po);
					GtkChImgWith(s.left, CtrlsImg::DA(), 2 * q, po);
					GtkChImgWith(s.right, CtrlsImg::DA(), 1 * q, po);
					s.pressoffset = po;
				}
				{
					SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
					GtkChImgWith(s.inc.look, q ? CtrlImg::spinup2() : CtrlImg::spinup3(), (1|GTK_BOTTOMLINE) * q, po);
					GtkChImgWith(s.dec.look, q ? CtrlImg::spindown2() : CtrlImg::spindown3(), 1 * q, po);
				}
			}

			int d = Diff(fc, SColorPaper());
			for(int x = 0; x < 4; x++)
				for(int y = 0; y < 4; y++) {
					RGBA c = sLastImage[x][y];
					if(c.a == 255) {
						int dd = Diff(c, SColorPaper());
						if(dd > d) {
							fc = c;
							d = dd;
						}
					}
				}
			FieldFrameColor_Write(fc);

			static GtkWidget *hscrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(adj));
			ChGtkNew(hscrollbar, "slider", GTK_SLIDER);
			GtkChSlider(s.hthumb);
			if(engine != "Nodoka") {
				ChGtkNew("trough", GTK_BGBOX);
				GtkChTrough(s.hupper);
				GtkChTrough(s.hlower);
			}
			if(atp) {
				ChGtkNew("hscrollbar", GTK_ARROW|GTK_VAL2);
				GtkCh(s.left.look, "02141111");
				GtkCh(s.left2.look, "02141111");
				ChGtkNew("hscrollbar", GTK_ARROW|GTK_VAL3);
				GtkCh(s.right.look, "02141111");
				GtkCh(s.right2.look, "02141111");
			}
			else {
				if(engine != "Nodoka") {
					GtkIml(CtrlsImg::I_LA, ChGtkLast(), 0, 0, "hscrollbar", GTK_ARROW|GTK_VAL2|GTK_LEFT|GTK_RANGEA, asz.cy, asz.cx);
					GtkIml(CtrlsImg::I_RA, ChGtkLast(), 0, 0, "hscrollbar", GTK_ARROW|GTK_VAL3|GTK_RIGHT|GTK_RANGED, asz.cy, asz.cx);
				}
				ChGtkNew("hscrollbar", GTK_BGBOX|GTK_LEFT|GTK_RANGEA);
				GtkChArrow(s.left.look, CtrlsImg::LA(), po);
				ChGtkNew("hscrollbar", GTK_BGBOX|GTK_VCENTER|GTK_RANGEC);
				GtkChArrow(s.left2.look, CtrlsImg::LA(), po);
				ChGtkNew("hscrollbar", GTK_BGBOX|GTK_VCENTER|GTK_RANGEB);
				GtkChArrow(s.right2.look, CtrlsImg::RA());
				ChGtkNew("hscrollbar", GTK_BGBOX|GTK_RIGHT|GTK_RANGED);
				GtkChArrow(s.right.look, CtrlsImg::RA(), po);
			}

			gtk_object_sink(adj);

			adj = gtk_adjustment_new(0, 0, 1000, 1, 1, 500);
			w = gtk_vscrollbar_new(NULL);
			Setup(w);
			s.overthumb = m != GetGTK(w, 0, 0, "slider", GTK_SLIDER|GTK_VAL1, 16, 32) && engine != "Qt";
			gtk_widget_destroy(w);
			gtk_object_sink(adj);
			
			w = gtk_hscale_new_with_range(0.0, 100.0, 1.0);
			Setup(w);
			int cx = GtkInt(w, "slider-length");
			int cy = 15;
			if(engine == "Glider")
				cy += cx-3;
			if(engine == "Human" || engine == "DarkRoom" || engine == "Crux")
				cy = 13;
			CtrlImg::Set("hthumb",GetGTK(w, 0, 0, "hscale", GTK_SLIDER, cx, cy));
			CtrlImg::Set("hthumb1",GetGTK(w, 2, 0, "hscale", GTK_SLIDER, cx, cy));
			gtk_widget_destroy(w);
			w = gtk_hscale_new_with_range(0.0, 100.0, 1.0);
			Setup(w);
			CtrlImg::Set("vthumb",GetGTK(w, 0, 0, "vscale", GTK_SLIDER|GTK_VAL1, cy, cx));
			CtrlImg::Set("vthumb1",GetGTK(w, 2, 0, "vscale", GTK_SLIDER|GTK_VAL1, cy, cx));
			gtk_widget_destroy(w);

		}
	}

	if(engine != "Qt")
	{
		TabCtrl::Style& s = TabCtrl::StyleDefault().Write();
		static GtkWidget *tabctrl = gtk_notebook_new();
		ChGtkNew(tabctrl, "tab", GTK_EXT|GTK_VAL3|GTK_MARGIN3);
		ImageBuffer ib(9, 9);
		ImageBuffer ib1(9, 9);
		Image m = GetGTK(tabctrl, 0, 2, "tab", GTK_EXT|GTK_VAL3, 12, 24);
		for(int i = 0; i < 5; i++) {
			RGBA *t = ~ib + i * 9 + i;
			RGBA *t1 = ~ib1 + i * 9 + i;
			for(int n = 9 - 2 * i; n--; t += 9, t1 += 9) {
				Fill(t, m[21][i], 9 - 2 * i);
				Fill(t1, m[21][11 - i], 9 - 2 * i);
			}
		}
		{
			RGBA *t = ~ib + 9 + 8;
			RGBA *t1 = ~ib1 + 9 + 8;
			for(int i = 1; i < 9; i++) {
				memcpy(t, t1, i * sizeof(RGBA));
				t += 9 - 1;
				t1 += 9 - 1;
			}
		}
		ib.SetHotSpot(Point(4, 4));
		s.body = Image(ib);
		GtkCh(s.normal[0], 2, 1);
		GtkCh(s.normal[1], 2, 1);
		GtkCh(s.normal[2], 0x82, 0);
		GtkCh(s.normal[3], 2, 1);
		for(int i = 0; i < 4; i++)
			s.first[i] = s.last[i] = s.both[i] = s.normal[i];
		s.margin = 0;

		s.sel = Rect(0, 2, 0, 4);
		s.extendleft = 2;
		ChGtkColor(s.text_color, 0);
	}

	int efm = 0;
	{
		EditField::Style& s = EditField::StyleDefault().Write();
		Image img;
		for(int i = 0; i < 4; i++) {
			GtkWidget *w = Setup(gtk_entry_new());
			if(i == CTRL_PRESSED)
				GTK_WIDGET_FLAGS (w) |= GTK_HAS_FOCUS;
			if(i == CTRL_DISABLED)
				GTK_WIDGET_FLAGS (w) &= GTK_SENSITIVE;
			if(i == 0) {
				img = GetGTK(w, GTK_STATE_NORMAL, GTK_SHADOW_IN, "entry", GTK_SHADOW, 20, 20);
				efm = max(ImageMargin(img, 4, 0), 1);
			}
			img = GetGTK(w, GTK_STATE_NORMAL, GTK_SHADOW_IN,
			             "entry", GTK_SHADOW, 2 * efm + 3, 2 * efm + 3);
			ImageBuffer eb(img);
			eb.SetHotSpot(Point(efm, efm));
			s.edge[i] = Image(eb);
			s.activeedge = true;
			gtk_widget_destroy(w);
		}
	}

	{
		MultiButton::Style& s = MultiButton::StyleDefault().Write();
		s.usetrivial = true;
		s.trivialsep = true;
		s.edge[0] = Null;
		s.overpaint = Button::StyleNormal().overpaint;
		s.margin.left = 3;
		for(int i = 0; i < 4; i++)
			s.left[i] = s.right[i] = s.lmiddle[i] = s.look[i] = Button::StyleNormal().look[i];
		s.loff = 1;
		s.roff = -1;
		s.trivialborder = s.border = 0;
		s.sep2 = SColorShadow();
		s.sep1 = SColorLight();
		s.sepm = 4;
		{
			MultiButton::Style& s = MultiButton::StyleFrame().Write();
			for(int i = 0; i < 4; i++)
				s.edge[i] = EditField::StyleDefault().edge[i];
			s.activeedge = true;
			s.sep1 = Null;
			s.trivialborder = s.border = efm;
			s.usetrivial = true;
			s.sepm = 0;
			s.margin = Rect(efm, efm, efm, efm);
		}
	}

	ImageBuffer ib;
	ib.Create(3, 3);
	Fill(~ib, fc, ib.GetLength());
	ib[1][1] = Color(Null);
	ib.SetHotSpot(Point(1, 1));
	CtrlsImg::Set(CtrlsImg::I_EFE, ib);
	ib.Create(5, 5);
	Fill(~ib, fc, ib.GetLength());
	for(int x = 1; x < 4; x++)
		Fill(ib[x] + 1, SColorPaper(), 3);
	ib[2][2] = Color(Null);
	ib.SetHotSpot(Point(2, 2));
	CtrlsImg::Set(CtrlsImg::I_VE, ib);

	static GtkWidget *popup;
	static GtkWidget *bar = Setup(gtk_menu_bar_new());
	static int shadowtype;
	static GtkWidget *top_item;
	static GtkWidget *menu_item;
	if(!popup) {
		gtk_widget_style_get(bar, "shadow_type", &shadowtype, NULL);
		top_item = gtk_menu_item_new_with_label("M");
		gtk_menu_shell_append(GTK_MENU_SHELL(bar), top_item);
		gtk_widget_realize(top_item);
		gtk_widget_show(top_item);
		popup = gtk_menu_new();
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(top_item), popup);
		gtk_widget_realize(popup);
		gtk_widget_show(popup);
		GTK_MENU_SHELL(bar)->active = true;
		menu_item = gtk_menu_item_new_with_label("M");
		gtk_menu_shell_append(GTK_MENU_SHELL(popup), menu_item);
		gtk_widget_realize(menu_item);
		gtk_widget_show(menu_item);
//		gtk_widget_show(GTK_MENU(GTK_MENU_ITEM(top_item)->submenu)->toplevel);
	}
	Image mimg = GetGTK(popup, 0, 2, "menu", GTK_BGBOX, 32, 32);
	Color c = mimg[16][16];
	Value rlook;
	if(!IsNull(c) && Diff(c, SColorPaper()) < 200)
		SColorMenu_Write(c);
	{
		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.pullshift.y = 0;
		int m = ImageMargin(mimg, 4, 5);
		s.popupframe = WithHotSpot(mimg, m, m);
		s.popupbody = Crop(mimg, m, m, 32 - 2 * m, 32 - 2 * m);
		s.leftgap = 26;
		ChGtkNew(menu_item, "menuitem", GTK_BOX);
		int sw = GTK_SHADOW_OUT;
		if(gtk_major_version > 2 || (gtk_major_version == 2 && gtk_minor_version >= 1))
			sw = GtkInt("selected_shadow_type");
		GtkCh(s.item, sw, GTK_STATE_PRELIGHT);
		s.itemtext = ChGtkColor(2, menu_item);

		ChGtkNew(top_item, "menuitem", GTK_BOX);
		if(gtk_major_version > 2 || (gtk_major_version == 2 && gtk_minor_version >= 1))
			sw = GtkInt("selected_shadow_type");
		s.topitemtext[0] = ChGtkColor(0, bar);
		s.topitemtext[1] = ChGtkColor(1, bar);
		s.topitemtext[2] = ChGtkColor(2, bar);
		SColorMenuText_Write(s.topitemtext[1]);
		s.topitem[1] = s.topitem[0];
		GtkCh(s.topitem[2], sw, GTK_STATE_PRELIGHT);
		s.topitemtext[2] = ChGtkColor(2, top_item);
		if(engine == "Redmond") {
			s.topitem[1] = ChBorder(ThinOutsetBorder(), SColorFace());
			s.topitem[2] = ChBorder(ThinInsetBorder(), SColorFace());
		}
		if(engine == "Geramik" || engine == "ThinGeramik")
			s.topitemtext[2] = SColorText();
		ChGtkNew(bar, "menubar", GTK_BGBOX);
		sw = GtkInt("shadow_type");
		s.look = GtkMakeCh(sw, GTK_STATE_NORMAL, Rect(0, 0, 0, 1));
		rlook = GtkMakeCh(sw, GTK_STATE_NORMAL, Rect(0, 1, 0, 1));
		Image img = GetGTK(bar, GTK_STATE_NORMAL, sw, "menubar", GTK_BGBOX, 32, 32);
		s.breaksep.l1 = Color(img[31][15]);
		TopSeparator1_Write(s.breaksep.l1);
		s.breaksep.l2 = Null;
	}
	{
		ToolBar::Style& s = ToolBar::StyleDefault().Write();
		static GtkWidget *toolbar;
		if(!toolbar) {
			GtkWidget *handle = Setup(gtk_handle_box_new());
			toolbar = gtk_toolbar_new();
			gtk_container_add(GTK_CONTAINER(handle), toolbar);
		}
		ChGtkNew(toolbar, "toolbar", GTK_BGBOX);
		int sw = GtkInt("shadow_type");
		s.arealook = Null;
		s.look = GtkMakeCh(sw, GTK_STATE_NORMAL, Rect(0, 1, 0, 1));
		if(engine == "Redmond")
			s.look = rlook;
		s.breaksep.l1 = MenuBar::StyleDefault().breaksep.l1;
		Image img = GetGTK(toolbar, GTK_STATE_NORMAL, sw, "toolbar", GTK_BGBOX, 32, 32);
		MenuBar::StyleDefault().Write().breaksep.l2 = s.breaksep.l2 = Color(img[0][15]);
		TopSeparator2_Write(s.breaksep.l2);
	}

	if(engine != "Qt")
	{
		ProgressIndicator::Style& s = ProgressIndicator::StyleDefault().Write();
		static GtkWidget *pg = gtk_progress_bar_new();
		ChGtkNew(pg, "trough", GTK_BOX);
		int mx = pg->style->xthickness;
		int my = pg->style->ythickness;
		s.hlook = GtkMakeCh(GTK_SHADOW_IN, GTK_STATE_NORMAL, Rect(0, 0, 0, 0), Rect(mx, my, mx, my));
		ChGtkNew(pg, "bar", GTK_BOX);
		GtkCh(s.hchunk, GTK_SHADOW_OUT, GTK_STATE_PRELIGHT);
		s.bound = true;
	}

	static GtkWidget *ve = Setup(gtk_text_view_new());
	if(engine == "Redmond")
		ViewEdge_Write(EditFieldEdge());
	else
		ViewEdge_Write(WithHotSpot(GetGTK(ve, GTK_STATE_NORMAL, GTK_SHADOW_IN, "frame", GTK_BOX, 32, 32), 2, 2));

	ChCtrlImg(CtrlImg::I_information, "gtk-dialog-info", 6);
	ChCtrlImg(CtrlImg::I_question, "gtk-dialog-question", 6);
	ChCtrlImg(CtrlImg::I_exclamation, "gtk-dialog-warning", 6);

	static struct {
		void  (*set)(Image);
		const char *gtk;
	} bimg[] = {
		{ YesButtonImage_Write, "gtk-yes" },
		{ NoButtonImage_Write, "gtk-no" },
		{ AbortButtonImage_Write, "gtk-stop" },
		{ RetryButtonImage_Write, "gtk-refresh" },
	};

	for(int i = 0; i < __countof(bimg); i++)
		(*bimg[i].set)(GtkImage(bimg[i].gtk, 4, 16));

	if(engine != "Redmond")
		DropEdge_Write(ViewEdge());

	SwapOKCancel_Write(true);

	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_DragFullWindow_Write(1);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	GUI_DropShadows_Write(1);
	GUI_AltAccessKeys_Write(1);
	GUI_AKD_Conservative_Write(0);
	
	ClearFtFaceCache();
}

END_UPP_NAMESPACE

#endif

#endif

