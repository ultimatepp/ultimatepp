#include "Core.h"

namespace Upp {

#ifdef _MULTITHREADED

#define LLOG(x)       // RLOG(x)
#define LDUMP(x)      // DDUMP(x)

#define LTIMING(x)    // RTIMING(x)
#define LHITCOUNT(x)  // RHITCOUNT(x)

#ifdef COMPILER_MINGW
FastMingwTls<bool>     CoWork::Pool::finlock;
FastMingwTls<int>      CoWork::worker_index = -1;
FastMingwTls<CoWork *> CoWork::current;
#else
thread_local bool    CoWork::Pool::finlock;
thread_local int     CoWork::worker_index = -1;
thread_local CoWork *CoWork::current;
#endif

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

void CoWork::Pool::InitThreads(int nthreads)
{
	LLOG("Pool::InitThreads: " << nthreads);
	for(int i = 0; i < nthreads; i++)
		CHECK(threads.Add().RunNice([=] { worker_index = i; ThreadRun(i); }, true));
}

void CoWork::Pool::ExitThreads()
{
	lock.Enter();
	quit = true;
	lock.Leave();
	waitforjob.Broadcast();
	for(int i = 0; i < threads.GetCount(); i++)
		threads[i].Wait();
	threads.Clear();
	lock.Enter();
	quit = false;
	lock.Leave();
}

int CoWork::GetPoolSize()
{
	int n = GetPool().threads.GetCount();
	return n ? n : CPU_Cores() + 2;
}

CoWork::Pool::Pool()
{
	ASSERT(!IsWorker());

	InitThreads(CPU_Cores() + 2);

	free = NULL;
	for(int i = 0; i < SCHEDULED_MAX; i++)
		Free(slot[i]);
	
	quit = false;
}

CoWork::Pool::~Pool()
{
	ASSERT(!IsWorker());
	LLOG("Quit");
	ExitThreads();
	for(int i = 0; i < SCHEDULED_MAX; i++)
		slot[i].LinkSelf();
	LLOG("Quit ended");
}

void CoWork::FinLock()
{
	if(current && !Pool::finlock) {
		Pool::finlock = true;
		GetPool().lock.Enter();
	}
}

void CoWork::Pool::DoJob(MJob& job)
{
	LLOG("DoJob (CoWork " << FormatIntHex(job.work) << ")");
	finlock = false;

	CoWork *work = job.work;
	CoWork::current = work;
	bool looper = job.looper;
	Function<void ()> fn;
	if(looper) {
		ASSERT(work);
		if(--work->looper_count <= 0) {
			job.UnlinkAll();
			Free(job);
		}
	}
	else {
		job.UnlinkAll();
		fn = pick(job.fn);
		Free(job); // using 'job' after this point is grave error....
	}

	lock.Leave();
	std::exception_ptr exc = nullptr;
	try {
		if(looper)
			work->looper_fn();
		else
			fn();
	}
	catch(...) {
		LLOG("DoJob caught exception");
		exc = std::current_exception();
	}
	CoWork::current = NULL;
	if(!finlock)
		lock.Enter();
	if(!work)
		return;
	if(exc && !work->exc) {
		work->canceled = true;
		work->Cancel0();
		work->exc = exc;
	}
	else
	if(looper)
		work->Cancel0();
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
			if(p.quit) {
				p.lock.Leave();
				return;
			}
			p.waiting_threads++;
			LLOG("#" << tno << " Waiting for job");
			p.waitforjob.Wait(p.lock);
			LLOG("#" << tno << " Waiting ended");
			p.waiting_threads--;
		}
		LLOG("#" << tno << " Job acquired");
		LHITCOUNT("CoWork: Running new job");
		p.DoJob(*p.jobs.GetNext());
		LLOG("#" << tno << " Job finished");
	}
	p.lock.Leave();
	LLOG("CoWork thread #" << tno << " finished");
}

