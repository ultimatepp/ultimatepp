#ifndef _AutoScrollerTest_AutoScrollerTest_h
#define _AutoScrollerTest_AutoScrollerTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <AutoScrollerTest/AutoScrollerTest.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <AutoScrollerTest/AutoScrollerTest.iml>
#include <Draw/iml_header.h>

//if use only ParentCtrl as pane
//#include <AutoScroller/AutoScroller.h>
//if use custom panes
#include <AutoScroller/AutoScroller.hpp>

class AutoScrollerTest : public WithAutoScrollerTestLayout<TopWindow> {
public:
	typedef AutoScrollerTest CLASSNAME;
	AutoScrollerTest();
	
	void Toggle();
	
	AutoScroller<WithTestLay<ParentCtrl> > sc;
	//WithTestLay<ParentCtrl> view;
};

#endif

