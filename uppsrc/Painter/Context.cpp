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
	if(clip.GetCount() != attr.cliplevel || attr.mask)
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
	current = Null;
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

void BufferPainter::ImageFilterOp(int filter)
{
	pathattr.filter = filter;
	if(IsNull(current))
		attr.invert = filter;
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
	ip->SetKind(IMAGE_ALPHA);

	if(mode_ != mode || (Size)size != ib.GetSize()) {
		mode = mode_;
	
		rasterizer.Create(ib.GetWidth(), ib.GetHeight(), mode == MODE_SUBPIXEL);
		cojob.Clear();
		cofill.Clear();
	
		render_cx = ib.GetWidth();
		if(mode == MODE_SUBPIXEL) {
			render_cx *= 3;
			subpixel.Alloc(render_cx + 30);
		}
		else
			subpixel.Clear();
		size = ib.GetSize();

		co_subpixel.Clear();
		co_span.Clear();
		span.Clear();
		
		co_clear.Clear();
	}

	SyncCo();

	if(!paths)
		paths.Alloc(BATCH_SIZE);

	path_info = paths;

	attr.mtx = Xform2D::Identity();
	attr.cap = LINECAP_BUTT;
	attr.join = LINEJOIN_MITER;
	attr.miter_limit = 4;
	attr.evenodd = false;
	attr.hasclip = false;
	attr.cliplevel = 0;
	attr.opacity = 1;
	attr.dash = NULL;
	attr.mask = false;
	attr.invert = false;
	attr.mtx_serial = 0;
	
	pathattr = attr;

	ClearPath();
	
	jobcount = fillcount = emptycount = 0;

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
