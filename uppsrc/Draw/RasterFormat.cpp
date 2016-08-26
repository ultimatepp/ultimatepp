#include "Draw.h"

namespace Upp {

static int sMaskPos16(dword mask)
{
	if(mask > 0xffff)
		return 0;
	int pos = 0;
	while(((mask << pos) & 0x18000) != 0x8000)
		pos++;
	return pos;
}

void RasterFormat::Set16le(dword _rmask, dword _gmask, dword _bmask)
{
	rpos = sMaskPos16(rmask = _rmask);
	gpos = sMaskPos16(gmask = _gmask);
	bpos = sMaskPos16(bmask = _bmask);
	type = RASTER_16;
}

void RasterFormat::Set16be(dword _rmask, dword _gmask, dword _bmask)
{
	Set16le(_rmask, _gmask, _bmask);
	type = RASTER_16|RASTER_MSBFIRST;
}

void RasterFormat::Set24le(dword rmask, dword gmask, dword bmask)
{
	rpos = GetChMaskPos32(rmask);
	gpos = GetChMaskPos32(gmask);
	bpos = GetChMaskPos32(bmask);
	apos = 0;
	type = RASTER_24;
}

void RasterFormat::Set32le(dword rmask, dword gmask, dword bmask, dword amask)
{
	Set24le(rmask, gmask, bmask);
	if(amask) {
		apos = GetChMaskPos32(amask);
		type = RASTER_32PREMULTIPLIED;
	}
	else
		type = RASTER_32;
}

void RasterFormat::Set24be(dword rmask, dword gmask, dword bmask)
{
	rpos = 2 - GetChMaskPos32(rmask);
	gpos = 2 - GetChMaskPos32(gmask);
	bpos = 2 - GetChMaskPos32(bmask);
	apos = 0;
	type = RASTER_24;
}

void RasterFormat::Set32be(dword rmask, dword gmask, dword bmask, dword amask)
{
	rpos = 3 - GetChMaskPos32(rmask);
	gpos = 3 - GetChMaskPos32(gmask);
	bpos = 3 - GetChMaskPos32(bmask);
	if(amask) {
		apos = 3 - GetChMaskPos32(amask);
		type = RASTER_32PREMULTIPLIED;
	}
	else
		type = RASTER_32;
}

void RasterFormat::Set32leStraight(dword rmask, dword gmask, dword bmask, dword amask)
{
	Set32le(rmask, gmask, bmask, amask);
	type = RASTER_32ALPHA;
}

void RasterFormat::Set32beStraight(dword rmask, dword gmask, dword bmask, dword amask)
{
	Set32be(rmask, gmask, bmask, amask);
	type = RASTER_32ALPHA;
}

void RasterFormat::SetRGBA()
{
	type = RASTER_32PREMULTIPLIED;
	bpos = 0;
	gpos = 1;
	rpos = 2;
	apos = 3;
}

void RasterFormat::SetRGBAStraight()
{
	type = RASTER_32ALPHA;
	bpos = 0;
	gpos = 1;
	rpos = 2;
	apos = 3;
}

int RasterFormat::IsRGBA() const
{
	return (type & 31) == RASTER_32PREMULTIPLIED && bpos == 0 && gpos == 1 && rpos == 2 && apos == 3;
}

static byte bits[16] = { 1, 2, 4, 8, 16, 16, 24, 32, 32, 32 };

int RasterFormat::GetByteCount(int cx) const
{
	int b = bits[type & 15];
	return (cx * b + 7) >> 3;
}

int RasterFormat::GetBpp() const
{
	return bits[type & 15];
}

bool RasterFormat::HasAlpha() const
{
	return (type & 15) == RASTER_32ALPHA;
}

int RasterFormat::GetColorCount() const
{
	static int colors[16] = { 2, 4, 16, 256, 256, 65536, 256 * 256 * 256, 256 * 256 * 256, 256 * 256 * 256 };
	return colors[type & 15];
}

int RasterFormat::GetPaletteCount() const
{
	static int colors[16] = { 2, 4, 16, 256 };
	return (type & 15) < 4 ? colors[type & 15] : 0;
}

void RasterFormat::TailBits(RGBA *t, const byte *src, int cx, byte andm, byte shift, const RGBA *palette)
{
	if(cx) {
		byte c = *src;
		while(cx > 0) {
			*t++ = palette[c & andm]; c >>= shift;
			cx--;
		}
	}
}

void RasterFormat::TailBitsMSB1st(RGBA *t, const byte *src, int cx, byte shift1, byte andm, byte shift, const RGBA *palette)
{
	if(cx) {
		byte c = *src;
		while(cx > 0) {
			*t++ = palette[(c >> shift1) & andm]; c <<= shift;
			cx--;
		}
	}
}

void RasterFormat::Read(RGBA *t, const byte *s, int cx, const RGBA *palette) const
{
	switch(type) {
	case RASTER_1:
		{
			const byte *e = s + (cx >> 3);
			while(s < e) {
				byte c = *s++;
				t[0] = palette[(c >> 0) & 1];
				t[1] = palette[(c >> 1) & 1];
				t[2] = palette[(c >> 2) & 1];
				t[3] = palette[(c >> 3) & 1];
				t[4] = palette[(c >> 4) & 1];
				t[5] = palette[(c >> 5) & 1];
				t[6] = palette[(c >> 6) & 1];
				t[7] = palette[(c >> 7) & 1];
				t += 8;
			}
			TailBits(t, s, cx & 7, 1, 1, palette);
		}
		break;
	case RASTER_1|RASTER_MSBFIRST:
		{
			const byte *e = s + (cx >> 3);
			while(s < e) {
				byte c = *s++;
				t[7] = palette[(c >> 0) & 1];
				t[6] = palette[(c >> 1) & 1];
				t[5] = palette[(c >> 2) & 1];
				t[4] = palette[(c >> 3) & 1];
				t[3] = palette[(c >> 4) & 1];
				t[2] = palette[(c >> 5) & 1];
				t[1] = palette[(c >> 6) & 1];
				t[0] = palette[(c >> 7) & 1];
				t += 8;
			}
			TailBitsMSB1st(t, s, cx & 7, 7, 1, 1, palette);
		}
		break;
	case RASTER_2:
		{
			const byte *e = s + (cx >> 2);
			while(s < e) {
				byte c = *s++;
				t[0] = palette[(c >> 0) & 3];
				t[1] = palette[(c >> 2) & 3];
				t[2] = palette[(c >> 4) & 3];
				t[3] = palette[(c >> 6) & 3];
				t += 4;
			}
			TailBits(t, s, cx & 3, 3, 2, palette);
		}
		break;
	case RASTER_2|RASTER_MSBFIRST:
		{
			const byte *e = s + (cx >> 2);
			while(s < e) {
				byte c = *s++;
				t[3] = palette[(c >> 0) & 3];
				t[2] = palette[(c >> 2) & 3];
				t[1] = palette[(c >> 4) & 3];
				t[0] = palette[(c >> 6) & 3];
				t += 4;
			}
			TailBitsMSB1st(t, s, cx & 3, 6, 3, 2, palette);
		}
		break;
	case RASTER_4:
		{
			const byte *e = s + (cx >> 1);
			while(s < e) {
				byte c = *s++;
				t[0] = palette[(c >> 0) & 15];
				t[1] = palette[(c >> 4) & 15];
				t += 2;
			}
			TailBits(t, s, cx & 1, 15, 4, palette);
		}
		break;
	case RASTER_4|RASTER_MSBFIRST:
		{
			const byte *e = s + (cx >> 1);
			while(s < e) {
				byte c = *s++;
				t[1] = palette[(c >> 0) & 15];
				t[0] = palette[(c >> 4) & 15];
				t += 2;
			}
			TailBitsMSB1st(t, s, cx & 1, 4, 15, 4, palette);
		}
		break;
	case RASTER_8:
	case RASTER_8|RASTER_MSBFIRST:
		{
			RGBA *e = t + cx;
			while(t < e)
				*t++ = palette[*s++];
		}
		break;
	case RASTER_8ALPHA:
	case RASTER_8ALPHA|RASTER_MSBFIRST:
		{
			RGBA *e = t + cx;
			RGBA *b = t;
			while(t < e) {
				RGBA pal = palette[*s];
				pal.a = s[1];
				*t++ = pal;
				s += 2;
			}
			Premultiply(b, b, cx);
			break;
		}
	case RASTER_16:
		{
			RGBA *e = t + cx;
			while(t < e) {
				word w = Peek16le(s);
				t->a = 255;
				t->r = byte((w & rmask) << rpos >> 8);
				t->g = byte((w & gmask) << gpos >> 8);
				t->b = byte((w & bmask) << bpos >> 8);
				s += 2;
				t++;
			}
		}
		break;
	case RASTER_16|RASTER_MSBFIRST:
		{
			RGBA *e = t + cx;
			while(t < e) {
				word w = Peek16be(s);
				t->a = 255;
				t->r = byte((w & rmask) << rpos >> 8);
				t->g = byte((w & gmask) << gpos >> 8);
				t->b = byte((w & bmask) << bpos >> 8);
				s += 2;
				t++;
			}
		}
		break;
	case RASTER_24:
	case RASTER_24|RASTER_MSBFIRST:
		{
			RGBA *e = t + cx;
			while(t < e) {
				t->a = 255;
				t->r = s[rpos];
				t->g = s[gpos];
				t->b = s[bpos];
				s += 3;
				t++;
			}
		}
		break;
	case RASTER_32:
	case RASTER_32|RASTER_MSBFIRST:
		if(bpos == 0 && gpos == 1 && rpos == 2) {
			RGBA *e = t + cx;
			while(t < e) {
				*t = *(RGBA *)s;
				t->a = 255;
				s += 4;
				t++;
			}
		}
		else
		{
			RGBA *e = t + cx;
			while(t < e) {
				t->a = 255;
				t->r = s[rpos];
				t->g = s[gpos];
				t->b = s[bpos];
				s += 4;
				t++;
			}
		}
		break;
	case RASTER_32ALPHA:
	case RASTER_32ALPHA|RASTER_MSBFIRST: {
			RGBA *e = t + cx;
			while(t < e) {
				t->a = s[apos];
				int alpha = t->a + (t->a >> 7);
				t->r = (alpha * s[rpos]) >> 8;
				t->g = (alpha * s[gpos]) >> 8;
				t->b = (alpha * s[bpos]) >> 8;
				s += 4;
				t++;
			}
		}
		break;
	case RASTER_32PREMULTIPLIED:
	case RASTER_32PREMULTIPLIED|RASTER_MSBFIRST:
		if(bpos == 0 && gpos == 1 && rpos == 2 && apos == 3)
			memcpy(t, s, cx * sizeof(RGBA));
		else {
			RGBA *e = t + cx;
			while(t < e) {
				t->a = s[apos];
				t->r = s[rpos];
				t->g = s[gpos];
				t->b = s[bpos];
				s += 4;
				t++;
			}
		}
		break;
	}
}

}
