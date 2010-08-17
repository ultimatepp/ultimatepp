#include "CoWork.h"

namespace MyCoWork
{

#ifdef _MULTITHREADED

#define LLOG(x)   //  LOG(x)
#define LDUMP(x)  //  DUMP(x)

CoWork::Pool& CoWork::pool()
{
	static Pool pool;
	return pool;
}

void CoWork::Pool::AddThread(int count) 
{
	lock.Enter();
	for(int i = 0; i < count; i++)
	{
		Thread * pt = new Thread();
		unsigned key = GetPtrHashValue(pt);
		threads.Add(key, pt).Run(THISBACK1(ThreadRun, key));
	}
	lock.Leave();
}

void CoWork::Pool::KillThread(int count, bool waitone)
{
	MJob job;
	job.work = NULL;
	bool finishall = false;

	lock.Enter();
	if(count < 0)
	{
		finishall = true;
		jobs.Clear();
		count = threads.GetCount();
	}
	count = min(count, threads.GetCount()); //clip
		
	for(int i = 0; i < count; i++)
	{
		jobs.Add(job);
		waitforjob.Release(); //threads will consume their Thread instances
	}
	lock.Leave();
	
	//wait only when finishing
	//scan the threads to find quited ones, if none available, handle over
	if(waitone || finishall)
	{
		Sleep(1); //give chance for some to have finished, the waiting at least
		lock.Enter();
		int i = 0;
		while(count > 0)
		{
			if(threads.GetKey(i) != 0)
			{
				//a used thread, check next
				if(++i >= threads.GetCount())
				{
					i=0;
					lock.Leave();
					Sleep(1); //give chance for some to have finished
					lock.Enter();
				}
				continue;
			}
			
			//a quited thread
			Thread * pt = threads.Detach(i);
			--count;
			if(i >= threads.GetCount()) i=0;

			lock.Leave();
			
			//finish a Thread context
			pt->Wait();
			delete pt;
			pt = NULL;
			
			lock.Enter();
		}
		lock.Leave();
	}
}

CoWork::Pool::Pool(int threadnr)
{
	LLOG("CoWork INIT with threadnr " << threadnr);
	if(threadnr < 0)
		threadnr = CPU_Cores() + 2;
	AddThread(threadnr);
}

CoWork::Pool::~Pool()
{
	LLOG("Quit");
	KillThread(-1); //all
	LLOG("Quit ended");
}

bool CoWork::Pool::DoJob()
{
	MJob job = jobs.Pop();
	if(job.work == NULL) {
		RLOG("Quit thread initiate");
		return true;
	}
	lock.Leave();
	job.cb();
	lock.Enter();
	if(--job.work->todo <= 0) {
		LLOG("Releasing waitforfinish of (CoWork " << FormatIntHex(job.work) << ")");
		job.work->waitforfinish.Release(); //multiple call, but semaphore cant become negative anyway
	}
	LLOG("Finished, remaining todo " << job.work->todo << " (CoWork " << FormatIntHex(job.work) << ")");
	return false;
}

void CoWork::Pool::ThreadRun(unsigned tno)
{
	LLOG("CoWork thread #" << tno << " started");
	lock.Enter();
	for(;;) {
		while(jobs.GetCount() <= 0) {
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
	int id = threads.Find(tno);
	Thread & t = threads[id];
	
	threads.SetKey(id, 0); //mark as quited
	lock.Leave();
	RLOG("!!! CoWork thread #" << tno << " finished");
}

void CoWork::Do(Callback cb) {
	Pool& p = _pool;
	p.lock.Enter();
	if(p.jobs.GetCount() > 128) {
		LLOG("Stack full: running in the main thread");
		p.lock.Leave();
		cb();
		return;
	}
	MJob job;
	job.cb = cb;
	job.work = this;
	p.jobs.Add(job);
	todo++;
	LLOG("Adding job; todo: " << todo << " (CoWork " << FormatIntHex(this) << ")");
	if(p.waiting_threads>0) {
		LLOG("Releasing thread waiting for job: " << p.waiting_threads);
		p.waiting_threads--;
		p.waitforjob.Release();
	}
	p.lock.Leave();
}

void CoWork::Finish() {
#ifdef _MULTITHREADED
	Pool &p = _pool;
	p.lock.Enter();
	while(todo>0) {
		LLOG("Finish: todo: " << todo << " (CoWork " << FormatIntHex(this) << ")");
		if(p.jobs.GetCount()>0)
			p.DoJob();
		else {
			p.lock.Leave();
			LLOG("WaitForFinish (CoWork " << FormatIntHex(this) << ")");
			waitforfinish.Wait();
			p.lock.Enter();
		}
	}
	p.lock.Leave();
	LLOG("CoWork finished");
#endif
}

CoWork::CoWork(int threadnr)
	: _pool((threadnr<0)?(pool()):(	_opool.Attach(new Pool(threadnr)),_opool.operator*()))
{
	LLOG("*** CoWork constructed " << FormatHex(this));
	todo = 0;
}

CoWork::~CoWork()
{
	Finish();
	LLOG("~~~ CoWork destructed");
}

#endif

} //namespace
