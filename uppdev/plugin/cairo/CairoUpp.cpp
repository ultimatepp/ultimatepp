#include "Cairo.h"

#include "CairoPattern.h"

NAMESPACE_UPP

void Cairo::SetSurface(cairo_surface_t *surface, int width, int height)
{
	ASSERT(surface != NULL);
	
	DestroySurface();
	
	_surface = surface;
	_context = cairo_create(_surface);
	_width = width;
	_height = height;
}

void Cairo::SetSurface(ImageBuffer &img)
{
	ASSERT(img.IsEmpty() == false);
	
	cairo_surface_t *surface = cairo_image_surface_create_for_data((unsigned char *)&(img[0]->b),
	                                                               CAIRO_FORMAT_ARGB32,
	                                                               img.GetSize().cx,
	                                                               img.GetSize().cy,
	                                                               img.GetSize().cx * 4);
	
	SetSurface(surface, img.GetSize().cx, img.GetSize().cy);
}

void Cairo::CreateSvgSurface(String const &filename, double widthInPoints, double heightInPoints)
{
	SetSurface(cairo_svg_surface_create(filename, widthInPoints, heightInPoints), static_cast<int>(widthInPoints), static_cast<int>(heightInPoints));
}

void Cairo::CreatePdfSurface(String const &filename, double widthInPoints, double heightInPoints)
{
	SetSurface(cairo_pdf_surface_create(filename, widthInPoints, heightInPoints), static_cast<int>(widthInPoints), static_cast<int>(heightInPoints));
}

#if 1
#define CAIRO_METHOD_ENTER() ASSERT(_context != NULL)
#define CAIRO_METHOD_LEAVE() { \
                                 cairo_status_t status = cairo_status(_context); \
                                 ASSERT_(status == CAIRO_STATUS_SUCCESS, cairo_status_to_string(status)); \
                             }
#else
#define CAIRO_METHOD_ENTER() 
#define CAIRO_METHOD_LEAVE()
#endif

