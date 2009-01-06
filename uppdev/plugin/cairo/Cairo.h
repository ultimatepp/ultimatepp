#ifndef _plugin_cairo_Cairo_h_
#define _plugin_cairo_Cairo_h_

#include <Core/Core.h>
#include <Draw/Draw.h>

#include "lib/cairo.h"
#include "lib/cairo-svg.h"
#include "lib/cairo-pdf.h"
#include "CairoPattern.h"

NAMESPACE_UPP

class Cairo : public NoCopy
{
private:
	cairo_surface_t *_surface;
	cairo_t *_context;
	int _width, _height;
	
	void DestroySurface()
	{
		if (_context != NULL)
		{
			cairo_status_t status = cairo_status(_context);
			ASSERT_(status == CAIRO_STATUS_SUCCESS, cairo_status_to_string(status));
			cairo_destroy(_context);
		}
		
		if (_surface != NULL)
			cairo_surface_destroy(_surface);
	}
	
public:
	Cairo()
		: _surface(NULL),
		  _context(NULL)
	{
	}
	
	~Cairo()
	{
		DestroySurface();
	}
	
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }
	
	void SetSurface(cairo_surface_t *surface, int width, int height);
	void SetSurface(ImageBuffer &img);
	void CreateSvgSurface(String const &filename, double widthInPoints = 595, double heightInPoints = 841); // DIN-A4 default size
	void CreatePdfSurface(String const &filename, double widthInPoints = 595, double heightInPoints = 841); // DIN-A4 default size
	
	cairo_t *GetContext()
	{
		return _context;
	}
	
	cairo_surface_t *GetSurface()
	{
		return _surface;
	}
	
	bool HasContext()
	{
		return _context != NULL;
	}
	
	void Clear(Color const &color)
	{
		SetSourceRGB(color);
		Paint();
	}
	
	/*******************************************************************************************************/
	/************** TODO: remove the following 2 methods!!! Replace them with a dedicated font system ******/
	/*******************************************************************************************************/
	void DrawText(double x, double y, char const * const text, double size, Color ink)
	{
		cairo_t *context = GetContext();
		ASSERT(context != NULL);
		
		cairo_set_font_size(context, size);
		
		Save();
		
		Translate(x, y + size);
		
		SetSourceRGB(ink);
		cairo_show_text(context, text);
		
		NewPath();
		
		Restore();
	}
	
	void DrawTextCenter(double x, double y, double width, char const * const text, double size, Color ink)
	{
		cairo_t *context = GetContext();
		ASSERT(context != NULL);
		
		cairo_set_font_size(context, size);
		
		// measure string
		cairo_text_extents_t extents;
		cairo_text_extents(context, text, &extents);
		
		double startX = x + (width / 2.0) - (extents.width / 2.0);
		
		Save();
		
		Translate(startX, y + size);
		
		SetSourceRGB(ink);
		cairo_show_text(context, text);
		
		NewPath();
		
		Restore();
	}
	
	/*
	 * Cairo cairo_t methods
	 * http://cairographics.org/manual/cairo-cairo-t.html
	 */
	void Save();
	void Restore();
	void PushGroup();
	//cairo_push_group_with_content
	void PopGroup();
	void PopGroupToSource();
	//cairo_get_group_target
	void SetSourceRGB(double r, double g, double b);
	void SetSourceRGB(Color const &color);
	void SetSourceRGBA(double r, double g, double b, double a);
	void SetSource(CairoPattern &pattern);
	//cairo_set_source_surface
	//cairo_get_source
	void SetAntialias(cairo_antialias_t antialias);
	cairo_antialias_t GetAntialias();
	void SetDash(double const * dashes, int numDashes, double offset);
	//cairo_get_dash_count
	//cairo_get_dash
	void SetFillRule(cairo_fill_rule_t fillRule);
	cairo_fill_rule_t GetFillRule();
	void SetLineCap(cairo_line_cap_t lineCap);
	cairo_line_cap_t GetLineCap();
	void SetLineJoin(cairo_line_join_t lineJoin);
	cairo_line_join_t GetLineJoin();
	void SetLineWidth(double width);
	double GetLineWidth();
	void SetMiterLimit(double limit);
	double GetMiterLimit();
	void SetOperator(cairo_operator_t op);
	cairo_operator_t GetOperator();
	void SetTolerance(double tolerance);
	double GetTolerance();
	void Clip(bool preserve = false);
	//cairo_clip_extents
	//cairo_reset_clip
	//cairo_rectangle_list_destroy
	//cairo_copy_clip_rectangle_list
	void Fill(bool preserve = false);
	//cairo_fill_extents
	//cairo_in_fill
	//cairo_mask
	//cairo_mask_surface
	void Paint(double alpha = 1.0);
	void Stroke(bool preserve = false);
	//cairo_stroke_extents
	//cairo_in_stroke
	//cairo_copy_page
	//cairo_show_page
	//cairo_get_reference_count         probably not needed
	//cairo_set_user_data          probably not needed
	//cairo_get_user_data         probably not needed
	
	/*
	 * Cairo transformation methods
	 * http://cairographics.org/manual/cairo-Transformations.html
	 */
	void Translate(double tx, double ty);
	void Scale(double sx, double sy);
	void Rotate(double angle);
	//cairo_transform
	//cairo_set_matrix
	//cairo_get_matrix
	void IdentityMatrix();
	//cairo_user_to_device
	//cairo_user_to_device_distance
	//cairo_device_to_user
	//cairo_device_to_user_distance
	
	/*
	 * Cairo path methods
	 * http://cairographics.org/manual/cairo-Paths.html
	 */
	//cairo_copy_path
	//cairo_copy_path_flat
	//cairo_path_destroy
	//cairo_append_path
	//cairo_has_current_point
	Pointf GetCurrentPoint();
	void NewPath();
	//cairo_new_sub_path
	void ClosePath();
	void Arc(double xc, double yc, double radius, double angle1, double angle2);
	//cairo_arc_negative 
	void CurveTo(double x1, double y1, double x2, double y2, double x3, double y3);
	void LineTo(double x, double y);
	void MoveTo(double x, double y);
	void Rectangle(double x, double y, double width, double height);
	//cairo_glyph_path
	//cairo_text_path
	void RelCurveTo(double x1, double y1, double x2, double y2, double x3, double y3);
	void RelLineTo(double x, double y);
	void RelMoveTo(double x, double y);
	//cairo_path_extents
};

END_UPP_NAMESPACE

#endif
