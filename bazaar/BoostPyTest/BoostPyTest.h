#ifndef _BoostPyTest_BoostPyTest_h_
#define _BoostPyTest_BoostPyTest_h_

//py stuff needs go first
#include "PyEx.h"

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define LAYOUTFILE <BoostPyTest/BoostPyTest.lay>
#include <CtrlCore/lay.h>

class BoostPyTest : public WithBoostPyTestLayout<TopWindow> {
public:
	typedef BoostPyTest CLASSNAME;
	BoostPyTest();
	~BoostPyTest();

	void InitPyEnv();
	void ExitHandler();
	void CBi(int i);
	void EvalCB();
	void EvalPyCv();

	boost::python::object main_namespace;

#if PUREVIRTEST
#else
	World w;
#endif
	Progress pr;
	
	Callback1<int> cbi;
	
	PyEvalConvert pycv;
};

#endif
