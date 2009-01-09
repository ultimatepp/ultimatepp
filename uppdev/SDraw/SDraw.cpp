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
	control = current;
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

void   SDraw::Transform(const Matrix2D& m)
{
	Cttr().mtx = m * attr.mtx;
}

SDraw& SDraw::Opacity(double o)            { Cttr().opacity *= o; return *this; }
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
	attr.dash_start = 0.0;
	attr.opacity = 1.0;
}

END_UPP_NAMESPACE
