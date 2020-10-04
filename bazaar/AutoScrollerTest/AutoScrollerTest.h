#ifndef _AutoScrollerTest_AutoScrollerTest_h
#define _AutoScrollerTest_AutoScrollerTest_h

#include <CtrlLib/CtrlLib.h>
#include <AutoScroller/AutoScroller.h>

namespace Upp {

#define LAYOUTFILE <AutoScrollerTest/AutoScrollerTest.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <AutoScrollerTest/AutoScrollerTest.iml>
#include <Draw/iml_header.h>

class AutoScrollerTest : public WithAutoScrollerTestLayout<TopWindow> {
public:
	AutoScrollerTest();
	
	void Toggle();

	AutoScroller<ParentCtrl> sc;
	WithTestLay<ParentCtrl> view;
};

}

#endif
