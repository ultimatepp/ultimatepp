#ifndef _CtrlPropTest_CtrlPropTest_h
#define _CtrlPropTest_CtrlPropTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlPropTest/CtrlPropTest.lay>
#include <CtrlCore/lay.h>

#include <CtrlProp/CtrlProp.h>
#include <CtrlFinder/CtrlFinder.h>
#include <RectCtrl/RectCtrl.h>

bool MyMouseHook(Ctrl *ctrl, bool inframe, int event, Point p,
	                          int zdelta, dword keyflags);
void DoList(Ctrl& c);
void DoEdit(Ctrl& c);
bool CanEdit();

class CtrlPropTest : public WithCtrlPropTestLayout<TopWindow> {
public:
	typedef CtrlPropTest CLASSNAME;
	CtrlPropTest();

	//hook
	void EditCB();

	void OnCtrlLeft(Ctrl& c, Point p, dword keyflags);
	void OnCtrlRight(Ctrl& c, Point p, dword keyflags);
	void OnRectChange();
	void OnMissed(Point p, dword keyflags);

	//misc
	void Test();
	void InitDummies();

public:
	ToolBar mb;
	ToolBar::Item * mbi;
	CtrlFinder hk;
	RectCtrl rc;
	RectCtrl::Style rcst;
	Ptr<Ctrl> ctrl;
	
	Array<Ctrl> arc;
};

#endif

