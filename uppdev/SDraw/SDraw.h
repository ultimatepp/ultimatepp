#ifndef _SDraw_SDraw_h
#define _SDraw_SDraw_h

#include <Draw/Draw.h>

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

NAMESPACE_UPP

struct Matrix2D : agg::trans_affine {};

class SDraw {
	typedef agg::renderer_base<agg::pixfmt_bgra32> renderer_base;
//	typedef agg::renderer_scanline_bin_solid<renderer_base> renderer_solid;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
	
	Size                          size;
	ImageBuffer&                  buffer;
	
	Matrix2D                      mtx;

	agg::path_storage             path;
	bool                          inpath;

	agg::rendering_buffer         rbuf;
	agg::rasterizer_scanline_aa<> rasterizer;
	agg::scanline_p8              scanline_p;
	renderer_base                 renb;
	renderer_solid                renderer;
	agg::pixfmt_bgra32            pixf;


	typedef agg::conv_curve<agg::path_storage>  Curved;
	typedef agg::conv_stroke<Curved>            CurvedStroked;
	typedef agg::conv_transform<CurvedStroked>  CurvedStrokedTrans;
	typedef agg::conv_transform<Curved>         CurvedTrans;

	Curved                        curved;
	CurvedStroked                 curved_stroked;
	CurvedStrokedTrans            curved_stroked_trans;
	CurvedTrans                   curved_trans;
	
public:
	SDraw& MoveTo(double x, double y);
	SDraw& LineTo(double x, double y);
	SDraw& Quadratic(double x1, double y1, double x, double y);
	SDraw& Quadratic(double x, double y);
	SDraw& Cubic(double x1, double y1, double x2, double y2, double x, double y);
	SDraw& Cubic(double x2, double y2, double x, double y);
	SDraw& Arc(double rx, double ry, double angle, bool large_arc_flag, bool sweep_flag,
	           double x, double y);
	
	SDraw& Fill(const RGBA& rgba);
	SDraw& Stroke(const RGBA& rgba, int width);
	
	void   Apply(const Matrix2D& m)       { mtx *= m; }
	void   operator*=(const Matrix2D& m)  { Apply(m); }
	
	void   Translate(double x, double y);
	void   Rotate(double a);
	void   Scale(double scalex, double scaley);
	void   Scale(double scale);

	SDraw(ImageBuffer& ib);
};

END_UPP_NAMESPACE

#endif
