#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::BeginMaskOp()
{
	attr.mask = true;
	Size sz = buffer.GetSize();
	mask.Add() = buffer;
	buffer.Create(sz);
	SetRbuf();
	Clear(RGBAZero());
	Begin();
}

void BufferPainter::FinishMask()
{
	RGBA *s = ~buffer;
	RGBA *e = ~buffer + buffer.GetLength();
	byte *t, *cs;
	if(clip.GetCount())
		cs = ~clip.Top();
	if(!attr.hasclip) {
		clip.Add().Alloc(size.cx * size.cy);
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
	}
	t = ~clip.Top();
	if(clip.GetCount() == 1)
		while(s < e) {
			byte v = ((s->a + (s->a >> 7)) * (77 * s->r + 151 * s->g + 28 * s->b)) >> 16;
			*t = v;
			s++;
			t++;
		}
	else
		while(s < e) {
			byte v = ((s->a + (s->a >> 7)) * (77 * s->r + 151 * s->g + 28 * s->b)) >> 16;
			*t = (*cs * (v + (v >> 7))) >> 8;
			s++;
			t++;
			cs++;
		}
	buffer = mask.Top();
	mask.Drop();
	SetRbuf();
	attr.mask = false;
}

END_UPP_NAMESPACE
