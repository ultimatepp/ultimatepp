#include "MapLevel.h"

Rect MapLevel::GetRect()
{
	Point p1(INT_MAX, INT_MAX);
	Point p2(INT_MIN, INT_MIN);

	bool found = false;
	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i])
		{
			Rect r = _layers[i]->GetRenderRect();

			if (r.BottomRight().x > p2.x)
				p2.x = r.BottomRight().x;

			if (r.BottomRight().y > p2.y)
				p2.y = r.BottomRight().y;

			if (r.TopLeft().x < p1.x)
				p1.x = r.TopLeft().x;

			if (r.TopLeft().y < p1.y)
				p1.y = r.TopLeft().y;

			found = true;
		}

	return found ? Rect(p1, p2) : Rect(Point(0, 0), Size(0, 0));
}

Point MapLevel::GetPageOffset()
{
	Size  rsz  = GetRenderSize();
	Size  psz  = GetPageSize();
	Point r    = Point(0, 0);

	if (rsz.cx < psz.cx) r.x = (psz.cx - rsz.cx) / 2;
	if (rsz.cy < psz.cy) r.y = (psz.cy - rsz.cy) / 2;

	return r;
}

Rect MapLevel::GetRenderRect()
{
	return GetRect();
}

Rect MapLevel::GetPageRect()
{
	return Rect(_pagePos, GetSize());
}
