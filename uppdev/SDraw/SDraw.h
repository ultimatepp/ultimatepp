#ifndef _SDraw_SDraw_h
#define _SDraw_SDraw_h

#include <Draw/Draw.h>

#include <agg24/agg_rendering_buffer.h>
#include <agg24/agg_basics.h>
#include <agg24/agg_renderer_base.h>
#include <agg24/agg_pixfmt_rgba.h>
#include <agg24/agg_renderer_scanline.h>
#include <agg24/agg_rasterizer_scanline_aa.h>
#include <agg24/agg_scanline_u.h>
#include <agg24/agg_bounding_rect.h>
#include <agg24/agg_path_storage.h>
#include <agg24/agg_conv_stroke.h>
#include <agg24/agg_ellipse.h>

NAMESPACE_UPP

struct SDraw {
	Size                                           sz;

	Rect                                           m_r;
	ImageBuffer                                    buffer;

	agg::rendering_buffer                          m_rbuf;
	agg::rasterizer_scanline_aa<>                  m_ras;
	agg::scanline_u8                               m_sl;

	typedef agg::renderer_base<agg::pixfmt_bgra32> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

	renderer_base                                  m_renb;
	renderer_solid                                 m_ren;
	agg::pixfmt_bgra32                             m_pixf;
	
	void AttachBuffers();

public:
	void DrawLine(int x1, int y1, int x2, int y2, int width = 0);
	void DrawEllipse(int x1, int y1, int cx, int cy, int width);
	
	void SetBackground(const agg::rgba8&  ct)	{ m_renb.clear(ct); }
	void SetBrushColor(const agg::rgba8&  ct)   { m_ren.color(ct); }
	
	Image Render();
	
	SDraw(int cx, int cy);
};

END_UPP_NAMESPACE

#endif

