#ifdef _MULTITHREADED

class CoWork : NoCopy {
	typedef StaticCriticalSection Lock;

	struct MJob : Moveable<MJob> {
		Function<void ()> fn;
		CoWork           *work;
	};
	
	enum { SCHEDULED_MAX = 2048 };

	struct Pool {
		int             scheduled;
		MJob            jobs[SCHEDULED_MAX];
		int             waiting_threads;
		Array<Thread>   threads;

		Mutex           lock;
		Semaphore       waitforjob;

		Pool();
		~Pool();
		
		static thread__ bool finlock;

		static bool DoJob();
		static void ThreadRun(int tno);
	};
	
	friend struct Pool;

	static Pool& pool();

	Semaphore waitforfinish;
	int       todo;

//	Mutex stepmutex;
//	Vector< BiVector<Callback> > step;

	
public:
	void     Do(Function<void ()>&& fn);
	void     Do(const Callback& cb)                           { Do(clone(cb)); }
	void     Do(const Function<void ()>& lambda)              { Do(clone(lambda)); }

	CoWork&  operator&(const Callback& cb)                    { Do(cb); return *this; }
	CoWork&  operator&(const Function<void ()>& lambda)       { Do(lambda); return *this; }
	CoWork&  operator&(Function<void ()>&& lambda)            { Do(lambda); return *this; }

//	void Step(int stepi, const Callback& cb);

	static void FinLock();

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
