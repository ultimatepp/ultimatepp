#ifndef _WithEnterActionTest_WithEnterActionTest_h
#define _WithEnterActionTest_WithEnterActionTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <WithEnterAction/WithEnterAction.h>

#define LAYOUTFILE <WithEnterActionTest/WithEnterActionTest.lay>
#include <CtrlCore/lay.h>



class WithEnterActionTest : public WithWithEnterActionTestLayout<TopWindow> {
public:
	typedef WithEnterActionTest CLASSNAME;
	WithEnterActionTest();
	
	void Change();
	void Change2();
	
	WithEnterAction<EditString> ed;
};

#endif

