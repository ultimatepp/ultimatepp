#ifndef _test4_test4_h
#define _test4_test4_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <test4/test4.lay>
#include <CtrlCore/lay.h>



class test4 : public Withtest4Layout<TopWindow> {
public:
	typedef test4 CLASSNAME;
	
	StaticRect              img;
	FrameRight<StaticRect>	fra;
	Button					btn;
	
	test4();
};

#endif

