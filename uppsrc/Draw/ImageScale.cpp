#include "Draw.h"

namespace Upp {

enum
{
	MAXAA = 4,

	MAP_COUNT   = 0,
	MAP_SEGMENT = 1,
	MAP_BLOCK   = 2,
	MAP_STEP    = 3,
	MAP_DATA    = 4,
};

enum { LOG2_STRETCH_CURVE = 10, COUNT_STRETCH_CURVE = 1 << LOG2_STRETCH_CURVE };

static const byte *GetStretchCurve()
{
	static byte cache[COUNT_STRETCH_CURVE];
	ONCELOCK {
		for(int i = 0; i < COUNT_STRETCH_CURVE; i++)
		{
			enum { HALF = COUNT_STRETCH_CURVE >> 1 };
			double a = (i <= HALF ? i / double(HALF) : (COUNT_STRETCH_CURVE - i) / double(HALF));
			double o = pow(a, 0.85);
			cache[i] = minmax<int>((int)((i <= HALF ? o : 2 - o) * 128), 0, 255);
		}
	}
	return cache;
}

Vector<dword> AAGetMap(int& dmin, int& dmax, int dclipmin, int dclipmax,
	                   int smin, int smax, int sclipmin, int sclipmax, int times, int avail)
{
	Vector<dword> map;
	if(dmax == dmin || smax == smin)
		return map;
	if(dmax < dmin)
	{
		Swap(dmin, dmax);
		Swap(smin, smax);
	}
	int dw = dmax - dmin, sw = smax - smin, spos;
	if(sw > 0)
	{
		int x0 = dmin;
		if(smin < sclipmin)
			x0 += iscalefloor(sclipmin - smin, dw, sw);
		if(x0 < dclipmin)
			x0 = dclipmin;
		spos = smin * dw + (x0 - dmin) * sw;
		dmin = x0;
		if(smax > sclipmax)
			dmax -= iscalefloor(smax - sclipmax, dw, sw);
		if(dmax > dclipmax)
			dmax = dclipmax;
	}
	else
	{
		int x0 = dmin;
		if(smin > sclipmax)
			x0 += iscalefloor(sclipmax - smin, dw, sw);
		if(x0 < dclipmin)
			x0 = dclipmin;
		spos = smin * dw + (x0 - dmin) * sw;
		dmin = x0;
		if(smax < sclipmin)
			dmax -= iscalefloor(smax - sclipmin, dw, sw);
		Swap(smin, smax);
	}
	int count = min(dclipmax, dmax) - dmin;
	if(smin < sclipmin)
		smin = sclipmin;
	if(smax > sclipmax)
		smax = sclipmax;
	if(smax <= smin || count <= 0)
		return map;
	int span = min(tabs(sw) % dw ? idivceil(tabs(sw), dw) + 1 : tabs(sw) / dw, smax - smin);
	bool bigseg = (span >= MAXAA);
	int segment = (bigseg ? MAXAA : span);
	int segstep = span / segment;
	map.SetCount(4 + count * (bigseg ? 1 : 1 + segment));
	map[MAP_COUNT] = dword(count);
	map[MAP_SEGMENT] = dword(segment);
	map[MAP_BLOCK] = 1 + (bigseg ? 0 : segment);
	map[MAP_STEP] = (span / segment) * times;
	dword *out = map.Begin() + MAP_DATA;
	int sendoffset = (smax - (segment - 1) * segstep - 1) * times;
	int last = 0;

	if(smax - smin == 1)
	{
		ASSERT(segment == 1);
		dword dval = dword(smin * times);
		*out++ = dval;
		*out++ = avail;
		while(--count > 0)
		{
			*out++ = 0;
			*out++ = avail;
		}
	}
	else if(tabs(sw) >= dw)
	{ // size reduction
		int sbegin = smin * dw, send = smax * dw, aw = tabs(sw);
		for(spos += min(sw, 0); --count >= 0; spos += sw)
		{
			int pb = max(spos, sbegin), pe = min(spos + aw, send);
			int total = pe - pb, left = avail;
			int start = idivfloor(pb, dw), end = idivceil(pe, dw) - 1, rem = pb % dw;
//			DUMP(start);
//			DUMP(end);
			if(pb >= send)
			{
				last += *out++ = sendoffset - last;
				if(!bigseg)
				{
					int i = segment - 1;
					while(--i >= 0)
						*out++ = 0;
					*out++ = left;
				}
			}
			else if(end <= start)
			{ // 1 source pixel only
//				ASSERT(!bigseg);
				int scomp = minmax(start + segment - smax, 0, start - smin);
				last += *out++ = (start - scomp) * times - last;
				if(!bigseg)
				{
					int i = scomp;
					while(--i >= 0)
						*out++ = 0;
					*out++ = dword(left);
					i = segment - scomp - 1;
					while(--i >= 0)
						*out++ = 0;
				}
			}
			else
			{
				int delta = (dw - rem) * left / total;
				if(!delta)
					start++;
				int scomp = minmax(start + span - smax, 0, start - smin);
				last += *out++ = (start - scomp) * times - last;
				if(!bigseg)
				{
					int i = scomp;
					while(--i >= 0)
						*out++ = 0;
					i = segment - scomp;
					if(delta)
					{
						*out++ = delta;
						left -= delta;
						total -= dw - rem;
						i--;
					}
					while(++start < end)
					{
						ASSERT(i > 0);
						delta = dw * left / total;
						*out++ = delta;
						left -= delta;
						total -= dw;
						--i;
					}
					if(left > 0)
					{
						ASSERT(i > 0);
						*out++ = left;
						--i;
					}
					while(--i >= 0)
						*out++ = 0;
				}
			}
//			LOG("-> " << map[rec] << " + " << map[rec + 1]);
		}
	}
	else
	{ // size inflation
		static const byte *curve = GetStretchCurve();
		ASSERT(segment == 2 && !bigseg);
		int sbegin = smin * dw, send = (smax - 1) * dw;
		for(spos += (sw - dw) >> 1; --count >= 0; spos += sw)
		{
			if(spos <= sbegin)
			{
				last += out[0] = smin * times - last;
				out[1] = avail;
				out[2] = 0;
			}
			else if(spos >= send)
			{
				last += out[0] = sendoffset - last;
				out[1] = 0;
				out[2] = avail;
			}
			else
			{
				int pos = spos / dw;
				int rel = spos % dw;
				last += out[0] = pos * times - last;
				out[1] = avail - (out[2] = curve[rel * COUNT_STRETCH_CURVE / dw]);
			}
			out += 3;
		}
	}

#ifdef _DEBUG
	ASSERT(out == map.End());
	int offs = 0, step = map[MAP_BLOCK], segspan = (map[MAP_SEGMENT] - 1) * map[MAP_STEP] + 1;
	for(int t = 0; t < (int)map[MAP_COUNT]; t++)
	{
		offs += map[MAP_DATA + t * step];
		ASSERT(offs >= times * smin && offs + segspan <= times * smax);
	}
#endif

	return map;
}

static void BltAAMapRGBA1(dword *dest, const RGBA *s, const dword *map)
{
	int count = map[MAP_COUNT];
	map += 4;
	while(count--) {
		s += map[0];
		dest[0] = s->b << 8;
		dest[1] = s->g << 8;
		dest[2] = s->r << 8;
		dest[3] = s->a << 8;
		map += 2;
		dest += 4;
	}
}

static void BltAAMapRGBA2(dword *dest, const RGBA *s, const dword *map)
{
	int count = map[MAP_COUNT];
	map += 4;
	while(count--) {
		s += map[0];
		dest[0] = s[0].b * map[1] + s[1].b * map[2];
		dest[1] = s[0].g * map[1] + s[1].g * map[2];
		dest[2] = s[0].r * map[1] + s[1].r * map[2];
		dest[3] = s[0].a * map[1] + s[1].a * map[2];
		map += 3;
		dest += 4;
	}
}


static void BltAAMapRGBA3(dword *dest, const RGBA *s, const dword *map)
{
	int count = map[MAP_COUNT];
	map += 4;
	while(count--) {
		s += map[0];
		dest[0] = s[0].b * map[1] + s[1].b * map[2] + s[2].b * map[3];
		dest[1] = s[0].g * map[1] + s[1].g * map[2] + s[2].g * map[3];
		dest[2] = s[0].r * map[1] + s[1].r * map[2] + s[2].r * map[3];
		dest[3] = s[0].a * map[1] + s[1].a * map[2] + s[2].a * map[3];
		map += 4;
		dest += 4;
	}
}

static void BltAAMapRGBA4(dword *dest, const RGBA *s, const dword *map)
{
	int step = map[MAP_STEP];
	int count = map[MAP_COUNT];
	map += 4;
	while(count--) {
		s += map[0];
		dest[0] = (s[0].b + s[step].b + s[2 * step].b + s[3 * step].b) << 6;
		dest[1] = (s[0].g + s[step].g + s[2 * step].g + s[3 * step].g) << 6;
		dest[2] = (s[0].r + s[step].r + s[2 * step].r + s[3 * step].r) << 6;
		dest[3] = (s[0].a + s[step].a + s[2 * step].a + s[3 * step].a) << 6;
		map += 1;
		dest += 4;
	}
}

void BltAASet2Fix(RGBA *dest, const dword *src1, dword w1, const dword *src2, dword w2, int count)
{
	while(count--) {
		dest->b = byte((src1[0] * w1 + src2[0] * w2) >> 16);
		dest->g = byte((src1[1] * w1 + src2[1] * w2) >> 16);
		dest->r = byte((src1[2] * w1 + src2[2] * w2) >> 16);
		dest->a = byte((src1[3] * w1 + src2[3] * w2) >> 16);
		dest++;
		src1 += 4;
		src2 += 4;
	}
}

void BltAASet3Fix(RGBA *dest,
                  const dword *src1, dword w1, const dword *src2, dword w2, const dword *src3, dword w3,
                  dword count)
{
	while(count--) {
		dest->b = byte((src1[0] * w1 + src2[0] * w2 + src3[0] * w3) >> 16);
		dest->g = byte((src1[1] * w1 + src2[1] * w2 + src3[1] * w3) >> 16);
		dest->r = byte((src1[2] * w1 + src2[2] * w2 + src3[2] * w3) >> 16);
		dest->a = byte((src1[3] * w1 + src2[3] * w2 + src3[3] * w3) >> 16);
		dest++;
		src1 += 4;
		src2 += 4;
		src3 += 4;
	}
}

void BltAASet4Fix(RGBA *dest, const dword *src1, const dword *src2,
                              const dword *src3, const dword *src4, int count)
{
	while(count--) {
		dest->b = byte((src1[0] + src2[0] + src3[0] + src4[0]) >> 10);
		dest->g = byte((src1[1] + src2[1] + src3[1] + src4[1]) >> 10);
		dest->r = byte((src1[2] + src2[2] + src3[2] + src4[2]) >> 10);
		dest->a = byte((src1[3] + src2[3] + src3[3] + src4[3]) >> 10);
		dest++;
		src1 += 4;
		src2 += 4;
		src3 += 4;
		src4 += 4;
	}
}

void BltAAFix2(RGBA *dest, const dword *src, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	const byte *s = (byte *)src + 1;
#else
	const byte *s = (byte *)src + 2;
#endif
	while(count--) {
		dest->b = s[0];
		dest->g = s[4];
		dest->r = s[8];
		dest->a = s[12];
		dest++;
		s += 16;
	}
}

void RescaleImage::Create(Size _tsz, Raster& _src, const Rect& src_rc)
{
	y = -1;
	src = &_src;
	tsz = _tsz;
	if(tsz.cx == 0 || tsz.cy == 0)
		return;

	size = src->GetSize();

	Rect dr = tsz;
	horz = AAGetMap(dr.left, dr.right, dr.left, dr.right,
	                src_rc.left, src_rc.right, 0, size.cx, 1, 0x100);
	if(horz.IsEmpty())
		return;

	vert = AAGetMap(dr.top, dr.bottom, dr.top, dr.bottom,
	                src_rc.top, src_rc.bottom, 0, size.cy, 1, 0x100);
	if(vert.IsEmpty())
		return;

	switch(horz[MAP_SEGMENT]) {
	case 1:  row_proc = BltAAMapRGBA1; break;
	case 2:  row_proc = BltAAMapRGBA2; break;
	case 3:  row_proc = BltAAMapRGBA3; break;
	case 4:  row_proc = BltAAMapRGBA4; break;
	default: NEVER(); return;
	}

	cx4 = 4 * tsz.cx;
	count = vert[MAP_COUNT];
	segment = vert[MAP_SEGMENT];
	entry = vert[MAP_BLOCK];
	step = vert[MAP_STEP];
	segspan = (segment - 1) * step + 1;
	bigseg = (segment == MAXAA);
	row_buffers.Alloc(cx4 * segment);
	first = vert[4];
	full = 0;
	offsets = vert.GetIter(4);
	offset = 0;
	y = 0;
	cii = 0;
	cache[0].ii = cache[1].ii = cache[2].ii = cache[3].ii = -1;
}

const RGBA *RescaleImage::GetLine(int ii)
{
	if(cache[0].ii == ii)
		return cache[0].line;
	if(cache[1].ii == ii)
		return cache[1].line;
	if(cache[2].ii == ii)
		return cache[2].line;
	if(cache[3].ii == ii)
		return cache[3].line;
	cache[cii].line = (*src)[ii];
	cache[cii].ii = ii;
	const RGBA *l = cache[cii].line;
	cii = (cii + 1) % 4;
	return l;
}

void RescaleImage::Get(RGBA *tgt)
{
	if(y < 0 || offsets >= vert.End()) {
		memset(tgt, 0, sizeof(RGBA) * tsz.cx);
		return;
	}
	offset += *offsets++;
	ASSERT(offset >= 0 && offset + segspan <= size.cy);
	if(bigseg) {
		row_proc(&row_buffers[0 * cx4], GetLine(offset + 0 * step), horz);
		row_proc(&row_buffers[1 * cx4], GetLine(offset + 1 * step), horz);
		row_proc(&row_buffers[2 * cx4], GetLine(offset + 2 * step), horz);
		row_proc(&row_buffers[3 * cx4], GetLine(offset + 3 * step), horz);
		BltAASet4Fix(tgt, &row_buffers[0 * cx4], &row_buffers[1 * cx4],
			              &row_buffers[2 * cx4], &row_buffers[3 * cx4], tsz.cx);
	}
	else {
		int endoff = offset + segment;
		for(int next = first + full; next < endoff; next++) {
			if(full >= segment)
				first++;
			else
				full++;
			row_proc(&row_buffers[next % segment * cx4], GetLine(next), horz);
		}
		while(first > offset) {
			if(full < segment)
				full++;
			--first;
			row_proc(&row_buffers[first % segment * cx4], GetLine(first), horz);
		}
		ASSERT(offset >= first && endoff <= first + full);
		switch(segment) {
		case 1:
			BltAAFix2(tgt, &row_buffers[offset % segment * cx4], tsz.cx);
			offsets++;
			break;
		case 2:
			if(offsets[0] == 0)
				BltAAFix2(tgt, &row_buffers[(offset + 1) % segment * cx4], tsz.cx);
			else
			if(offsets[1] == 0)
				BltAAFix2(tgt, &row_buffers[offset % segment * cx4], tsz.cx);
			else
				BltAASet2Fix(tgt, &row_buffers[(offset + 0) % segment * cx4], offsets[0],
					              &row_buffers[(offset + 1) % segment * cx4], offsets[1],
					         tsz.cx);
			offsets += 2;
			break;
		case 3:
			BltAASet3Fix(tgt,
			             &row_buffers[(offset + 0) % segment * cx4], offsets[0],
			             &row_buffers[(offset + 1) % segment * cx4], offsets[1],
			             &row_buffers[(offset + 2) % segment * cx4], offsets[2], tsz.cx);
			offsets += 3;
			break;
		default:
			NEVER();
			break;
		}
	}
}

bool Rescale(RasterEncoder& tgt, Size tsz, Raster& src, const Rect& src_rc,
             Gate<int, int> progress)
{
	tgt.Create(tsz, src);
	RescaleImage rs;
	rs.Create(tsz, src, src_rc);
	for(int i = 0; i < tsz.cy; i++) {
		if(progress(i, tsz.cy))
			return false;
		rs.Get(tgt);
		tgt.WriteLine();
	}
	return true;
}

Image Rescale(const Image& src, Size sz, const Rect& src_rc, Gate<int, int> progress)
{
	if(src.GetSize() == sz && src_rc == sz)
		return src;
	ImageRaster isrc(src);
	ImageEncoder tgt;
	Rescale(tgt, sz, isrc, src_rc);
	return tgt;
}

Image Rescale(const Image& src, Size sz, Gate<int, int> progress)
{
	return Rescale(src, sz, src.GetSize(), progress);
}

Image Rescale(const Image& src, int cx, int cy, Gate<int, int> progress)
{
	return Rescale(src, Size(cx, cy), progress);
}

}
