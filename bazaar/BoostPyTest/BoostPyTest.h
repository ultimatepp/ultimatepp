#ifndef _BoostPyTest_BoostPyTest_h_
#define _BoostPyTest_BoostPyTest_h_

#include <PyConsoleCtrl/PyConsoleCtrl.h>
using namespace boost::python;

#include "world.h"
#include "PyConsoleCtrl.h"
#include <CtrlLibBoostPy/CtrlLibBoostPy.h>
using namespace Upp;

NAMESPACE_UPP

decl_export_Callback1(int);
decl_export_Gate1(int);
decl_export_Callback1(Value);
decl_export_Gate1(Value);

decl_export_Callback2(int, double);
decl_export_Gate2(int, double);

decl_export_Callback3(int, double, bool);
decl_export_Gate3(int, double, bool);

decl_export_Callback4(int, double, bool, int64);
decl_export_Gate4(int, double, bool, int64);

END_UPP_NAMESPACE

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
	void EvalPyCv();

	object main_namespace;

#if PUREVIRTEST
#else
	World w;
#endif
	Progress pr;
	
	Callback1<int> cbi;
	
	PyEvalConvert pycv;
};

#endif
