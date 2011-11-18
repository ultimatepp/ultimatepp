#ifndef FORM_EDITOR_COMMON__MENU_BAR_EX_H
#define FORM_EDITOR_COMMON__MENU_BAR_EX_H

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class MenuBarEx : public MenuBar
{
	class MenuHeader : public Ctrl
	{
		String _Title;
		int _CY;
	
	public:
		MenuHeader() : _CY(0) {}
		MenuHeader& Title(const char* title) { _Title = String(title); return *this; }
		MenuHeader& CY(int cy) { _CY = cy; return *this; }
		virtual void Paint(Draw& w)
		{
			Rect r = GetRect();
			w.DrawRect(r, LtGray());
			w.DrawText(5, r.top + _CY / 2 - StdFont().GetHeight() / 2, _Title,
				StdFont(), Blue());
		}
	};

	Array<MenuHeader> _Headers;

public:
	MenuBarEx& AddMenuHeader(const char* title);
};

#endif
