#include "ExGridCtrl.hpp"

ExGridCtrl::ExGridCtrl()
{
	_Offset = - GD_HDR_HEIGHT / 2;
	_UseKeys = true;
	_Info = "";
	_X = 0;
	_Y = 0;

	WhenLeftClick = THISBACK(OnLeftClick);
}

void ExGridCtrl::Paint(Draw &w)
{
	GridCtrl::Paint(w);

	if (GetCount() == 0)
		w.DrawText(GetSize().cx / 2 - _X, GetSize().cy / 2 - _Y - _Offset,
			_Info, _Font, _Ink);
}

void ExGridCtrl::SetInfo(const String& info, Font font, Color ink)
{
	_Info = info;
	_Font = font;
	_Ink  = ink;
	
	FontInfo fontInfo = _Font.Info();

	Size sz = ::GetTextSize(info, font);

	_X = sz.cx / 2;
	_Y = sz.cy / 2;
}

bool ExGridCtrl::Key(dword key, int count)
{
	if (!_UseKeys && !IsEdit())
		return GridCtrl::Key(key, count);

	bool result;

	if (key == K_UP)
	{
		if (0 < curpos.y || curpos.y <= GetRowCount() + 1)
		{
			EndEdit();
			SetCursor(curpos.y - 1);
			result = GridCtrl::Key(key, count);
			StartEdit();
			return result;
		}
	}

	if (key == K_DOWN)
	{
		if (0 <= curpos.y || curpos.y < GetRowCount())
		{
			EndEdit();
			SetCursor(curpos.y - 1);
			result = GridCtrl::Key(key, count);
			StartEdit();
			return result;
		}
	}

	return GridCtrl::Key(key, count);
}

void ExGridCtrl::LeftDown(Point p, dword keyflags)
{
	if (!(keyflags & K_SHIFT || keyflags & K_CTRL))
		ClearSelection();
	GridCtrl::LeftDown(p, keyflags);
}

void ExGridCtrl::OnLeftClick()
{
	if (!_UseKeys)
		return;
	SetCursor( Point(1, curpos.y - 1) );
	StartEdit();
}
