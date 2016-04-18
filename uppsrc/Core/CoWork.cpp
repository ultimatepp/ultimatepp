#include "Core.h"

NAMESPACE_UPP

#ifdef _MULTITHREADED

#define LLOG(x)      // DLOG(x)
#define LDUMP(x)     // DDUMP(x)

#define LHITCOUNT(x) // RHITCOUNT(x)

CoWork::Pool& CoWork::pool()
{
	static Pool pool;
	return pool;
}

CoWork::Pool::Pool()
{
	LLOG("CoWork INIT pool: " << CPU_Cores() + 2);
	scheduled = 0;
	for(int i = 0; i < CPU_Cores() + 2; i++)
		threads.Add().Run([=] { ThreadRun(i); });
}

CoWork::Pool::~Pool()
{
	LLOG("Quit");
	lock.Enter();
	jobs[0].work = NULL;
	scheduled = 1;
	lock.Leave();
	for(int i = 0; i < threads.GetCount(); i++)
		waitforjob.Release();
	for(int i = 0; i < threads.GetCount(); i++)
		threads[i].Wait();
	LLOG("Quit ended");
}

thread__ bool CoWork::Pool::finlock;

void CoWork::FinLock()
{
	Pool::finlock = true;
	pool().lock.Enter();
}

bool CoWork::Pool::DoJob()
{
	Pool& p = pool();
	MJob& job = p.jobs[p.scheduled - 1];
	if(job.work == NULL) {
		LLOG("Quit thread");
		return true;
	}
	LLOG("DoJob " << p.scheduled - 1 << ", todo: " << job.work->todo << " (CoWork " << FormatIntHex(job.work) << ")");
	finlock = false;
	Function<void ()> fn = pick(job.fn);
	CoWork *work = job.work;
	p.scheduled--;
	p.lock.Leave();
	fn();
	if(!finlock)
		p.lock.Enter();
	if(--work->todo == 0) {
		LLOG("Releasing waitforfinish of (CoWork " << FormatIntHex(work) << ")");
		work->waitforfinish.Release();
	}
	LLOG("DoJobA " << p.scheduled << ", todo: " << work->todo << " (CoWork " << FormatIntHex(work) << ")");
	ASSERT(work->todo >= 0);
	LLOG("Finished, remaining todo " << work->todo);
	return false;
}

void CoWork::Pool::ThreadRun(int tno)
{
	LLOG("CoWork thread #" << tno << " started");
	Pool& p = pool();
	p.lock.Enter();
	for(;;) {
		while(p.scheduled == 0) {
			LHITCOUNT("CoWork: Parking thread to Wait");
			p.waiting_threads++;
			p.lock.Leave();
			LLOG("#" << tno << " Waiting for job");
			p.waitforjob.Wait();
			LLOG("#" << tno << " Waiting ended");
			p.lock.Enter();
		}
		LLOG("#" << tno << " Job acquired");
		LHITCOUNT("CoWork: Running new job");
		if(DoJob())
			break;
		LLOG("#" << tno << " Job finished");
	}
	p.lock.Leave();
	LLOG("CoWork thread #" << tno << " finished");
}

void CoWork::Do(Function<void ()>&& fn)
{
	LHITCOUNT("CoWork: Sheduling callback");
	Pool& p = pool();
	p.lock.Enter();
	if(p.scheduled >= SCHEDULED_MAX) {
		LLOG("Stack full: running in the originating thread");
		LHITCOUNT("CoWork: Stack full: Running in originating thread");
		p.lock.Leave();
		fn();
		if(Pool::finlock)
			p.lock.Leave();
		return;
	}
	MJob& job = p.jobs[p.scheduled++];
	job.work = this;
	job.fn = pick(fn);
	todo++;
	LLOG("Adding job " << p.scheduled - 1 << "; todo: " << todo << " (CoWork " << FormatIntHex(this) << ")");
	if(p.waiting_threads) {
		LLOG("Releasing thread waiting for job: " << p.waiting_threads);
		p.waiting_threads--;
		p.waitforjob.Release();
	}
	p.lock.Leave();
}

void CoWork::Finish() {
	Pool& p = pool();
	p.lock.Enter();
	while(todo) {
		LLOG("Finish: todo: " << todo << " (CoWork " << FormatIntHex(this) << ")");
		if(todo == 0)
			break;
		if(p.scheduled)
			Pool::DoJob();
		else {
			p.lock.Leave();
			LLOG("WaitForFinish (CoWork " << FormatIntHex(this) << ")");
			waitforfinish.Wait();
			p.lock.Enter();
		}
	}
	p.lock.Leave();
	LLOG("CoWork " << FormatIntHex(this) << " finished");
}

/*
void CoWork::Pipe(int stepi, const Callback& cb)
{
	Mutex::Lock __(stepmutex);
	step.At(stepi).AddHead(cb);
	if(step.GetCount() == 1) {
		*this & [=]() {
			bool second = false;
			for(;;) {
				Callback cb;
				{
					Mutex::Lock __(stepmutex);
					BiVector<Callback>& q = step[stepi];
					if(second)
						q.DropTail();
					if(q.GetCount() == 0)
						return;
					cb = pick(q.Tail());
					second = true;
				}
				cb();
			}
		}
	}
}
*/

bool CoWork::IsFinished()
{
	Pool& p = pool();
	p.lock.Enter();
	bool b = todo == 0;
	p.lock.Leave();
	return b;
}

CoWork::CoWork()
{
	LLOG("CoWork constructed " << FormatHex(this));
	todo = 0;
}

CoWork::~CoWork()
{
	Finish();
	LLOG("~CoWork " << FormatIntHex(this));
}

#endif

END_UPP_NAMESPACE
