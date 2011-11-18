#include "IMapItem.h"
#include "IMapRender.h"

Vector<Ptr<IMapItem> > IMapItem::_globalItemList;

IMapItem::IMapItem(IMapRender* parent)
{
	IMapItem::RegisterInGlobalList(this);
	StateOn(STATE_SHOWN);

	_topRender = NULL;
	_parent    = parent;

	if (_parent   && _parent->GetTopRender())
		_topRender = _parent->GetTopRender();
}

void IMapItem::RegisterInGlobalList(Ptr<IMapItem> item)
{
	_globalItemList.Add(item);
}

void IMapItem::UnregisterFromGlobalList(Ptr<IMapItem> item)
{
	for (int i = _globalItemList.GetCount() - 1; i >= 0; --i)
		if (_globalItemList[i] == item)
			_globalItemList.Remove(i);
}

double IMapItem::GetOpacity()    { return GetParent()->GetOpacity(); }
dword  IMapItem::GetQuality()    { return GetParent()->GetQuality(); }
Color  IMapItem::GetClearColor() { return GetParent()->GetClearColor(); }
dword  IMapItem::GetRenderType() { return GetParent()->GetRenderType(); }
Rect   IMapItem::GetRenderRect() { return GetParent()->GetRenderRect(); }