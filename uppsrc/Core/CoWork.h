#ifdef _MULTITHREADED

class CoWork : NoCopy {
	typedef StaticCriticalSection Lock;

	struct MJob : Moveable<MJob> {
		std::function<void ()> fn;
		Callback               cb;
		CoWork                *work;
	};
	
	enum { SCHEDULED_MAX = 2048 };

	struct Pool {
		int             scheduled;
		MJob            jobs[SCHEDULED_MAX];
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

	void Do(const Callback *cb, const std::function<void ()> *fn);
	
public:
	void     Do(const Callback& cb)                       { Do(&cb, NULL); }
#ifdef CPP_11
	void     Do(const std::function<void ()>& fn)         { Do(NULL, &fn); }
#endif

	CoWork&  operator&(const Callback& cb)                { Do(&cb, NULL); return *this; }
#ifdef CPP_11
	CoWork&  operator&(const std::function<void ()>& fn)  { Do(NULL, &fn); return *this; }
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
