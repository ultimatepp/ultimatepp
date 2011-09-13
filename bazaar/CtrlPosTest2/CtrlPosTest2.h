#ifndef _CtrlPosTest2_CtrlPosTest2_h
#define _CtrlPosTest2_CtrlPosTest2_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <CtrlPos/CtrlPos.h>
#include <AutoScroller/AutoScroller.hpp>

#define LAYOUTFILE <CtrlPosTest2/CtrlPosTest2.lay>
#include <CtrlCore/lay.h>

class CtrlPosTest2 : public WithMainLay<TopWindow> {
public:
	typedef CtrlPosTest2 CLASSNAME;

	CtrlPosTest2();
	virtual void Activate();

	//keep the visiting control separated from the CtrlFinder (aka CtrlPos)
	//so simply visiting this TopWindow is not possible (cause of Ctrl Tree search from bottom)

	AutoScroller<ParentCtrl> sc;	
	WithCtrlPosTest2Layout<ParentCtrl> vis;
	CtrlPos hk;
};

#endif

