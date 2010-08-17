#include "WorkQueueTest.h"

#define IMAGEFILE <WorkQueueTest/WorkQueueTest.iml>
#include <Draw/iml_source.h>

WorkQueueTest::WorkQueueTest()
{
	CtrlLayout(*this, "Window title");
	
	self <<= THISBACK(WorkSelf);
	workq <<= THISBACK(WorkQ);
	
	inf.Insert(0, "Cant use slider when Work Self, but can use it when offloading to workqueue");
	sl.SetData(50);
}

void WorkQueueTest::WorkSelf()
{
	inf.Clear();
	for(int i = 0; i < 100; i++)
		Work(i);
}

void WorkQueueTest::WorkQ()
{
	inf.Clear();
	for(int i = 0; i < 100; i++)
		wq & THISBACK1(Work, i);	
}

void WorkQueueTest::Work(int i)
{
	Sleep(Random()%200); //some loong work to process

	//show results
	GuiLock __;
	inf.Insert(inf.GetCursor(), String().Cat() << "-|" << i << "|-");
	inf.SetCursor(inf.GetLength());
}

GUI_APP_MAIN
{
	WorkQueueTest().Run();
}

