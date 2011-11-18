#include "MipMap.h"
#include <Painter/Painter.h>

void MipMapItem::Render(Draw* w, const Rect& r)
{
	if (w)
		w->DrawImage(r, _mipMap);
}

void MipMapItem::Prepare(const char* fp, const Size& sz)
{
	if (_ready)
		return;
	_ready = true;

	FileIn in(fp);
	One<StreamRaster> r = StreamRaster::OpenAny(in);
	if (!r)
	{
		LOG(NFormat("Ошибка при загрузке файла карты: %s", fp));
		ImageBuffer ib(sz);
		BufferPainter sw(ib);
		sw.DrawRect(sz, Yellow());
		_mipMap = ib;
		return;
	}

	_mipMap = r->GetImage();
}

void MipMapItem::Release()
{
	if (!_ready)
		return;
	_mipMap.Clear();
	_ready = false;
}
