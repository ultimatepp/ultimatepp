#ifndef _test2_test2_h
#define _test2_test2_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <test2/test2.lay>
#include <CtrlCore/lay.h>



class test2 : public Withtest2Layout<TopWindow> {
public:
	typedef test2 CLASSNAME;
	test2();
};

#endif
