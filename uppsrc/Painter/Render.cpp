#include "Painter.h"
#include "Fillers.h"

namespace Upp {

#define LLOG(x)   // DLOG(x)
#define LDUMP(x)  // DDUMP(x)

void PainterTarget::Fill(double width, SpanSource *ss, const RGBA& color) {}

void BufferPainter::ClearOp(const RGBA& color)
{
	UPP::Fill(~ib, color, ib.GetLength());
}

BufferPainter::PathJob::PathJob(Rasterizer& rasterizer, double width, bool ischar, int dopreclip,
                                Pointf path_min, Pointf path_max, const Attr& attr)
:	trans(attr.mtx)
{
	evenodd = attr.evenodd;
	regular = attr.mtx.IsRegular() && width < 0 && !ischar;

	g = &rasterizer;
	Rectf preclip = Null;
	preclipped = false;
	if((dopreclip == 1 || dopreclip == 2 && attr.dash.GetCount()) && width != ONPATH) {
		preclip = rasterizer.GetClip();
		Xform2D imx = Inverse(attr.mtx);
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
			preclipped = true;
			return;
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
		stroker.Init(width, attr.miter_limit, tolerance, attr.cap, attr.join, preclip);
		stroker.target = g;
		if(attr.dash.GetCount()) {
			dasher.Init(attr.dash, attr.dash_start);
			dasher.target = &stroker;
			g = &dasher;
		}
		else
			g = &stroker;
		evenodd = false;
	}
}

void BufferPainter::RenderPathSegments(LinearPathConsumer *g, const String& path,
                                       const Attr *attr, double tolerance)
{
	Pointf pos = Pointf(0, 0);
	const char *data = ~path;
	const char *end = path.end();
	while(data < end)
		switch(*data++) {
		case MOVE: {
			const LinearData *d = (LinearData *)data;
			data += sizeof(LinearData);
			g->Move(pos = attr ? attr->mtx.Transform(d->p) : d->p);
			break;
		}
		case LINE: {
			PAINTER_TIMING("LINE");
			const LinearData *d = (LinearData *)data;
			data += sizeof(LinearData);
			g->Line(pos = attr ? attr->mtx.Transform(d->p) : d->p);
			break;
		}
		case QUADRATIC: {
			PAINTER_TIMING("QUADRATIC");
			const QuadraticData *d = (QuadraticData *)data;
			data += sizeof(QuadraticData);
			if(attr) {
				Pointf p = attr->mtx.Transform(d->p);
				ApproximateQuadratic(*g, pos, attr->mtx.Transform(d->p1), p, tolerance);
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
			if(attr) {
				Pointf p = attr->mtx.Transform(d->p);
				ApproximateCubic(*g, pos, attr->mtx.Transform(d->p1),
				                 attr->mtx.Transform(d->p2), p, tolerance);
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
			g->End();
			return;
		}
	g->End();
}

Buffer<ClippingLine> BufferPainter::RenderPath(double width, Event<One<SpanSource>&> ss, const RGBA& color)
{
	PAINTER_TIMING("RenderPath");
	Buffer<ClippingLine> newclip;

	if(width == FILL)
		Close();

	if(width == 0 || !ss && color.a == 0 && width >= FILL) {
		current = Null;
		return newclip;
	}

	if(co && width >= FILL && !ss && !alt && mode == MODE_ANTIALIASED) {
		for(const String& p : path) {
			while(jobcount >= cojob.GetCount())
				cojob.Add().rasterizer.Create(ib.GetWidth(), ib.GetHeight(), false);
			CoJob& job = cojob[jobcount++];
			job.path = p;
			job.attr = pathattr;
			job.width = width;
			job.color = color;
			job.ischar = ischar;
			job.path_min = path_min;
			job.path_max = path_max;
			current = Null;
			if(jobcount >= 1024) {
				LDUMP("Finish A");
				Finish();
			}
		}
		return newclip;
	}
	
	Finish();
	
	rasterizer.Reset();

	PathJob j(rasterizer, width, ischar, dopreclip, path_min, path_max, pathattr);
	if(j.preclipped) {
		current = Null;
		return newclip;
	}
	
	bool doclip = width == CLIP;
	auto fill = [&](CoWork *co) {
		int opacity = int(256 * pathattr.opacity);
		Rasterizer::Filler *rg;
		SpanFiller          span_filler;
		SolidFiller         solid_filler;
		SubpixelFiller      subpixel_filler;
		ClipFiller          clip_filler(render_cx);
		NoAAFillerFilter    noaa_filler;
		MaskFillerFilter    mf;
		subpixel_filler.sbuffer = subpixel;
		subpixel_filler.invert = pathattr.invert;
		Buffer<RGBA>        co_span;
		One<SpanSource>     rss;
		
		if(doclip) {
			rg = &clip_filler;
			newclip.Alloc(ib.GetHeight());
		}
		else
		if(ss) {
			ss(rss);
			RGBA           *lspan;
			if(co) {
				co_span.Alloc((subpixel ? 3 : 1) * ib.GetWidth() + 3);
				lspan = co_span;
			}
			else {
				if(!span)
					span.Alloc((subpixel ? 3 : 1) * ib.GetWidth() + 3);
				lspan = span;
			}
			if(subpixel) {
				subpixel_filler.ss = ~rss;
				subpixel_filler.buffer = lspan;
				subpixel_filler.alpha = opacity;
				rg = &subpixel_filler;
			}
			else {
				span_filler.ss = ~rss;
				span_filler.buffer = lspan;
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
		if(width != ONPATH) {
			if(alt)
				alt->Fill(width, ~rss, color);
			else {
				PAINTER_TIMING("Fill");
				int ii = 0;
				for(;;) {
					int y = (co ? co->Next() : ii++) + rasterizer.MinY();
					if(y > rasterizer.MaxY())
						break;
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
					rasterizer.Render(y, *rf, j.evenodd);
					if(doclip)
						clip_filler.Finish(newclip[y]);
				empty:;
				}
			}
		}
	};
	PAINTER_TIMING("RenderPath2");
	for(const auto& p : path) {
		RenderPathSegments(j.g, p, j.regular ? &pathattr : NULL, j.tolerance);
		int n = rasterizer.MaxY() - rasterizer.MinY();
		if(n >= 0) {
			if(co && !doclip && !alt && n > 6) {
				CoWork co;
				co * [&] { fill(&co); };
			}
			else
				fill(NULL);
			rasterizer.Reset();
		}
	}
	current = Null;
	if(width == ONPATH) {
		onpath = pick(j.onpathtarget.path);
		pathlen = j.onpathtarget.len;
	}
	return newclip;
}

void BufferPainter::CoJob::DoPath(const BufferPainter& sw)
{
	rasterizer.Reset();

	PathJob j(rasterizer, width, ischar, sw.dopreclip, path_min, path_max, attr);
	if(j.preclipped)
		return;
	evenodd = j.evenodd;
	BufferPainter::RenderPathSegments(j.g, path, j.regular ? &attr : NULL, j.tolerance);
}

void BufferPainter::Finish()
{
	if(jobcount == 0)
		return;
	CoWork co;
	const int TH = 3;
	if(jobcount >= TH)
		co * [&] {
			int i;
			while((i = co.Next()) < jobcount)
				cojob[i].DoPath(*this);
		};
	int miny = ib.GetHeight() - 1;
	int maxy = 0;
	for(int i = 0; i < jobcount; i++) {
		CoJob& j = cojob[i];
		if(jobcount < TH)
			j.DoPath(*this);
		miny = min(miny, j.rasterizer.MinY());
		maxy = max(maxy, j.rasterizer.MaxY());
		j.c = Mul8(j.color, int(256 * j.attr.opacity));
	}
	auto fill = [&](CoWork *co) {
		SolidFiller solid_filler;
		int y;
		int ii = 0;
		while((y = (co ? co->Next() : ii++) + miny) <= maxy)
			for(int i = 0; i < jobcount; i++) {
				CoJob& j = cojob[i];
				if(y >= j.rasterizer.MinY() && y <= j.rasterizer.MaxY()) {
					solid_filler.c = j.c;
					solid_filler.invert = j.attr.invert;
					solid_filler.t = ib[y];
					if(clip.GetCount()) {
						MaskFillerFilter mf;
						const ClippingLine& s = clip.Top()[y];
						if(!s.IsEmpty() && !s.IsFull()) {
							mf.Set(&solid_filler, s);
							j.rasterizer.Render(y, mf, j.evenodd);
						}
					}
					else
						j.rasterizer.Render(y, solid_filler, j.evenodd);
				}
			}
	};

	int n = maxy - miny;
	if(n >= 0) {
		if(n > 6)
			co * [&] { fill(&co); };
		else
			fill(NULL);
	}

	jobcount = 0;
}

void BufferPainter::FillOp(const RGBA& color)
{
	RenderPath(FILL, Null, color);
}

void BufferPainter::StrokeOp(double width, const RGBA& color)
{
	RenderPath(width, Null, color);
}

void BufferPainter::ClipOp()
{
	Finish();
	Buffer<ClippingLine> newclip = RenderPath(CLIP, Null, RGBAZero());
	if(attr.hasclip)
		clip.Top() = pick(newclip);
	else {
		clip.Add() = pick(newclip);
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
}

}
