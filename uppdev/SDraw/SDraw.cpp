#include "SDraw.h"

NAMESPACE_UPP

SDraw& SDraw::MoveTo(double x, double y)
{
	if(inpath)
		path.close_polygon();
	else
		path.remove_all();
	path.move_to(x, y);
	inpath = true;
	return *this;
}

SDraw& SDraw::LineTo(double x, double y)
{
	if(!inpath)
		path.remove_all();
	path.line_to(x, y);
	inpath = true;
	return *this;
}

SDraw& SDraw::Quadratic(double x1, double y1, double x, double y)
{
	path.curve3(x1, y1, x, y);
	return *this;
}

SDraw& SDraw::Quadratic(double x, double y)
{
	path.curve3(x, y);
	return *this;
}

SDraw& SDraw::Cubic(double x1, double y1, double x2, double y2, double x, double y)
{
	path.curve4(x1, y1, x2, y2, x, y);
	return *this;
}

SDraw& SDraw::Cubic(double x2, double y2, double x, double y)
{
	path.curve4(x2, y2, x, y);
	return *this;
}

SDraw& SDraw::Arc(double rx, double ry, double angle, bool large_arc_flag,
                  bool sweep_flag, double x2, double y2)
{
	path.arc_to(rx, ry, angle, large_arc_flag, sweep_flag, x2, y2);
	return *this;
}

SDraw& SDraw::Fill(const RGBA& c)
{
	STIMING("Fill");
	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	{ STIMING("arrange");
	path.arrange_orientations_all_paths(agg::path_flags_cw); }
	{ STIMING("rasterize path");
	rasterizer.add_path(curved_trans); }
	{ RTIMING("render");
	renderer.color(agg::rgba8(c.r, c.g, c.b, c.a));
	agg::render_scanlines(rasterizer, scanline_p, renderer); }
	inpath = false;
	return *this;
}

/*
    enum line_cap_e
    {
        butt_cap,
        square_cap,
        round_cap
    };

    //------------------------------------------------------------line_join_e
    enum line_join_e
    {
        miter_join         = 0,
        miter_join_revert  = 1,
        round_join         = 2,
        bevel_join         = 3,
        miter_join_round   = 4
    };


    //-----------------------------------------------------------inner_join_e
    enum inner_join_e
    {
        inner_bevel,
        inner_miter,
        inner_jag,
        inner_round
    };
*/

SDraw& SDraw::Stroke(const RGBA& c, int width)
{


	double scl = mtx.scale();
	curved.approximation_scale(scl);
	curved.angle_tolerance(0.0);
	curved_stroked.width(width);

//m_curved_stroked.line_join(attr.line_join);
//m_curved_stroked.line_cap(attr.line_cap);
//m_curved_stroked.miter_limit(attr.miter_limit);
//m_curved_stroked.inner_join(inner_round);

	curved_stroked.approximation_scale(scl);
	if(width * scl > 1.0)
		curved.angle_tolerance(0.2);

//	agg::conv_transform<agg::path_storage, agg::trans_affine> trans(path, mtx);
//	agg::conv_stroke<agg::conv_transform<agg::path_storage, agg::trans_affine>> stroke(trans);
//	stroke.line_join(agg::miter_join);
//	stroke.width(width);

	rasterizer.reset();
	rasterizer.filling_rule(agg::fill_non_zero);
	rasterizer.add_path(curved_stroked_trans);
	renderer.color(agg::rgba8(c.r, c.g, c.b, c.a));
	agg::render_scanlines(rasterizer, scanline_p, renderer);
	rasterizer.reset();
	inpath = false;
	return *this;
}

void SDraw::Translate(double x, double y)
{
	mtx.tx += x;
	mtx.ty += y;
}

void SDraw::Rotate(double a)
{
	mtx *= agg::trans_affine_rotation(a);
}

void SDraw::Scale(double scalex, double scaley)
{
	mtx *= agg::trans_affine_scaling(scalex, scaley);
}

void SDraw::Scale(double scale)
{
	mtx *= agg::trans_affine_scaling(scale);
}

SDraw::SDraw(ImageBuffer& ib)
:	buffer(ib),
	curved(path),
	curved_stroked(curved),
    curved_stroked_trans(curved_stroked, mtx),
	curved_trans(curved, mtx)
{
	size = ib.GetSize();
	UPP::Fill(~buffer, White(), buffer.GetLength()); //!!!
	rbuf.attach((agg::int8u *)~buffer, size.cx, size.cy, size.cx * 4);
	pixf.attach(rbuf);
	renb.attach(pixf);
	renderer.attach(renb);
	inpath = false;
}

END_UPP_NAMESPACE
