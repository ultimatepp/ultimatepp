#ifdef _MULTITHREADED

class CoWork : NoCopy {
	struct MJob : Moveable<MJob>, Link<MJob, 2> {
		Function<void ()> fn;
		CoWork           *work = NULL;
	};
	
	enum { SCHEDULED_MAX = 2048 };

public:
	struct Pool {
		MJob             *free;
		Link<MJob, 2>     jobs;
		MJob              slot[SCHEDULED_MAX];
		int               waiting_threads;
		Array<Thread>     threads;
		bool              quit;

		Mutex             lock;
		ConditionVariable waitforjob;
		
		void              Free(MJob& m);
		void              DoJob(MJob& m);
		void              PushJob(Function<void ()>&& fn, CoWork *work);

		void              InitThreads(int nthreads);
		void              ExitThreads();

		Pool();
		~Pool();
		
		static thread__ bool finlock;

		bool DoJob();
		static void ThreadRun(int tno);
	};
	
	friend struct Pool;

	static Pool& GetPool();

	static thread_local bool is_worker;

//	byte magic[sizeof(ConditionVariable)];
	ConditionVariable waitforfinish;
	Link<MJob, 2> jobs; // global stack and CoWork stack as double-linked lists
	int todo;

// experimental pipe support
	Mutex stepmutex;
	Array<BiVector<Function<void ()>>> step;
	Vector<bool> steprunning;
	
public:
	static bool TrySchedule(Function<void ()>&& fn);
	static bool TrySchedule(const Function<void ()>& fn)      { return TrySchedule(clone(fn)); }
	static void Schedule(Function<void ()>&& fn);
	static void Schedule(const Function<void ()>& fn)         { return Schedule(clone(fn)); }

	void     Do(Function<void ()>&& fn);
	void     Do(const Function<void ()>& fn)                  { Do(clone(fn)); }

	CoWork&  operator&(const Function<void ()>& fn)           { Do(fn); return *this; }
	CoWork&  operator&(Function<void ()>&& fn)                { Do(pick(fn)); return *this; }

	void Pipe(int stepi, Function<void ()>&& lambda); // experimental

	static void FinLock();

	void Finish();
	
	bool IsFinished();

	static bool IsWorker()                                    { return is_worker; }
	static void SetPoolSize(int n);

	CoWork();
	~CoWork();
};

#else

class CoWork : NoCopy {
public:
	void     Do(Event<>  cb)        { cb(); }
	CoWork&  operator&(Event<>  cb) { cb(); return *this; }
	void     Finish()               {}
	bool     IsFinished()           { return true; }

	static void FinLock()           {}

	static bool IsWorker()          { return false; }
	static void StartPool(int n)    {}
	static void ShutdownPool()      {}
};

#endif
