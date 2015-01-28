#include "Painter.h"
#include "Fillers.h"

NAMESPACE_UPP

#define LLOG(x) // DLOG(x)

void BufferPainter::ClearOp(const RGBA& color)
{
	UPP::Fill(~ib, color, ib.GetLength());
}

struct BufferPainter::OnPathTarget : LinearPathConsumer {
	Vector<BufferPainter::PathLine> path;
	Pointf pos;
	double len;
	
	virtual void Move(const Pointf& p) {
		BufferPainter::PathLine& t = path.Add();
		t.len = 0;
		pos = t.p = p;
	}
	virtual void Line(const Pointf& p) {
		BufferPainter::PathLine& t = path.Add();
		len += (t.len = Distance(pos, p));
		pos = t.p = p;
	}
	
	OnPathTarget() { len = 0; pos = Pointf(0, 0); }
};

Buffer<ClippingLine> BufferPainter::RenderPath(double width, SpanSource *ss, const RGBA& color)
{
	PAINTER_TIMING("RenderPath");
	Buffer<ClippingLine> newclip;
	if(width == 0) {
		current = Null;
		return newclip;
	}
	Transformer trans(pathattr.mtx);
	Stroker stroker;
	Dasher dasher;
	OnPathTarget onpathtarget;
	bool evenodd = pathattr.evenodd;
	bool regular = pathattr.mtx.IsRegular() && width < 0 && !ischar;
	double tolerance;
	LinearPathConsumer *g = &rasterizer;
	Rectf preclip = Null;
	if(dopreclip && width != ONPATH) {
		preclip = rasterizer.GetClip();
		Xform2D imx = Inverse(pathattr.mtx);
		Pointf tl, br, a;
		tl = br = imx.Transform(preclip.TopLeft());
		a = imx.Transform(preclip.TopRight());
		tl = min(a, tl);
		br = max(a, br);
		a = imx.Transform(preclip.BottomLeft());
		tl = min(a, tl);
		br = max(a, br);
		a = imx.Transform(preclip.BottomRight());
		tl = min(a, tl);
		br = max(a, br);
		preclip = Rectf(tl, br);
		
		if(!preclip.Intersects(
				Rectf(path_min, path_max).Inflated(max(width, 0.0) * (1 + attr.miter_limit)))) {
			LLOG("Preclipped " << preclip << ", min " << path_min << ", max " << path_max);
			current = Null;
			return newclip;
		}
	}
	if(regular)
		tolerance = 0.3;
	else {
		trans.target = g;
		g = &trans;
		tolerance = 0.3 / attr.mtx.GetScale();
	}
	if(width == ONPATH) {
		g = &onpathtarget;
		regular = false;
	}
	else
	if(width > 0) {
		stroker.Init(width, pathattr.miter_limit, tolerance, pathattr.cap, pathattr.join, preclip);
		stroker.target = g;
		if(pathattr.dash.GetCount()) {
			dasher.Init(pathattr.dash, pathattr.dash_start);
			dasher.target = &stroker;
			g = &dasher;
		}
		else
			g = &stroker;
		evenodd = false;
	}
	else
		Close();
	byte *data = path.data;
	int opacity = int(256 * pathattr.opacity);
	Pointf pos = Pointf(0, 0);
	int i = 0;
	Rasterizer::Filler *rg;
	SpanFiller          span_filler;
	SolidFiller         solid_filler;
	SubpixelFiller      subpixel_filler;
	ClipFiller          clip_filler(render_cx);
	NoAAFillerFilter    noaa_filler;
	MaskFillerFilter    mf;
	bool doclip = width == CLIP;
	subpixel_filler.sbuffer = subpixel;
	subpixel_filler.invert = pathattr.invert;
	if(doclip) {
		rg = &clip_filler;
		newclip.Alloc(ib.GetHeight());
	}
	else
	if(ss) {
		if(!span)
			span.Alloc((subpixel ? 3 : 1) * ib.GetWidth() + 3);
		if(subpixel) {
			subpixel_filler.ss = ss;
			subpixel_filler.buffer = span;
			subpixel_filler.alpha = opacity;
			rg = &subpixel_filler;
		}
		else {
			span_filler.ss = ss;
			span_filler.buffer = span;
			span_filler.alpha = opacity;
			rg = &span_filler;
		}
	}
	else {
		if(subpixel) {
			subpixel_filler.color = Mul8(color, opacity);
			subpixel_filler.ss = NULL;
			rg = &subpixel_filler;
		}
		else {
			solid_filler.c = Mul8(color, opacity);
			solid_filler.invert = pathattr.invert;
			rg = &solid_filler;
		}
	}
	if(mode == MODE_NOAA) {
		noaa_filler.Set(rg);
		rg = &noaa_filler;
	}
	PAINTER_TIMING("RenderPath2");
	for(;;) {
		if(i >= path.type.GetCount() || path.type[i] == DIV) {
			g->End();
			if(width != ONPATH) {
				PAINTER_TIMING("Fill");
				for(int y = rasterizer.MinY(); y <= rasterizer.MaxY(); y++) {
					solid_filler.t = subpixel_filler.t = span_filler.t = ib[y];
					subpixel_filler.end = subpixel_filler.t + ib.GetWidth();
					span_filler.y = subpixel_filler.y = y;
					Rasterizer::Filler *rf = rg;
					if(clip.GetCount()) {
						const ClippingLine& s = clip.Top()[y];
						if(s.IsEmpty()) goto empty;
						if(!s.IsFull()) {
							mf.Set(rg, s);
							rf = &mf;
						}
					}
					if(doclip)
						clip_filler.Clear();
					rasterizer.Render(y, *rf, evenodd);
					if(doclip)
						clip_filler.Finish(newclip[y]);
				empty:;
				}
				rasterizer.Reset();
			}
			if(i >= path.type.GetCount())
				break;
		}
		else
		switch(path.type[i]) {
		case MOVE: {
			const LinearData *d = (LinearData *)data;
			data += sizeof(LinearData);
			g->Move(pos = regular ? pathattr.mtx.Transform(d->p) : d->p);
			break;
		}
		case LINE: {
			PAINTER_TIMING("LINE");
			const LinearData *d = (LinearData *)data;
			data += sizeof(LinearData);
			g->Line(pos = regular ? pathattr.mtx.Transform(d->p) : d->p);
			break;
		}
		case QUADRATIC: {
			PAINTER_TIMING("QUADRATIC");
			const QuadraticData *d = (QuadraticData *)data;
			data += sizeof(QuadraticData);
			if(regular) {
				Pointf p = pathattr.mtx.Transform(d->p);
				ApproximateQuadratic(*g, pos, pathattr.mtx.Transform(d->p1), p, tolerance);
				pos = p;
			}
			else {
				ApproximateQuadratic(*g, pos, d->p1, d->p, tolerance);
				pos = d->p;
			}
			break;
		}
		case CUBIC: {
			PAINTER_TIMING("CUBIC");
			const CubicData *d = (CubicData *)data;
			data += sizeof(CubicData);
			if(regular) {
				Pointf p = pathattr.mtx.Transform(d->p);
				ApproximateCubic(*g, pos, pathattr.mtx.Transform(d->p1),
				                 pathattr.mtx.Transform(d->p2), p, tolerance);
				pos = p;
			}
			else {
				ApproximateCubic(*g, pos, d->p1, d->p2, d->p, tolerance);
				pos = d->p;
			}
			break;
		}
		case CHAR:
			ApproximateChar(*g, *(CharData *)data, tolerance);
			data += sizeof(CharData);
			break;
		default:
			NEVER();
			return newclip;
		}
		i++;
	}
	current = Null;
	if(width == ONPATH) {
		onpath = pick(onpathtarget.path);
		pathlen = onpathtarget.len;
	}
	return newclip;
}

void BufferPainter::FillOp(const RGBA& color)
{
	RenderPath(FILL, NULL, color);
}

void BufferPainter::StrokeOp(double width, const RGBA& color)
{
	RenderPath(width, NULL, color);
}

void BufferPainter::ClipOp()
{
	Buffer<ClippingLine> newclip = RenderPath(CLIP, NULL, RGBAZero());
	if(attr.hasclip)
		clip.Top() = pick(newclip);
	else {
		clip.Add() = pick(newclip);
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
}

END_UPP_NAMESPACE
