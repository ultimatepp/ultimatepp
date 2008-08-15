#ifndef _VisualTest_VisualTest_h
#define _VisualTest_VisualTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <VisualTest/VisualTest.lay>
#include <CtrlCore/lay.h>



class VisualTest : public WithVisualTestLayout<TopWindow> {
public:
	void PI() { pi.Set(~slider, 0); vpi.Set(~slider, 100); }

	typedef VisualTest CLASSNAME;
	VisualTest();
};

#endif
