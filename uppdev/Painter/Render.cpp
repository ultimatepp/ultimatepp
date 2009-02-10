#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::ClearOp(const RGBA& color)
{
	UPP::Fill(~ib, color, ib.GetLength());
}

void BufferPainter::RenderPath(double width, SpanSource *ss, const RGBA& color)
{
	if(width == 0)
		return;
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
	RGBA c;
	if(!ss)
		c = Mul8(color, opacity);
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
	if(ss) {
		if(!span)
			span.Alloc(ib.GetWidth() + 1);
		Render(ib, rasterizer, ss, span, opacity, evenodd);
	}
	else
		Render(ib, rasterizer, c, evenodd);
	rasterizer.Reset();
	current = Null;
}

void BufferPainter::FillOp(const RGBA& color)
{
	RenderPath(-1, NULL, color);
}

void BufferPainter::StrokeOp(double width, const RGBA& color)
{
	RenderPath(width, NULL, color);
}

END_UPP_NAMESPACE
