#ifndef _PointCtrlTest_PointCtrlTest_h
#define _PointCtrlTest_PointCtrlTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <PointCtrl/PointCtrl.h>

#define LAYOUTFILE <PointCtrlTest/PointCtrlTest.lay>
#include <CtrlCore/lay.h>

class PointCtrlTest : public WithPointCtrlTestLayout<TopWindow> {
public:
	typedef PointCtrlTest CLASSNAME;
	PointCtrlTest();

	void ToInfo(const String& s) { info.Insert(info.GetLength(), s + "\n"); info.SetCursor(info.GetLength()); }
	void PosCB();
	void SlXCB();
	void SlYCB();
	
	void OnAddPoint();
	void OnRemovePoint(int i);
	void OnMovePoint(int i);
};

#endif

