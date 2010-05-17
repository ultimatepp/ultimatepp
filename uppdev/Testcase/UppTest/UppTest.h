#ifndef _UppTest_UppTest_h
#define _UppTest_UppTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <UppTest/UppTest.lay>
#include <CtrlCore/lay.h>



class UppTest : public WithUppTestLayout<TopWindow> {
public:
	typedef UppTest CLASSNAME;
	UppTest();
	virtual ~UppTest();
};

#endif

