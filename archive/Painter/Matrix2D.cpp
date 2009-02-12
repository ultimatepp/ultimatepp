#include "Painter.h"

NAMESPACE_UPP

Matrix2D Translate2D(double x, double y)
{
	Matrix2D m;
	m.tx = x;
	m.ty = y;
	return m;
}

Matrix2D Rotate2D(double angle)
{
	Matrix2D m;
	*(agg::trans_affine *)&m = agg::trans_affine_rotation(angle);
	return m;
}

Matrix2D Scale2D(double scalex, double scaley)
{
	Matrix2D m;
	*(agg::trans_affine *)&m = agg::trans_affine_scaling(scalex, scaley);
	return m;
}

Matrix2D Scale2D(double scale)
{
	return Scale2D(scale, scale);
}

END_UPP_NAMESPACE
