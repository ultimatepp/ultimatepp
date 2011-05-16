#ifndef _BoostPyTest_BoostPyTest_h_
#define _BoostPyTest_BoostPyTest_h_

#include <PyConsoleCtrl/PyConsoleCtrl.h>
using namespace boost::python;

#include "world.h"
#include <CtrlLibBoostPy/CtrlLibBoostPy.h>
using namespace Upp;

#define LAYOUTFILE <BoostPyTest/BoostPyTest.lay>
#include <CtrlCore/lay.h>

class BoostPyTest : public WithBoostPyTestLayout<TopWindow> {
public:
	typedef BoostPyTest CLASSNAME;
	BoostPyTest();
	~BoostPyTest();

	void ExitHandler();
	void CBi(int i);
	void EvalCB();

	object main_namespace;

#if PUREVIRTEST
#else
	World w;
#endif
	Progress pr;
	
	Callback1<int> cbi;
};

#endif
