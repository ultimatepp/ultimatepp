#ifndef _CtrlPosTest2_CtrlPosTest2_h
#define _CtrlPosTest2_CtrlPosTest2_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <CtrlPos/CtrlPos.h>

#define LAYOUTFILE <CtrlPosTest2/CtrlPosTest2.lay>
#include <CtrlCore/lay.h>

class CtrlPosTest2 : public TopWindow {
public:
	typedef CtrlPosTest2 CLASSNAME;
	typedef Visiting<Ctrl> V;
	CtrlPosTest2();

	//keep the visiting control separated from the CtrlFinder (aka CtrlPos)
	//so simply visiting this TopWindow is not possible (cause of Ctrl Tree search from bottom)
	
	WithCtrlPosTest2Layout<ParentCtrl> vis;
	CtrlPos hk;
};

#endif

