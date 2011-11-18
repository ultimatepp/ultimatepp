#include "IMapRender.h"

void IMapRender::StateOff(dword state)
{
	IMapItem::StateOff(state);
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i])
			_items[i]->StateOff(state);
}

void IMapRender::StateOn (dword state)
{
	IMapItem::StateOn(state);
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i])
			_items[i]->StateOn(state);
}

bool IMapRender::IsState (dword state)
{
	return IMapItem::IsState(state);
}

void IMapRender::PerformState(dword state, const String& param)
{
	IMapItem::PerformState(state, param);
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i])
			_items[i]->PerformState(state, param);
}
