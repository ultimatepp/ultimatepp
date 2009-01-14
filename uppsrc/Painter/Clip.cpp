#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::RenderClip(byte *t, int alpha)
{
	agg::rendering_buffer mask_rbuf;
	mask_rbuf.attach(t, size.cx, size.cy, size.cx);
	typedef agg::renderer_base<agg::pixfmt_gray8> ren_base;
	agg::pixfmt_gray8 pixf(mask_rbuf);
	ren_base rb(pixf);
	agg::scanline_p8 sl;
	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	rasterizer.filling_rule(pathattr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	rasterizer.add_path(curved_trans);
	agg::renderer_scanline_aa_solid<ren_base> r(rb);
	r.color(agg::gray8(alpha, 255));
	agg::render_scanlines(rasterizer, sl, r);
	rasterizer.reset();
	inpath = false;
}

void BufferPainter::ClipOp()
{
	int l = size.cx * size.cy;
	Buffer<byte> cl(l);
	memset(~cl, 0, l);
	RenderClip(~cl, 255);
	if(clip.GetCount()) {
		byte *s = ~clip.Top();
		for(int i = 0; i < l; i++)
			cl[i] = ((cl[i] + (cl[i] >> 7)) * s[i]) >> 8;
	}
	if(attr.hasclip)
		clip.Top() = cl;
	else {
		clip.Add() = cl;
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
}

END_UPP_NAMESPACE
