#ifdef _MULTITHREADED

class CoWork : NoCopy {
	typedef StaticCriticalSection Lock;

	struct MJob : Moveable<MJob> {
	#ifdef CPP_11
		std::function<void ()> fn;
	#endif
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

		void Start();
		void Shutdown();

		~Pool()         { Shutdown(); }
		
		static thread__ bool finlock;

		static bool DoJob();
		static void ThreadRun(int tno);
	};
	
	friend struct Pool;

	static Pool& pool();

	Semaphore waitforfinish;
	int       todo;

#ifdef CPP_11
	void Do(const Callback *cb, const std::function<void ()> *fn);
#else
	void Do(const Callback *cb, void *);
#endif
	
public:
	void     Do(const Callback& cb)                           { Do(&cb, NULL); }
#ifdef CPP_11
	void     Do(const std::function<void ()>& lambda)         { Do(NULL, &lambda); }
#endif

	CoWork&  operator&(const Callback& cb)                    { Do(&cb, NULL); return *this; }
#ifdef CPP_11
	CoWork&  operator&(const std::function<void ()>& lambda)  { Do(NULL, &lambda); return *this; }
#endif

	static void FinLock();

	void Finish();
	
	bool IsFinished();

	CoWork();
	~CoWork();
	
	static void Shutdown()          { pool().Shutdown(); }
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
