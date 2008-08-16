#include "Core.h"

NAMESPACE_UPP

#ifdef _MULTITHREADED

#define LLOG(x)   //  LOG(x)
#define LDUMP(x)  //  DUMP(x)

CoWork::Pool& CoWork::pool()
{
	static Pool pool;
	return pool;
}

CoWork::Pool::Pool()
{
	LLOG("CoWork INIT");
	for(int i = 0; i < CPU_Cores() + 2; i++)
		threads.Add().Run(callback1(&ThreadRun, i));
}

CoWork::Pool::~Pool()
{
	LLOG("Quit");
	MJob job;
	job.work = NULL;
	lock.Enter();
	jobs.Clear();
	jobs.Add(job);
	lock.Leave();
	for(int i = 0; i < threads.GetCount(); i++)
		waitforjob.Release();
	for(int i = 0; i < threads.GetCount(); i++)
		threads[i].Wait();
	LLOG("Quit ended");
}

bool CoWork::Pool::DoJob()
{
	Pool& p = pool();
	if(p.jobs.Top().work == NULL) {
		LLOG("Quit thread");
		return true;
	}
	MJob job = p.jobs.Pop();
	p.lock.Leave();
	job.cb();
	p.lock.Enter();
	if(--job.work->todo == 0) {
		LLOG("Releasing waitforfinish of (CoWork " << FormatIntHex(job.work) << ")");
		job.work->waitforfinish.Release();
	}
	LLOG("Finished, remaining todo " << job.work->todo << " (CoWork " << FormatIntHex(job.work) << ")");
	return false;
}

void CoWork::Pool::ThreadRun(int tno)
{
	LLOG("CoWork thread #" << tno << " started");
	Pool& p = pool();
	p.lock.Enter();
	for(;;) {
		while(p.jobs.GetCount() == 0) {
			p.waiting_threads++;
			p.lock.Leave();
			LLOG("#" << tno << " Waiting for job");
			p.waitforjob.Wait();
			LLOG("#" << tno << " Waiting ended");
			p.lock.Enter();
		}
		LLOG("#" << tno << " Job acquired");
		if(DoJob())
			break;
		LLOG("#" << tno << " Job finished");
	}
	p.lock.Leave();
	LLOG("CoWork thread #" << tno << " finished");
}

void CoWork::Do(Callback cb) {
#ifdef _MULTITHREADED
	Pool& p = pool();
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
	if(p.waiting_threads) {
		LLOG("Releasing thread waiting for job: " << p.waiting_threads);
		p.waiting_threads--;
		p.waitforjob.Release();
	}
	p.lock.Leave();
#else
	cb();
#endif
}

void CoWork::Finish() {
#ifdef _MULTITHREADED
	Pool &p = pool();
	p.lock.Enter();
	while(todo) {
		LLOG("Finish: todo: " << todo << " (CoWork " << FormatIntHex(this) << ")");
		if(todo == 0)
			break;
		if(p.jobs.GetCount())
			Pool::DoJob();
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

CoWork::CoWork()
{
	LLOG("CoWork constructed " << FormatHex(this));
	todo = 0;
}

CoWork::~CoWork()
{
	Finish();
}

#endif

END_UPP_NAMESPACE
