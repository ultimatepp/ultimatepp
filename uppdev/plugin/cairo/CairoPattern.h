#ifndef _plugin_cairo_CairoPattern_h_
#define _plugin_cairo_CairoPattern_h_

#include <Core/Core.h>

#include "lib/cairo.h"

NAMESPACE_UPP

#if 1
#define CAIRO_METHOD_ENTER() ASSERT(_pattern != NULL)
#define CAIRO_METHOD_LEAVE() { \
                                 cairo_status_t status = cairo_pattern_status(_pattern); \
                                 ASSERT_(status == CAIRO_STATUS_SUCCESS, cairo_status_to_string(status)); \
                             }
#else
#define CAIRO_METHOD_ENTER() 
#define CAIRO_METHOD_LEAVE()
#endif

class CairoPattern : public NoCopy /*abstract*/
{
protected:
	cairo_pattern_t *_pattern;
	
	CairoPattern()
		: _pattern(NULL)
	{}
	
	virtual ~CairoPattern()
	{
		if (_pattern != NULL)
			cairo_pattern_destroy(_pattern);
	}
	
public:
	cairo_pattern_t *GetPattern()
	{
		return _pattern;
	}
	
	void SetExtend(cairo_extend_t extend)
	{
		CAIRO_METHOD_ENTER();
		cairo_pattern_set_extend(_pattern, extend);
		CAIRO_METHOD_LEAVE();
	}
	
	//cairo_pattern_get_extend ()
	
	void SetFilter(cairo_filter_t filter)
	{
		CAIRO_METHOD_ENTER();
		cairo_pattern_set_filter(_pattern, filter);
		CAIRO_METHOD_LEAVE();
	}
	
	//cairo_pattern_get_filter ()
	//cairo_pattern_set_matrix ()
	//cairo_pattern_get_matrix ()
	//cairo_pattern_get_type ()
};

class CairoGradient : public CairoPattern /*abstract*/
{
protected:
	CairoGradient() {}
	
public:
	void AddColorStopRGB(double offset, double r, double g, double b)
	{
		CAIRO_METHOD_ENTER();
		cairo_pattern_add_color_stop_rgb(_pattern, offset, r, g, b);
		CAIRO_METHOD_LEAVE();
	}
	
	void AddColorStopRGB(double offset, Color const &color)
	{
		AddColorStopRGB(offset, color.GetR() / 255.0, color.GetG() / 255.0, color.GetB() / 255.0);
	}
	
	void AddColorStopRGBA(double offset, double r, double g, double b, double a)
	{
		CAIRO_METHOD_ENTER();
		cairo_pattern_add_color_stop_rgba(_pattern, offset, r, g, b, a);
		CAIRO_METHOD_LEAVE();
	}
	
	//cairo_pattern_get_color_stop_count ()
	//cairo_pattern_get_color_stop_rgba ()
};

class CairoSolidPattern : public CairoPattern
{
public:
	CairoSolidPattern(double r, double g, double b) { SetRGB(r, g, b); }
	CairoSolidPattern(Color const &color) { SetRGB(color); }
	CairoSolidPattern(double r, double g, double b, double a) { SetRGBA(r, g, b, a); }
	
	void SetRGB(double r, double g, double b)
	{
		if (_pattern != NULL)
			cairo_pattern_destroy(_pattern);
		
		_pattern = cairo_pattern_create_rgb(r, g, b);
		CAIRO_METHOD_LEAVE();
	}
	
	void SetRGB(Color const &color)
	{
		SetRGB(color.GetR() / 255.0, color.GetG() / 255.0, color.GetB() / 255.0);
	}
	
	void SetRGBA(double r, double g, double b, double a)
	{
		if (_pattern != NULL)
			cairo_pattern_destroy(_pattern);
		
		_pattern = cairo_pattern_create_rgba(r, g, b, a);
		CAIRO_METHOD_LEAVE();
	}
	
	//cairo_pattern_get_rgba ()
};

class CairoLinearGradient : public CairoGradient
{
public:
	CairoLinearGradient(double x0, double y0, double x1, double y1) { Set(x0, y0, x1, y1); }
	
	void Set(double x0, double y0, double x1, double y1)
	{
		if (_pattern != NULL)
			cairo_pattern_destroy(_pattern);
		
		_pattern = cairo_pattern_create_linear(x0, y0, x1, y1);
		CAIRO_METHOD_LEAVE();
	}
	
	//cairo_pattern_get_linear_points ()
};

class CairoRadialGradient : public CairoGradient
{
public:
	CairoRadialGradient(double cx0, double cy0, double radius0, double cx1, double cy1, double radius1) 
	{
		Set(cx0, cy0, radius0, cx1, cy1, radius1); 
	}
	
	void Set(double cx0, double cy0, double radius0, double cx1, double cy1, double radius1)
	{
		if (_pattern != NULL)
			cairo_pattern_destroy(_pattern);
		
		_pattern = cairo_pattern_create_radial(cx0, cy0, radius0, cx1, cy1, radius1);
		CAIRO_METHOD_LEAVE();
	}
	
	//cairo_pattern_get_radial_circles ()
};

class CairoSurfacePattern : public CairoPattern
{
public:
	CairoSurfacePattern(cairo_surface_t *surface) { Set(surface); }
	
	void Set(cairo_surface_t *surface)
	{
		if (_pattern != NULL)
			cairo_pattern_destroy(_pattern);
		
		_pattern = cairo_pattern_create_for_surface(surface);
		CAIRO_METHOD_LEAVE();
	}
	
	//cairo_pattern_get_surface ()
};

END_UPP_NAMESPACE

#undef CAIRO_METHOD_ENTER 
#undef CAIRO_METHOD_LEAVE

#endif
