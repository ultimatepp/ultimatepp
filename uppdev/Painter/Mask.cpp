#include "Painter.h"

NAMESPACE_UPP

ClipFiller::ClipFiller(int _cx)
{
	cx = _cx;
	buffer.Alloc(2 * cx + 2);
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
				int r = val * m >> 8;
				t->Render(r);
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

END_UPP_NAMESPACE
