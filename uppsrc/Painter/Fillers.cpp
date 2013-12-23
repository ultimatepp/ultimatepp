#include "Painter.h"
#include "Fillers.h"

NAMESPACE_UPP

void FillRGBA(RGBA *t, RGBA c, int len)
{
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
}

void SolidFiller::Start(int minx, int maxx)
{
	t += minx;
}

inline RGBA InvertRGBA(const RGBA& c)
{
	RGBA a;
	a.r = ~c.r;
	a.g = ~c.g;
	a.b = ~c.b;
	a.a = 255;
	return a;
}

void SolidFiller::Render(int val)
{
	if(invert) {
		AlphaBlendCover8(*t, InvertRGBA(*t), val);
		t++;
	}
	else
		AlphaBlendCover8(*t++, c, val);
}

void SolidFiller::Render(int val, int len)
{
	if(val == 0) {
		t += len;
		return;
	}
	if(invert) {
		RGBA *e = t + len;
		if(val == 256)
			while(t < e) {
				*t = InvertRGBA(*t);
				t++;
			}
		else
			while(t < e) {
				AlphaBlendCover8(*t, InvertRGBA(*t), val);
				t++;
			}
	}
	else {
		if(((val - 256) | (c.a - 255)) == 0) {
			FillRGBA(t, c, len);
			t += len;
		}
		else {
			RGBA c1;
			if(val != 256)
				c1 = Mul8(c, val);
			else
				c1 = c;
			RGBA *e = t + len;
			while(t < e)
				AlphaBlend(*t++, c1);
		}
	}
}

void SubpixelFiller::Start(int minx, int maxx)
{
	int x = minx / 3;
	if(x > 0) {
		begin = sbuffer;
		x--;
	}
	else
		begin = sbuffer + 3;
	t += x;
	sbuffer[0] = sbuffer[1] = sbuffer[2] = sbuffer[3] =
	sbuffer[4] = sbuffer[5] = sbuffer[6] = sbuffer[7] = 0;
	v = sbuffer + 3 + minx % 3;
	if(ss) {
		int xx = maxx / 3;
		ss->Get(buffer, x, y, xx - x + 2);
		s = buffer;
	}
}

void SubpixelFiller::Render(int val)
{
	int16 *w = v;
	int h = val / 9;
	int h2 = h + h;
	w[-2] += h;
	w[2] += h;
	w[-1] += h2;
	w[1] += h2;
	w[0] += val - h2 - h2 - h2;
	w[3] = 0;
	v++;
}

void SubpixelFiller::RenderN(int val, int h, int n)
{
	int16 *w = v;
	int h2 = h + h;
	int hv2 = val - h2 - h2;
	int h3 = h2 + h;
	int hh;
	v += n;
	switch(n) {
	case 1: 
		w[-2] += h;
		w[-1] += h2;
		w[1] += h2;
		w[0] += hv2 - h2;
		w[2] += h;
		w[3] = 0;
		break;
	case 2:
		w[-2] += h;
		w[3] = h;
		w[-1] += h3;
		w[2] += h3;
		w[0] += hv2;
		w[1] += hv2;
		w[4] = 0;
		break;
	case 3:
		w[-2] += h;
		w[4] = h;
		w[-1] += h3;
		w[3] = h3;
		hh = hv2 + h;
		w[0] += hh;
		w[2] += hh;
		w[1] += hv2 + h2;
		w[5] =   0;
		break;
	case 4:
		w[-2] += h;
		w[5] = h;
		w[-1] += h3;
		w[4] = h3;
		hh = hv2 + h;
		w[0] +=  hh;
		w[3] = hh;
		hh = hv2 + h3;
		w[1] += hh;
		w[2] += hh;
		w[6] = 0;
		break;
	case 5:
		w[-2] += h;
		w[6] =   h;
		w[-1] += h3;
		w[5] =   h3;
		hh = hv2 + h;
		w[0] += hh;
		w[4] = hh;
		hh = h3 + hv2;
		w[1] += hh;
		w[3] = hh;
		w[2] += h3 + hv2 + h;
		w[7] = 0;
		break;
	case 6:
		w[-2] += h;
		w[7] = h;
		w[-1] += h3;
		w[6] = h3;
		hh = hv2 + h;
		w[0] += hh;
		w[5] = hh;
		hh = h3 + hv2;
		w[1] += hh;
		w[4] = hh;
		hh = h3 + hv2 + h;
		w[2] += hh;
		w[3] = hh;
		w[8] = 0;
		break;
	}
}

void SubpixelFiller::Render(int val, int len)
{
	int h = val / 9;
	if(len > 6) {
		int q = (3333333 - (v + 2 - begin)) % 3;
		len -= q + 2;
		int l = int(v + 2 + q - begin);
		RenderN(val, h, q + 4);
		Write(l / 3);
		l = len / 3;
		len -= 3 * l;
		RGBA *e = min(t + l, end);
		if(invert && !ss) {
			if(val == 256)
				while(t < e) {
					*t = InvertRGBA(*t);
					t++;
				}
			else
				while(t < e) {
					AlphaBlend(*t, Mul8(InvertRGBA(*t), val));
					t++;
				}
		}
		else {
			if(val == 256)
				if(!ss && color.a == 255) {
					FillRGBA(t, color, int(e - t));
					t = e;
				}
				else
					while(t < e)
						AlphaBlend(*t++, ss ? Mul8(*s++, alpha) : color);
			else
				if(ss)
					while(t < e)
						AlphaBlendCover8(*t++, Mul8(*s++, alpha), val);
				else {
					RGBA c = Mul8(color, val);
					while(t < e)
						AlphaBlend(*t++, c);
				}
		}
		v = begin = sbuffer + 3;
		v[0] = h + h + h;
		v[1] = h;
		v[2] = 0;
	}
	RenderN(val, h, len);
}

