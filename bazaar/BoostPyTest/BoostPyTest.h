#ifndef _BoostPyTest_BoostPyTest_h_
#define _BoostPyTest_BoostPyTest_h_

#include <PyConsoleCtrl/PyConsoleCtrl.h>
using namespace boost::python;

#include "world.h"
#include "SliderCtrlPy.h"
#include "UppString.h"
#include "UppValue.h"

#define LAYOUTFILE <BoostPyTest/BoostPyTest.lay>
#include <CtrlCore/lay.h>

class BoostPyTest : public WithBoostPyTestLayout<TopWindow> {
public:
	typedef BoostPyTest CLASSNAME;
	BoostPyTest();
	~BoostPyTest();

	void ExitHandler();

	World w;	
	SliderCtrlPy slpy;
};

#endif
