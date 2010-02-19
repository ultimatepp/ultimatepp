#include "TimerTest.h"

#define REPEAT_TEST

TimerTest::TimerTest()
{
	CtrlLayout(*this, "Window title");
#ifdef REPEAT_TEST
	t.SetTimeCallback(-1000, THISBACK(Test), 0);
#else
	PostCallback(THISBACK(Test));
#endif
}

void TimerTest::Info(const String & s)
{
	info.Insert(info.GetLength(), s);
	info.SetCursor(info.GetLength());	
}

void TimerTest::Test()
{
	Info("O");
#ifdef REPEAT_TEST
#else
	t.SetTimeCallback(1000, THISBACK(Test), 0);
#endif
}

GUI_APP_MAIN
{
	TimerTest().Run();
}

