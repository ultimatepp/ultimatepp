#ifndef _TimerTest_TimerTest_h
#define _TimerTest_TimerTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <TimerTest/TimerTest.lay>
#include <CtrlCore/lay.h>

#include <Timer/Timer.h>

class TimerTest : public WithLayout<TopWindow> {
public:
	typedef TimerTest CLASSNAME;
	TimerTest();
	
	void Info(const String & s);	
	void Test();
private:
	Timer t;
};

#endif

