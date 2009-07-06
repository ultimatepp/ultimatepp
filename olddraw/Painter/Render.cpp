#include "Painter.h"
#include "Fillers.h"

NAMESPACE_UPP

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

Buffer<ClipLine> BufferPainter::RenderPath(double width, SpanSource *ss, const RGBA& color)
{
	Buffer<ClipLine> newclip;
	if(width == 0)
		return newclip;
	Transformer trans(pathattr.mtx);
	Stroker stroker;
	Dasher dasher;
	OnPathTarget onpathtarget;
	bool evenodd = pathattr.evenodd;
	bool regular = pathattr.mtx.IsRegular() && width < 0 && !ischar;
	double tolerance;
	LinearPathConsumer *g = &rasterizer;
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
		stroker.Init(width, pathattr.miter_limit, tolerance, pathattr.cap, pathattr.join);
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
			rg = &solid_filler;
		}
	}
	if(mode == MODE_NOAA) {
		noaa_filler.Set(rg);
		rg = &noaa_filler;
	}
	for(;;) {
		if(i >= path.type.GetCount() || path.type[i] == DIV) {
			g->End();
			if(width != ONPATH) {
				for(int y = rasterizer.MinY(); y <= rasterizer.MaxY(); y++) {
					solid_filler.t = subpixel_filler.t = span_filler.t = ib[y];
					subpixel_filler.end = subpixel_filler.t + ib.GetWidth();
					span_filler.y = subpixel_filler.y = y;
					Rasterizer::Filler *rf = rg;
					if(clip.GetCount()) {
						const ClipLine& s = clip.Top()[y];
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
			const LinearData *d = (LinearData *)data;
			data += sizeof(LinearData);
			g->Line(pos = regular ? pathattr.mtx.Transform(d->p) : d->p);
			break;
		}
		case QUADRATIC: {
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
		onpath = onpathtarget.path;
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
	Buffer<ClipLine> newclip = RenderPath(CLIP, NULL, RGBAZero());
	if(attr.hasclip)
		clip.Top() = newclip;
	else {
		clip.Add() = newclip;
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
}

END_UPP_NAMESPACE
