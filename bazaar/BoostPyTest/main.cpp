#include "BoostPyTest.h"

BoostPyTest::BoostPyTest()
{
	CtrlLayout(*this, "Boost Test");
	Sizeable().Zoomable();

	InitPyEnv();
}

BoostPyTest::~BoostPyTest()
{
//	Py_Finalize();	
}

void BoostPyTest::ExitHandler()
{
	SetTimeCallback(1000, Breaker());
}

GUI_APP_MAIN
{
	BoostPyTest().Run();
}

