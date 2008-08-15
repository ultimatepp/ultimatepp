#include "aggdraw.h"

#include <plugin/agg24/agg24.h>
/*
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_scanline.h"

#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_gamma_lut.h"
#include "agg_conv_dash.h"
#include "agg_conv_stroke.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_gouraud_rgba.h"
#include "agg_span_allocator.h"
#include "agg_conv_stroke.h"
#include "agg_path_storage.h"*/

#define LLOG(a) LOG(a)
#if defined(CPU_64)
#endif
namespace Upp
{
	typedef agg::pixfmt_bgra32                    				pixfmt_type;   
	typedef agg::renderer_base<pixfmt_type>                     renderer_base_type;
	typedef agg::renderer_scanline_aa_solid<renderer_base_type> renderer_aa; //renderer_solid
	typedef agg::scanline_p8	                                  scanline_type;
	typedef agg::rasterizer_scanline_aa<>                       rasterizer_type;
	

	struct AggDraw::AggDrawPrivate
	{
		AggDrawPrivate(int cx, int cy)
			: buff(cx,cy)
			, rbuf((agg::int8u *)buff[0], cx, cy, cx * 4)
			, pixf(rbuf)
			, ren_base(pixf)
			, ren_aa(ren_base)
		{
        	ren_base.clear(agg::rgba(1, 1, 1));
		}
		
		ImageBuffer buff;
		agg::rendering_buffer rbuf;
        pixfmt_type pixf;
		renderer_base_type ren_base;
		renderer_aa ren_aa;
		rasterizer_type ras_aa;
		scanline_type sl;
		scanline_type m_sl_p8;
	};
	
	AggDraw::AggDraw(int cx, int cy) 
		: data ( new AggDrawPrivate(cx,cy))
	{}
	
	AggDraw::AggDraw(Size sz)
		: data ( new AggDrawPrivate(sz.cx,sz.cy))
	{}
	
	
	AggDraw::~AggDraw()
	{
		delete data;
	}


	void AggDraw::BeginOp()
	{
		LLOG("AGG:BeginOP");
	}
	
	void AggDraw::EndOp()
	{
		LLOG("AGG:EndOP");
	}
	
	inline agg::rgba8 AGGColor(Color c) { return agg::rgba8(c.GetR(),c.GetB(),c.GetG());}
	
	void AggDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
	{
		data->ren_aa.color(agg::rgba((double)color.GetR()/ 256, (double)color.GetG()/256, (double)color.GetB()/256));
		data->ras_aa.move_to(x1,y1);
		data->ras_aa.line_to(x2,y2);
		
		agg::path_storage path;
		path.move_to(x1, y1);
		path.line_to(x2, y2);

		agg::conv_stroke<agg::path_storage> stroke(path);
		stroke.line_cap(agg::round_cap);
		stroke.width(width);
		data->ras_aa.add_path(stroke);
	}
	
	
	AggDraw::operator Image()
	{
		agg::render_scanlines(data->ras_aa, data->m_sl_p8, data->ren_aa);
		return data->buff;
	}


}

