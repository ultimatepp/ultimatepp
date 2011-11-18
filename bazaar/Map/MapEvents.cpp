#include "MapLevel.h"

bool MapLevel::Zoom(int level)
{
	_zoomLevel = level;
	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i])
		{
			if (!_layers[i]->Zoom(level))
				return false;
		}

	return true;
}

bool MapLevel::ProcessLeftDouble(Point p, dword keyflags)
{
	p = SceneToView(p);

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessLeftDouble(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessLeftDouble(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessLeftDown(Point p, dword keyflags)
{
	p = SceneToView(p);

	DUMP(IsEditMode());
	if (IsEditMode())
		_currPoint = GetEditPointIndex(p);

	if (!_highQuality)
		NoAA();

//	StatesOff(STATE_SELECTED);
	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessLeftDown(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessLeftDown(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessLeftDrag(Point p, dword keyflags)
{
	p = SceneToView(p);

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessLeftDrag(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessLeftDrag(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessLeftUp(Point p, dword keyflags)
{
	p = SceneToView(p);

	if (IsEditMode())
		_currPoint = -1;

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessLeftUp(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessLeftUp(p, keyflags);
	if (!_highQuality)
		Subpixel();
	if (r || !_highQuality) Refresh();
	return r;
}

bool MapLevel::ProcessRightDouble(Point p, dword keyflags)
{
	p = SceneToView(p);

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessRightDouble(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessRightDouble(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessRightDown(Point p, dword keyflags)
{
	p = SceneToView(p);

	if (IsEditMode())
	{
		AddEditPoint(p);
		WhenChangePoint();
		Refresh();
	}

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessRightDown(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessRightDown(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessRightDrag(Point p, dword keyflags)
{
	p = SceneToView(p);

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessRightDrag(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessRightDrag(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessRightUp(Point p, dword keyflags)
{
	p = SceneToView(p);

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessRightUp(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessRightUp(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessMouseMove(Point p, dword keyflags)
{
	p = SceneToView(p);

	if (IsEditMode())
	{
		_editModePos = p;

		if (_currPoint != -1)
		{
			SetEditPoint(_currPoint, p);
			Refresh();
			return true;
		}
	}

	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessMouseMove(p, keyflags))
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessMouseMove(p, keyflags);
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessMouseEnter()
{
	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessMouseEnter())
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessMouseEnter();
	if (r) Refresh();
	return r;
}

bool MapLevel::ProcessMouseLeave()
{
	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
		{
			if (_layers[i]->ProcessMouseLeave())
			{
				Refresh();
				return true;
			}
		}

	bool r = IMapRender::ProcessMouseLeave();
	if (!_highQuality)
		Subpixel();
	if (r || !_highQuality) Refresh();
	return r;
}
