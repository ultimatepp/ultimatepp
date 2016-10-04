#include "agg_upp_bind.h"

NAMESPACE_UPP
//typedef agg::renderer_base<pixfmt> renderer_base;
//typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

void AggDrawer::DrawLine(int x1, int y1, int x2, int y2, int width)
{
//	m_renb.clip_box( m_clipboxdev.x, m_clipboxdev.y, m_clipboxdev.x + m_clipboxdev.width, m_clipboxdev.y + m_clipboxdev.height);

	agg::path_storage path;
	path.move_to(x1, y1);
	path.line_to(x2, y2);

	agg::conv_stroke<agg::path_storage> stroke(path);
//	stroke.line_join();
//	stroke.line_cap();
	stroke.line_cap(agg::round_cap);
	//stroke.miter_limit();
	stroke.width(width);
	m_ras.add_path(stroke);

//	RenderScanlines(); // at the end or after all same color?
}

//void AggDrawer::DrawLine(Point p1, Point p2, color_type  ct)
//{
//	m_ren.col	
//agg::ellipse e1;
//}

void AggDrawer::DrawEllipse(int x1, int y1, int cx, int cy, int width)
{
	agg::ellipse el;
//	el.init(double x¸ double y¸ double rx¸ double ry¸ unsigned num_steps=0¸ bool cw=false);
	el.init(x1, y1, cx, cy);

	agg::conv_stroke<agg::ellipse> stroke(el);

	stroke.width(width);
	m_ras.add_path(stroke);
	
//	agg::conv_transform<agg::ellipse> tr(ellipse, mtx);
//	m_ras.add_path(tr);
//	ren.color( color );
	
//	typedef agg::wrap_mode_reflect_auto_pow2 wrap_x_type;
//	typedef agg::wrap_mode_reflect_auto_pow2 wrap_y_type;
//	typedef agg::span_pattern_rgb<agg::rgba8, agg::order_rgb, wrap_x_type, wrap_y_type> span_gen_type;
//	typedef agg::renderer_scanline_aa<ren_base, span_gen_type> renderer_type;

//	agg::span_allocator<agg::rgba8> sa;
//	span_gen_type sg(sa, m_pattern_rbuf, offset_x, offset_y);
//	sg.alpha(span_gen_type::value_type(m_FillOpacity)); 
//	renderer_type rp(renb, sg);
//	agg::render_scanlines(m_ras, m_sl, rp);
//	agg::render_scanlines_aa_solid(m_ras, m_sl, m_ren, agg::rgba(1,1,0.5) );
}

void AggDrawer::RenderScanlines()
{
//	m_ras.filling_rule(filling_rule_e filling_rule);
	agg::render_scanlines(m_ras, m_sl, m_ren);
//	agg::render_scanlines_aa_solid(m_ras, m_sl, m_ren, agg::rgba(1,1,0.5) );
}

      
void AggDrawer::AttachBuffers()  //or better name - ConnectPipelineBuffers?
{
	m_rbuf.attach((agg::int8u *)uibuf[0], sz.cx, sz.cy, sz.cx*4);
	m_pixf.attach(m_rbuf);
	m_renb.attach(m_pixf);  //ren_base
	m_ren.attach(m_renb);
}


END_UPP_NAMESPACE
