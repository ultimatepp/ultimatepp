#include "SDraw.h"

NAMESPACE_UPP

SDraw& SDraw::Fill(const RGBA& c)
{
	if(inpath)
		path.close_polygon();
	RGBA color = c;
	if(PathVisible(0) && color.a) {
		if(pathattr.opacity != 1.0) {
			color.a = int(color.a * pathattr.opacity);
			color.r = int(color.r * pathattr.opacity);
			color.g = int(color.g * pathattr.opacity);
			color.b = int(color.b * pathattr.opacity);
		}
		if(color.a) {
			rasterizer.reset();
			rasterizer.filling_rule(pathattr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
			rasterizer.add_path(curved_trans);
			if(clip.GetCount()) {
				agg::rendering_buffer mask_rbuf;
				mask_rbuf.attach(~clip.Top(), size.cx, size.cy, size.cx);
				agg::alpha_mask_gray8 mask(mask_rbuf);
				agg::scanline_u8_am<agg::alpha_mask_gray8> sl(mask);
				renderer.color(*(color_type *)&color);
				agg::render_scanlines(rasterizer, sl, renderer);
			}
			else {
				renderer.color(*(color_type *)&color);
				agg::render_scanlines(rasterizer, scanline_p, renderer);
			}
			rasterizer.reset();
		}
	}
	inpath = false;
	return *this;
}

SDraw& SDraw::Fill(const Image& image, const Matrix2D& transsrc, dword flags)
{
	if(image.GetWidth() == 0 || image.GetHeight() == 0)
		return *this;
	span_alloc sa;
	Matrix2D m = transsrc * pathattr.mtx;
	m.invert();
	interpolator_type interpolator(m);
	Size isz = image.GetSize();
	agg::rendering_buffer buf((agg::int8u*)~image, isz.cx, isz.cy, isz.cx * sizeof(RGBA));
	pixfmt img_pixf(buf);
	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	rasterizer.filling_rule(pathattr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	rasterizer.add_path(curved_trans);
	span_gen_type sg(img_pixf, agg::rgba8_pre(0, 0, 0, 0), interpolator);
	sg.alpha(int(pathattr.opacity * 255));
	sg.tile(flags);
	if(clip.GetCount()) {
		agg::rendering_buffer mask_rbuf;
		mask_rbuf.attach(~clip.Top(), size.cx, size.cy, size.cx);
		agg::alpha_mask_gray8 mask(mask_rbuf);
		agg::scanline_u8_am<agg::alpha_mask_gray8> sl(mask);
		agg::render_scanlines_aa(rasterizer, sl, renb, sa, sg);
	}
	else
		agg::render_scanlines_aa(rasterizer, scanline_p, renb, sa, sg);
	rasterizer.reset();
	inpath = false;
	return *this;
}

SDraw& SDraw::Fill(const Image& image, double x1, double y1, double x2, double y2, dword flags)
{
	Matrix2D m;
	Size sz = image.GetSize();
	m.scale(agg::calc_distance(x1, y1, x2, y2) / sz.cx);
	if(abs(x2 - x1) < abs(y2 - y1) / 1000000)
		m.rotate(y2 > y1 ? M_PI_2 : -M_PI_2);
	else
		m.rotate(atan((y2 - y1) / (x2 - x1)));
	m.translate(x1, y1);
	return Fill(image, m, flags);
}

END_UPP_NAMESPACE
