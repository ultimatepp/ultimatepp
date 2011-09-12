#ifndef _AutoScrollerTest_AutoScrollerTest_h
#define _AutoScrollerTest_AutoScrollerTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <AutoScrollerTest/AutoScrollerTest.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <AutoScrollerTest/AutoScrollerTest.iml>
#include <Draw/iml_header.h>

//if use only ParentCtrl as base
//#include <AutoScroller/AutoScroller.h>
//if use custom base
#include <AutoScroller/AutoScroller.hpp>

class AutoScrollerTest : public WithAutoScrollerTestLayout<TopWindow> {
public:
	typedef AutoScrollerTest CLASSNAME;
	AutoScrollerTest();
	
	void Toggle();

	AutoScroller<ParentCtrl> sc;	
	WithTestLay<ParentCtrl> view;
};

#endif

