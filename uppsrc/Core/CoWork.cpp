#include "Core.h"

NAMESPACE_UPP

#ifdef _MULTITHREADED

#define LLOG(x)      // DLOG(x)
#define LDUMP(x)     // DDUMP(x)

#define LHITCOUNT(x) // RHITCOUNT(x)

thread_local bool CoWork::Pool::finlock;
thread_local bool CoWork::is_worker;
thread_local CoWork::Pool *CoWork::pool;

CoWork::Pool& CoWork::GetPool()
{
	return GetPool(CPU_Cores() + 2);
}

CoWork::Pool& CoWork::GetPool(int nthreads)
{
	if(!pool)
		pool = new Pool(nthreads);
	return *pool;
}

void CoWork::StartPool(int n)
{
	if(!is_worker) {
		ShutdownPool();
		GetPool(n);
	}
}

void CoWork::ShutdownPool()
{
	if(!is_worker && pool) {
		delete pool;
		pool = NULL;
	}
}

CoWork::Pool::Pool(int nthreads)
{
	ASSERT(!is_worker);
	LLOG("CoWork INIT pool: " << nthreads);
	scheduled = 0;
	for(int i = 0; i < nthreads; i++)
		threads.Add().Run([=] { is_worker = true; pool = this; ThreadRun(i); });
}

CoWork::Pool::~Pool()
{
	ASSERT(!IsWorker());
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

void CoWork::FinLock()
{
	Pool::finlock = true;
	GetPool().lock.Enter();
}

bool CoWork::Pool::DoJob()
{
	Pool& p = GetPool();
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
	Pool& p = GetPool();
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
	Pool& p = GetPool();
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

void CoWork::Step(int stepi, Function<void ()>&& fn)
{
	Mutex::Lock __(stepmutex);
	auto& q = step.At(stepi);
	LLOG("Step " << stepi << ", count: " << q.GetCount() << ", running: " << steprunning.GetCount());
	q.AddHead(pick(fn));
	if(!steprunning.At(stepi, false)) {
		steprunning.At(stepi) = true;
		*this & [=]() {
			LLOG("Starting step " << stepi << " processor");
			stepmutex.Enter();
			for(;;) {
				Function<void ()> f;
				auto& q = step[stepi];
				LLOG("StepWork " << stepi << ", todo:" << q.GetCount());
				if(q.GetCount() == 0)
					break;
				f = pick(q.Tail());
				q.DropTail();
				stepmutex.Leave();
				f();
				stepmutex.Enter();
			}
			steprunning.At(stepi) = false;
			stepmutex.Leave();
			LLOG("Exiting step " << stepi << " processor");
		};
	}
}

void CoWork::Finish() {
	Pool& p = GetPool();
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

bool CoWork::IsFinished()
{
	Pool& p = GetPool();
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
