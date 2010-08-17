#include "WorkQueueTest.h"

#define IMAGEFILE <WorkQueueTest/WorkQueueTest.iml>
#include <Draw/iml_source.h>

WorkQueueTest::WorkQueueTest()
{
	CtrlLayout(*this, "Window title");
	phase = 0;
	sl.SetData(50);
}

void WorkQueueTest::Work()
{
	Sleep(1000); //some loooong work to process

	//show results
	Size sz = GetSize();
	(++phase) %= sz.cx;
	Refresh();
}

void WorkQueueTest::LeftDown(Point p, dword keyflags)
{
	//do something, and offload / queue independant work to the workqueue, it will be processed in order
	//but in a worker thread, so main thread can react on user interaction (slider)
	wq & THISBACK(Work);
}

void WorkQueueTest::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(0,0,sz.cx,sz.cy, SColorFace());
	w.DrawText(0, 10, "Multiple Click left to queue work, use slider while processing");
	w.DrawImage(phase, sz.cy/2, IMAGECLASS::icon());
}

GUI_APP_MAIN
{
	WorkQueueTest().Run();
}

