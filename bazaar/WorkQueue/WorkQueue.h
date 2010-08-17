#ifndef _WorkQueue_WorkQueue_h_
#define _WorkQueue_WorkQueue_h_

#include <Core/Core.h>

using namespace Upp;

namespace MyWQ
{

#ifdef _MULTITHREADED

class WorkQueue : NoCopy {

	struct MJob 
		: public Link<MJob>
		, Moveable<MJob> 
	{
		Callback cb;
		WorkQueue  *work;
		MJob() : work(NULL) {}
	};

	struct Pool {
		typedef Pool CLASSNAME;
		//Vector<MJob>    jobs;
		LinkOwner<MJob> _jobs;
		int			    jobcount;
		MJob &          NextFree();
		Link<MJob> *          lastjob;
		
		int             waiting_threads;
		Array<Thread>   threads;

		CriticalSection lock;
		Semaphore       waitforjob;

		Pool(int threadnr = -1);
		~Pool();

		bool DoJob();
		void ThreadRun(int tno);
	};
	
	friend struct Pool;

	//static Pool& pool();
	Pool _pool;
	Pool& pool();

	Semaphore waitforfinish;
	int       todo;

public:
	void     Do(Callback cb);
	WorkQueue&  operator&(Callback cb) { Do(cb); return *this; }

	void Finish();

	WorkQueue();
	~WorkQueue();
};

#else

class WorkQueue : NoCopy {
public:
	void     Do(Callback cb)        { cb(); }
	WorkQueue&  operator&(Callback cb) { cb(); return *this; }
	void     Finish()               {}
};

#endif

} //namespace

#endif