void Cairo::Save()
{
	CAIRO_METHOD_ENTER();
	cairo_save(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Restore()
{
	CAIRO_METHOD_ENTER();
	cairo_restore(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::PushGroup()
{
	CAIRO_METHOD_ENTER();
	cairo_push_group(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::PopGroup()
{
	CAIRO_METHOD_ENTER();
	cairo_pop_group(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::PopGroupToSource()
{
	CAIRO_METHOD_ENTER();
	cairo_pop_group_to_source(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::SetSourceRGB(double r, double g, double b)
{
	CAIRO_METHOD_ENTER();
	cairo_set_source_rgb(_context, r, g, b);
	CAIRO_METHOD_LEAVE();
}

void Cairo::SetSourceRGB(Color const &color)
{
	CAIRO_METHOD_ENTER();
	cairo_set_source_rgb(_context, color.GetR() / 255.0, color.GetG() / 255.0, color.GetB() / 255.0);
	CAIRO_METHOD_LEAVE();
}

void Cairo::SetSourceRGBA(double r, double g, double b, double a)
{
	CAIRO_METHOD_ENTER();
	cairo_set_source_rgba(_context, r, g, b, a);
	CAIRO_METHOD_LEAVE();
}

void Cairo::SetSource(CairoPattern &pattern)
{
	CAIRO_METHOD_ENTER();
	cairo_set_source(_context, pattern.GetPattern());
	CAIRO_METHOD_LEAVE();
}

void Cairo::SetAntialias(cairo_antialias_t antialias)
{
	CAIRO_METHOD_ENTER();
	cairo_set_antialias(_context, antialias);
	CAIRO_METHOD_LEAVE();
}

cairo_antialias_t Cairo::GetAntialias()
{
	CAIRO_METHOD_ENTER();
	cairo_antialias_t result = cairo_get_antialias(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::SetDash(double const * dashes, int numDashes, double offset)
{
	CAIRO_METHOD_ENTER();
	cairo_set_dash(_context, dashes, numDashes, offset);
	CAIRO_METHOD_LEAVE();
}

void Cairo::SetFillRule(cairo_fill_rule_t fillRule)
{
	CAIRO_METHOD_ENTER();
	cairo_set_fill_rule(_context, fillRule);
	CAIRO_METHOD_LEAVE();
}

cairo_fill_rule_t Cairo::GetFillRule()
{
	CAIRO_METHOD_ENTER();
	cairo_fill_rule_t result = cairo_get_fill_rule(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::SetLineCap(cairo_line_cap_t lineCap)
{
	CAIRO_METHOD_ENTER();
	cairo_set_line_cap(_context, lineCap);
	CAIRO_METHOD_LEAVE();
}

cairo_line_cap_t Cairo::GetLineCap()
{
	CAIRO_METHOD_ENTER();
	cairo_line_cap_t result = cairo_get_line_cap(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::SetLineJoin(cairo_line_join_t lineJoin)
{
	CAIRO_METHOD_ENTER();
	cairo_set_line_join(_context, lineJoin);
	CAIRO_METHOD_LEAVE();
}

cairo_line_join_t Cairo::GetLineJoin()
{
	CAIRO_METHOD_ENTER();
	cairo_line_join_t result = cairo_get_line_join(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::SetLineWidth(double width)
{
	CAIRO_METHOD_ENTER();
	cairo_set_line_width(_context, width);
	CAIRO_METHOD_LEAVE();
}

double Cairo::GetLineWidth()
{
	CAIRO_METHOD_ENTER();
	double result = cairo_get_line_width(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::SetMiterLimit(double limit)
{
	CAIRO_METHOD_ENTER();
	cairo_set_miter_limit(_context, limit);
	CAIRO_METHOD_LEAVE();
}

double Cairo::GetMiterLimit()
{
	CAIRO_METHOD_ENTER();
	double result = cairo_get_miter_limit(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::SetOperator(cairo_operator_t op)
{
	CAIRO_METHOD_ENTER();
	cairo_set_operator(_context, op);
	CAIRO_METHOD_LEAVE();
}

cairo_operator_t Cairo::GetOperator()
{
	CAIRO_METHOD_ENTER();
	cairo_operator_t result = cairo_get_operator(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::SetTolerance(double tolerance)
{
	CAIRO_METHOD_ENTER();
	cairo_set_tolerance(_context, tolerance);
	CAIRO_METHOD_LEAVE();
}

double Cairo::GetTolerance()
{
	CAIRO_METHOD_ENTER();
	double result = cairo_get_tolerance(_context);
	CAIRO_METHOD_LEAVE();
	return result;
}

void Cairo::Clip(bool preserve)
{
	CAIRO_METHOD_ENTER();
	if (!preserve)
		cairo_clip(_context);
	else
		cairo_clip_preserve(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Fill(bool preserve)
{
	CAIRO_METHOD_ENTER();
	if (!preserve)
		cairo_fill(_context);
	else
		cairo_fill_preserve(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Paint(double alpha)
{
	CAIRO_METHOD_ENTER();
	if (alpha >= 1.0)
		cairo_paint(_context);
	else
		cairo_paint_with_alpha(_context, alpha);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Stroke(bool preserve)
{
	CAIRO_METHOD_ENTER();
	if (!preserve)
		cairo_stroke(_context);
	else
		cairo_stroke_preserve(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Translate(double tx, double ty)
{
	CAIRO_METHOD_ENTER();
	cairo_translate(_context, tx, ty);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Scale(double sx, double sy)
{
	CAIRO_METHOD_ENTER();
	cairo_scale(_context, sx, sy);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Rotate(double angle)
{
	CAIRO_METHOD_ENTER();
	cairo_rotate(_context, angle);
	CAIRO_METHOD_LEAVE();
}

void Cairo::IdentityMatrix()
{
	CAIRO_METHOD_ENTER();
	cairo_identity_matrix(_context);
	CAIRO_METHOD_LEAVE();
}

Pointf Cairo::GetCurrentPoint()
{
	Pointf p;
	CAIRO_METHOD_ENTER();
	cairo_get_current_point(_context, &p.x, &p.y);
	CAIRO_METHOD_LEAVE();
	return p;
}

void Cairo::NewPath()
{
	CAIRO_METHOD_ENTER();
	cairo_new_path(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::ClosePath()
{
	CAIRO_METHOD_ENTER();
	cairo_close_path(_context);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Arc(double xc, double yc, double radius, double angle1, double angle2)
{
	CAIRO_METHOD_ENTER();
	cairo_arc(_context, xc, yc, radius, angle1, angle2);
	CAIRO_METHOD_LEAVE();
}

void Cairo::CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
{
	CAIRO_METHOD_ENTER();
	cairo_curve_to(_context, x1, y1, x2, y2, x3, y3);
	CAIRO_METHOD_LEAVE();
}

void Cairo::LineTo(double x, double y)
{
	CAIRO_METHOD_ENTER();
	cairo_line_to(_context, x, y);
	CAIRO_METHOD_LEAVE();
}

void Cairo::MoveTo(double x, double y)
{
	CAIRO_METHOD_ENTER();
	cairo_move_to(_context, x, y);
	CAIRO_METHOD_LEAVE();
}

void Cairo::Rectangle(double x, double y, double width, double height)
{
	CAIRO_METHOD_ENTER();
	cairo_rectangle(_context, x, y, width, height);
	CAIRO_METHOD_LEAVE();
}

void Cairo::RelCurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
{
	CAIRO_METHOD_ENTER();
	cairo_rel_curve_to(_context, x1, y1, x2, y2, x3, y3);
	CAIRO_METHOD_LEAVE();
}

void Cairo::RelLineTo(double x, double y)
{
	CAIRO_METHOD_ENTER();
	cairo_rel_line_to(_context, x, y);
	CAIRO_METHOD_LEAVE();
}

void Cairo::RelMoveTo(double x, double y)
{
	CAIRO_METHOD_ENTER();
	cairo_rel_move_to(_context, x, y);
	CAIRO_METHOD_LEAVE();
}

END_UPP_NAMESPACE
