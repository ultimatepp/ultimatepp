#include "glyph.h"

#if 0

Image RenderGlyph(Font fnt, int chr, Color color, int angle)
{
	int cx = fnt[chr];
	int cy = fnt.GetLineHeight();
	ImageBuffer ib(2 * (cx + cy), 2 * (cx + cy));
	BufferPainter sw(ib, MODE_ANTIALIASED);
	sw.Clear(RGBAZero());
	sw.DrawText(cx + cy, cx + cy, angle, WString(chr, 1), fnt, color);
	ib.SetHotSpot(Point(cx + cy, cx + cy));
	return AutoCrop(ib, RGBAZero());
}

/*
Image RenderGlyph(Point at, int angle, int chr, Font fnt, Color color, Size sz)
{
	int cx = fnt[chr];
	int cy = fnt.GetLineHeight();
	ImageBuffer ib(sz);
	BufferPainter sw(ib);
	sw.Clear(RGBAZero());
	sw.DrawText(at.x, at.y, angle, WString(chr, 1), fnt, color);
	ib.SetHotSpot(at);
	for(RGBA *s = ib; s < ib.End(); s++)
		s->a = (s->a & 0xf8);
	return ib;
}
*/

String CompressGlyph(const Image& m)
{
	const RGBA *s = m;
	const RGBA *e = s + m.GetLength();
	StringStream r;
	Size sz = m.GetSize();
	r / sz.cx / sz.cy;
	Point p = m.GetHotSpot();
	r / p.x / p.y;
	while(s < e) {
		const RGBA *b = s;
		if(s->a == 0 || s->a == 255) {
			byte a = s->a;
			s++;
			while(s < e && s - b < 63 && s->a == a)
				s++;
			if(a == 0 && s >= e)
				return r;
			r.Put((a & 0x40) | (s - b));
		}
		else {
			r.Put((s->a >> 1) | 0x80);
			s++;
		}
	}
	return r;
}

Image DecompressGlyph(const String& g, Color c)
{
	StringStream r(g);
	Size sz;
	r / sz.cx / sz.cy;
	ImageBuffer ib(sz);
	Point p;
	ib.SetHotSpot(p);
	r / p.x / p.y;
	RGBA *t = ib;
	RGBA *te = ib.End();
	RGBA full = c;
	while(!r.IsEof()) {
		byte b = r.Get();
		ASSERT(t < te);
		if(b & 0x80)
			*t++ = (((b & 0x7f) << 1) | ((b & 0x40) >> 6)) * c;
		else {
			int n = b & 63;
			ASSERT(t + n <= te);
			Fill(t, b & 0x40 ? full : RGBAZero(), n);
			t += n;
		}
	}
	Fill(t, RGBAZero(), te - t);
	return ib;
}

#endif
