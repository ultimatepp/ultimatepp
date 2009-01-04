#include "SDraw.h"

NAMESPACE_UPP

#define LTIMING(x) RTIMING(x)

void SDraw::PathPoint(double x, double y)
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
		pathattr = attr;
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
	p = pathattr.mtx.Transformed(p);
	minv.x = min(minv.x, p.x);
	minv.y = min(minv.y, p.y);
	maxv.x = max(maxv.x, p.x);
	maxv.y = max(maxv.y, p.y);
}

bool SDraw::PathVisible(double w) const
{
	Pointf h = pathattr.mtx.Transformed(w, w);
	w = max(abs(h.x), abs(h.y));
	Pointf min;
	Pointf max;
	min = max = pathattr.mtx.Transformed(pathrect.TopLeft());
	MinMax(min, max, pathrect.TopRight());
	MinMax(min, max, pathrect.BottomLeft());
	MinMax(min, max, pathrect.BottomRight());
	return max.x + w >= 0 && max.y + w >= 0 && min.x - w <= sizef.cx && min.y - w <= sizef.cy;
}

SDraw& SDraw::Fill(const RGBA& color)
{
	LTIMING("Fill");
	if(inpath)
		path.close_polygon();
	if(PathVisible(0)) {
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
	inpath = false;
	return *this;
}

SDraw& SDraw::Fill(const Image& image, const Matrix2D& transsrc, dword flags, int alpha)
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
	sg.alpha(alpha);
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

SDraw& SDraw::Fill(const Image& image, double x1, double y1, double x2, double y2,
                   dword flags, int alpha)
{
	Matrix2D m;
	Size sz = image.GetSize();
	m.scale(agg::calc_distance(x1, y1, x2, y2) / sz.cx);
	if(abs(x2 - x1) < abs(y2 - y1) / 1000000)
		m.rotate(y2 > y1 ? M_PI_2 : -M_PI_2);
	else
		m.rotate(atan((y2 - y1) / (x2 - x1))); //!!!
	m.translate(x1, y1);
	Fill(image, m, flags, alpha);
	return *this;
}

SDraw::path_storage SDraw::MakeStroke(double width)
{
	double scl = pathattr.mtx.scale();
	curved.approximation_scale(scl);
	curved.angle_tolerance(0.0);
	if(width * scl > 1.0)
		curved.angle_tolerance(0.2);
	rasterizer.reset();
	rasterizer.filling_rule(agg::fill_non_zero);
	path_storage b;
	if(pathattr.dash.GetCount()) {
		agg::conv_dash<Curved> dashed(curved);
		dashed.Set(&pathattr.dash, pathattr.dash_start);
		agg::conv_stroke<agg::conv_dash<Curved> > curved_stroked(dashed);
		curved_stroked.width(width);
		curved_stroked.line_join((agg::line_join_e)pathattr.join);
		curved_stroked.line_cap((agg::line_cap_e)pathattr.cap);
		curved_stroked.miter_limit(pathattr.miter_limit);
		curved_stroked.approximation_scale(scl);
		b.concat_path(curved_stroked);
	}
	else {
		agg::conv_stroke<Curved> curved_stroked(curved);
		curved_stroked.width(width);
		curved_stroked.line_join((agg::line_join_e)pathattr.join);
		curved_stroked.line_cap((agg::line_cap_e)pathattr.cap);
		curved_stroked.miter_limit(pathattr.miter_limit);
		curved_stroked.approximation_scale(scl);
		b.concat_path(curved_stroked);
	}
	return b;
}

SDraw& SDraw::Stroke(double width, const RGBA& color)
{
	path_storage b = MakeStroke(width);
	Swap(b, path);
	inpath = false;
	Fill(color);
	Swap(b, path);
	return *this;
}

SDraw& SDraw::Stroke(double width, const Image& image, const Matrix2D& transsrc,
                     dword flags, int alpha)
{
	path_storage b = MakeStroke(width);
	Swap(b, path);
	inpath = false;
	Fill(image, transsrc, flags, alpha);
	Swap(b, path);
	return *this;
}

SDraw& SDraw::Clip(int alpha)
{
	int l = size.cx * size.cy;
	Buffer<byte> cl(l);
	memset(~cl, 0, l);
	agg::rendering_buffer mask_rbuf;
	mask_rbuf.attach(~cl, size.cx, size.cy, size.cx);
	typedef agg::renderer_base<agg::pixfmt_gray8> ren_base;
	agg::pixfmt_gray8 pixf(mask_rbuf);
	ren_base rb(pixf);
	agg::scanline_p8 sl;
	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	rasterizer.filling_rule(pathattr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	rasterizer.add_path(curved_trans);
	agg::renderer_scanline_aa_solid<ren_base> r(rb);
	r.color(agg::gray8(alpha, 255));
	agg::render_scanlines(rasterizer, sl, r);
	rasterizer.reset();
	inpath = false;
	if(clip.GetCount()) {
		byte *s = ~clip.Top();
		for(int i = 0; i < l; i++)
			cl[i] = ((cl[i] + (cl[i] >> 7)) * *s) >> 8;
	}
	if(attr.hasclip)
		clip.Top() = cl;
	else {
		clip.Add() = cl;
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
	return *this;
}

void SDraw::Translate(double x, double y)
{
	Transform(Translate2D(x, y));
}

void SDraw::Rotate(double a)
{
	Transform(Rotate2D(a));
}

void SDraw::Scale(double scalex, double scaley)
{
	Transform(Scale2D(scalex, scaley));
}

void SDraw::Scale(double scale)
{
	Transform(Scale2D(scale));
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
	clip.SetCount(attr.cliplevel);
}

void   SDraw::Transform(const Matrix2D& m) { Cttr().mtx = m * attr.mtx; }

SDraw& SDraw::LineCap(int linecap)         { Cttr().cap = linecap; return *this; }
SDraw& SDraw::LineJoin(int linejoin)       { Cttr().join = linejoin; return *this; }
SDraw& SDraw::MiterLimit(double l)         { Cttr().miter_limit = l; return *this; }
SDraw& SDraw::EvenOdd(bool evenodd)        { Cttr().evenodd = evenodd; return *this; }

SDraw& SDraw::Dash(const Vector<double>& dash, double start)
{
	Attr& a = Cttr();
	a.dash <<= dash;
	if(dash.GetCount() & 1)
		a.dash.Append(dash);
	a.dash_start = start;
	return *this;
}

SDraw& SDraw::Dash(const char *dash, double start)
{
	Vector<double> d;
	CParser p(dash);
	try {
		while(!p.IsEof())
			if(p.Char(':'))
				start = p.ReadDouble();
			else
				d.Add(p.ReadDouble());
	}
	catch(CParser::Error) {}
	Dash(d, start);
	return *this;
}

SDraw::SDraw(ImageBuffer& ib)
:	buffer(ib),
	curved(path),
	curved_trans(curved, attr.mtx)
{
	size = ib.GetSize();
	sizef = size;
	UPP::Fill(~buffer, White(), buffer.GetLength()); //!!!
	rbuf.attach((agg::int8u *)~buffer, size.cx, size.cy, size.cx * 4);
	pixf.attach(rbuf);
	renb.attach(pixf);
	renderer.attach(renb);
	inpath = false;
	pathrect = Null;
	control = current = Null;

	attr.cap = LINECAP_BUTT;
	attr.join = LINEJOIN_MITER;
	attr.miter_limit = 4;
	attr.evenodd = false;
	attr.hasclip = false;
	attr.cliplevel = 0;
}

END_UPP_NAMESPACE
