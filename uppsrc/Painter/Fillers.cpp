#include "Painter.h"
#include "Fillers.h"

NAMESPACE_UPP

void SolidFiller::Start(int minx, int maxx)
{
	t += minx;
}

void SolidFiller::Render(int val)
{
	AlphaBlendCover8(*t++, c, val);
}

void SolidFiller::Render(int val, int len)
{
	if(val == 0) {
		t += len;
		return;
	}
	if(((val - 256) | (c.a - 255)) == 0) {
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

void SubpixelFiller::Start(int minx, int maxx)
{
	x = minx / 3;
	t += x;
	v = sbuffer;
	int n = minx % 3 + 3;
	while(n--)
		*v++ = 0;
	v[0] = v[1] = v[2] = v[3] = v[4] = 0;
	if(ss) {
		int xx = maxx / 3;
		ss->Get(buffer, x, y, xx - x + 2);
		s = buffer;
	}
}

void SubpixelFiller::Render(int val)
{
	int h = (7282 * val) >> 16;
	int h2 = h + h;
	v[-2] += h;
	v[-1] += h2;
	v[0] += val - h2 - h2 - h2;
	v[1] += h2;
	v[2] += h;
	v[3] = 0;
	v++;
	x++;
}

void SubpixelFiller::Render(int val, int len)
{
	while(len--)
		Render(val);
}

void SubpixelFiller::End()
{
	int16 *q = sbuffer + 3;
	while(q < v) {
		RGBA c = ss ? Mul8(*s++, alpha) : color;
		int a, alpha;
		a = c.a * q[0] >> 8;
		alpha = 256 - (a + (a >> 7));
		t->r = (c.r * q[0] >> 8) + (alpha * t->r >> 8);
		a = c.a * q[1] >> 8;
		alpha = 256 - (a + (a >> 7));
		t->g = (c.g * q[1] >> 8) + (alpha * t->g >> 8);
		a = c.a * q[2] >> 8;
		alpha = 256 - (a + (a >> 7));
		t->b = (c.b * q[2] >> 8) + (alpha * t->b >> 8);
		a = c.a * (q[0] + q[1] + q[2]) / 3 >> 8;
		alpha = 256 - (a + (a >> 7));
		t->a = a + (alpha * t->a >> 8);
		t++;
		q += 3;
	}
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

void ClipFiller::Finish(ClipLine& cl)
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
	if(full)
		cl.SetFull();
	else
		cl.Set(~buffer, t - ~buffer);
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
