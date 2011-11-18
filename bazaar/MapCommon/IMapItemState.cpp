#include "IMapItem.h"
#include "IMapRender.h"

void IMapItem::PerformState(dword state, const String& param)
{
	StatesPerformed(state, param);
}

void IMapItem::StateOff(dword state)
{
	for (int i = 0; i < _states.GetCount(); ++i)
		if (_states[i] == state)
		{
			_states.Remove(i);
			StateChanged(state);
			return;
		}
}

void IMapItem::StateOn(dword state)
{
	for (int i = 0; i < _states.GetCount(); ++i)
		if (_states[i] == state)
			return;
	_states.Add(state);
	StateChanged(state);
}

bool IMapItem::IsState(dword state)
{
	for (int i = 0; i < _states.GetCount(); ++i)
		if (_states[i] == state)
			return true;
	return false;
}

void IMapItem::PerformStates(dword state, const String& param)
{
	for (int i = 0; i < _globalItemList.GetCount(); ++i)
		if (_globalItemList[i])
			_globalItemList[i]->PerformState(state, param);
}

void IMapItem::StatesOff(dword state)
{
	for (int i = 0; i < _globalItemList.GetCount(); ++i)
		if (_globalItemList[i])
			_globalItemList[i]->StateOff(state);
}

void IMapItem::StatesOn(dword state)
{
	for (int i = 0; i < _globalItemList.GetCount(); ++i)
		if (_globalItemList[i])
			_globalItemList[i]->StateOn(state);
}

bool IMapItem::IsAnyState(dword state)
{
	for (int i = 0; i < _globalItemList.GetCount(); ++i)
		if (_globalItemList[i])
		{
			if (_globalItemList[i]->IsState(state))
				return true;
		}
	return false;
}

void IMapItem::StatesChanged(dword state)
{
	for (int i = 0; i < _globalItemList.GetCount(); ++i)
		if (_globalItemList[i])
			_globalItemList[i]->StateChanged(state);
}

void IMapItem::StatesPerformed(dword state, const String& param)
{
	for (int i = 0; i < _globalItemList.GetCount(); ++i)
		if (_globalItemList[i])
			_globalItemList[i]->StatePerformed(state, param);
}
