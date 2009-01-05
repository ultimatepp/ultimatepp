#include "SDraw.h"

NAMESPACE_UPP

SDraw::path_storage SDraw::MakeStroke(double width)
{
	double scl = pathattr.mtx.scale();
	curved.approximation_scale(scl);
	curved.angle_tolerance(0.0);
	if(width * scl > 1.0)
		curved.angle_tolerance(0.2);
	rasterizer.reset();
	rasterizer.filling_rule(agg::fill_non_zero);
	path_storage b;
	if(pathattr.dash.GetCount()) {
		agg::conv_dash<Curved> dashed(curved);
		dashed.Set(&pathattr.dash, pathattr.dash_start);
		agg::conv_stroke<agg::conv_dash<Curved> > curved_stroked(dashed);
		curved_stroked.width(width);
		curved_stroked.line_join((agg::line_join_e)pathattr.join);
		curved_stroked.line_cap((agg::line_cap_e)pathattr.cap);
		curved_stroked.miter_limit(pathattr.miter_limit);
		curved_stroked.approximation_scale(scl);
		b.concat_path(curved_stroked);
	}
	else {
		agg::conv_stroke<Curved> curved_stroked(curved);
		curved_stroked.width(width);
		curved_stroked.line_join((agg::line_join_e)pathattr.join);
		curved_stroked.line_cap((agg::line_cap_e)pathattr.cap);
		curved_stroked.miter_limit(pathattr.miter_limit);
		curved_stroked.approximation_scale(scl);
		b.concat_path(curved_stroked);
	}
	return b;
}

SDraw& SDraw::Stroke(double width, const RGBA& color)
{
	path_storage b = MakeStroke(width);
	Swap(b, path);
	inpath = false;
	Fill(color);
	Swap(b, path);
	return *this;
}

SDraw& SDraw::Stroke(double width, const Image& image, const Matrix2D& transsrc, dword flags)
{
	path_storage b = MakeStroke(width);
	Swap(b, path);
	inpath = false;
	Fill(image, transsrc, flags);
	Swap(b, path);
	return *this;
}

END_UPP_NAMESPACE
