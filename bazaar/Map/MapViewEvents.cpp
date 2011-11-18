#include "MapView.h"

void MapView::Layout()
{
	if (GetCurrentLevel())
		GetCurrentLevel()->Refresh();
	TopWindow::Layout();
}

void MapView::LeftDouble(Point p, dword keyflags)
{
	if (GetCurrentLevel())
		GetCurrentLevel()->ProcessLeftDouble(p, keyflags);
}

void MapView::LeftDown(Point p, dword keyflags)
{
	if (GetCurrentLevel())
		GetCurrentLevel()->ProcessLeftDown(p, keyflags);

	_mousePos = p;
	_move = true;
}

void MapView::LeftDrag(Point p, dword keyflags)
{
	if (GetCurrentLevel() && GetCurrentLevel()->ProcessLeftDrag(p, keyflags))
		return;
	_drag = true;
}

void MapView::LeftUp(Point p, dword keyflags)
{
	if (GetCurrentLevel() && GetCurrentLevel()->ProcessLeftUp(p, keyflags))
		return;
	_move = false;
	_drag = false;
}

void MapView::RightDouble(Point p, dword keyflags)
{
	if (GetCurrentLevel())
		GetCurrentLevel()->ProcessRightDouble(p, keyflags);
}

void MapView::RightDown(Point p, dword keyflags)
{
	if (GetCurrentLevel())
		GetCurrentLevel()->ProcessRightDown(p, keyflags);
}

void MapView::RightDrag(Point p, dword keyflags)
{
	if (GetCurrentLevel() && GetCurrentLevel()->ProcessRightDrag(p, keyflags))
		return;
}

void MapView::RightUp(Point p, dword keyflags)
{
	if (GetCurrentLevel() && GetCurrentLevel()->ProcessRightUp(p, keyflags))
		return;
}

void MapView::MouseMove(Point p, dword keyflags)
{
	MapLevel* level = GetCurrentLevel();
	if (!level) return;

	if (level->ProcessMouseMove(p, keyflags))
		return;

	if (_move)
	{
		level->MoveBy(_mousePos - p);
		_mousePos = p;
	}
}

void MapView::MouseEnter()
{
	_move = false;
	_drag = false;
}

void MapView::MouseLeave()
{
	_move = false;
	_drag = false;
}
