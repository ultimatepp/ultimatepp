#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::upp_pixfmt_clip::blend_hline(int x, int y, int len, byte, byte cover)
{
	memset(ptr(x, y), cover, len);
}

void BufferPainter::upp_pixfmt_clip::blend_solid_hspan(int x, int y, int len, byte, const byte *covers)
{
	memcpy(ptr(x, y), covers, len);
}

void BufferPainter::RenderClip(byte *t)
{
	upp_pixfmt_clip pixf;
	pixf.sz = size;
	pixf.buffer = t;
	typedef agg::renderer_base<upp_pixfmt_clip> ren_base;
	ren_base rb(pixf);
	agg::scanline_p8 sl;
	if(inpath)
		path.close_polygon();
	rasterizer.reset();
	rasterizer.filling_rule(pathattr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	rasterizer.add_path(curved_trans);
	agg::renderer_scanline_aa_solid<ren_base> r(rb);
	r.color(255);
	agg::render_scanlines(rasterizer, sl, r);
	rasterizer.reset();
	inpath = false;
}

void BufferPainter::ClipOp()
{
	int l = size.cx * size.cy;
	Buffer<byte> cl(l);
	memset(~cl, 0, l);
	RenderClip(~cl);
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
