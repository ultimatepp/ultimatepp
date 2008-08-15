#ifndef _LTest_LTest_h
#define _LTest_LTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <LTest/LTest.lay>
#include <CtrlCore/lay.h>



class LTest : public WithLTestLayout<TopWindow> {
public:
	typedef LTest CLASSNAME;
	LTest();
};

#endif