void SubpixelFiller::Write(int len)
{
	RGBA *e = min(t + len, end);
	int16 *q = begin;
	while(t < e) {
		RGBA c = ss ? Mul8(*s++, alpha) : invert ? InvertRGBA(*t) : color;
		int a;
		if(t->a != 255)
			AlphaBlendCover8(*t, c, (q[0] + q[1] + q[2]) / 3);
		else
		if(c.a == 255) {
			t->r = (c.r * q[0] >> 8) + ((257 - q[0]) * t->r >> 8);
			t->g = (c.g * q[1] >> 8) + ((257 - q[1]) * t->g >> 8);
			t->b = (c.b * q[2] >> 8) + ((257 - q[2]) * t->b >> 8);
		}
		else {
			a = c.a * q[0] >> 8;
			t->r = (c.r * q[0] >> 8) + ((256 - a - (a >> 7)) * t->r >> 8);
			a = c.a * q[1] >> 8;
			t->g = (c.g * q[1] >> 8) + ((256 - a - (a >> 7)) * t->g >> 8);
			a = c.a * q[2] >> 8;
			t->b = (c.b * q[2] >> 8) + ((256 - a - (a >> 7)) * t->b >> 8);
		}
		t++;
		q += 3;
	}
}

void SubpixelFiller::End()
{
	v[3] = v[4] = v[5] = 0;
	Write(int(v + 3 - begin) / 3);
}

void SpanFiller::Start(int minx, int maxx)
{
	t += minx;
	ss->Get(buffer, minx, y, maxx - minx + 1);
	s = buffer;
}

void SpanFiller::Render(int val)
{
	if(alpha != 256)
		val = alpha * val >> 8;
	AlphaBlendCover8(*t++, *s++, val);
} 

void SpanFiller::Render(int val, int len)
{
	if(val == 0) {
		t += len;
		s += len;
		return;
	}
	const RGBA *e = t + len;
	if(alpha != 256)
		val = alpha * val >> 8;
	if(val == 256)
		while(t < e) {
			if(s->a == 255)
				*t++ = *s++;
			else
				AlphaBlend(*t++, *s++);
		}
	else
		while(t < e)
			AlphaBlendCover8(*t++, *s++, val);
}

ClipFiller::ClipFiller(int _cx)
{
	cx = _cx;
	buffer.Alloc(2 * cx);
}

void ClipFiller::Clear()
{
	t = ~buffer;
	x = 0;
	empty = true;
	full = true;
	last = -1;
}

void ClipFiller::Start(int xmin, int xmax)
{
	Render(0, xmin);
}

void ClipFiller::Span(int val, int len)
{
	int v = val >> 1;
	if(last == val) {
		int n = min(v + 128 - *lastn - 1, len);
		*lastn += n;
		len -= n;
	}
	last = -1;
	while(len > 128) {
		int n = min(len, 128);
		*t++ = 0;
		*t++ = v + n - 1;
		len -= n;
	}
	if(len) {
		*t++ = 0;
		last = val;
		lastn = t;
		*t++ = v + len - 1;
	}
}

void ClipFiller::Render(int val, int len)
{
	if(val == 256) {
		Span(256, len);
		empty = false;
	}
	else {
		full = false;
		if(val == 0)
			Span(0, len);
		else {
			memset(t, val, len);
			t += len;
			empty = false;
			last = -1;
		}
	}
	x += len;
}

void ClipFiller::Render(int val)
{
	Render(val, 1);
}

void ClipFiller::Finish(ClippingLine& cl)
{
	if(empty)
		return;
	while(x < cx) {
		int n = min(cx - x, 128);
		*t++ = 0;
		*t++ = n - 1;
		x += n;
		full = false;
	}
	ASSERT(t - ~buffer <= 2 * cx);
	if(full)
		cl.SetFull();
	else
		cl.Set(~buffer, int(t - ~buffer));
}

void MaskFillerFilter::Render(int val)
{
	for(;;) {
		if(empty) {
			t->Render(0);
			empty--;
			return;
		}
		if(full) {
			t->Render(val);
			full--;
			return;
		}
		byte m = *mask++;
		if(m) {
			t->Render(val * m >> 8);
			return;
		}
		m = *mask++;
		if(m < 128)
			empty = m + 1;
		else
			full = m - 128 + 1;
	}
}

void MaskFillerFilter::Render(int val, int len)
{
	while(len)
		if(empty) {
			int n = min(len, empty);
			t->Render(0, n);
			empty -= n;
			len -= n;
		}
		else
		if(full) {
			int n = min(len, full);
			t->Render(val, n);
			full -= n;
			len -= n;
		}
		else {
			byte m = *mask++;
			if(m) {
				t->Render(val * m >> 8);
				len--;
			}
			else {
				m = *mask++;
				if(m < 128)
					empty = m + 1;
				else
					full = m - 128 + 1;
			}
		}
}

struct NilFiller : Rasterizer::Filler {
	void Start(int minx, int maxx) {}
	void Render(int val, int len)  {}
	void Render(int val)           {}
};

void MaskFillerFilter::Start(int minx, int maxx)
{
	t->Start(minx, maxx);
	Rasterizer::Filler *h = t;
	NilFiller nil;
	t = &nil;
	Render(0, minx);
	t = h;
}

void NoAAFillerFilter::Start(int minx, int maxx)
{
	t->Start(minx, maxx);
}

void NoAAFillerFilter::Render(int val, int len)
{
	t->Render(val < 128 ? 0 : 256, len);
}

void NoAAFillerFilter::Render(int val)
{
	t->Render(val < 128 ? 0 : 256);
}

END_UPP_NAMESPACE
