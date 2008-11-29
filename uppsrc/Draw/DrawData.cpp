#include "Draw.h"

NAMESPACE_UPP

#define LTIMING(x)
// #define BENCHMARK_RLE

VectorMap<String, void *>& DataDrawer::Map()
{
	static VectorMap<String, void *> x;
	return x;
}

static StaticCriticalSection sDataDrawer;

void DataDrawer::AddFormat(const char *id, Factory factory)
{
	INTERLOCKED_(sDataDrawer)
		Map().Add(id, (void *)factory);
}

One<DataDrawer> DataDrawer::Create(const String& id)
{
	INTERLOCKED_(sDataDrawer) {
		Factory q = (Factory) Map().Get(id, NULL);
		if(q)
			return (*q)();
	}
	return NULL;
}

bool IsEqColumn(const Image& m, int x, RGBA c)
{
	LTIMING("IsEqColumn");
	Size sz = m.GetSize();
	const RGBA *s = ~m + x;
	if(c.a != 255)
		return false;
	while(sz.cy > 1) {
		s += sz.cx;
		if(*s != c)
			return false;
		sz.cy--;
	}
	return true;
}


#ifdef BENCHMARK_RLE
static int sTotal;
static int sRle;

EXITBLOCK
{
	DUMP(sTotal);
	DUMP(sRle);
}
#endif

void DrawImageBandRLE(Draw& w, int x, int y, const Image& m, int minp)
{
	int xi = 0;
	int cx = m.GetWidth();
	int ccy = m.GetHeight();
	Buffer<bool> todo(cx, true);
#ifdef BENCHMARK_RLE
	sTotal += cx;
#endif
	while(xi < cx) {
		int xi0 = xi;
		RGBA c = m[0][xi0];
		while(w.Dots() && IsEqColumn(m, xi, c) && xi < cx)
			xi++;
		if(xi - xi0 >= 16) {
#ifdef BENCHMARK_RLE
			sRle += xi - xi0;
#endif
			w.DrawRect(x + xi0, y, xi - xi0, ccy, c);
			Fill(~todo + xi0, ~todo + xi, false);
		}
		xi++;
	}
	
	xi = 0;
	while(xi < cx)
		if(todo[xi]) {
			int xi0 = xi;
			while(xi < cx && todo[xi] && xi - xi0 < 2000)
				xi++;
			w.DrawImage(x + xi0, y, Crop(m, xi0, 0, xi - xi0, ccy));
		}
		else
			xi++;
}

void Draw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id)
{
	DrawLock __;
	One<DataDrawer> dd = DataDrawer::Create(id);
	if(dd) {
		dd->Open(data, cx, cy);
		if(cx > 2048 || cy > 2048) {
			int yy = 0;
			while(yy < cy) {
				int ccy = min(cy - yy, 16);
				ImageBuffer ib(cx, ccy);
				dd->Render(ib);
				DrawImageBandRLE(*this, x, y + yy, ib, 16);
				yy += ccy;
			}
		}
		else {
			ImageBuffer m(cx, cy);
			dd->Render(m);
			DrawImage(x, y, m);
		}
	}
}

DataDrawer::~DataDrawer() {}

END_UPP_NAMESPACE
