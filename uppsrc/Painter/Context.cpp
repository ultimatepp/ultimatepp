#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::BeginOp()
{
	attrstack.Add(attr);
	attr.hasclip = false;
}

void BufferPainter::EndOp()
{
	if(attrstack.GetCount() == 0) {
		NEVER_("Painter::End: attribute stack is empty");
		return;
	}
	pathattr = attr = attrstack.Pop();
	clip.SetCount(attr.cliplevel);
	if(attr.mask)
		FinishMask();
}

void   BufferPainter::TransformOp(const Matrix2D& m)
{
	ASSERT_(!inpath, "Cannot change transformation during path definition");
	attr.mtx = m * attr.mtx;
	pathattr.mtx = attr.mtx;
}

void BufferPainter::OpacityOp(double o)
{
	pathattr.opacity *= o;
	if(!inpath)
		attr.opacity *= o;
}

void BufferPainter::LineCapOp(int linecap)
{
	pathattr.cap = linecap;
	if(!inpath)
		attr.cap = linecap;
}

void BufferPainter::LineJoinOp(int linejoin)
{
	pathattr.join = linejoin;
	if(!inpath)
		attr.join = linejoin;
}

void BufferPainter::MiterLimitOp(double l)
{
	pathattr.miter_limit = l;
	if(!inpath)
		attr.miter_limit = l;
}

void BufferPainter::EvenOddOp(bool evenodd)
{
	pathattr.evenodd = evenodd;
	if(!inpath)
		attr.evenodd = evenodd;
}

void BufferPainter::DashOp(const Vector<double>& dash, double start)
{
	pathattr.dash <<= dash;
	pathattr.dash_start = start;
	if(!inpath) {
		attr.dash <<= dash;
		attr.dash_start = start;
	}
}

void BufferPainter::NoAAOp(bool noaa)
{
	pathattr.noaa = noaa;
	if(!inpath)
		attr.noaa = noaa;
}

END_UPP_NAMESPACE
