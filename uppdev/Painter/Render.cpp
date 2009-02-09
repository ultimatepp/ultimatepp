#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::ClearOp(const RGBA& color)
{
	UPP::Fill(~ib, color, ib.GetLength());
}

void BufferPainter::RenderPath(LinearPathFilter *first_filter,
                               LinearPathFilter *last_filter, const RGBA& color, bool evenodd)
{
	Transformer trans(pathattr.mtx);
	trans.target = &rasterizer;
	LinearPathConsumer *g;
	if(first_filter) {
		last_filter->target = &trans;
		g = first_filter;
	}
	else
		g = &trans;
	byte *data = path.data;
	Pointf pos = Pointf(0, 0);
	{
		PAINTER_TIMING("Pipeline");
		for(int i = 0; i < path.type.GetCount(); i++) {
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
			case ARC:
			default:
				NEVER();
				return;
			}
		}
	}
	g->End();
	Render(ib, rasterizer, color, evenodd);
	rasterizer.Reset();
	current = Null;
}

void BufferPainter::FillOp(const RGBA& color)
{
	Close();
	RenderPath(NULL, NULL, color, pathattr.evenodd);
}

void BufferPainter::StrokeOp(double width, const RGBA& color)
{
	Stroker stroker(width, pathattr.miter_limit, pathattr.tolerance, pathattr.cap, pathattr.join);
	if(pathattr.dash.GetCount()) {
		Dasher dasher(pathattr.dash, pathattr.dash_start);
		dasher.target = &stroker;
		RenderPath(&dasher, &stroker, color, false);
	}
	else
		RenderPath(&stroker, &stroker, color, false);
}

END_UPP_NAMESPACE
