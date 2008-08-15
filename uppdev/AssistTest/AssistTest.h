#ifndef _AssistTest_AssistTest_h
#define _AssistTest_AssistTest_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <AssistTest/AssistTest.lay>
#include <CtrlCore/lay.h>

void Test(::String x);

struct MyApp : TopWindow {
	void Go();
};

class AssistTest : public WithAssistTestLayout<TopWindow> {
public:
	typedef AssistTest CLASSNAME;
	AssistTest();
};

#endif
