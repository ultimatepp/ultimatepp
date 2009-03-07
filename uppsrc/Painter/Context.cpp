#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::BeginOp()
{
	attr.onpath = false;
	attrstack.Add(attr);
	attr.hasclip = false;
}

void BufferPainter::EndOp()
{
	if(attrstack.GetCount() == 0) {
		NEVER_("Painter::End: attribute stack is empty");
		return;
	}
	pathattr = attr = attrstack.Top();
	attrstack.Drop();
	clip.SetCount(attr.cliplevel);
	if(attr.mask)
		FinishMask();
	if(attr.onpath) {
		attr.onpath = false;
		onpath = onpathstack.Top();
		onpathstack.Drop();
		pathlen = pathlenstack.Pop();
	}
}

void   BufferPainter::TransformOp(const Xform2D& m)
{
	ASSERT_(IsNull(current), "Cannot change transformation during path definition");
	pathattr.mtx = attr.mtx = m * attr.mtx;
}

void BufferPainter::OpacityOp(double o)
{
	pathattr.opacity *= o;
	if(IsNull(current))
		attr.opacity *= o;
}

void BufferPainter::LineCapOp(int linecap)
{
	pathattr.cap = linecap;
	if(IsNull(current))
		attr.cap = linecap;
}

void BufferPainter::LineJoinOp(int linejoin)
{
	pathattr.join = linejoin;
	if(IsNull(current))
		attr.join = linejoin;
}

void BufferPainter::MiterLimitOp(double l)
{
	pathattr.miter_limit = l;
	if(IsNull(current))
		attr.miter_limit = l;
}

void BufferPainter::EvenOddOp(bool evenodd)
{
	pathattr.evenodd = evenodd;
	if(IsNull(current))
		attr.evenodd = evenodd;
}

void BufferPainter::DashOp(const Vector<double>& dash, double start)
{
	pathattr.dash <<= dash;
	pathattr.dash_start = start;
	if(IsNull(current)) {
		attr.dash <<= dash;
		attr.dash_start = start;
	}
}

void BufferPainter::ColorStop0(Attr& a, double pos, const RGBA& color)
{
	pos = minmax(pos, 0.0, 1.0);
	int i = FindLowerBound(a.stop, pos);
	a.stop.Insert(i, pos);
	a.stop_color.Insert(i, color);
}

void BufferPainter::ColorStopOp(double pos, const RGBA& color)
{
	ColorStop0(pathattr, pos, color);
	if(IsNull(current))
		ColorStop0(attr, pos, color);
}

void BufferPainter::ClearStopsOp()
{
	pathattr.stop.Clear();
	pathattr.stop_color.Clear();
	if(IsNull(current)) {
		attr.stop.Clear();
		attr.stop_color.Clear();
	}
}

BufferPainter::BufferPainter(ImageBuffer& ib, int mode)
:	ib(ib),
	mode(mode),
	rasterizer(ib.GetWidth(), ib.GetHeight(), mode == MODE_SUBPIXEL)
{
	ClearPath();

	render_cx = ib.GetWidth();
	if(mode == MODE_SUBPIXEL) {
		render_cx *= 3;
		subpixel.Alloc(render_cx + 30);
	}
	attr.cap = LINECAP_BUTT;
	attr.join = LINEJOIN_MITER;
	attr.miter_limit = 4;
	attr.evenodd = false;
	attr.hasclip = false;
	attr.cliplevel = 0;
	attr.dash_start = 0.0;
	attr.opacity = 1.0;
	attr.mask = false;
	pathattr = attr;
	
	gradientn = Null;
}

END_UPP_NAMESPACE