void CoWork::Pool::PushJob(Function<void ()>&& fn, CoWork *work, bool looper)
{
	ASSERT(free);
	MJob& job = *free;
	free = job.link_next[0];
	job.LinkAfter(&jobs);
	if(work)
		job.LinkAfter(&work->jobs, 1);
	job.work = work;
	job.looper = looper;
	if(looper) {
		work->looper_fn = pick(fn);
		work->looper_count = GetPoolSize();
	}
	else
		job.fn = pick(fn);
	LLOG("Adding job");
	if(looper)
		waitforjob.Broadcast();
	else
	if(waiting_threads) {
		LTIMING("Releasing thread waiting for job");
		LLOG("Releasing thread waiting for job, waiting threads: " << waiting_threads);
		waitforjob.Signal();
	}
}

bool CoWork::TrySchedule(Function<void ()>&& fn)
{
	Pool& p = GetPool();
	Mutex::Lock __(p.lock);
	if(!p.free)
		return false;
	p.PushJob(pick(fn), NULL);
	return true;
}

void CoWork::Schedule(Function<void ()>&& fn)
{
	while(!TrySchedule(pick(fn))) Sleep(0);
}

void CoWork::Do0(Function<void ()>&& fn, bool looper)
{
	LTIMING("Scheduling callback");
	LHITCOUNT("CoWork: Scheduling callback");
	LLOG("Do0, looper: " << looper << ", previous todo: " << todo);
	Pool& p = GetPool();
	p.lock.Enter();
	if(!p.free) {
		LLOG("Stack full: running in the originating thread");
		LHITCOUNT("CoWork: Stack full: Running in originating thread");
		p.lock.Leave();
		Pool::finlock = false;
		fn();
		if(Pool::finlock)
			p.lock.Leave();
		return;
	}
	p.PushJob(pick(fn), this, looper);
	if(looper)
		todo += GetPoolSize();
	else
		++todo;
	p.lock.Leave();
}

void CoWork::Loop(Function<void ()>&& fn)
{
	index = 0;
	Do0(pick(fn), true);
	Finish();
}

void CoWork::Cancel0()
{
	LLOG("CoWork Cancel0");
	Pool& p = GetPool();
	while(!jobs.IsEmpty(1)) {
		LHITCOUNT("CoWork::Canceling scheduled Job");
		MJob& job = *jobs.GetNext(1);
		job.UnlinkAll();
		if(job.looper)
			todo -= job.work->looper_count;
		else
			--todo;
		p.Free(job);
	}
}

void CoWork::Finish0()
{
	Pool& p = GetPool();
	while(todo) {
		LLOG("WaitForFinish (CoWork " << FormatIntHex(this) << ")");
		waitforfinish.Wait(p.lock);
	}
	canceled = false;
	if(exc) {
		LLOG("CoWork rethrowing worker exception");
		auto e = exc;
		exc = nullptr;
		p.lock.Leave();
		std::rethrow_exception(e);
	}
}

void CoWork::Cancel()
{
	Pool& p = GetPool();
	p.lock.Enter();
	canceled = true;
	Cancel0();
	Finish0();
	p.lock.Leave();
	LLOG("CoWork " << FormatIntHex(this) << " canceled and finished");
}

void CoWork::Finish() {
	Pool& p = GetPool();
	p.lock.Enter();
	while(!jobs.IsEmpty(1)) {
		LLOG("Finish: todo: " << todo << " (CoWork " << FormatIntHex(this) << ")");
		p.DoJob(*jobs.GetNext(1));
	}
	Finish0();
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

void CoWork::SetPoolSize(int n)
{
	Pool& p = GetPool();
	p.ExitThreads();
	p.InitThreads(n);
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

void CoWork::Reset()
{
	try {
		Cancel();
	}
	catch(...) {}
	todo = 0;
	canceled = false;
}

bool CoWork::IsCanceled()
{
	return current && current->canceled;
}

int CoWork::GetWorkerIndex()
{
	return worker_index;
}

CoWork::CoWork()
{
	LLOG("CoWork constructed " << FormatHex(this));
	todo = 0;
	canceled = false;
}

CoWork::~CoWork() noexcept(false)
{
	Finish();
	LLOG("~CoWork " << FormatIntHex(this));
}

#endif

}
