#include "ScanLine.h"

struct SolidFiller : Rasterizer::Target {
	RGBA *t;
	RGBA  c;
	
	void Start(int minx, int maxx)      { t += minx; }
	void Render(int val)                { AlphaBlendCover8(*t++, c, val); } 
	void Render(int val, int len);
};

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

void Render(ImageBuffer& ib, Rasterizer& r, const RGBA& color, bool evenodd)
{
	Size sz = ib.GetSize();
	SolidFiller f;
	f.c = color;
	for(int y = r.MinY(); y <= r.MaxY(); y++) {
		f.t = ib[y];
		r.Render(y, f, evenodd);
	}
	r.Reset();
}
