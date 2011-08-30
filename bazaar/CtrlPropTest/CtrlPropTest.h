#ifndef _CtrlPropTest_CtrlPropTest_h
#define _CtrlPropTest_CtrlPropTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlPropTest/CtrlPropTest.lay>
#include <CtrlCore/lay.h>

#include <CtrlMover/CtrlMover.h>
#include <CtrlProp/CtrlProp.h>

class CtrlPropTest : public TopWindow {
public:
	typedef CtrlPropTest CLASSNAME;
	CtrlPropTest();

	//misc
	void Test();
	void InitDummies();

	//edit
	void DoList(Ctrl& c);
	void DoEdit(Ctrl& c);
	bool CanEdit();

	void OnEdit();
	void OnCtrlRight(Ctrl& c, Point p, dword keyflags);	
	void OnMoveAction();

public:
	ToolBar mb;
	ToolBar::Item * mbi;
	Array<Ctrl> arc;

	PropEdit pe;
	PropList pl;

	WithCtrlPropTestLayout<ParentCtrl> vis;
	CtrlMover hk;
	
	bool edit;
};

#endif

