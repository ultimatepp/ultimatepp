#include "ScanLine.h"

struct RecScan : Rasterizer::Target {
	ScanLine sl;
	byte    *t;
	int      x;
	
	virtual void Render(int val);
	virtual void Render(int val, int len);
	virtual void Start(int x, int len);
	void Finish();
};

void RecScan::Start(int xmin, int xmax)
{
	sl.data.Alloc((xmax - xmin + 1) * 2);
	t = ~sl.data;
	sl.xmin = 0;
}

void RecScan::Render(int val)
{
	if(val == 0) {
		*t++ = 0;
		*t++ = 0;
	}
	else
	if(val == 256) {
		*t++ = 0;
		*t++ = 0;
	}
	else
		*t++ = val;
	x++;
}

void RecScan::Render(int val, int len)
{
	x += len;
	if(val == 256) {
		while(len > 0) {
			int n = min(len, 128);
			*t++ = 0;
			*t++ = 128 + n - 1;
			len -= n;
		}
		return;
	}
	if(val == 0) {
		while(len > 0) {
			int n = min(len, 128);
			*t++ = 0;
			*t++ = n - 1;
			len -= n;
		}
		return;
	}
	while(len--)
		*t++ = val;
}
/*
void RecScan::Finish(int cx)
{
	cx = cx - x;
	while(cx > 0) {
		int n = min(cx, 128);
		*t++ = 0;
		*t++ = n - 1;
		cx -= n;
	}
	sl.datalen = t - ~sl.data;
}

void RecScan::Shrink()
{
	if(sl.datalen + 30 < 
}
*/