#ifndef MAP_COMMON__POLYGON_ITEM_H
#define MAP_COMMON__POLYGON_ITEM_H

#include "IMapRender.h"
#include <Map/MapSerialize.h>

class PolygonItemDisplay;

class PolygonItem : public IMapItem, public Room
{
public:
	PolygonItem(IMapRender* render);
	virtual ~PolygonItem();

	virtual void  Render(Draw* w);

	virtual bool  ProcessLeftDown(Point p, dword keyflags);
	virtual bool  ProcessLeftUp(Point p, dword keyflags);

	virtual void  StatePerformed (dword state, const String& param = String());
	virtual bool  Contains(const Point& p);
	virtual Point GetTopLeft();
	virtual Point GetCenter();
	virtual Rect  GetRect();

	virtual String GetItemType() { return "PolygonItem"; }

public:
	static Callback1<Ptr<PolygonItem> > WhenClick;
	PolygonItem* SetDisplay(PolygonItemDisplay* disp);

private:
	Point _click;
	bool _stdDisplay;
	PolygonItemDisplay* _display;
};

class PolygonItemDisplay
{
public:
	virtual ~PolygonItemDisplay() {}
	virtual void Paint(Draw* w, PolygonItem* item) const;
};

#endif
