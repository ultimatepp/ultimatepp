#include "Painter.h"

namespace Upp {

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
	if(clip.GetCount() != attr.cliplevel || attr.mask || attr.onpath)
		Finish();
	clip.SetCount(attr.cliplevel);
	if(attr.mask)
		FinishMask();
	if(attr.onpath) {
		attr.onpath = false;
		onpath = pick(onpathstack.Top());
		onpathstack.Drop();
		pathlen = pathlenstack.Pop();
	}
}

void   BufferPainter::TransformOp(const Xform2D& m)
{
	ASSERT_(IsNull(current), "Cannot change transformation during path definition");
	pathattr.mtx = attr.mtx = m * attr.mtx;
	pathattr.mtx_serial = attr.mtx_serial = ++mtx_serial;
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

void BufferPainter::InvertOp(bool invert)
{
	pathattr.invert = invert;
	if(IsNull(current))
		attr.invert = invert;
}

Vector<double> StringToDash(const String& dash, double& start);

void BufferPainter::DashOp(const String& dash, double start)
{
	String key;
	if(start) {
		key.Cat(1);
		RawCat(key, start);
	}
	else
		key.Cat(2);
	RawCat(key, dash);
	const DashInfo *d = dashes.FindPtr(key);
	if(!d) {
		DashInfo& h = dashes.Add(key);
		h.dash = StringToDash(dash, start);
		h.start = start;
		d = &h;
	}
	pathattr.dash = d;
	if(IsNull(current))
		attr.dash = d;
}

void BufferPainter::DashOp(const Vector<double>& dash, double start)
{
	String key;
	RawCat(key, 0);
	RawCat(key, start);
	for(double h : dash)
		RawCat(key, h);
	const DashInfo *d = dashes.FindPtr(key);
	if(!d) {
		DashInfo& h = dashes.Add(key);
		h.start = start;
		h.dash = clone(dash);
		d = &h;
	}
	pathattr.dash = d;
	if(IsNull(current))
		attr.dash = d;
}

void BufferPainter::ColorStop0(Attr& a, double pos, const RGBA& color)
{
	pos = minmax(pos, 0.0, 1.0);
	ColorStop c;
	c.color = color;
	c.stop = pos;
	int i = FindLowerBound(a.color_stop, c);
	a.color_stop.Insert(i, c);
}

void BufferPainter::ColorStopOp(double pos, const RGBA& color)
{
	ColorStop0(pathattr, pos, color);
	if(IsNull(current))
		ColorStop0(attr, pos, color);
}

void BufferPainter::ClearStopsOp()
{
	pathattr.color_stop.Clear();
	if(IsNull(current))
		attr.color_stop.Clear();
}

void BufferPainter::Create(ImageBuffer& ib, int mode_)
{
	ip = &ib;
	
	if(mode_ != mode || (Size)size != ib.GetSize()) {
		mode = mode_;
	
		rasterizer.Create(ib.GetWidth(), ib.GetHeight(), mode == MODE_SUBPIXEL);
		paths.Alloc(BATCH_SIZE);
		path_info = paths;
	
		ClearPath();
	
		render_cx = ib.GetWidth();
		if(mode == MODE_SUBPIXEL) {
			render_cx *= 3;
			subpixel.Alloc(render_cx + 30);
		}
		size = ib.GetSize();
	}

	Attr& a = attr;
	a.cap = LINECAP_BUTT;
	a.join = LINEJOIN_MITER;
	a.miter_limit = 4;
	a.evenodd = false;
	a.hasclip = false;
	a.cliplevel = 0;
	a.opacity = 1;
	a.dash = NULL;
	a.mask = false;
	a.invert = false;
	a.mtx_serial = 0;
	
	gradientn = Null;
	
	jobcount = fillcount = 0;
	cojob.Clear();
	cofill.Clear();

	attrstack.Clear();
	clip.Clear();
	mask.Clear();
	onpathstack.Clear();
	pathlenstack.Clear();
	onpath.Clear();

	preclip_mtx_serial = -1;
	path_index = 0;
}

BufferPainter::BufferPainter(PainterTarget& t, double tolerance)
:	BufferPainter(dummy, MODE_ANTIALIASED)
{
	alt = &t;
	alt_tolerance = tolerance;
	dummy.Create(1, 1);
}

}
