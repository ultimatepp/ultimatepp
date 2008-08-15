#ifndef _test_test_h
#define _test_test_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <test/test.lay>
#include <CtrlCore/lay.h>



class test : public WithtestLayout<TopWindow> {
public:
	typedef test CLASSNAME;
	test();
};

#endif
