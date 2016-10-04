#ifndef _Test_Test_h
#define _Test_Test_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Test/Test.lay>
#include <CtrlCore/lay.h>



class Test : public WithTestLayout<TopWindow> {
public:
	typedef Test CLASSNAME;
	Test();
};

#endif

