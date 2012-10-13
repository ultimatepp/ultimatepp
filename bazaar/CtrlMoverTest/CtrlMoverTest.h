#ifndef _CtrlMoverTest_CtrlMoverTest_h
#define _CtrlMoverTest_CtrlMoverTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlMoverTest/CtrlMoverTest.lay>
#include <CtrlCore/lay.h>

#include <CtrlMover/CtrlMover.h>
#include <AutoScroller/AutoScroller.h>

class CtrlMoverTest : public TopWindow {
public:
	typedef CtrlMoverTest CLASSNAME;
	CtrlMoverTest();

	void VisitCB();
	void ClearCB();
	void EnableCB();
	void DisableCB();
	void DeepCB();
	void FrameCB();
	void ViewCB();

	void ToInfo(const String& s);
	void OnSelect(Ctrl& c, Point p, dword keyflags);
	void OnSelectMulti(const Vector<Ctrl*>* pctrls, Rect r, dword keyflags);

	AutoScroller<ParentCtrl> sc;

	WithCtrlMoverTestLayout<ParentCtrl> vis;
	FrameLeft<WithLeftBarLay<ParentCtrl> > sb;
	CtrlMover hk;

	FrameTop<WithControlLay<StaticRect> > ft;
	EditString es;
	
	FrameBottom<WithBotLay<ParentCtrl> > bot;
};

#endif

