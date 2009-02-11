#include "Painter.h"

NAMESPACE_UPP

RecFiller::RecFiller(int _cx)
{
	cx = _cx;
	buffer.Alloc(2 * cx + 2);
}

void RecFiller::Start(int xmin, int xmax)
{
	t = ~buffer;
	x = 0;
	maxlen = 0;
	maxx = 0;
	Render(0, xmin);
}

void RecFiller::Render(int val)
{
	Render(val, 1); //!Optimize later!
/*
	if(val == 0) {
		*t++ = 0;
		*t++ = 0;
	}
	else
	if(val == 256) {
		*t++ = 0;
		*t++ = 128;
	}
	else
		*t++ = val;
	x++;
*/
}

void RecFiller::Span(int c, int len)
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
	}
}

void RecFiller::Render(int val, int len)
{
	DLOG(val << " " << len);
	if(val == 256) {
		Span(128, len);
		if(len > maxlen) {
			maxx = x;
			maxlen = len;
		}
	}
	else
	if(val == 0)
		Span(0, len);
	else {
		memset(t, val, len);
		t += len;
	}
	x += len;
}

void RecFiller::Finish()
{
	while(x < cx) {
		int n = min(cx - x, 128);
		*t++ = 0;
		*t++ = n - 1;
		x += n;
	}
}

void RecFiller::GetResult(Buffer<byte>& tgt, int& _maxx, int& _maxlen)
{
	tgt.Clear();
	int l = t - ~buffer;
	tgt.Alloc(l);
	memcpy(~tgt, ~buffer, l);
	_maxx = maxx;
	_maxlen = maxlen;
	LOGHEXDUMP(~tgt, l);
	DDUMP(maxx);
	DDUMP(maxlen);
}

struct RasterizerMaskFilter : Rasterizer::Target {
	Rasterizer::Target *t;
	byte               *mask;
	int                 empty;
	int                 full;
	
	void Start(int minx, int maxx);
	void Render(int val, int len);
	void Render(int val);
	RasterizerMaskFilter()                        { empty = full = 0; }
};

void RasterizerMaskFilter::Render(int val)
{
	Render(val, 1);
/*	for(;;) {
		if(empty) {
			f.t++;
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
		else {
			if(m < 128)
				empty = m + 1;
			else
				full = m - 128 + 1;
		}
	}
*/
}

void RasterizerMaskFilter::Render(int val, int len)
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
			t->Render(256, n);
			full -= n;
			len -= n;
			empty = false;
		}
		else {
			byte m = *mask++;
			if(m) {
				int r = val * m >> 8;
				if(r)
					empty = false;
				t->Render(r);
				len--;
			}
			else {
				if(m < 128)
					empty = m + 1;
				else
					full = m - 128 + 1;
			}
		}
}

struct RasterizerNil : Rasterizer::Target {
	void Start(int minx, int maxx) {}
	void Render(int val, int len)  {}
	void Render(int val)           {}
};

void RasterizerMaskFilter::Start(int minx, int maxx)
{
	t->Start(minx, maxx);
	Rasterizer::Target *h = t;
	RasterizerNil nil;
	t = &nil;
	Render(0, minx);
	t = h;
}

END_UPP_NAMESPACE
