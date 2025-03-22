#include "Painter.h"
#include "Fillers.h"

namespace Upp {

#define LLOG(x)   // DLOG(x)
#define LDUMP(x)  // DDUMP(x)

void PainterTarget::Fill(double width, SpanSource *ss, const RGBA& color) {}

BufferPainter::PathJob::PathJob(Rasterizer& rasterizer, double width, const PathInfo *path_info,
                                const SimpleAttr& attr, const Rectf& preclip, bool isregular)
:	trans(attr.mtx)
{
	evenodd = attr.evenodd;
	regular = isregular && width < 0 && !path_info->ischar;

	g = &rasterizer;

	if(!IsNull(preclip.left) && !path_info->ischar) {
		double ex = max(width, 0.0) * (1 + attr.miter_limit);
		if(path_info->path_max.y + ex < preclip.top || path_info->path_min.y - ex > preclip.bottom ||
		   path_info->path_max.x + ex < preclip.left || path_info->path_min.x - ex > preclip.right) {
			preclipped = true;
			return;
		}
	}

	preclipped = false;

	if(regular)
		tolerance = 0.3;
	else {
		trans.target = g;
		g = &trans;
		tolerance = 0.3 / attr.mtx.GetScale();
	}

	if(width == BufferPainter::ONPATH) {
		g = &onpathtarget;
		regular = false;
	}
	else
	if(width > 0) {
		stroker.Init(width, attr.miter_limit, tolerance, attr.cap, attr.join, preclip);
		stroker.target = g;
		if(attr.dash) {
			dasher.Init(attr.dash->dash, attr.dash->start);
			dasher.target = &stroker;
			g = &dasher;
		}
		else
			g = &stroker;
		evenodd = false;
	}
}

void BufferPainter::RenderPathSegments(LinearPathConsumer *g, const Vector<byte>& path,
                                       const SimpleAttr *attr, double tolerance)
{
	Pointf pos = Pointf(0, 0);
	const byte *data = path.begin();
	const byte *end = path.end();
	while(data < end) {
		const LinearData *d = (LinearData *)data;
		switch(d->type) {
		case MOVE: {
			g->Move(pos = attr ? attr->mtx.Transform(d->p) : d->p);
			data += sizeof(LinearData);
			break;
		}
		case LINE: {
			PAINTER_TIMING("LINE");
			g->Line(pos = attr ? attr->mtx.Transform(d->p) : d->p);
			data += sizeof(LinearData);
			break;
		}
		case QUADRATIC: {
			PAINTER_TIMING("QUADRATIC");
			const QuadraticData *d = (QuadraticData *)data;
			if(attr) {
				Pointf p = attr->mtx.Transform(d->p);
				ApproximateQuadratic(*g, pos, attr->mtx.Transform(d->p1), p, tolerance);
				pos = p;
			}
			else {
				ApproximateQuadratic(*g, pos, d->p1, d->p, tolerance);
				pos = d->p;
			}
			data += sizeof(QuadraticData);
			break;
		}
		case CUBIC: {
			PAINTER_TIMING("CUBIC");
			const CubicData *d = (CubicData *)data;
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
			data += sizeof(CubicData);
			break;
		}
		case CHAR: {
			const CharData *ch = (CharData *)data;
			ApproximateChar(*g, ch->p, ch->ch, ch->fnt, tolerance);
			data += sizeof(CharData);
			break;
		}
		default:
			NEVER();
			g->End();
			return;
		}
	}
	g->End();
}

void BufferPainter::SyncCo()
{
	if(co && subpixel && !co_subpixel) {
		int n = CoWork::GetPoolSize();
		co_subpixel.Alloc(n);
		for(int i = 0; i < n; i++)
			co_subpixel[i].Alloc(render_cx + 30);
	}
}

Buffer<ClippingLine> BufferPainter::RenderPath(double width, One<SpanSource>& ss, const RGBA& color)
{
	PAINTER_TIMING("RenderPath");
	Buffer<ClippingLine> newclip;

	if(width == FILL)
		Close();

	current = Null;

	if(width == 0 || !ss && color.a == 0 && width >= FILL) {
		if(co && ++emptycount + jobcount >= BATCH_SIZE) { // not to exhaust 'paths'
			FinishPathJob();
			FinishFillJob();
		}
		return newclip;
	}

	if(pathattr.mtx_serial != preclip_mtx_serial) {
		if(dopreclip) {
			Pointf tl, br, a;
			Xform2D imx = Inverse(pathattr.mtx);
			tl = br = imx.Transform(0, 0);
			a = imx.Transform(size.cx, 0);
			tl = min(a, tl);
			br = max(a, br);
			a = imx.Transform(0, size.cy);
			tl = min(a, tl);
			br = max(a, br);
			a = imx.Transform(size.cx, size.cy);
			tl = min(a, tl);
			br = max(a, br);
			preclip = Rectf(tl, br);
		}
		else
			preclip = Null;
		regular = pathattr.mtx.IsRegular();
		preclip_mtx_serial = pathattr.mtx_serial;
	}

	if(co) {
		if(ss && !co_span) {
			int n = CoWork::GetPoolSize();
			co_span.Alloc(n);
			for(int i = 0; i < n; i++)
				co_span[i].Alloc((subpixel ? 3 : 1) * ip->GetWidth() + 3);
		}

		if(width >= FILL && !alt && findarg(mode, MODE_ANTIALIASED, MODE_SUBPIXEL) >= 0) {
			for(int i = 0; i < path_info->path.GetCount(); i++) {
				while(jobcount >= cojob.GetCount())
					cojob.Add().rasterizer.Create(ip->GetWidth(), ip->GetHeight(), mode == MODE_SUBPIXEL);
				CoJob& job = cojob[jobcount++];
				job.path_info = path_info;
				job.subpath = i;
				job.attr = pathattr;
				job.width = width;
				job.color = color;
				job.preclip = preclip;
				job.regular = regular;
				job.ss = ~ss;
				if(i + 1 == path_info->path.GetCount()) // last subpath
					job.sso = pick(ss); // transfer SpanSource ownership to last subpath
				if(jobcount + emptycount >= BATCH_SIZE)
					FinishPathJob();
			}
			return newclip;
		}
	
		FinishPathJob();
		FinishFillJob();
	}
	
	rasterizer.Reset();

	PathJob j(rasterizer, width, path_info, pathattr, preclip, regular);
	if(j.preclipped)
		return newclip;

	bool doclip = width == CLIP;
	auto fill = [&](CoWork *co) {
		int opacity = int(256 * pathattr.opacity);
		if(!opacity)
			return;
		Rasterizer::Filler *rg;
		SpanFiller          span_filler;
		SolidFiller         solid_filler;
		SubpixelFiller      subpixel_filler;
		ClipFiller          clip_filler;
		NoAAFillerFilter    noaa_filler;
		MaskFillerFilter    mf;

		if(subpixel) {
			int ci = CoWork::GetWorkerIndex();
			subpixel_filler.sbuffer = co && ci >= 0 ? co_subpixel[ci] : subpixel;
		}
		
		if(doclip) {
			clip_filler.Init(render_cx);
			rg = &clip_filler;
			newclip.Alloc(ip->GetHeight());
		}
		else
		if(ss) {
			RGBA           *lspan;
			int ci = CoWork::GetWorkerIndex();
			if(co && ci >= 0)
				lspan = co_span[ci];
			else {
				if(!span)
					span.Alloc((subpixel ? 3 : 1) * ip->GetWidth() + 3);
				lspan = span;
			}
			if(subpixel) {
				subpixel_filler.ss = ~ss;
				subpixel_filler.buffer = lspan;
				subpixel_filler.alpha = opacity;
				rg = &subpixel_filler;
			}
			else {
				span_filler.ss = ~ss;
				span_filler.buffer = lspan;
				span_filler.alpha = opacity;
				rg = &span_filler;
			}
		}
		else {
			if(subpixel) {
				subpixel_filler.ss = nullptr;
				subpixel_filler.color = Mul8(color, opacity);
				subpixel_filler.invert = pathattr.invert;
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
				alt->Fill(width, ~ss, color);
			else {
				PAINTER_TIMING("Fill");
				int ii = 0;
				for(;;) {
					int y = (co ? co->Next() : ii++) + rasterizer.MinY();
					if(y > rasterizer.MaxY())
						break;
					if(!doclip && co_clear && co_clear[y]) {
						UPP::Fill((*ip)[y], co_clear_color, ip->GetWidth());
						co_clear[y] = false;
					}
					solid_filler.t = subpixel_filler.t = span_filler.t = (*ip)[y];
					subpixel_filler.end = subpixel_filler.t + ip->GetWidth();
					span_filler.y = subpixel_filler.y = y;
					Rasterizer::Filler *rf = rg;
					if(clip.GetCount() && clip.Top()) {
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
	bool doco = co && !doclip && !alt && !(subpixel && clip.GetCount());
	for(const auto& p : path_info->path) {
		RenderPathSegments(j.g, p, j.regular ? &pathattr : NULL, j.tolerance);
		if(width != ONPATH) {
			if(rasterizer.IsValid()) {
				PAINTER_TIMING("RenderPath2 Fill");
				int n = rasterizer.MaxY() - rasterizer.MinY();
				if(doco && n > 6) {
					CoWork co;
					co * [&] { fill(&co); };
				}
				else
					fill(NULL);
				rasterizer.Reset();
			}
		}
	}
	if(width == ONPATH) {
		onpath = pick(j.onpathtarget.path);
		pathlen = j.onpathtarget.len;
	}
	return newclip;
}

void BufferPainter::FinishPathJob()
{
	PAINTER_TIMING("FinishPathJob");
	if(jobcount == 0)
		return;
	{
		std::atomic<int> ii(0);
		CoDo([&] {
			for(int i = ii++; i < jobcount; i = ii++) {
				CoJob& b = cojob[i];
				b.rasterizer.Reset();
				PathJob j(b.rasterizer, b.width, b.path_info, b.attr, b.preclip, b.regular);
				if(!j.preclipped) {
					b.evenodd = j.evenodd;
					BufferPainter::RenderPathSegments(j.g, b.path_info->path[b.subpath], j.regular ? &b.attr : NULL, j.tolerance);
				}
			}
		});
	}

	FinishFillJob(); // finish running fill job (if any) so that we can start new one
	
	fillcount = jobcount;
	Swap(cofill, cojob); // Swap to keep allocated rasters (instead of pick)
	
	fill_job & [=] {
		PAINTER_TIMING("CO FILL JOB");
		int miny = ip->GetHeight() - 1;
		int maxy = 0;
		
		for(int i = 0; i < fillcount; i++) {
			CoJob& j = cofill[i];
			miny = min(miny, j.rasterizer.MinY());
			maxy = max(maxy, j.rasterizer.MaxY());
			int alpha = int(256 * j.attr.opacity);
			if(!j.ss)
				j.c = Mul8(j.color, alpha);
			else
				j.alpha = alpha;
		}

		auto fill = [&](int y) {
			if(co_clear && co_clear[y]) {
				UPP::Fill((*ip)[y], co_clear_color, ip->GetWidth());
				co_clear[y] = false;
			}
			
			int ci = CoWork::GetWorkerIndex();
			if(subpixel) {
				SubpixelFiller subpixel_filler;
				subpixel_filler.sbuffer = ci >= 0 ? co_subpixel[ci] : subpixel;
				for(int i = 0; i < fillcount; i++) {
					CoJob& j = cofill[i];
					if(y >= j.rasterizer.MinY() && y <= j.rasterizer.MaxY()) {
						subpixel_filler.color = j.c;
						subpixel_filler.ss = j.ss;
						subpixel_filler.invert = j.attr.invert;
						subpixel_filler.t = (*ip)[y];
						subpixel_filler.end = subpixel_filler.t + ip->GetWidth();

						if(j.ss) {
							RGBA  *lspan;
							if(ci >= 0)
								lspan = co_span[ci];
							else {
								if(!span)
									span.Alloc(3 * ip->GetWidth() + 3);
								lspan = span;
							}
							subpixel_filler.buffer = lspan;
							subpixel_filler.alpha = j.alpha;
							subpixel_filler.y = y;
						}
						
						if(clip.GetCount()) {
							if(clip.Top()) {
								MaskFillerFilter mf;
								const ClippingLine& s = clip.Top()[y];
								if(!s.IsEmpty() && !s.IsFull()) {
									mf.Set(&subpixel_filler, s);
									j.rasterizer.Render(y, mf, j.evenodd);
								}
							}
						}
						else
							j.rasterizer.Render(y, subpixel_filler, j.evenodd);
					}
				}
			}
			else {
				SolidFiller solid_filler;
				SpanFiller  span_filler;
				for(int i = 0; i < fillcount; i++) {
					CoJob& j = cofill[i];
					if(y >= j.rasterizer.MinY() && y <= j.rasterizer.MaxY()) {
						Rasterizer::Filler *rg;
						if(j.ss) {
							if(ci >= 0)
								span_filler.buffer = co_span[ci];
							else {
								if(!span)
									span.Alloc(ip->GetWidth() + 3);
								span_filler.buffer = span;
							}
							span_filler.ss = j.ss;
							span_filler.alpha = j.alpha;
							span_filler.y = y;
							span_filler.t = (*ip)[y];
							rg = &span_filler;
						}
						else {
							solid_filler.c = j.c;
							solid_filler.invert = j.attr.invert;
							solid_filler.t = (*ip)[y];
							rg = &solid_filler;
						}
						if(clip.GetCount()) {
							if(clip.Top()) {
								MaskFillerFilter mf;
								const ClippingLine& s = clip.Top()[y];
								if(!s.IsEmpty() && !s.IsFull()) {
									mf.Set(rg, s);
									j.rasterizer.Render(y, mf, j.evenodd);
								}
							}
						}
						else
							j.rasterizer.Render(y, *rg, j.evenodd);
					}
				}
			}
		};

		if(miny < maxy) {
			int n = maxy - miny;
			if(n > 6) {
				std::atomic<int> ii(0);
				CoDo([&] {
					for(;;) {
						int y = ii++ + miny;
						if(y > maxy)
							break;
						fill(y);
					}
				});
			}
			else
				for(int y = miny; y <= maxy; y++)
					fill(y);
		}
		for(int i = 0; i < fillcount; i++) // we can release SpanSources now
			cofill[i].sso.Clear();
	};
	jobcount = emptycount = 0;
}

void BufferPainter::Finish()
{
	PAINTER_TIMING("Finish");
	FinishPathJob();
	FinishFillJob();
	if(co_clear)
		CoFor(ip->GetHeight(), [&](int y) { // clear remaning lines that were not painted yet
			if(co_clear[y]) {
				UPP::Fill((*ip)[y], co_clear_color, ip->GetWidth());
				co_clear[y] = false;
			}
		});
}

void BufferPainter::FillOp(const RGBA& color)
{
	PAINTER_TIMING("FillOp");
	One<SpanSource> none;
	RenderPath(FILL, none, color);
}

void BufferPainter::StrokeOp(double width, const RGBA& color)
{
	PAINTER_TIMING("StrokeOp");
	One<SpanSource> none;
	RenderPath(width, none, color);
}

void BufferPainter::ClipOp()
{
	FinishPathJob();
	One<SpanSource> none;
	Buffer<ClippingLine> newclip = RenderPath(CLIP, none, RGBAZero());
	if(attr.hasclip)
		clip.Top() = pick(newclip);
	else {
		clip.Add() = pick(newclip);
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
}

}
