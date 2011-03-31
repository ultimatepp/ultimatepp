#include "LogCtrlTest.h"

LogCtrlTest::LogCtrlTest()
{
	CtrlLayout(*this, "Window title");
	
	Add(log.HSizePos().VSizePos(0,20));
	clear <<= THISBACK(Clear);
	save <<= THISBACK(Save);

	//SetTimeCallback(-500, callback(&log, &LogCtrl::Flush));

	log.Log();
	LOG("This text comes from the LOG macro");
	
	log.Log(false);
	LOG("This should go to the previous Log stream, whatever it is");
	
	log.Reset();
	LOG("And this should go to the StdLog in any case");

	///	
	log.Log();
	RLOG("Using the log levels:");

	ERR("Error message");
	INFO("Hallo, info");
	WARN("A Warning");
	
	LogLev(LERR);
	
	ERR("Another Error Message");
	INFO("Another info");
	WARN("Another Warning");
	
	ONLL(1)
	{
		int a = 0; //do log level dependant stuff	
	}
}

GUI_APP_MAIN
{
	LogCtrlTest().Run();
}

