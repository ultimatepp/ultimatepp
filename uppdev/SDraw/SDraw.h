#ifndef _SDraw_SDraw_h
#define _SDraw_SDraw_h

#include <Draw/Draw.h>

#define  STIMING(x)          //   RTIMING(x)

#define  AGGUPP 0

#include "agg_basics.h"
#include "agg_renderer_base.h"
#include "agg_pixfmt_rgba.h"
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

NAMESPACE_UPP

struct Matrix2D : agg::trans_affine {
	Matrix2D operator*(const Matrix2D& s) const { Matrix2D x = *this; x *= s; return x; }
};

enum {
	LINECAP_BUTT = agg::butt_cap,
	LINECAP_SQUARE = agg::square_cap,
	LINECAP_ROUND = agg::round_cap,

	LINEJOIN_MITER = agg::miter_join,
	LINEJOIN_ROUND = agg::round_join,
	LINEJOIN_BEVEL = agg::bevel_join,
};

class SDraw {
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
	ImageBuffer&                  buffer;
	
	struct Attr : Moveable<Attr> {
		Matrix2D          mtx;
		RGBA              fill;
		RGBA              stroke;
		double            width;
		bool              evenodd;
		byte              join;
		byte              cap;
		double            miter_limit;
		bool              antialiased;
	};

	Attr                          attr;
	Vector<Attr>                  attrstack;

	agg::path_storage             path;
	bool                          inpath;

	agg::rendering_buffer         rbuf;
	agg::rasterizer_scanline_aa<> rasterizer;
	agg::scanline_p8              scanline_p;
	renderer_base                 renb;
	renderer_solid                renderer;
	renderer_solid_bin            rendererb;
	agg::pixfmt_rgba32_pre        pixf;


	typedef agg::conv_curve<agg::path_storage>  Curved;
	typedef agg::conv_stroke<Curved>            CurvedStroked;
	typedef agg::conv_transform<CurvedStroked>  CurvedStrokedTrans;
	typedef agg::conv_transform<Curved>         CurvedTrans;

	Curved                        curved;
	CurvedStroked                 curved_stroked;
	CurvedStrokedTrans            curved_stroked_trans;
	CurvedTrans                   curved_trans;
	
public:
	Pointf Current() const;

	void   Begin();
	void   End();
	
	SDraw& MoveTo(double x, double y);
	SDraw& LineTo(double x, double y);
	SDraw& Quadratic(double x1, double y1, double x, double y);
	SDraw& Quadratic(double x, double y);
	SDraw& Cubic(double x1, double y1, double x2, double y2, double x, double y);
	SDraw& Cubic(double x2, double y2, double x, double y);
	SDraw& Close();

	SDraw& Fill();
	SDraw& Stroke();
	
	SDraw& Fill(const Image& image, const Matrix2D& transsrc = Matrix2D(), int alpha = 255, bool tile = false);
	
	SDraw& Ellipse(double rx, double ry);
	
	SDraw& Fill(const RGBA& rgba);
	SDraw& Stroke(const RGBA& rgba, double width);
	SDraw& Stroke(const RGBA& rgba);
	
	SDraw& FillColor(const RGBA& rgba)                  { attr.fill = rgba; return *this; }
	SDraw& StrokeColor(const RGBA& rgba)                { attr.stroke = rgba; return *this; }
	SDraw& Width(double width)                          { attr.width = width; return *this; }
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

void Arc(SDraw& sw, double x, double y, double rx, double ry,
                    double start_angle, double sweep_angle, bool startline);

void RenderCharacter(SDraw& sw, int ch, Font fnt);

END_UPP_NAMESPACE

#endif
