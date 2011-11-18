#include "MapLevel.h"

MapLevel& MapLevel::PagePos(const Point& p)
{
	Size rsz = GetRenderSize();
	Size psz = GetPageSize();

	if ( psz.cx < rsz.cx )
	{
		if (p.x < 0)
			_pagePos.x = 0;
		else if (p.x >= rsz.cx - psz.cx)
			_pagePos.x = rsz.cx - psz.cx - 1;
		else
			_pagePos.x = p.x;
	}
	else
	{
		_pagePos.x = 0;
	}

	if ( psz.cy < rsz.cy )
	{
		if (p.y < 0)
			_pagePos.y = 0;
		else if (p.y >= rsz.cy - psz.cy)
			_pagePos.y = rsz.cy - psz.cy - 1;
		else
			_pagePos.y = p.y;
	}
	else
	{
		_pagePos.y = 0;
	}

	return *this;
}

MapLevel& MapLevel::MoveTo(const Point& p)
{
	PagePos(p);
	UpdateMap();
	return *this;
}

MapLevel& MapLevel::MoveBy(const Point& p)
{
	PagePos(GetPagePos() + p);
	UpdateMap();
	return *this;
}

Point MapLevel::PointZoomIn()
{
	if (!_zoomLevels.GetCount())
		return Point(0, 0);

	int newZoom  = _zoomLevel + 1;
	if (newZoom >= _zoomLevels.GetCount())
		return Point(0, 0);

	_zoomLevel = newZoom;

	// before
	Point s = GetPagePos();
	Size ss = GetSize();

	// after
	double x  = (double)s.x   * _zoomLevels[_zoomLevel].GetZoom();
	double y  = (double)s.y   * _zoomLevels[_zoomLevel].GetZoom();

	double cx = (double)ss.cx * _zoomLevels[_zoomLevel].GetZoom();
	double cy = (double)ss.cy * _zoomLevels[_zoomLevel].GetZoom();

	// calculating new pos
	x -= (ss.cx - cx) / 2;
	y -= (ss.cy - cy) / 2;
	Point pos = Point((int)x, (int)y);

	return pos;
}

Point MapLevel::PointZoomOut()
{
	if (!_zoomLevels.GetCount())
		return Point(0, 0);

	int newZoom = _zoomLevel - 1;
	if (newZoom < 0)
		return Point(0, 0);

	_zoomLevel = newZoom;

	// before
	Point s = GetPagePos();
	Size ss = GetPageSize();

	// after
	double x  = (double)s.x   / _zoomLevels[_zoomLevel].GetZoom();
	double y  = (double)s.y   / _zoomLevels[_zoomLevel].GetZoom();

	double cx = (double)ss.cx / _zoomLevels[_zoomLevel].GetZoom();
	double cy = (double)ss.cy / _zoomLevels[_zoomLevel].GetZoom();

	// calculating new pos
	x -= (ss.cx - cx) / 2;
	y -= (ss.cy - cy) / 2;
	Point pos = Point((int)x, (int)y);

	return pos;
}

MapLevel& MapLevel::ZoomTo(int zoom)
{
	if (zoom == _zoomLevel)
		return *this;

	Point pos = Point(0, 0);

	if (zoom > _zoomLevel)
	{
		for (int i = _zoomLevel; i < zoom; ++i)
			pos = PointZoomIn();
	}

	if (zoom < _zoomLevel)
	{
		for (int i = _zoomLevel; i > zoom; --i)
			pos = PointZoomOut();
	}

	Zoom(_zoomLevel);
	MoveTo(pos);

	return *this;
}

MapLevel& MapLevel::ZoomIn()
{
	Point pos = PointZoomIn();
	Zoom(_zoomLevel);
	MoveTo(pos);
	return *this;
}

MapLevel& MapLevel::ZoomOut()
{
	Point pos = PointZoomOut();
	Zoom(_zoomLevel);
	MoveTo(pos);
	return *this;
}

Point MapLevel::SceneToView(const Point& p)
{
	if (!GetZoomCount())
		return p;

	Point res = p - GetPageOffset() + GetPagePos();
	for (int i = GetCurrentZoom() + 1; i < GetZoomCount(); ++i)
		res = Point((double)res.x * GetZoom(i), (double)res.y * GetZoom(i));

	return res;
}

Point MapLevel::ViewToScene(const Point& p)
{
	if (!GetZoomCount())
		return p;

	Point res = p;
	for (int i = GetZoomCount() - 1; i > GetCurrentZoom(); --i)
		res = Point((double)res.x / GetZoom(i), (double)res.y / GetZoom(i));

	return res + GetPageOffset() - GetPagePos();
}

Size MapLevel::SceneToView(const Size& p)
{
	if (!GetZoomCount())
		return p;

	Size res = p;
	for (int i = GetCurrentZoom() + 1; i < GetZoomCount(); ++i)
		res = Size((double)res.cx * GetZoom(i), (double)res.cy * GetZoom(i));

	return res;
}

Size MapLevel::ViewToScene(const Size& p)
{
	if (!GetZoomCount())
		return p;

	Size res = p;
	for (int i = GetZoomCount() - 1; i > GetCurrentZoom(); --i)
		res = Size((double)res.cx / GetZoom(i), (double)res.cx / GetZoom(i));

	return res;
}

Rect MapLevel::SceneToView(const Rect& rt)
{
	return GetZoomCount() > 0 ? Rect(SceneToView(rt.TopLeft()), SceneToView(rt.GetSize())) : rt;
}

Rect MapLevel::ViewToScene(const Rect& rt)
{
	return GetZoomCount() > 0 ? Rect(ViewToScene(rt.TopLeft()), ViewToScene(rt.GetSize())) : rt;
}

Array<Point> MapLevel::SceneToView(const Array<Point>& p)
{
	if (!GetZoomCount())
		return p;
	Array<Point> res;
	for (int i = 0; i < p.GetCount(); ++i)
		res << SceneToView(p[i]);
	return res;
}

Array<Point> MapLevel::ViewToScene(const Array<Point>& p)
{
	if (!GetZoomCount())
		return p;
	Array<Point> res;
	for (int i = 0; i < p.GetCount(); ++i)
		res << ViewToScene(p[i]);
	return res;
}

Vector<Point> MapLevel::SceneToView(const Vector<Point>& p)
{
	if (!GetZoomCount())
		return p;
	Vector<Point> res;
	for (int i = 0; i < p.GetCount(); ++i)
		res << SceneToView(p[i]);
	return res;
}

Vector<Point> MapLevel::ViewToScene(const Vector<Point>& p)
{
	if (!GetZoomCount())
		return p;
	Vector<Point> res;
	for (int i = 0; i < p.GetCount(); ++i)
		res << ViewToScene(p[i]);
	return res;
}

Rect MapLevel::CurrentViewRect()
{
	return SceneToView(CurrentSceneRect());
}

Rect MapLevel::CurrentSceneRect()
{
	return GetPageRect() - GetPagePos();
}
