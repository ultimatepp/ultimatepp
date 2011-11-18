#ifndef MAP_COMMON__ICON_ITEM_H
#define MAP_COMMON__ICON_ITEM_H

#include "IMapItem.h"

class ButtonItem : public IMapItem
{
public:
	ButtonItem(IMapRender* render);
	virtual ~ButtonItem() {}

	virtual void  Render(Draw* w);

	virtual bool  ProcessLeftDown(Point p, dword keyflags);

	virtual void  StatePerformed (dword state, const String& param = String());
	virtual bool  Contains(const Point& p);
	virtual Point GetCenter();
	virtual Rect  GetRect();

	virtual String GetItemType() { return "ButtonItem"; }

public:
	virtual ButtonItem* SetIcon(const Image& icon);
	virtual ButtonItem* SetPos(const Point& pos);

private:
	Image _icon;
	Point _pos;
};

#endif
