#include "IMapRender.h"

IMapRender::IMapRender(IMapRender* render)
	: IMapItem(render), _renderType(RENDER_DRAW), _quality(MODE_NOAA), _opacity(-1),
	 _clearColor(RGBAZero())
{}

void IMapRender::Clear()
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i])
			delete _items[i];
	_items.Clear();
}

void IMapRender::Render(Draw* w)
{
	if (!w || !IsShown())
		return;

/*	Rect r = GetRenderRect();
	Size rsz = r.GetSize();
	Size vsz = CurrentSceneSize();
	int px = 0;
	int py = 0;

	if (rsz.cx > vsz.cx)
		rsz.cx = vsz.cx;
	else
		px = r.TopLeft().x;

	if (rsz.cy > vsz.cy)
		rsz.cy = vsz.cy;
	else
		py = r.TopLeft().y;*/

	Size rsz = GetPageRect().GetSize();

	if (_renderType == RENDER_PAINTER)
	{
		ImageBuffer ib(rsz);
		BufferPainter sw(ib, _quality);
		sw.Clear(_clearColor);
//		if (_opacity >= 0)
//			sw.Opacity(_opacity);
		sw.Opacity(0.5);

		RenderItems(&sw);
		w->DrawImage(0, 0, ib);
	}
	else
		RenderItems(w);
}

void IMapRender::RenderItems(Draw* w)
{
	Rect view = CurrentViewRect();
	Vector<int> selected;

	for (int i = 0; i < _items.GetCount(); ++i)
	{
		if (!_items[i] || !_items[i]->IsShown())
			continue;

		if (!view.Intersects(_items[i]->GetRect()))
			continue;

		if (_items[i]->IsSelected())
		{
			selected << i;
			continue;
		}

		_items[i]->Render(w);
	}

	for (int i = 0; i < selected.GetCount(); ++i)
	{
		_items[selected[i]]->Render(w);
	}
}

IMapRender* IMapRender::Antialiased(bool use, bool subpixel)
{
	_quality = use ? (use + subpixel) : 0;
	_renderType = RENDER_PAINTER;
	return this;
}

IMapRender* IMapRender::Subpixel()
{
	_renderType = RENDER_PAINTER;
	_quality = MODE_SUBPIXEL;
	return this;
}

IMapRender* IMapRender::NoAA()
{
	_renderType = RENDER_DRAW;
	_quality = MODE_NOAA;
	return this;
}

Rect IMapRender::GetRect()
{
	Point rb = Point(INT_MAX, INT_MAX);
	Point lt = Point(-1, -1);

	for (int i = 0; i < _items.GetCount();  i++)
	{
		Rect r = _items[i]->GetRect();

		lt.x = min(r.TopLeft().x, lt.x);
		lt.y = min(r.TopLeft().y, lt.y);

		rb.x = max(r.BottomRight().x, rb.x);
		rb.y = max(r.BottomRight().y, rb.y);
	}

	return Rect(lt, rb);
}
