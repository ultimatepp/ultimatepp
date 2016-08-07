#include "Core.h"

#ifdef COWORK2

#include "Core.h"

NAMESPACE_UPP

#ifdef _MULTITHREADED

#define LLOG(x)       DLOG(x)
#define LDUMP(x)      DDUMP(x)

#define LHITCOUNT(x) // RHITCOUNT(x)

thread_local bool CoWork::Pool::finlock;
thread_local bool CoWork::is_worker;

CoWork::Pool& CoWork::GetPool()
{
	static CoWork::Pool pool;
	return pool;
}

void CoWork::Pool::Free(MJob& job)
{
	job.link_next[0] = free;
	free = &job;
}

CoWork::Pool::Pool()
{
	ASSERT(!is_worker);

	int nthreads = CPU_Cores() + 2;
	LLOG("CoWork INIT pool: " << nthreads);
	for(int i = 0; i < nthreads; i++)
		threads.Add().Run([=] { is_worker = true; ThreadRun(i); });

	free = NULL;
	for(int i = 0; i < SCHEDULED_MAX; i++)
		Free(slot[i]);
	
	quit = false;
}

CoWork::Pool::~Pool()
{
	ASSERT(!IsWorker());
	LLOG("Quit");
	lock.Enter();
	quit = true;
	lock.Leave();
	for(int i = 0; i < threads.GetCount(); i++) {
		waitforjob.Broadcast();
		threads[i].Wait();
	}
	for(int i = 0; i < SCHEDULED_MAX; i++)
		slot[i].LinkSelf();
	LLOG("Quit ended");
}

void CoWork::FinLock()
{
	Pool::finlock = true;
	GetPool().lock.Enter();
}

void CoWork::Pool::DoJob(MJob& job)
{
	job.UnlinkAll();
	LLOG("DoJob (CoWork " << FormatIntHex(job.work) << ")");
	finlock = false;
	Function<void ()> fn = pick(job.fn);
	Free(job);
	CoWork *work = job.work;
	lock.Leave();
	fn();
	if(!finlock)
		lock.Enter();
	if(!work)
		return;
	if(--work->todo == 0) {
		LLOG("Releasing waitforfinish of (CoWork " << FormatIntHex(work) << ")");
		work->waitforfinish.Signal();
	}
	LLOG("DoJobA, todo: " << work->todo << " (CoWork " << FormatIntHex(work) << ")");
	ASSERT(work->todo >= 0);
	LLOG("Finished, remaining todo " << work->todo);
}

void CoWork::Pool::ThreadRun(int tno)
{
	LLOG("CoWork thread #" << tno << " started");
	Pool& p = GetPool();
	p.lock.Enter();
	for(;;) {
		while(!p.jobs.InList()) {
			LHITCOUNT("CoWork: Parking thread to Wait");
			p.waiting_threads++;
			LLOG("#" << tno << " Waiting for job");
			p.waitforjob.Wait(p.lock);
			LLOG("#" << tno << " Waiting ended");
			p.waiting_threads--;
			if(p.quit) {
				p.lock.Leave();
				return;
			}
		}
		LLOG("#" << tno << " Job acquired");
		LHITCOUNT("CoWork: Running new job");
		p.DoJob(*p.jobs.GetNext());
		LLOG("#" << tno << " Job finished");
	}
	p.lock.Leave();
	LLOG("CoWork thread #" << tno << " finished");
}

void CoWork::Pool::PushJob(Function<void ()>&& fn, CoWork *work)
{
	ASSERT(free);
	MJob& job = *free;
	free = job.link_next[0];
	job.LinkAfter(&jobs);
	if(work)
		job.LinkAfter(&work->jobs, 1);
	job.fn = pick(fn);
	job.work = work;
	LLOG("Adding job");
	if(waiting_threads) {
		LLOG("Releasing thread waiting for job, waiting threads: " << waiting_threads);
		waitforjob.Signal();
	}
}

void CoWork::Start(Function<void ()>&& fn)
{
	Pool& p = GetPool();
	Mutex::Lock __(p.lock);
	while(!p.free) { // this is quite unlikely, so we can be ugly here
		p.lock.Leave();
		Sleep(0);
		p.lock.Enter();
	}
	p.PushJob(pick(fn), NULL);
}

void CoWork::Do(Function<void ()>&& fn)
{
	LHITCOUNT("CoWork: Sheduling callback");
	Pool& p = GetPool();
	p.lock.Enter();
	if(!p.free) {
		LLOG("Stack full: running in the originating thread");
		LHITCOUNT("CoWork: Stack full: Running in originating thread");
		p.lock.Leave();
		fn();
		if(Pool::finlock)
			p.lock.Leave();
		return;
	}
	p.PushJob(pick(fn), this);
	todo++;
	p.lock.Leave();
}

void CoWork::Finish() {
	Pool& p = GetPool();
	p.lock.Enter();
	while(!jobs.IsEmpty(1)) {
		LLOG("Finish: todo: " << todo << " (CoWork " << FormatIntHex(this) << ")");
		p.DoJob(*jobs.GetNext(1));
	}
	while(todo) {
		LLOG("WaitForFinish (CoWork " << FormatIntHex(this) << ")");
		waitforfinish.Wait(p.lock);
	}
	p.lock.Leave();
	LLOG("CoWork " << FormatIntHex(this) << " finished");
}

bool CoWork::IsFinished()
{
	Pool& p = GetPool();
	p.lock.Enter();
	bool b = jobs.IsEmpty(1);
	p.lock.Leave();
	return b;
}

void CoWork::Pipe(int stepi, Function<void ()>&& fn)
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

#endif

