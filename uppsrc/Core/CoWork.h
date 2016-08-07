#ifdef _MULTITHREADED

#define COWORK2

#ifdef COWORK2

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
	Link<MJob, 2> jobs;
	int todo;

	Mutex stepmutex;
	Array<BiVector<Function<void ()>>> step;
	Vector<bool> steprunning;
	
public:
	static void Start(Function<void ()>&& fn);

	void     Do(Function<void ()>&& fn);
	void     Do(const Function<void ()>& fn)                  { Do(clone(fn)); }

	CoWork&  operator&(const Function<void ()>& fn)           { Do(fn); return *this; }
	CoWork&  operator&(Function<void ()>&& fn)                { Do(pick(fn)); return *this; }

	void Pipe(int stepi, Function<void ()>&& lambda); // experimental

	static void FinLock();

	void Finish();
	
	bool IsFinished();

	static bool IsWorker()                                    { return is_worker; }
//	static void StartPool(int n);
//	static void ShutdownPool();

	CoWork();
	~CoWork();
};

#else
class CoWork : NoCopy {
	typedef StaticCriticalSection Lock;

	struct MJob : Moveable<MJob> {
		Function<void ()> fn;
		CoWork           *work = NULL;
		bool             *started = NULL;
	};
	
	enum { SCHEDULED_MAX = 2048 };

public:
	struct Pool {
		int             scheduled;
		MJob            jobs[SCHEDULED_MAX];
		int             waiting_threads;
		Array<Thread>   threads;

		Mutex             lock;
		ConditionVariable waitforjob;
		ConditionVariable waitforstart;

		Pool(int nthreads);
		~Pool();
		
		static thread__ bool finlock;

		static bool DoJob();
		static void ThreadRun(int tno);
	};
	
	friend struct Pool;

	static Pool& GetPool(int n);
	static Pool& GetPool();

	static thread_local bool is_worker;
	static thread_local Pool *pool;

//	byte magic[sizeof(ConditionVariable)];
	ConditionVariable waitforfinish;
	int               todo;

	static MJob& PushJob(Function<void ()>&& fn);

	Mutex stepmutex;
	Array<BiVector<Function<void ()>>> step;
	Vector<bool> steprunning;
	
public:
	static void Start(Function<void ()>&& fn);

	void     Do(Function<void ()>&& fn);
	void     Do(const Function<void ()>& fn)                  { Do(clone(fn)); }

	CoWork&  operator&(const Function<void ()>& fn)           { Do(fn); return *this; }
	CoWork&  operator&(Function<void ()>&& fn)                { Do(pick(fn)); return *this; }

	void Pipe(int stepi, Function<void ()>&& lambda); // experimental

	static void FinLock();

	void Finish();
	
	bool IsFinished();

	static bool IsWorker()                                    { return is_worker; }
	static void StartPool(int n);
	static void ShutdownPool();

	CoWork();
	~CoWork();
};

#endif

#else

class CoWork : NoCopy {
public:
	void     Do(Callback cb)        { cb(); }
	CoWork&  operator&(Callback cb) { cb(); return *this; }
	void     Finish()               {}
	bool     IsFinished()           { return true; }

	static void FinLock()           {}

	static bool IsWorker()          { return false; }
	static void StartPool(int n)    {}
	static void ShutdownPool()      {}
};

#endif
