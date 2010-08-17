#ifndef _WorkQueueTest_WorkQueueTest_h
#define _WorkQueueTest_WorkQueueTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <WorkQueueTest/WorkQueueTest.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <WorkQueueTest/WorkQueueTest.iml>
#include <Draw/iml_header.h>

#include <CoWork/CoWork.h>

class WorkQueueTest : public WithWorkQueueTestLayout<TopWindow> {
public:
	typedef WorkQueueTest CLASSNAME;
	WorkQueueTest();
	
	void Work();
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keyflags);

	virtual void Close() 
	{
		//PostCallback(THISBACK(DoClose)); //results in deadlock, since PostCallback stuff is executed in GuiLock
											//so the the worqueue cant complete. it's trying to do
											//Refresh stuff in GuiLock environment, but cant, 
											//since the Postcallback is executing, and waiting for the thread to complete
											//classical deadlock, mismatched order.. but is same with CoWork
		Thread::Start(THISBACK(DoClose)); 	//so schedule Finish Wait in GuiLock free env.
	}
	void DoClose() 
	{
		wq.Finish(); 
		TopWindow::Close(); 
	}
	
	MyCoWork::WorkQueue wq;
	int phase;
};

#endif

