#include "TimerTest.h"

#define REPEAT_TEST
#define TIMEOUT 1000

TimerTest::TimerTest()
	: demo(0)
{
	CtrlLayout(*this, "Window title");

	int a = AtomicInc(demo);
	ASSERT(a==1); //preload, 1 means running demo mode, 2 means a cb is executing

#ifdef REPEAT_TEST
	t.SetTimeCallback(-TIMEOUT, THISBACK(Test), (int)this);
#else
	t.SetTimeCallback(0, THISBACK(Test), (int)this);
#endif
}

void TimerTest::Info(const String & s)
{
	info.Insert(info.GetLength(), s);
	info.SetCursor(info.GetLength());	
}

void TimerTest::Test()
{
	int a = AtomicInc(demo);
	ASSERT(a==2);

	Info("O");
	RLOG("O");

	a = AtomicDec(demo);
	if(a<=0)
		return;
	ASSERT(a>=0);
	
#ifdef REPEAT_TEST
#else
	t.SetTimeCallback(TIMEOUT, THISBACK(Test), (int)this);
#endif
}

void TimerTest::Close()
{
	t.KillTimeCallback((int)this);
	int a = AtomicDec(demo);
	Thread::Start(THISBACK(Close0));	
}

void TimerTest::Close0()
{
	while(AtomicRead(demo) > 0) Sleep(1);
	ASSERT(AtomicRead(demo)==0);
	TopWindow::Close();
}

TimerTest::~TimerTest()
{
}

GUI_APP_MAIN
{
	TimerTest().Run();
}

