#include "Painter.h"

NAMESPACE_UPP

struct PainterRadialSpan : SpanSource {
	LinearInterpolator interpolator;
	double      cx, cy, r, fx, fy;
	int         style;
	int         alpha;
	const RGBA *gradient;
	double      C;
	
	void Set(double _x, double _y, double _r, double _fx, double _fy)
	{
		cx = _x;
		cy = _y;
		r = _r;
		fx = _fx;
		fy = _fy;
		fx -= cx;
		fy -= cy;
		C = fx * fx + fy * fy - r * r;
	}

	void Get(RGBA *_span, int x, int y, unsigned len)
	{
		if(r <= 0)
			return;
		interpolator.Begin(x, y, len);
		RGBA *span = (RGBA *)_span;
		while(len--) {
			Point p = interpolator.Get();
			int h;
			const double q256 = 1 / 256.0;
			double dx = q256 * p.x - cx - fx;
			double dy = q256 * p.y - cy - fy;
			if(dx == 0 && dy == 0)
				h = 0;
			else {
				double A = dx * dx + dy * dy;
				double b = (fx * dx + fy * dy);
				double t = (sqrt(b * b - A * C) - b) / (A);
				h = t >= 0.001 ? int(2047 / t) : 2047;
			}
			if(style == GRADIENT_REPEAT)
				h = h & 2047;
			else
			if(style == GRADIENT_REFLECT)
				h = (h & 2048) ? (2047 - h & 2047) : (h & 2047);
			else
				h = minmax(h, 0, 2047);
			*span++ = gradient[h];
		}
	}
};

void BufferPainter::RenderRadial(double width, const Pointf& f, const RGBA& color1,
                                 const Pointf& c, double r, const RGBA& color2,
                                 const Xform2D& m, int style)
{
	PainterRadialSpan sg;
	sg.interpolator.Set(Inverse(m));
	sg.style = style;
	sg.Set(c.x, c.y, r, f.x, f.y);
	MakeGradient(color1, color2, 2048);
	sg.gradient = gradient[0];
	RenderPath(width, &sg, RGBAZero());
}

void BufferPainter::FillOp(const Pointf& f, const RGBA& color1, const Pointf& c, double r, const RGBA& color2, int style)
{
	RenderRadial(-1, f, color1, c, r, color2, pathattr.mtx, style);
}

void BufferPainter::StrokeOp(double width, const Pointf& f, const RGBA& color1, const Pointf& c, double r, const RGBA& color2, int style)
{
	RenderRadial(width, f, color1, c, r, color2, pathattr.mtx, style);
}

void BufferPainter::RenderRadial(double width, const Pointf& f, const RGBA& color1,
                                 const RGBA& color2, const Xform2D& transsrc, int style)
{
	RenderRadial(width, f, color1, Pointf(0, 0), 1, color2, transsrc * pathattr.mtx, style);
}

void BufferPainter::FillOp(const Pointf& f, const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	RenderRadial(-1, f, color1, color2, transsrc, style);
}

void BufferPainter::StrokeOp(double width, const Pointf& f, const RGBA& color1, const RGBA& color2, const Xform2D& transsrc, int style)
{
	RenderRadial(width, f, color1, color2, transsrc, style);
}

END_UPP_NAMESPACE
