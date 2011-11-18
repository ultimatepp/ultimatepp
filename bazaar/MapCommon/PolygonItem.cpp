#include "PolygonItem.h"
#include "IMapRender.h"

bool IsPointInsidePolygon(const Array<Point>& p, Point s)
{
	int i, j;
	bool parity = false;
	for (i = 0, j = p.GetCount() - 1; i < p.GetCount(); j = i++)
	{
		if ( ( p[i].y < s.y && s.y <= p[j].y || p[j].y < s.y && s.y <= p[i].y ) 
			&& (s.x > (p[j].x - p[i].x) * (s.y - p[i].y) / (p[j].y - p[i].y) + p[i].x))
				parity = !parity;
	}
	return parity;
}

Rect RectOfFigure(const Array<Point>& p)
{
	if (!p.GetCount())
		return Null;

	Point s = Point(INT_MAX, INT_MAX);
	Point e = Point(-1, -1);

	for (int i = 0; i < p.GetCount();  i++)
	{
		s.x = min(p[i].x, s.x);
		s.y = min(p[i].y, s.y);

		e.x = max(p[i].x, e.x);
		e.y = max(p[i].y, e.y);
	}

	return Rect(s, e);
}

Point CenterOfFigure(const Array<Point>& p)
{
	return RectOfFigure(p).CenterPoint();
}

Point TopLeftOfFigure(const Array<Point>& p)
{
	if (!p.GetCount())
		return Null;

	Point s = p[0];

	for (int i = 1; i < p.GetCount();  i++)
	{
		if (s.x > p[i].x && s.y > p[i].y)
			s = p[i];
	}

	return s;
}

PolygonItem::PolygonItem(IMapRender* render)
	: IMapItem(render), _display(new PolygonItemDisplay()), _stdDisplay(true)
{
}

Callback1<Ptr<PolygonItem> > PolygonItem::WhenClick;

PolygonItem::~PolygonItem()
{
	if (_stdDisplay && _display)
		delete _display;
}

PolygonItem* PolygonItem::SetDisplay(PolygonItemDisplay* disp)
{
	if (_stdDisplay && _display)
		delete _display;
	_display = disp;
	return this;
}

Rect PolygonItem::GetRect()
{
	return RectOfFigure(GetVertices());
}

Point PolygonItem::GetCenter()
{
	return CenterOfFigure(GetVertices());
}

Point PolygonItem::GetTopLeft()
{
	return TopLeftOfFigure(GetVertices());
}

bool PolygonItem::Contains(const Point& p)
{
	return IsPointInsidePolygon(GetVertices(), p);
}

void PolygonItem::Render(Draw* w)
{
	_display->Paint(w, this);
}

void PolygonItemDisplay::Paint(Draw* w, PolygonItem* item) const
{
	if (!item->GetTopRender())
		return;

	Vector<Point> points;
	for (int i = 0; i < item->GetVertices().GetCount(); ++i)
		points << item->GetTopRender()->ViewToScene(item->GetVertices()[i]);

	if (item->IsFound())
	{
		w->DrawPolygon(points,
			item->IsSelected()
				? Yellow()
				: LtRed(), 1, Black());
	}

	else if (item->IsSelected() && !item->IsFound())
		w->DrawPolygon(points, Yellow(), 1, Black());

	else if (item->IsState(STATE_EDIT))
		w->DrawPolygon(points, Green(),  1, Black());
}
