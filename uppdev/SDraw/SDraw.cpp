#include "SDraw.h"

NAMESPACE_UPP

#define LTIMING(x) RTIMING(x)

inline void SDraw::PathPoint(double x, double y)
{
	if(inpath) {
		pathrect.left = min(pathrect.left, x);
		pathrect.top = min(pathrect.top, y);
		pathrect.right = max(pathrect.right, x);
		pathrect.bottom = max(pathrect.bottom, y);
	}
	else {
		path.remove_all();
		pathrect.left = pathrect.right = x;
		pathrect.top = pathrect.bottom = y;
	}
	inpath = true;
	current = Pointf(x, y);
}

inline void SDraw::ControlPoint(double x, double y)
{
	control = Pointf(x, y);
	PathPoint(x, y);
}

SDraw& SDraw::Move(double x, double y)
{
	PathPoint(x, y);
	path.move_to(x, y);
	inpath = true;
	return *this;
}

SDraw& SDraw::Line(double x, double y)
{
	PathPoint(x, y);
	path.line_to(x, y);
	inpath = true;
	return *this;
}

SDraw& SDraw::Quadratic(double x1, double y1, double x, double y)
{
	ControlPoint(x1, y1);
	PathPoint(x, y);
	path.curve3(x1, y1, x, y);
	return *this;
}

Pointf SDraw::Reflection() const
{
	return current + current - control;
}

SDraw& SDraw::Quadratic(double x, double y)
{
	Pointf c = Reflection();
	path.curve3(c.x, c.y, x, y);
	return *this;
}

SDraw& SDraw::Cubic(double x1, double y1, double x2, double y2, double x, double y)
{
	ControlPoint(x1, y1);
	ControlPoint(x2, y2);
	PathPoint(x, y);
	path.curve4(x1, y1, x2, y2, x, y);
	return *this;
}

SDraw& SDraw::Cubic(double x2, double y2, double x, double y)
{
	Pointf c = Reflection();
	path.curve4(c.x, c.y, x2, y2, x, y);
	return *this;
}

SDraw& SDraw::Close()
{
	path.close_polygon();
	return *this;
}

inline void SDraw::MinMax(Pointf& minv, Pointf& maxv, Pointf p) const
{
	p = attr.mtx.Transformed(p);
	minv.x = min(minv.x, p.x);
	minv.y = min(minv.y, p.y);
	maxv.x = max(maxv.x, p.x);
	maxv.y = max(maxv.y, p.y);
}

bool SDraw::PathVisible(double w) const
{
	Pointf h = attr.mtx.Transformed(w, w);
	w = max(abs(h.x), abs(h.y));
	Pointf min;
	Pointf max;
	min = max = attr.mtx.Transformed(pathrect.TopLeft());
	MinMax(min, max, pathrect.TopRight());
	MinMax(min, max, pathrect.BottomLeft());
	MinMax(min, max, pathrect.BottomRight());
	return max.x + w >= 0 && max.y + w >= 0 && min.x - w <= sizef.cx && min.y - w <= sizef.cy;
}

