#include "SDraw.h"

NAMESPACE_UPP

void SDraw::DrawLine(int x1, int y1, int x2, int y2, int width)
{
//	m_renb.clip_box( m_clipboxdev.x, m_clipboxdev.y, m_clipboxdev.x + m_clipboxdev.width, m_clipboxdev.y + m_clipboxdev.height);

	agg::path_storage path;
	path.move_to(x1, y1);
	path.line_to(x2, y2);

	agg::conv_stroke<agg::path_storage> stroke(path);
//	stroke.line_join();
//	stroke.line_cap();
	stroke.line_cap(agg::round_cap);
//	stroke.miter_limit();
	stroke.width(width);
	m_ras.add_path(stroke);
}

void SDraw::DrawEllipse(int x1, int y1, int cx, int cy, int width)
{
	agg::ellipse el;
//	el.init(double x¸ double y¸ double rx¸ double ry¸ unsigned num_steps=0¸ bool cw=false);
	el.init(x1, y1, cx, cy);

	agg::conv_stroke<agg::ellipse> stroke(el);

	stroke.width(width);
	m_ras.add_path(stroke);
}

Image SDraw::Render()
{
//	m_ras.filling_rule(filling_rule_e filling_rule);
	agg::render_scanlines(m_ras, m_sl, m_ren);
//	agg::render_scanlines_aa_solid(m_ras, m_sl, m_ren, agg::rgba(1,1,0.5) );
	return buffer;
}

SDraw::SDraw(int cx, int cy)
{
	buffer.Create(cx, cy);
	Fill(~buffer, White(), buffer.GetLength());
	m_rbuf.attach((agg::int8u *)~buffer, cx, cy, cx * 4);
	m_pixf.attach(m_rbuf);
	m_renb.attach(m_pixf);  //ren_base
	m_ren.attach(m_renb);
}

END_UPP_NAMESPACE
