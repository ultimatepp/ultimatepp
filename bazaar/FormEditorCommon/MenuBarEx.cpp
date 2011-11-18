#include "MenuBarEx.h"

MenuBarEx& MenuBarEx::AddMenuHeader(const char* title)
{
	MenuHeader& r = _Headers.Add().Title(title).CY(16);
	if (_Headers.GetCount() == 1)
	{
		AddMenu(r, 100, 20);
		r.TopPos(0, 20).HSizePos();
	}
	else
	{
		AddMenu(r, 100, 30);
		r.TopPos(5, 25).HSizePos();
	}
	return *this;
}
