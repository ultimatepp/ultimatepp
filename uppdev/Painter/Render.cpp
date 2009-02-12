#include "Painter.h"
#include "Fillers.h"

NAMESPACE_UPP

void BufferPainter::ClearOp(const RGBA& color)
{
	UPP::Fill(~ib, color, ib.GetLength());
}

Buffer<ClipLine> BufferPainter::RenderPath(double width, SpanSource *ss, const RGBA& color)
{
	Buffer<ClipLine> newclip;
	if(width == 0)
		return newclip;
	Transformer trans(pathattr.mtx);
	trans.target = &rasterizer;
	LinearPathConsumer *g;
	Stroker stroker;
	Dasher dasher;
	bool evenodd = pathattr.evenodd;
	if(width > 0) {
		stroker.Init(width, pathattr.miter_limit, pathattr.tolerance, pathattr.cap, pathattr.join);
		stroker.target = &trans;
		if(pathattr.dash.GetCount()) {
			dasher.Init(pathattr.dash, pathattr.dash_start);
			dasher.target = &stroker;
			g = &dasher;
		}
		else
			g = &stroker;
		evenodd = false;
	}
	else {
		Close();
		g = &trans;
	}
	byte *data = path.data;
	int opacity = int(256 * pathattr.opacity);
	Pointf pos = Pointf(0, 0);
	int i = 0;
	Rasterizer::Filler *rg;
	SpanFiller          span_filler;
	SolidFiller         solid_filler;
	ClipFiller          clip_filler(ib.GetWidth());
	NoAAFillerFilter    noaa_filler;
	bool doclip = width == -2;
	if(doclip) {
		rg = &clip_filler;
		newclip.Alloc(ib.GetHeight());
	}
	else
	if(ss) {
		if(!span)
			span.Alloc(ib.GetWidth() + 1);
		span_filler.ss = ss;
		span_filler.buffer = span;
		span_filler.alpha = opacity;
		rg = &span_filler;
	}
	else {
		solid_filler.c = Mul8(color, opacity);
		rg = &solid_filler;
	}
	if(pathattr.noaa) {
		noaa_filler.Set(rg);
		rg = &noaa_filler;
	}
	for(;;) {
		if(i >= path.type.GetCount() || path.type[i] == DIV) {
			g->End();
			for(int y = rasterizer.MinY(); y <= rasterizer.MaxY(); y++) {
				solid_filler.t = span_filler.t = ib[y];
				span_filler.y = y;
				Rasterizer::Filler *rf = rg;
				MaskFillerFilter mf;
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
			if(i >= path.type.GetCount())
				break;
		}
		else
		switch(path.type[i]) {
		case MOVE: {
			const LinearData *d = (LinearData *)data;
			data += sizeof(LinearData);
			g->Move(pos = d->p);
			break;
		}
		case LINE: {
			const LinearData *d = (LinearData *)data;
			data += sizeof(LinearData);
			g->Line(pos = d->p);
			break;
		}
		case QUADRATIC: {
			const QuadraticData *d = (QuadraticData *)data;
			data += sizeof(QuadraticData);
			ApproximateQuadratic(*g, pos, d->p1, d->p, pathattr.tolerance);
			pos = d->p;
			break;
		}
		case CUBIC: {
			const CubicData *d = (CubicData *)data;
			data += sizeof(CubicData);
			ApproximateCubic(*g, pos, d->p1, d->p2, d->p, pathattr.tolerance);
			pos = d->p;
			break;
		}
		case ARC: {
			const ArcData *d = (ArcData *)data;
			data += sizeof(ArcData);
			ApproximateArc(*g, d->p, d->r, d->angle, d->sweep, pathattr.tolerance);
			pos = d->EndPoint();
			break;
		}
		default:
			NEVER();
			return newclip;
		}
		i++;
	}
	current = Null;
	return newclip;
}

void BufferPainter::FillOp(const RGBA& color)
{
	RenderPath(-1, NULL, color);
}

void BufferPainter::StrokeOp(double width, const RGBA& color)
{
	RenderPath(width, NULL, color);
}

void BufferPainter::ClipOp()
{
	Buffer<ClipLine> newclip = RenderPath(-2, NULL, RGBAZero());
	if(attr.hasclip)
		clip.Top() = newclip;
	else {
		clip.Add() = newclip;
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
}

END_UPP_NAMESPACE
