#include "IMapRender.h"

bool IMapRender::ProcessLeftDouble(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessLeftDouble(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessLeftDown(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessLeftDown(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessLeftDrag(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessLeftDrag(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessLeftUp(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessLeftUp(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessRightDouble(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessRightDouble(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessRightDown(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessRightDown(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessRightDrag(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessRightDrag(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessRightUp(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessRightUp(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessMouseMove(Point p, dword keyflags)
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessMouseMove(p, keyflags))
				return true;
		}

	return false;
}

bool IMapRender::ProcessMouseEnter()
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessMouseEnter())
				return true;
		}

	return false;
}

bool IMapRender::ProcessMouseLeave()
{
	for (int i = 0; i < _items.GetCount(); ++i)
		if (_items[i] && _items[i]->IsShown())
		{
			if (_items[i]->ProcessMouseLeave())
				return true;
		}

	return false;
}
