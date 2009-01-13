#ifndef _SDraw_SDraw_h
#define _SDraw_SDraw_h

#include <Draw/Draw.h>

#define  STIMING(x)          //   RTIMING(x)

#define  AGGUPP 1

#include "agg_basics.h"
#include "agg_renderer_base.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_curve.h"
#include "agg_conv_dash.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "agg_image_accessors.h"
#include "agg_span_image_filter_upp.h"
#include "agg_alpha_mask_u8.h"

NAMESPACE_UPP

struct Matrix2D : agg::trans_affine {
	Matrix2D operator*(const Matrix2D& s) const  { Matrix2D x = *this; x *= s; return x; }
	
	void   Transform(double& x, double& y) const { transform(&x, &y); }
	Pointf Transformed(double x, double y) const { Transform(x, y); return Pointf(x, y); }
	Pointf Transformed(Pointf p) const           { Transform(p.x, p.y); return p; }
	Matrix2D Inverted() const                    { Matrix2D h = *this; h.invert(); return h; }
};

Matrix2D Translate2D(double x, double y);
Matrix2D Rotate2D(double angle);
Matrix2D Scale2D(double scalex, double scaley);
Matrix2D Scale2D(double scale);
Matrix2D Sheer2D();

enum {
	LINECAP_BUTT = agg::butt_cap,
	LINECAP_SQUARE = agg::square_cap,
	LINECAP_ROUND = agg::round_cap,

	LINEJOIN_MITER = agg::miter_join,
	LINEJOIN_ROUND = agg::round_join,
	LINEJOIN_BEVEL = agg::bevel_join,
	
	FILL_EXACT      = 0,

	FILL_HPAD       = 1,
	FILL_HREPEAT    = 2,
	FILL_HREFLECT   = 3,

	FILL_VPAD       = 4,
	FILL_VREPEAT    = 8,
	FILL_VREFLECT   = 12,

	FILL_PAD        = FILL_HPAD|FILL_VPAD,
	FILL_REPEAT     = FILL_HREPEAT|FILL_VREPEAT,
	FILL_REFLECT    = FILL_HREFLECT|FILL_VREFLECT,
	
	FILL_FAST       = 256,
	
	GRADIENT_PAD     = 0,
	GRADIENT_REPEAT  = 1,
	GRADIENT_REFLECT = 2,
};

class SDraw : public Draw {
public:
	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual Rect GetClipOp() const;
	virtual bool IsPaintingOp(const Rect& r) const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);

	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);

	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);

private:
	class vertex_upp_storage {
		Vector<Pointf> v;
		Vector<int>    c;
    public:
		typedef double value_type;
		void remove_all() { v.Clear(); c.Clear(); }
		void free_all()   { remove_all(); }
		void add_vertex(double x, double y, unsigned cmd) { v.Add(Pointf(x, y)); c.Add(cmd); }
		void modify_vertex(int i, double x, double y) { v[i].x = x; v[i].y = y; }
		void modify_vertex(int i, double x, double y, unsigned cmd) { modify_vertex(i, x, y); c[i] = cmd; }
		void modify_command(int i, unsigned cmd) { c[i] = cmd; }
		void swap_vertices(unsigned v1, unsigned v2) { v.Swap(v1, v2); c.Swap(v1, v2); }
		unsigned last_command() const { return c.GetCount() ? c.Top() : agg::path_cmd_stop; }
		unsigned last_vertex(double* x, double* y) const {
			if(c.GetCount() == 0) {
				*x = *y = 0.0;
				return agg::path_cmd_stop;
			}
			return vertex(v.GetCount() - 1, x, y);
		}
		unsigned prev_vertex(double* x, double* y) const {
			if(c.GetCount() < 2) {
				*x = *y = 0.0;
				return agg::path_cmd_stop;
			}
			return vertex(v.GetCount() - 2, x, y);
		}
		double last_x() const { return v.GetCount() ? v.Top().x : 0.0; }
		double last_y() const { return v.GetCount() ? v.Top().y : 0.0; }
		unsigned total_vertices() const { return v.GetCount(); }
		unsigned vertex(int i, double* x, double* y) const {
			*x = v[i].x;
			*y = v[i].y;
			return c[i];
		}
		unsigned command(int i) const { return (int)i < c.GetCount() ? c[i] : agg::path_cmd_stop; }
    };

	typedef agg::path_base<vertex_upp_storage> path_storage;

	typedef agg::pixfmt_rgba32_pre pixfmt;
	typedef agg::rgba8 color_type;
	typedef agg::renderer_base<agg::pixfmt_rgba32_pre> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
	typedef agg::span_interpolator_linear<> interpolator_type;
	typedef agg::image_accessor_clip<pixfmt> img_source_type;
	typedef agg::span_image_filter_rgba_bilinear_clip<pixfmt, interpolator_type> span_gen_type;
	typedef agg::span_allocator<color_type> span_alloc;
	typedef renderer_base::color_type x;
	
	Size                          size;
	Sizef                         sizef;
	ImageBuffer&                  buffer;
	Array< Buffer<byte> >         clip;
	Array< ImageBuffer >          mask;
	
	struct Attr : Moveable<Attr> {
		Matrix2D                        mtx;
		bool                            evenodd;
		byte                            join;
		byte                            cap;
		double                          miter_limit;
		WithDeepCopy< Vector<double> >  dash;
		WithDeepCopy< Vector<double> >  stop;
		WithDeepCopy< Vector<RGBA> >    stop_color;
		double                          dash_start;
		double                          opacity;
		int                             cliplevel;
		bool                            hasclip;
		bool                            mask;
	};

	Attr                          attr;
	Attr                          pathattr;
	Vector<Attr>                  attrstack;

	path_storage                  path;
	bool                          inpath;

	agg::rendering_buffer         rbuf;
	agg::rasterizer_scanline_aa<> rasterizer;
	agg::scanline_p8              scanline_p;
	renderer_base                 renb;
	renderer_solid                renderer;
	agg::pixfmt_rgba32_pre        pixf;

	typedef agg::conv_curve<path_storage>       Curved;
	typedef agg::conv_transform<Curved>         CurvedTrans;

	Curved                        curved;
	CurvedTrans                   curved_trans;
	
	Rectf                         pathrect;
	Pointf                        current, control;
	
	void   PathPoint(double x, double y);
	void   ControlPoint(double x, double y);
	void   MinMax(Pointf& min, Pointf& max, Pointf p) const;
	bool   PathVisible(double d) const;
	Pointf Reflection() const;
	Pointf Current() const        { return current; }
	Rectf  PathRect() const       { return pathrect; }
	path_storage MakeStroke(double width);
	Pointf ReadPoint(CParser& p, bool rel);
	void   RenderClip(byte *t, int alpha);
	void   RectPath(const Rect& r);
	void   RectPath(int x, int y, int cx, int cy);
	void   MakeGradient(RGBA *t, RGBA color1, RGBA color2, int cx);
	void   ColorStop0(Attr& a, double pos, const RGBA& color);
	void   FinishMask();
	void   SetRbuf();

