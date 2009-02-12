#include "Painter.h"

NAMESPACE_UPP

BufferPainter::StrokeInfo BufferPainter::BeginStroke(double width)
{
	StrokeInfo f;
	double scl = pathattr.mtx.scale();
	curved.approximation_scale(scl);
	curved.angle_tolerance(0.0);
	if(width * scl > 1.0)
		curved.angle_tolerance(0.2);
	if(pathattr.dash.GetCount()) {
		agg::conv_dash<Curved> dashed(curved);
		dashed.Set(&pathattr.dash, pathattr.dash_start);
		agg::conv_stroke<agg::conv_dash<Curved> > curved_stroked(dashed);
		curved_stroked.width(width);
		curved_stroked.line_join((agg::line_join_e)pathattr.join);
		curved_stroked.line_cap((agg::line_cap_e)pathattr.cap);
		curved_stroked.miter_limit(pathattr.miter_limit);
		curved_stroked.approximation_scale(scl);
		f.path.concat_path(curved_stroked);
	}
	else {
		agg::conv_stroke<Curved> curved_stroked(curved);
		curved_stroked.width(width);
		curved_stroked.line_join((agg::line_join_e)pathattr.join);
		curved_stroked.line_cap((agg::line_cap_e)pathattr.cap);
		curved_stroked.miter_limit(pathattr.miter_limit);
		curved_stroked.approximation_scale(scl);
		f.path.concat_path(curved_stroked);
	}
	Swap(f.path, path);
	f.evenodd = pathattr.evenodd;
	pathattr.evenodd = false;
	inpath = false;	
	return f;
}

void BufferPainter::EndStroke(StrokeInfo& f)
{
	Swap(f.path, path);
	pathattr.evenodd = f.evenodd;
}

void BufferPainter::StrokeOp(double width, const RGBA& color)
{
	StrokeInfo f = BeginStroke(width);
	Fill(color);
	EndStroke(f);
}

void BufferPainter::StrokeOp(double width, const Image& image, const Matrix2D& transsrc, dword flags)
{
	StrokeInfo f = BeginStroke(width);
	Fill(image, transsrc, flags);
	EndStroke(f);
}

void BufferPainter::StrokeOp(double width, double x1, double y1, const RGBA& color1,
                             double x2, double y2, const RGBA& color2, int style)
{
	StrokeInfo f = BeginStroke(width);
	Fill(x1, y1, color1, x2, y2, color2, style);
	EndStroke(f);
}

void BufferPainter::StrokeOp(double width, double fx, double fy, const RGBA& color1,
                             double x, double y, double r, const RGBA& color2, int style)
{
	StrokeInfo f = BeginStroke(width);
	Fill(fx, fy, color1, x, y, r, color2, style);
	EndStroke(f);
}

END_UPP_NAMESPACE
