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

SDraw& SDraw::Fill()
{
	STIMING("Fill");
	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	rasterizer.filling_rule(attr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	path.arrange_orientations_all_paths(agg::path_flags_cw);
	rasterizer.add_path(curved_trans);
	renderer.color(*(color_type *)&attr.fill);
	if(attr.antialiased) {
		renderer.color(*(color_type *)&attr.fill);
		agg::render_scanlines(rasterizer, scanline_p, renderer);
	}
	else {
		rendererb.color(*(color_type *)&attr.fill);
		agg::render_scanlines(rasterizer, scanline_p, rendererb);
	}
	rasterizer.reset();
	inpath = false;
	return *this;
}

SDraw& SDraw::Stroke()
{
	double scl = attr.mtx.scale();
	curved.approximation_scale(scl);
	curved.angle_tolerance(0.0);
	curved_stroked.width(attr.width);
	curved_stroked.line_join((agg::line_join_e)attr.join);
	curved_stroked.line_cap((agg::line_cap_e)attr.cap);
	curved_stroked.miter_limit(attr.miter_limit);
	curved_stroked.approximation_scale(scl);
	if(attr.width * scl > 1.0)
		curved.angle_tolerance(0.2);
	rasterizer.reset();
	rasterizer.filling_rule(agg::fill_non_zero);
	rasterizer.add_path(curved_stroked_trans);
	if(attr.antialiased) {
		renderer.color(*(color_type *)&attr.stroke);
		agg::render_scanlines(rasterizer, scanline_p, renderer);
	}
	else {
		rendererb.color(*(color_type *)&attr.stroke);
		agg::render_scanlines(rasterizer, scanline_p, rendererb);
	}
	rasterizer.reset();
	inpath = false;
	return *this;
}

SDraw& SDraw::Fill(const Image& image, const Matrix2D& transsrc, int alpha, bool tile)
{
	span_alloc sa;
	Matrix2D m = attr.mtx * transsrc;
	m.invert();
	interpolator_type interpolator(m);
	Size isz = image.GetSize();
	agg::rendering_buffer buf((agg::int8u*)~image, isz.cx, isz.cy, isz.cx * sizeof(RGBA));
	pixfmt img_pixf(buf);
	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	rasterizer.filling_rule(attr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	path.arrange_orientations_all_paths(agg::path_flags_cw);
	rasterizer.add_path(curved_trans);
	span_gen_type sg(img_pixf, agg::rgba8_pre(0, 0, 0, 0), interpolator);
	sg.alpha(alpha);
	sg.tile(tile);
	agg::render_scanlines_aa(rasterizer, scanline_p, renb, sa, sg);
	rasterizer.reset();
	inpath = false;
	return *this;
}

SDraw& SDraw::Fill(const RGBA& rgba)
{
	RGBA h = attr.fill;
	attr.fill = rgba;
	Fill();
	attr.fill = h;
	return *this;
}

Pointf SDraw::Current() const
{
	return Pointf(path.last_x(), path.last_y());
}

SDraw& SDraw::Stroke(const RGBA& rgba, double width)
{
	RGBA h = attr.stroke;
	double hw = attr.width;
	attr.stroke = rgba;
	attr.width = width;
	Stroke();
	attr.stroke = h;
	attr.width = hw;
	return *this;
}

SDraw& SDraw::Stroke(const RGBA& rgba)
{
	RGBA h = attr.stroke;
	attr.stroke = rgba;
	Stroke();
	attr.stroke = h;
	return *this;
}

void SDraw::Translate(double x, double y)
{
	attr.mtx.tx += x;
	attr.mtx.ty += y;
}

void SDraw::Rotate(double a)
{
	attr.mtx *= agg::trans_affine_rotation(a);
}

void SDraw::Scale(double scalex, double scaley)
{
	attr.mtx *= agg::trans_affine_scaling(scalex, scaley);
}

void SDraw::Scale(double scale)
{
	attr.mtx *= agg::trans_affine_scaling(scale);
}

void SDraw::Begin()
{
	attrstack.Add(attr);
}

void SDraw::End()
{
	if(attrstack.GetCount() == 0) {
		ASSERT_(0, "SDraw::End: attribute stack is empty");
		return;
	}
	attr = attrstack.Pop();
}

SDraw::SDraw(ImageBuffer& ib)
:	buffer(ib),
	curved(path),
	curved_stroked(curved),
    curved_stroked_trans(curved_stroked, attr.mtx),
	curved_trans(curved, attr.mtx)
{
	size = ib.GetSize();
	UPP::Fill(~buffer, White(), buffer.GetLength()); //!!!
	rbuf.attach((agg::int8u *)~buffer, size.cx, size.cy, size.cx * 4);
	pixf.attach(rbuf);
	renb.attach(pixf);
	renderer.attach(renb);
	rendererb.attach(renb);
	inpath = false;

	attr.cap = LINECAP_BUTT;
	attr.join = LINEJOIN_MITER;
	attr.miter_limit = 4;
	attr.width = 1;
	attr.evenodd = false;
	attr.fill = attr.stroke = Black();
	attr.antialiased = true;
}

END_UPP_NAMESPACE
