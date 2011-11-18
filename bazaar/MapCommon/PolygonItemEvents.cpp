#include "PolygonItem.h"
#include "IMapRender.h"

void PolygonItem::StatePerformed(dword state, const String& param)
{
	if (state == PERFORM_SEARCH)
	{
		if (param.IsEmpty() || ToLower(GetName() + GetTags()).Find(ToLower(param)) < 0)
			StateOff(STATE_FOUND);
		else
			StateOn (STATE_FOUND);
	}
}

bool PolygonItem::ProcessLeftDown(Point p, dword keyflags)
{
	bool sel = Contains(p);
	
	if (sel)
	{
		_click = GetTopRender()->ViewToScene(p);
		StatesOff(STATE_SELECTED);
		StateOn(STATE_SELECTED);
	}

	return sel || IMapItem::ProcessLeftDown(p, keyflags);
}

bool PolygonItem::ProcessLeftUp(Point p, dword keyflags)
{
	bool sel = Contains(p);

	p = GetTopRender()->ViewToScene(p);

	if (sel && !IsNull(_click))
	{
		if (abs(_click.x - p.x) < 5 && abs(_click.y - p.y) < 5)
			WhenClick(this);
	}

	_click = Null;
	return IMapItem::ProcessLeftUp(p, keyflags);
}
