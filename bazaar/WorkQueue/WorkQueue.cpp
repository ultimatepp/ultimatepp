#include "WorkQueue.h"

namespace MyWQ
{

#ifdef _MULTITHREADED

#define LLOG(x)   //  LOG(x)
#define LDUMP(x)  //  DUMP(x)

WorkQueue::Pool& WorkQueue::pool()
{
//	static Pool pool;
	return _pool;
}

WorkQueue::Pool::Pool(int threadnr)
	: jobcount(0), lastjob(_jobs.GetPtr())
{
	LLOG("WorkQueue INIT with threadnr " << threadnr);
	if(threadnr < 0)
		threadnr = CPU_Cores() + 2;
	for(int i = 0; i < threadnr; i++)
		threads.Add().Run(THISBACK1(ThreadRun, i));
}

WorkQueue::Pool::~Pool()
{
	LLOG("Quit");
	//MJob job;
	//job.work = NULL;
	lock.Enter();
	//jobs.Clear();
	_jobs.DeleteList();
	jobcount = 0;
	//jobs.Add(job);
	MJob & job = NextFree();
	lock.Leave();
	for(int i = 0; i < threads.GetCount(); i++)
		waitforjob.Release();
	for(int i = 0; i < threads.GetCount(); i++)
		threads[i].Wait();
	LLOG("Quit ended");
}

bool WorkQueue::Pool::DoJob()
{
	MJob * list = _jobs.GetPtr();
	MJob * e = list->GetNext();
	ASSERT(e != list);
	//if(jobs.Top().work == NULL) {
	if(e->work == NULL) {
		LLOG("Quit thread");
		return true;
	}
	//MJob job = jobs.Pop(); //already done above
	e->Unlink();
	--jobcount;
	if(e == lastjob) 
		lastjob = list; //so we knoe where to insert next
	MJob & job = *e;
	lock.Leave();
	job.cb();
	lock.Enter();
	e->LinkBefore(list); //part of pop
	if(--job.work->todo <= 0) {
		LLOG("Releasing waitforfinish of (WorkQueue " << FormatIntHex(job.work) << ")");
		job.work->waitforfinish.Release(); //multiple call, but semaphore cant become negative anyway
	}
	LLOG("Finished, remaining todo " << job.work->todo << " (WorkQueue " << FormatIntHex(job.work) << ")");
	return false;
}

void WorkQueue::Pool::ThreadRun(int tno)
{
	LLOG("WorkQueue thread #" << tno << " started");
	lock.Enter();
	for(;;) {
//		while(jobs.GetCount() <= 0) {
		while(jobcount <= 0) {
			waiting_threads++;
			lock.Leave();
			LLOG("#" << tno << " Waiting for job");
			waitforjob.Wait();
			LLOG("#" << tno << " Waiting ended");
			lock.Enter();
		}
		LLOG("#" << tno << " Job acquired");
		if(DoJob())
			break;
		LLOG("#" << tno << " Job finished");
	}
	lock.Leave();
	LLOG("WorkQueue thread #" << tno << " finished");
}

WorkQueue::MJob & WorkQueue::Pool::NextFree()
{
	MJob *list = _jobs.GetPtr();
	MJob *e = list->GetNext();
	if(e != list && e->work == NULL) //as long as not end
	{
		//found a free one
		e->Unlink();
		e->LinkAfter(lastjob); //list
	}
	else
	{
		//need one more, either end or used
		//e = list->InsertNext();
		e = lastjob->InsertNext();
		lastjob = e;
	}
	++jobcount;
	return *e;
}

void WorkQueue::Do(Callback cb) {
	Pool& p = pool();
	p.lock.Enter();
//	if(p.jobs.GetCount() > 128) {
	if(p.jobcount > 128) {
		LLOG("Stack full: running in the main thread");
		p.lock.Leave();
		cb();
		return;
	}
	//MJob job;
	MJob & job = p.NextFree();	
	job.cb = cb;
	job.work = this;
	//p.jobs.Add(job); //already done
	todo++;
	LLOG("Adding job; todo: " << todo << " (WorkQueue " << FormatIntHex(this) << ")");
	if(p.waiting_threads>0) {
		LLOG("Releasing thread waiting for job: " << p.waiting_threads);
		p.waiting_threads--;
		p.waitforjob.Release();
	}
	p.lock.Leave();
}

void WorkQueue::Finish() {
#ifdef _MULTITHREADED
	Pool &p = pool();
	p.lock.Enter();
	while(todo>0) {
		LLOG("Finish: todo: " << todo << " (WorkQueue " << FormatIntHex(this) << ")");
		if(p.jobcount>0)
			p.DoJob();
		else {
			p.lock.Leave();
			LLOG("WaitForFinish (WorkQueue " << FormatIntHex(this) << ")");
			waitforfinish.Wait();
			p.lock.Enter();
		}
	}
	p.lock.Leave();
	LLOG("WorkQueue finished");
#endif
}

WorkQueue::WorkQueue()
	: _pool(1)
{
	LLOG("*** WorkQueue constructed " << FormatHex(this));
	todo = 0;
}

WorkQueue::~WorkQueue()
{
	Finish();
	LLOG("~~~ WorkQueue destructed");
}

#endif

} //namespace