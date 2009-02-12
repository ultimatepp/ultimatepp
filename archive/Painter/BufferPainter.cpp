#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::ClearOp(const RGBA& color)
{
	Upp::Fill(~buffer, color, buffer.GetLength());
}

inline void BufferPainter::PathPoint0(double x, double y)
{
	pathrect.left = min(pathrect.left, x);
	pathrect.top = min(pathrect.top, y);
	pathrect.right = max(pathrect.right, x);
	pathrect.bottom = max(pathrect.bottom, y);
}

inline void BufferPainter::PathPoint(double& x, double& y, bool rel)
{
	x = IsNull(x) ? current.x : rel ? x + current.x : x;
	y = IsNull(y) ? current.y : rel ? y + current.y : y;
	if(inpath)
		PathPoint0(x, y);
	else {
		path.remove_all();
		pathrect.left = pathrect.right = x;
		pathrect.top = pathrect.bottom = y;
		pathattr = attr;
	}
	inpath = true;
}

inline void BufferPainter::EndPoint(double& x, double& y, bool rel)
{
	PathPoint(x, y, rel);
	current = Pointf(x, y);
}

void BufferPainter::MoveOp(double x, double y, bool rel)
{
	EndPoint(x, y, rel);
	ccontrol = qcontrol = current;
	path.move_to(x, y);
	inpath = true;
}

void BufferPainter::LineOp(double x, double y, bool rel)
{
	EndPoint(x, y, rel);
	ccontrol = qcontrol = current;
	path.line_to(x, y);
	inpath = true;
}

void BufferPainter::QuadraticOp(double x1, double y1, double x, double y, bool rel)
{
	PathPoint(x1, y1, rel);
	qcontrol = Pointf(x1, y1);
	EndPoint(x, y, rel);
	ccontrol = current;
	path.curve3(x1, y1, x, y);
}

void BufferPainter::QuadraticOp(double x, double y, bool rel)
{
	qcontrol = current + current - qcontrol;
	PathPoint0(qcontrol.x, qcontrol.y);
	EndPoint(x, y, rel);
	ccontrol = current;
	path.curve3(qcontrol.x, qcontrol.y, x, y);
}

void BufferPainter::CubicOp(double x1, double y1, double x2, double y2, double x, double y, bool rel)
{
	PathPoint(x1, y1, rel);
	PathPoint(x2, y2, rel);
	ccontrol = Pointf(x2, y2);
	EndPoint(x, y, rel);
	qcontrol = current;
	path.curve4(x1, y1, x2, y2, x, y);
}

void BufferPainter::CubicOp(double x2, double y2, double x, double y, bool rel)
{
	Pointf c = current + current - ccontrol;
	PathPoint0(c.x, c.y);
	PathPoint(x2, y2, rel);
	ccontrol = Pointf(x2, y2);
	EndPoint(x, y, rel);
	qcontrol = current;
	path.curve4(c.x, c.y, x2, y2, x, y);
}

void BufferPainter::CloseOp()
{
	path.close_polygon();
}

inline void BufferPainter::MinMax(Pointf& minv, Pointf& maxv, Pointf p) const
{
	p = pathattr.mtx.Transformed(p);
	minv.x = min(minv.x, p.x);
	minv.y = min(minv.y, p.y);
	maxv.x = max(maxv.x, p.x);
	maxv.y = max(maxv.y, p.y);
}

bool BufferPainter::PathVisible(double w) const
{
	Pointf h = pathattr.mtx.Transformed(w, w);
	w = max(fabs(h.x), fabs(h.y));
	Pointf min;
	Pointf max;
	min = max = pathattr.mtx.Transformed(pathrect.TopLeft());
	MinMax(min, max, pathrect.TopRight());
	MinMax(min, max, pathrect.BottomLeft());
	MinMax(min, max, pathrect.BottomRight());
	return max.x + w >= 0 && max.y + w >= 0 && min.x - w <= sizef.cx && min.y - w <= sizef.cy;
}

void BufferPainter::SetRbuf()
{
	pixf.attach(buffer);
	renb.attach(pixf);
	renderer.attach(renb);
}

BufferPainter::BufferPainter(ImageBuffer& ib)
:	buffer(ib),
	curved(path),
	curved_trans(curved, attr.mtx)
{
	size = ib.GetSize();
	sizef = size;
	inpath = false;
	pathrect = Null;
	ccontrol = qcontrol = current = Point(0, 0);

	attr.cap = LINECAP_BUTT;
	attr.join = LINEJOIN_MITER;
	attr.miter_limit = 4;
	attr.evenodd = false;
	attr.hasclip = false;
	attr.cliplevel = 0;
	attr.dash_start = 0.0;
	attr.opacity = 1.0;
	attr.mask = false;
	attr.noaa = false;
	pathattr = attr;

	SetRbuf();
}

END_UPP_NAMESPACE
