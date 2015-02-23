#include "CtrlLib.h"

#ifdef PLATFORM_X11
#ifdef flagNOGTK

NAMESPACE_UPP

void ChHostSkin() {}

END_UPP_NAMESPACE

#else

#include "ChGtk.h"

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

static Image sLastImage;

Image GtkGetLastImage()
{
	return sLastImage;
}

void (*chgtkspy__)(const char *name, int state, int shadow, const char *detail, int type, int cx, int cy, const Value& look);

void SetChGtkSpy_(void (*spy)(const char *name, int state, int shadow, const char *detail, int type, int cx, int cy, const Value& look))
{
	chgtkspy__ = spy;
}

INITBLOCK { // This is required to avoid animation related problems with Oxygen-GTK theme
	setenv("OXYGEN_APPLICATION_NAME_OVERRIDE", "firefox", 1);
}

Image GetGTK(GtkWidget *widget, int state, int shadow, const char *detail, int type, int cx, int cy,
             Rect rect)
{
	MemoryIgnoreLeaksBlock __;
	GdkPixbuf *icon = NULL;

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

	int ht = type & 0xf000;
	int rcx, rcy;
	int margin = (type >> 4) & 7;
	Rect crop(0, 0, cx, cy);
	if(IsNull(rect)) {
		rect.top = ht == GTK_VCENTER ? cy : ht == GTK_BOTTOM ? 2 * cy : 0;
		rect.left = ht == GTK_HCENTER ? cx : ht == GTK_RIGHT ? 2 * cx : 0;
		rect.SetSize(cx, cy);
		rcx = max(rect.GetWidth() - 2 * margin, 0);
		rcy = max(rect.GetHeight() - 2 * margin, 0);
		crop = rect.Inflated(margin);
	}
	else {
		rcx = rect.GetWidth();
		rcy = rect.GetHeight();
	}
	if(ht >= GTK_LEFT && ht <= GTK_RIGHT)
		cx *= 3;
	if(ht >= GTK_TOP && ht <= GTK_BOTTOM)
		cy *= 3;
	type &= ~0xf000;
	if(GTK_IS_RANGE(widget)) {
		GtkRange *r = GTK_RANGE(widget);
	#ifndef GTK_NEWSCROLLBAR
		r->has_stepper_a = r->has_stepper_b = r->has_stepper_c = r->has_stepper_d = 1;
	#endif
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
	for(int i = 0; i < 2; i++) {
		Size isz(cx + 2 * margin, cy + 2 * margin);
		ImageDraw iw(isz);
#ifdef GUI_GTK
		GdkPixmap *pixmap = gdk_pixmap_new(gdk_get_default_root_window(), isz.cx, isz.cy, -1);
		cairo_t *cairo = gdk_cairo_create(pixmap);
		SystemDraw sw(cairo, NULL);
		sw.DrawRect(isz, back);
		cairo_destroy(cairo);
#else
		iw.DrawRect(0, 0, cx + 2 * margin, cy + 2 * margin, back);
		static GdkColormap *cm = gdk_x11_colormap_foreign_new(
			                        gdkx_visual_get(XVisualIDFromVisual(Xvisual)), Xcolormap);
		GdkPixmap *pixmap = gdk_pixmap_foreign_new(iw.GetDrawable());
		gdk_drawable_set_colormap(pixmap, cm);
#endif
		GdkRectangle cr;
		cr.x = 0;
		cr.y = 0;
		cr.width = cx;
		cr.height = cy;
		GtkStyle *style = gtk_widget_get_style(widget);
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
#ifdef GUI_GTK
		gdk_cairo_set_source_pixmap(iw, pixmap, 0, 0);
		cairo_paint(iw);
#endif
		m[i] = Crop(iw, crop);
		g_object_unref(pixmap);
		back = Black;
	}
	if(type == GTK_ICON)
		g_object_unref(icon);
	sLastImage = RecreateAlpha(m[0], m[1]);
	if(chgtkspy__)
		chgtkspy__(G_OBJECT_TYPE_NAME(widget), state, shadow, detail, type, cx, cy, sLastImage);
	return sLastImage;
}

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
	Value v = RawToValue(e);
	if(chgtkspy__) {
		ChGtkI& q = ChGtkIs().Top();
		chgtkspy__(String("Look: ") + G_OBJECT_TYPE_NAME(q.widget), state, shadow, q.detail, q.type, Null, Null, v);
	}
	return v;
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

void GtkChWith(Value& look, int shadow, int state, const Image& img, Color c, Point offset)
{
	GtkElement e;
	e.gtki = ChGtkIs().GetCount() - 1;
	e.shadow = shadow;
	e.state = state;
	look = ChLookWith(GtkMakeCh(shadow, state), img, c, offset);
}

void GtkChWith(Value *look, const char *d, const Image& img, Point offset)
{
	GtkChWith(look[CTRL_NORMAL], d[4] - '0', d[0] - '0', img, ButtonMonoColor(0));
	GtkChWith(look[CTRL_HOT], d[5] - '0', d[1] - '0', img, ButtonMonoColor(1));
	GtkChWith(look[CTRL_PRESSED], d[6] - '0', d[2] - '0', img, ButtonMonoColor(2), offset);
	GtkChWith(look[CTRL_DISABLED], d[7] - '0', d[3] - '0', img, ButtonMonoColor(3));
}

void GtkChButtonWith(Value *look, const Image& img) { GtkChWith(look, "02142222", img); }

void GtkChArrow(Value *look, const Image& img, Point offset)
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

void GtkIml(int uii, GtkWidget *w, int shadow, int state, const char *detail, int type, int cx, int cy,
            const Rect& rect)
{
	CtrlsImg::Set(uii, GetGTK(w, state, shadow, detail, type, cx, cy, rect));
}

void GtkIml(int uii, GtkWidget *w, int shadow, const char *detail, int type, int cx, int cy,
            const Rect& rect)
{
	GtkIml(uii + 0, w, shadow, 0, detail, type, cx, cy, rect);
	GtkIml(uii + 1, w, shadow, 2, detail, type, cx, cy, rect);
	GtkIml(uii + 2, w, shadow, 1, detail, type, cx, cy, rect);
	GtkIml(uii + 3, w, shadow, 4, detail, type, cx, cy, rect);
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

Image GtkImage(const char *id, int size, int maxh)
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

void ChCtrlImg(int ii, const char *id, int size, int maxh)
{
	Image m = GtkImage(id, size, maxh);
	if(!IsNull(m))
		CtrlImg::Set(ii, m);
}

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

void GtkChImgWith(Value& look, int shadow, int state, const Image& img, Color c, int kind, Point offset)
{
	Value m = GtkChImgLook(shadow, state, kind);
	look = IsNull(img) ? m : ChLookWith(m, img, c, offset);
}

void GtkChImgWith(Value *look, const Image& img, int kind, Point offset)
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

Image GtkThemeIcon(const char *name, bool large)
{
	int rsz = large ? 48 : 16;
	Image m = GetGTK(gtk__parent(), rsz, 0, name, GTK_THEMEICON, 0, 0);
	Size sz = m.GetSize();
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


void GtkChSlider(Value *look)       { GtkCh(look, "02242222"); }
void GtkChTrough(Value *look)       { GtkCh(look, "11141111"); }


void GtkChArrow(Value *look, int index, Point po)
{
	GtkChWith(look, "02142212", CtrlsImg::Get(index), po);
}

void GtkChScrollBar(Value *lbutton, Value *lbutton2,
                    Value *lower, Value *thumb, Value *upper,
                    Value *ubutton2, Value *ubutton,
                    int i_larrow, int i_uarrow,
                    bool horz)
{
	const char *detail = horz ? "hscrollbar" : "vscrollbar";
	
	GtkChSlider(thumb);

	ChGtkNew("trough", GTK_BGBOX);
	GtkChTrough(upper);
	GtkChTrough(lower);

	const ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
	Size sz = Size(s.barsize, s.arrowsize);
	if(horz)
		Swap(sz.cx, sz.cy);
	Rect r = Rect(sz).CenterRect(sz / 2);

	// Fix for themes that replace button background with arrow; e.g. Sphere Crystal
	Image a = GetGTK(ChGtkLast(), 0, 0, detail, GTK_ARROW|GTK_TOP|GTK_RANGEA, 10, 10);
	int trans = 0;
	const RGBA *end = ~a + a.GetLength();
	for(const RGBA *i = a; i < end; i++)
		if(i->a < 255)
			trans++;
	if(trans > a.GetLength() / 4) { // 'normal' correct theme, fetch arrows
		GtkIml(i_larrow, ChGtkLast(), 0, 0, detail,
		       horz ? GTK_ARROW|GTK_VAL2|GTK_LEFT|GTK_RANGEA : GTK_ARROW|GTK_TOP|GTK_RANGEA,
		       sz.cx, sz.cy, r);
		GtkIml(i_uarrow, ChGtkLast(), 0, 0, detail,
		       horz ? GTK_ARROW|GTK_VAL3|GTK_RIGHT|GTK_RANGED : GTK_ARROW|GTK_VAL1|GTK_BOTTOM|GTK_RANGED,
		       sz.cx, sz.cy, r);
		Point po(GtkInt("arrow-displacement-x"), GtkInt("arrow-displacement-y"));
		ChGtkNew(detail, horz ? GTK_BGBOX|GTK_LEFT|GTK_RANGEA : GTK_BGBOX|GTK_TOP|GTK_RANGEA);
		GtkChArrow(lbutton, i_larrow, po);
		ChGtkNew(detail, GTK_BGBOX|GTK_VCENTER|GTK_RANGEB);
		GtkChArrow(lbutton2, i_larrow, po);
		ChGtkNew(detail, GTK_BGBOX|GTK_VCENTER|GTK_RANGEC);
		GtkChArrow(ubutton2, i_uarrow, po);
		ChGtkNew(detail, horz ? GTK_BGBOX|GTK_RIGHT|GTK_RANGED : GTK_BGBOX|GTK_BOTTOM|GTK_RANGED);
		GtkChArrow(ubutton, i_uarrow, po);
	}
/* Crystal sphere: does not look good, let us stay with standard buttons
	else {
		ChGtkNew(detail, horz ? GTK_ARROW|GTK_VAL2|GTK_LEFT|GTK_RANGEA|GTK_MARGIN4 : GTK_ARROW|GTK_TOP|GTK_RANGEA|GTK_MARGIN4);
		GtkCh(lbutton, "02142212");
		ChGtkNew(detail, horz ? GTK_ARROW|GTK_VAL2|GTK_LEFT|GTK_RANGEA|GTK_MARGIN4 : GTK_ARROW|GTK_TOP|GTK_RANGEA|GTK_MARGIN4);
		GtkCh(lbutton2, "02142212");
		ChGtkNew(detail, horz ? GTK_ARROW|GTK_VAL3|GTK_RIGHT|GTK_RANGED|GTK_MARGIN4 : GTK_ARROW|GTK_VAL1|GTK_BOTTOM|GTK_RANGED|GTK_MARGIN4);
		GtkCh(ubutton2, "02142212");
		ChGtkNew(detail, horz ? GTK_ARROW|GTK_VAL3|GTK_RIGHT|GTK_RANGED|GTK_MARGIN4 : GTK_ARROW|GTK_VAL1|GTK_BOTTOM|GTK_RANGED|GTK_MARGIN4);
		GtkCh(ubutton, "02142212");
	}
*/
}

END_UPP_NAMESPACE

#endif
#endif
