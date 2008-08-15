//#ifndef _agg_upp_bind_AggDrawer_h_
//#define _agg_upp_bind_AggDrawer_h_

//from DrawingDraw?
//#include "agg_upp_bind.h"

class AggDrawer {
private:
	typedef agg::renderer_base<pixfmt> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

	Rect                            m_r;
	Size                            sz;
	ImagBuffer                      uibuf;
	agg::rendering_buffer           m_rbuf;
	
	agg::rasterizer_scanline_aa<>   m_ras;
	agg::scanline_p8                m_sl;
	renderer_base                   m_renb; //renderer base
	renderer_solid                  m_ren; //(renb);  //
	pixfmt                          m_pixf;
	
	void AttachBuffers();
public:
	void DrawLine(int x1, int y1, int x2, int y2, int width = 0);
	void DrawLine(Point p1, Point p2, color_type  ct);
	void DrawEllipse(int x1, int y1, int cx, int cy, const color_type&  fill_ct, int width, const color_type&  ct);
	void DrawEllipse(int x1, int y1, int cx, int cy, int width);
	
	void SetBackground(const color_type&  ct)	{ m_renb.clear(ct); }
	void SetBrushColor(const color_type&  ct)   { m_ren.color(ct); }
	
	void RenderScanlines();

	void End(Draw& w, bool norr=false)           { RenderScanlines(); uibuf.Rectangalize(w, norr); uibuf.Clear(); }
	
//	AggDrawer(Draw& w, const Rect& r)     { m_w=w; m_r=r; sz=r.GetSize(); uibuf.Create(sz); AttachBuffers(); }
	AggDrawer(const Rect& r)     { m_r=r; sz=r.GetSize(); uibuf.Create(sz); AttachBuffers(); }

//virtual	~AggDrawer()              { uibuf.Rectangalize(m_w, true); uibuf.Clear(); }
};

//#endif
