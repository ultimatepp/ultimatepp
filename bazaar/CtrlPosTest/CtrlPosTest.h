#ifndef _CtrlPosTest_CtrlPosTest_h
#define _CtrlPosTest_CtrlPosTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlPosTest/CtrlPosTest.lay>
#include <CtrlCore/lay.h>

#include <CtrlPos/CtrlPos.h>

class CtrlPosTest : public TopWindow {
public:
	typedef CtrlPosTest CLASSNAME;
	CtrlPosTest();

	void VisitCB();
	void ClearCB();
	void EnableCB();
	void DisableCB();
	void DeepCB();
	void IgnoreFrameCB();

	void ToInfo(const String& s);
	void OnSelect(Ctrl& c, Point p, dword keyflags);
	void OnMissed(Point p, dword keyflags);

	WithCtrlPosTestLayout<ParentCtrl> vis;
	FrameLeft<WithLeftBarLay<ParentCtrl> > sb;
	CtrlPos hk;

	FrameTop<WithControlLay<ParentCtrl> > ft;
	EditString es;
};

#endif

