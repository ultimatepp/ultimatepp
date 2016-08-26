#include "Draw.h"

namespace Upp {

struct cDrawRasterData : DataDrawer {
	int                cx;
	StringStream       ss;
	One<StreamRaster>  raster;
	RescaleImage       si;

	virtual void Open(const String& data, int cx, int cy);
	virtual void Render(ImageBuffer& ib);
};

void cDrawRasterData::Open(const String& data, int _cx, int cy)
{
	cx = _cx;
	ss.Open(data);
	raster = StreamRaster::OpenAny(ss);
	if(raster)
		si.Create(Size(cx, cy), *raster, raster->GetSize());
}

void cDrawRasterData::Render(ImageBuffer& ib)
{
	for(int y = 0; y < ib.GetHeight(); y++)
		si.Get(ib[y]);
}

INITBLOCK
{
	DataDrawer::Register<cDrawRasterData>("image_data");
};

void DrawRasterData(Draw& w, int x, int y, int cx, int cy, const String& data)
{
	w.DrawData(x, y, cx, cy, data, "image_data");
}

}