public:
	void   Clear(const RGBA& color);

	SDraw& Move(double x, double y);
	SDraw& Line(double x, double y);
	SDraw& Quadratic(double x1, double y1, double x, double y);
	SDraw& Quadratic(double x, double y);
	SDraw& Cubic(double x1, double y1, double x2, double y2, double x, double y);
	SDraw& Cubic(double x2, double y2, double x, double y);
	SDraw& Close();

	SDraw& Fill(const RGBA& color);
	SDraw& Fill(const Image& image, const Matrix2D& transsrc = Matrix2D(),
	            dword flags = 0);
	SDraw& Fill(const Image& image, double x1, double y1, double x2, double y2,
	            dword flags = 0);
	SDraw& Fill(double x1, double y1, const RGBA& color1,
	            double x2, double y2, const RGBA& color2,
	            int style = GRADIENT_PAD);
	SDraw& Fill(double x1, double y1, double r, double fx, double fy,
	            const RGBA& color1, const RGBA& color2,
	            int style = GRADIENT_PAD);
	SDraw& Fill(double x1, double y1, double r,
	            const RGBA& color1, const RGBA& color2,
	            int style = GRADIENT_PAD);
	
	SDraw& Stroke(double width, const RGBA& rgba);
	SDraw& Stroke(double width, const Image& image, const Matrix2D& transsrc, dword flags = 0);

	SDraw& Clip(double opacity = 1.0);

	SDraw& Arc(double x, double y, double rx, double ry,
	           double start_angle, double sweep_angle, bool startline = false);

	SDraw& Character(double x, double y, int ch, Font fnt);
	SDraw& Text(double x, double y, const wchar *text, Font fnt, int n = -1, double *dx = NULL);
	SDraw& Text(double x, double y, const WString& s, Font fnt, double *dx = NULL);
	SDraw& Text(double x, double y, const String& s, Font fnt, double *dx = NULL);
	SDraw& Text(double x, double y, const char *text, Font fnt, int n = -1, double *dx = NULL);
	
	SDraw& Path(CParser& p);
	SDraw& Path(const char *path);
	
	SDraw& ColorStop(double pos, const RGBA& color);
	SDraw& ClearStops();
	
	SDraw& Opacity(double o);
	SDraw& LineCap(int linecap);
	SDraw& LineJoin(int linejoin);
	SDraw& MiterLimit(double l);
	SDraw& EvenOdd(bool evenodd);
	SDraw& Dash(const Vector<double>& dash, double start = 0);
	SDraw& Dash(const char *dash, double start = 0);

	void   Transform(const Matrix2D& m);

	void   Begin();
	void   End();
	
	void   BeginMask();
	
	void   Translate(double x, double y);
	void   Rotate(double a);
	void   Scale(double scalex, double scaley);
	void   Scale(double scale);

	SDraw(ImageBuffer& ib);
};

END_UPP_NAMESPACE

#endif
