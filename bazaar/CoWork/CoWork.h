#ifndef _CoWork_CoWork_h
#define _CoWork_CoWork_h

#include <Core/Core.h>

using namespace Upp;

namespace MyCoWork
{

#ifdef _MULTITHREADED

class CoWork : NoCopy {

	struct MJob : Moveable<MJob> {
		Callback cb;
		CoWork  *work;
	};

	struct Pool {
		typedef Pool CLASSNAME;
		Vector<MJob>    jobs;
		int             waiting_threads;
		ArrayMap<unsigned, Thread> threads;

		CriticalSection lock;
		Semaphore       waitforjob;

		Pool(int threadnr = -1);
		~Pool();

		void AddThread(int count = 1);
		void KillThread(int count = 1, bool waitone = false);

		bool DoJob();
		void ThreadRun(unsigned tno);
	};
	
	friend struct Pool;

	One<Pool> _opool;    //local pool, needs to be here, before _pool, to be initialized before
	static Pool& pool(); //global pool, switch in CoWork()
	Pool & _pool;        //this ref is actually used, set depending on threadnr

	Semaphore waitforfinish;
	int       todo;

public:
	void     Do(Callback cb);
	CoWork&  operator&(Callback cb) { Do(cb); return *this; }

	int GetThreadCount() const { return _pool.threads.GetCount(); }
	int GetThreadCount() { _pool.lock.Enter(); int c = _pool.threads.GetCount(); _pool.lock.Leave(); return c; } 
	void AddThread(int count = 1) { ASSERT(&_pool != &pool()); _pool.AddThread(count); }
	void KillThread(int count = 1, bool waitone = false) { ASSERT(&_pool != &pool()); _pool.KillThread(count, waitone); }
	
	void Finish();

	CoWork(int threadnr = -1);
	~CoWork();
};

#else

class CoWork : NoCopy {
public:
	void     Do(Callback cb)        { cb(); }
	CoWork&  operator&(Callback cb) { cb(); return *this; }
	void     Finish()               {}
	CoWork(int threadnr = -1)       {}
};

#endif

class WorkQueue : public CoWork
{
public:
	WorkQueue()
		: CoWork(1)
	{}	
};

} //namespace

#endif
