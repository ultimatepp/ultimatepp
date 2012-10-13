#ifndef _CtrlPosTest_CtrlPosTest_h
#define _CtrlPosTest_CtrlPosTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlPosTest/CtrlPosTest.lay>
#include <CtrlCore/lay.h>

#include <CtrlPos/CtrlPos.h>
#include <AutoScroller/AutoScroller.h>

class CtrlPosTest : public TopWindow {
public:
	typedef CtrlPosTest CLASSNAME;
	CtrlPosTest();

	virtual void Activate();

	void VisitCB();
	void ClearCB();
	void EnableCB();
	void DisableCB();
	void DeepCB();
	void FrameCB();
	void ViewCB();

	void ToInfo(const String& s);
	void OnSelect(Ctrl& c, Point p, dword keyflags);

	AutoScroller<ParentCtrl> sc;

	WithCtrlPosTestLayout<ParentCtrl> vis;
	FrameLeft<WithLeftBarLay<ParentCtrl> > sb;
	CtrlPos hk;

	FrameTop<WithControlLay<StaticRect> > ft;
	EditString es;
	StaticText st;

	FrameBottom<WithBotLay<ParentCtrl> > bot;
};

#endif

