#ifdef _MULTITHREADED

class CoWork : NoCopy {
	typedef StaticCriticalSection Lock;

	struct MJob : Moveable<MJob> {
		std::function<void ()> fn; // TODO: Rework!
		Callback               cb;
		CoWork                *work;
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

	Mutex stepmutex;
	Vector< BiVector<Callback> > step;

	void Do(const Callback *cb, const std::function<void ()> *fn);
	
public:
	void     Do(const Callback& cb)                           { Do(&cb, NULL); }
	void     Do(const std::function<void ()>& lambda)         { Do(NULL, &lambda); }

	CoWork&  operator&(const Callback& cb)                    { Do(&cb, NULL); return *this; }
	CoWork&  operator&(const std::function<void ()>& lambda)  { Do(NULL, &lambda); return *this; }

	void Step(int stepi, const Callback& cb);

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
