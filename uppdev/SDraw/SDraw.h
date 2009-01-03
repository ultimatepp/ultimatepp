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
#include "agg_conv_stroke.h"
#include "agg_conv_curve.h"
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

struct Translate2D : Matrix2D {
	Translate2D(double x, double y) { tx = x; ty = y; }
};

enum {
	LINECAP_BUTT = agg::butt_cap,
	LINECAP_SQUARE = agg::square_cap,
	LINECAP_ROUND = agg::round_cap,

	LINEJOIN_MITER = agg::miter_join,
	LINEJOIN_ROUND = agg::round_join,
	LINEJOIN_BEVEL = agg::bevel_join,
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
		void modify_vertex(unsigned i, double x, double y) { v[i].x = x; v[i].y = y; }
		void modify_vertex(unsigned i, double x, double y, unsigned cmd) { modify_vertex(i, x, y); c[i] = cmd; }
		void modify_command(unsigned i, unsigned cmd) { c[i] = cmd; }
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
		unsigned vertex(unsigned i, double* x, double* y) const {
			*x = v[i].x;
			*y = v[i].y;
			return c[i];
		}
		unsigned command(unsigned i) const { return c[i]; }
		
		void Swap(vertex_upp_storage& b) { UPP::Swap(v, b.v); UPP::Swap(c, b.c); }
    };

	typedef agg::path_base<vertex_upp_storage> path_storage;

	typedef agg::pixfmt_rgba32_pre pixfmt;
	typedef agg::rgba8 color_type;
	typedef agg::renderer_base<agg::pixfmt_rgba32_pre> renderer_base;
	typedef agg::renderer_scanline_bin_solid<renderer_base> renderer_solid_bin;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
	typedef agg::span_interpolator_linear<> interpolator_type;
	typedef agg::image_accessor_clip<pixfmt> img_source_type;
	typedef agg::span_image_filter_rgba_bilinear_clip<pixfmt, interpolator_type> span_gen_type;
	typedef agg::span_allocator<color_type> span_alloc;
	typedef renderer_base::color_type x;
	
	Size                          size;
	Sizef                         sizef;
	ImageBuffer&                  buffer;
	Array< Buffer<byte> >         mask;
	
	struct Attr : Moveable<Attr> {
		Matrix2D          mtx;
		bool              evenodd;
		byte              join;
		byte              cap;
		double            miter_limit;
		bool              antialiased;
	};

	Attr                          attr;
	Vector<Attr>                  attrstack;

	path_storage                  path;
	bool                          inpath;

	agg::rendering_buffer         rbuf;
	agg::rasterizer_scanline_aa<> rasterizer;
	agg::scanline_p8              scanline_p;
	renderer_base                 renb;
	renderer_solid                renderer;
	renderer_solid_bin            rendererb;
	agg::pixfmt_rgba32_pre        pixf;

	typedef agg::conv_curve<path_storage>       Curved;
	typedef agg::conv_stroke<Curved>            CurvedStroked;
	typedef agg::conv_transform<Curved>         CurvedTrans;

	Curved                        curved;
	CurvedStroked                 curved_stroked;
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
	
public:
	void   Begin();
	void   End();
	
	SDraw& Move(double x, double y);
	SDraw& Line(double x, double y);
	SDraw& Quadratic(double x1, double y1, double x, double y);
	SDraw& Quadratic(double x, double y);
	SDraw& Cubic(double x1, double y1, double x2, double y2, double x, double y);
	SDraw& Cubic(double x2, double y2, double x, double y);
	SDraw& Close();

	SDraw& Fill(const Image& image, const Matrix2D& transsrc = Matrix2D(),
	            int alpha = 255, dword flags = false);
	SDraw& FillMask(int alpha);
	
	SDraw& Fill(const RGBA& rgba);
	SDraw& Stroke(const RGBA& rgba, double width);

	SDraw& Arc(double x, double y, double rx, double ry,
	           double start_angle, double sweep_angle, bool startline = false);

	SDraw& Character(double x, double y, int ch, Font fnt);
	SDraw& Text(double x, double y, const wchar *text, Font fnt, int n = -1, double *dx = NULL);
	SDraw& Text(double x, double y, const WString& s, Font fnt, double *dx = NULL);
	SDraw& Text(double x, double y, const String& s, Font fnt, double *dx = NULL);
	SDraw& Text(double x, double y, const char *text, Font fnt, int n = -1, double *dx = NULL);
	
	SDraw& LineCap(int linecap)                         { attr.cap = linecap; return *this; }
	SDraw& LineJoin(int linejoin)                       { attr.join = linejoin; return *this; }
	SDraw& MiterLimit(double l)                         { attr.miter_limit = l; return *this; }
	SDraw& EvenOdd(bool evenodd)                        { attr.evenodd = evenodd; return *this; }
	SDraw& AntiAliased(bool aa)                         { attr.antialiased = aa; return *this; }

	void   Translate(double x, double y);
	void   Rotate(double a);
	void   Scale(double scalex, double scaley);
	void   Scale(double scale);
	
	void   Apply(const Matrix2D& m)       { attr.mtx *= m; }
	void   operator*=(const Matrix2D& m)  { Apply(m); }

	SDraw(ImageBuffer& ib);
};


END_UPP_NAMESPACE

#endif
