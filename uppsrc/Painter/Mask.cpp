#include "Painter.h"

namespace Upp {

void BufferPainter::BeginMaskOp()
{
	Finish();
	attr.mask = true;
	Size sz = ip->GetSize();
	mask.Add() = *ip;
	ip->Create(sz);
	UPP::Fill(~*ip, RGBAZero(), ip->GetLength());
	Begin();
}

static inline byte *sSpan(byte *t, int c, int& len)
{
	while(len > 128) {
		int n = min(len, 128);
		*t++ = 0;
		*t++ = c + n - 1;
		len -= n;
	}
	if(len) {
		*t++ = 0;
		*t++ = c + len - 1;
		len = 0;
	}
	return t;
}

void BufferPainter::FinishMask()
{
	Buffer<byte> wb(mode == MODE_SUBPIXEL ? 6 * ip->GetWidth() : 2 * ip->GetWidth());
	bool creating = false;
	if(!attr.hasclip) {
		clip.Add().Alloc(ip->GetHeight());
		attr.hasclip = true;
		attr.cliplevel = clip.GetCount();
		creating = true;
	}
	Buffer<ClippingLine>& cl = clip.Top();
	for(int y = 0; y < ip->GetHeight(); y++)
		if(creating || !cl[y].IsEmpty()) {
			int  c0 = 0;
			int  c256 = 0;
			const RGBA *s = (*ip)[y];
			const RGBA *e = (*ip)[y] + ip->GetWidth();
			byte  *t = wb;
			while(s < e) {
				int val = s->a * (56 * s->r + 183 * s->g + 20 * s->b) >> 16;
				if(val == 0) {
					if(c256) t = sSpan(t, 128, c256);
					c0++;
					if(mode == MODE_SUBPIXEL)
						c0 += 2;
				}
				else
				if(val == 256) {
					if(c0) t = sSpan(t, 0, c0);
					c256++;
					if(mode == MODE_SUBPIXEL)
						c256 += 2;
				}
				else {
					if(c256) t = sSpan(t, 128, c256);
					if(c0) t = sSpan(t, 0, c0);
					*t++ = val;
					if(mode == MODE_SUBPIXEL) {
						*t++ = val;
						*t++ = val;
					}
				}
				s++;
			}
			if(c256) t = sSpan(t, 128, c256);
			if(c0) t = sSpan(t, 0, c0);
			cl[y].Clear();
			cl[y].Set(~wb, int(t - ~wb));
		}
	*ip = mask.Top();
	mask.Drop();
	attr.mask = false;
}

}
