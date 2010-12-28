#ifndef _RectCtrlTest_RectCtrlTest_h
#define _RectCtrlTest_RectCtrlTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <RectCtrl/RectCtrl.h>

#define LAYOUTFILE <RectCtrlTest/RectCtrlTest.lay>
#include <CtrlCore/lay.h>

class RectCtrlTest : public WithRectCtrlTestLayout<TopWindow> {
public:
	typedef RectCtrlTest CLASSNAME;
	RectCtrlTest();
};

#endif

