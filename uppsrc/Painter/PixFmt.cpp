#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::upp_pixfmt::blend_hline(int x, int y, int len, RGBA c, byte cover)
{
	PAINTER_TIMING("Blend hline");
	if(c.a == 0) return;
	RGBA *t = ptr(x, y);
	if(noaa)
		cover = cover > 127 ? 255 : 0;
	if((c.a & cover) == 255) {
#if 1
		while(len >= 16) {
			t[0] = c; t[1] = c; t[2] = c; t[3] = c;
			t[4] = c; t[5] = c; t[6] = c; t[7] = c;
			t[8] = c; t[9] = c; t[10] = c; t[11] = c;
			t[12] = c; t[13] = c; t[14] = c; t[15] = c;
			t += 16;
			len -= 16;
		}
		switch(len) {
		case 15: t[14] = c;
		case 14: t[13] = c;
		case 13: t[12] = c;
		case 12: t[11] = c;
		case 11: t[10] = c;
		case 10: t[9] = c;
		case 9: t[8] = c;
		case 8: t[7] = c;
		case 7: t[6] = c;
		case 6: t[5] = c;
		case 5: t[4] = c;
		case 4: t[3] = c;
		case 3: t[2] = c;
		case 2: t[1] = c;
		case 1: t[0] = c;
		}
#else                    
		do
			*t++ = c;
		while(--len);
#endif
	}
	else {
		if(cover != 255)
			c = MulA(c, cover);
		RGBA *e = t + len;
		int alpha = 256 - (c.a + (c.a >> 7));
		while(t < e)
			AlphaBlend(*t++, c);
	}
	FinishBlend();
}

void BufferPainter::upp_pixfmt::blend_solid_hspan(int x, int y, int len,
                                                  const RGBA& c, const byte *covers)
{
	PAINTER_TIMING("Blend solid hspan");
	if(c.a == 0) return;
	RGBA *t = ptr(x, y);
#ifdef USE_MMX
	if(!noaa) {
		AlphaBlendCover((dword *)t, *(dword *)&c, covers, len);
		return;
	}
#else
	RGBA *e = t + len;
	while(t < e) {
		byte cover = *covers++;
		if(noaa)
			cover = cover > 127 ? 255 : 0;
		if((cover & c.a) == 255) // is it worth it?
			*t++ = c;
		else
			AlphaBlendCover(*t++, c, cover);
	}
#endif
	FinishBlend();
}

void BufferPainter::upp_pixfmt::blend_color_hspan(int x, int y, int len, const RGBA *colors,
                                                  const byte *covers, byte cover)
{
	PAINTER_TIMING("Blend color hspan");
	RGBA *t = ptr(x, y);
	RGBA *e = t + len;
	if(noaa)
		cover = cover > 127 ? 255 : 0;
	while(t < e) {
		if(covers) {
			cover = *covers++;
			if(noaa)
				cover = cover > 127 ? 255 : 0;
		}
		if((cover & colors->a) == 255)
			*t = *colors;
		else
		if(cover == 255)
			AlphaBlend(*t, *colors);
		else
			AlphaBlendCover(*t, *colors, cover);
		colors++;
		t++;
	}
	FinishBlend();
}

END_UPP_NAMESPACE
