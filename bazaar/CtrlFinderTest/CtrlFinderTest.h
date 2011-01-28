#ifndef _CtrlFinderTest_CtrlFinderTest_h
#define _CtrlFinderTest_CtrlFinderTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlFinderTest/CtrlFinderTest.lay>
#include <CtrlCore/lay.h>

#include <CtrlFinder/CtrlFinder.h>

class CtrlFinderTest : public WithCtrlFinderTestLayout<TopWindow> {
public:
	typedef CtrlFinderTest CLASSNAME;
	CtrlFinderTest();

	void VisitCB();
	void ClearCB();
	void EnableCB();
	void DisableCB();

	void ToInfo(const String& s);
	void OnSelect(Ctrl& c, Point p, dword keyflags);
	void OnMissed(Point p, dword keyflags);
	
	CtrlFinder hk;
	FrameTop<WithControlLay<ParentCtrl> > ft;
};

#endif

