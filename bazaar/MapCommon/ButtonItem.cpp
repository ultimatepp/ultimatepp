#include "ButtonItem.h"
#include "IMapRender.h"

ButtonItem::ButtonItem(IMapRender* render)
	: IMapItem(render), _pos(Null)
{}

Rect ButtonItem::GetRect()
{
	return Rect();
}

Point ButtonItem::GetCenter()
{
	return GetRect().CenterPoint();
}

bool ButtonItem::Contains(const Point& p)
{
	if (!GetParent() || !GetTopRender())
		return false;
	Point pos = GetTopRender()->ViewToScene(_pos == Null ? GetParent()->GetTopLeft() : _pos);
	Rect r = Rect(Point(pos.x - _icon.GetSize().cx / 2 - 5, pos.y - _icon.GetSize().cy / 2 - 5),
		_icon.GetSize() + Size(10, 10));
	return r.Contains(GetTopRender()->ViewToScene(p));
}

void ButtonItem::Render(Draw* w)
{
	if (!GetParent() || !GetTopRender())
		return;
	if (!GetParent()->IsState(STATE_SELECTED))
		return;
	LOG("Render");
	Point p = GetTopRender()->ViewToScene(_pos == Null ? GetParent()->GetTopLeft() : _pos);
	w->DrawImage(p.x - _icon.GetSize().cx / 2, p.y - _icon.GetSize().cy / 2, _icon);
}

ButtonItem* ButtonItem::SetIcon(const Image& icon)
{
	_icon = icon;
	GetTopRender()->RefreshView();
	return this;
}

ButtonItem* ButtonItem::SetPos(const Point& pos)
{
	_pos = pos;
	GetTopRender()->RefreshView();
	return this;
}