SDraw& SDraw::Fill()
{
	LTIMING("Fill");
	if(inpath)
		path.close_polygon();
	if(PathVisible(0)) {
		rasterizer.reset();
		rasterizer.filling_rule(attr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
		rasterizer.add_path(curved_trans);
		renderer.color(*(color_type *)&attr.fill);
		if(mask.GetCount()) {
			agg::rendering_buffer mask_rbuf;
			mask_rbuf.attach(~mask.Top(), size.cx, size.cy, size.cx);
			agg::alpha_mask_gray8 mask(mask_rbuf);
			agg::scanline_u8_am<agg::alpha_mask_gray8> sl(mask);
			if(attr.antialiased) {
				renderer.color(*(color_type *)&attr.fill);
				agg::render_scanlines(rasterizer, sl, renderer);
			}
			else {
				rendererb.color(*(color_type *)&attr.fill);
				agg::render_scanlines(rasterizer, sl, rendererb);
			}
		}
		else
		if(attr.antialiased) {
			renderer.color(*(color_type *)&attr.fill);
			agg::render_scanlines(rasterizer, scanline_p, renderer);
		}
		else {
			rendererb.color(*(color_type *)&attr.fill);
			agg::render_scanlines(rasterizer, scanline_p, rendererb);
		}
		rasterizer.reset();
	}
	inpath = false;
	return *this;
}

SDraw& SDraw::FillMask(int alpha)
{
	if(mask.GetCount() == 0) {
		mask.Add().Alloc(size.cx * size.cy);
		memset(~mask.Top(), 255, size.cx * size.cy);
	}

	agg::rendering_buffer mask_rbuf;
	mask_rbuf.attach(~mask.Top(), size.cx, size.cy, size.cx);

	typedef agg::renderer_base<agg::pixfmt_gray8> ren_base;
	
	agg::pixfmt_gray8 pixf(mask_rbuf);
	ren_base rb(pixf);
	agg::scanline_p8 sl;

	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	rasterizer.filling_rule(attr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	rasterizer.add_path(curved_trans);
	if(attr.antialiased) {
		agg::renderer_scanline_aa_solid<ren_base> r(rb);
		r.color(agg::gray8(alpha, 255));
		agg::render_scanlines(rasterizer, sl, r);
	}
	else {
		agg::renderer_scanline_bin_solid<ren_base> r(rb);
		r.color(agg::gray8(alpha, 255));
		agg::render_scanlines(rasterizer, sl, r);
	}
	rasterizer.reset();
	inpath = false;
	return *this;
}

SDraw& SDraw::Fill(const Image& image, const Matrix2D& transsrc, int alpha, bool tile)
{
	if(image.GetWidth() == 0 || image.GetHeight() == 0)
		return *this;
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
	if(mask.GetCount()) {
		agg::rendering_buffer mask_rbuf;
		mask_rbuf.attach(~mask.Top(), size.cx, size.cy, size.cx);
		agg::alpha_mask_gray8 mask(mask_rbuf);
		agg::scanline_u8_am<agg::alpha_mask_gray8> sl(mask);
		renderer.color(*(color_type *)&attr.fill);
		agg::render_scanlines_aa(rasterizer, sl, renb, sa, sg);
	}
	else
		agg::render_scanlines_aa(rasterizer, scanline_p, renb, sa, sg);
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
	if(mask.GetCount()) {
		agg::rendering_buffer mask_rbuf;
		mask_rbuf.attach(~mask.Top(), size.cx, size.cy, size.cx);
		agg::alpha_mask_gray8 mask(mask_rbuf);
		agg::scanline_u8_am<agg::alpha_mask_gray8> sl(mask);
		if(attr.antialiased) {
			renderer.color(*(color_type *)&attr.stroke);
			agg::render_scanlines(rasterizer, sl, renderer);
		}
		else {
			rendererb.color(*(color_type *)&attr.stroke);
			agg::render_scanlines(rasterizer, sl, rendererb);
		}
	}
	else
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

SDraw& SDraw::Fill(const RGBA& rgba)
{
	attr.fill = rgba;
	Fill();
	return *this;
}

SDraw& SDraw::Stroke(const RGBA& rgba, double width)
{
	attr.stroke = rgba;
	attr.width = width;
	Stroke();
	return *this;
}

SDraw& SDraw::Stroke(const RGBA& rgba)
{
	attr.stroke = rgba;
	Stroke();
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
	sizef = size;
	UPP::Fill(~buffer, White(), buffer.GetLength()); //!!!
	rbuf.attach((agg::int8u *)~buffer, size.cx, size.cy, size.cx * 4);
	pixf.attach(rbuf);
	renb.attach(pixf);
	renderer.attach(renb);
	rendererb.attach(renb);
	inpath = false;
	pathrect = Null;
	control = current = Null;

	attr.cap = LINECAP_BUTT;
	attr.join = LINEJOIN_MITER;
	attr.miter_limit = 4;
	attr.width = 1;
	attr.evenodd = false;
	attr.fill = attr.stroke = Black();
	attr.antialiased = true;
}

END_UPP_NAMESPACE
