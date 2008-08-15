#ifndef _aggtest_aggtest_h
#define _aggtest_aggtest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <aggtest/aggtest.lay>
#include <CtrlCore/lay.h>


int x = 30, y = 30;

class aggtest : public WithaggtestLayout<TopWindow> {
public:
	typedef aggtest CLASSNAME;
	aggtest();
	
		virtual void Paint(Draw& draw);

	void MouseMove(Point p, dword keyflags) { x = p.x; y = p.y;
	Refresh(); }
};

#endif

