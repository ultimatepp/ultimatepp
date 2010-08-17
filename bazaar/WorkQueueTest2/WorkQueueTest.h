#ifndef _WorkQueueTest_WorkQueueTest_h
#define _WorkQueueTest_WorkQueueTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <WorkQueueTest2/WorkQueueTest.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <WorkQueueTest2/WorkQueueTest.iml>
#include <Draw/iml_header.h>

#include <WorkQueue/WorkQueue.h>

class WorkQueueTest : public WithWorkQueueTestLayout<TopWindow> {
public:
	typedef WorkQueueTest CLASSNAME;
	WorkQueueTest();
	
	void Work(int i);
	void WorkSelf();
	void WorkQ();

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
	
	MyWQ::WorkQueue wq;
};

#endif

