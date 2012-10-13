#ifndef _CtrlFinderTest_CtrlFinderTest_h
#define _CtrlFinderTest_CtrlFinderTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlFinderTest/CtrlFinderTest.lay>
#include <CtrlCore/lay.h>

#include <CtrlFinder/CtrlFinder.h>

class CtrlFinderTest : public TopWindow {
public:
	typedef CtrlFinderTest CLASSNAME;
	CtrlFinderTest();

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

	WithCtrlFinderTestLayout<ParentCtrl> vis;
	FrameLeft<WithLeftBarLay<ParentCtrl> > sb;
	CtrlFinder hk;

	FrameTop<WithControlLay<StaticRect> > ft;
	EditString es;

	FrameBottom<WithBotLay<ParentCtrl> > bot;
};

#endif

