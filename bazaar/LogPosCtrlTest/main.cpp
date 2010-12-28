#include "LogPosCtrlTest.h"

void LogPosCtrlTest::ChangeLogPosCB()
{
	ac.SetPos(le.Get());	
}

LogPosCtrlTest::LogPosCtrlTest()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();

	Add(le.LeftPos(10, 200).BottomPos(10, 20));
	le <<= THISBACK(ChangeLogPosCB);

	//init
	Ctrl& c = ac;	
	le.Set(c); 
}

GUI_APP_MAIN
{
	LogPosCtrlTest().Run();
}

