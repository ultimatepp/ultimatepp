#include "Draw.h"

namespace Upp {

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
	Mutex::Lock __(sDataDrawer);
	Map().Add(id, (void *)factory);
}

One<DataDrawer> DataDrawer::Create(const String& id)
{
	Mutex::Lock __(sDataDrawer);
	Factory q = (Factory) Map().Get(id, NULL);
	if(q)
		return (*q)();
	return NULL;
}

bool IsWhiteColumn(const Image& m, int x)
{
	LTIMING("IsEqColumn");
	Size sz = m.GetSize();
	const RGBA *s = ~m + x;
	while(sz.cy > 1) {
		s += sz.cx;
		if((s->a & s->r & s->g & s->b) != 255)
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
{ // optimizing for some old printers with limited memory which results in problems with printing e.g. full page image
	int xi = 0;
	int cx = m.GetWidth();
	int ccy = m.GetHeight();
	Buffer<bool> todo(cx, true);
#ifdef BENCHMARK_RLE
	sTotal += cx;
#endif
	while(xi < cx) {
		int xi0 = xi;
		while(w.Dots() && IsWhiteColumn(m, xi) && xi < cx)
			xi++;
		if(xi - xi0 >= 16) {
#ifdef BENCHMARK_RLE
			sRle += xi - xi0;
#endif
			w.DrawRect(x + xi0, y, xi - xi0, ccy, White);
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
			w.DrawImage(x + xi0, y, m, RectC(xi0, 0, xi - xi0, ccy));
		}
		else
			xi++;
}

void Draw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id)
{
	bool tonative = !IsNative();
	if(tonative) {
		BeginNative();
		Native(x, y);
		Native(cx, cy);
	}
	One<DataDrawer> dd = DataDrawer::Create(id);
	if(dd) {
		dd->Open(data, cx, cy);
		if((cx > 2048 || cy > 2048) && (GetInfo() & DATABANDS)) {
			int yy = 0;
			while(yy < cy) {
				int ccy = min(cy - yy, 32); // Must be multiply of 4 because of dithering
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
	if(tonative)
		EndNative();
}

DataDrawer::~DataDrawer() {}

}
