#ifdef _MULTITHREADED

class CoWork : NoCopy {
	typedef StaticCriticalSection Lock;

	struct MJob : Moveable<MJob> {
		Callback cb;
		CoWork  *work;
	};

	struct Pool {
		Vector<MJob>    jobs;
		int             waiting_threads;
		Array<Thread>   threads;

		CriticalSection lock;
		Semaphore       waitforjob;

		Pool();
		~Pool();

		static bool DoJob();
		static void ThreadRun(int tno);
	};
	
	friend struct Pool;

	static Pool& pool();

	Semaphore waitforfinish;
	int       todo;

public:
	void     Do(Callback cb);
#ifdef CPP_11
	void     Do(std::function<void ()> fn)            { Do(Callback(lambda(fn))); }
#endif

	CoWork&  operator&(Callback cb)                   { Do(cb); return *this; }
#ifdef CPP_11
	CoWork&  operator&(std::function<void ()> fn)     { Do(Callback(lambda(fn))); return *this; }
#endif

	void Finish();
	
	bool IsFinished();

	CoWork();
	~CoWork();
};

#else

class CoWork : NoCopy {
public:
	void     Do(Callback cb)        { cb(); }
	CoWork&  operator&(Callback cb) { cb(); return *this; }
	void     Finish()               {}
	bool     IsFinished()           { return true; }
};

#endif
