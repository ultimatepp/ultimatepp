#ifndef _menubug_menubug_h
#define _menubug_menubug_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class MenuBug : public TopWindow {
public:
	typedef MenuBug CLASSNAME;
	MenuBug();

	virtual void RightDown(Point pt, dword keyflags);
};

#endif
