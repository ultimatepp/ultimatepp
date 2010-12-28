#include "CtrlPropTest.h"

bool MyMouseHook(Ctrl *ctrl, bool inframe, int event, Point p,
	                          int zdelta, dword keyflags)
{
	if(event & (Ctrl::MOUSEMOVE | Ctrl::MOUSEENTER | Ctrl::MOUSELEAVE | Ctrl::CURSORIMAGE)) return false;
	if((keyflags & K_MOUSERIGHT))
	if((keyflags & K_SHIFT_CTRL))
	{
		if(!CanEdit()) return false;
		
		CallbackArgTarget<int> m;
		MenuBar menu;
		menu.Add("List Properties",m[0]);
		menu.Add("Edit Properties",m[1]);
		menu.Execute();
		if(IsNull(m)) return true;
		switch(m)
		{
			case 0: DoList(*ctrl); break;
			case 1: DoEdit(*ctrl); break;
		}
		return true;
	}
	return false;
}
