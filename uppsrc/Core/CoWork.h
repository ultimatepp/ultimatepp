class CoWork : NoCopy {
	struct MJob : Moveable<MJob>, Link<MJob, 2> {
		Function<void ()> fn;
		CoWork           *work = NULL;
		bool              looper = false;
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
		void              PushJob(Function<void ()>&& fn, CoWork *work, bool looper = false);

		void              InitThreads(int nthreads);
		void              ExitThreads();

		Pool();
		~Pool();

	#ifdef COMPILER_MINGW
		static FastMingwTls<bool>   finlock;
	#else
		static thread_local bool    finlock;
	#endif

		bool DoJob();
		static void ThreadRun(int tno);
	};
	
	friend struct Pool;

	static Pool& GetPool();

#ifdef COMPILER_MINGW
	static FastMingwTls<int>      worker_index;
	static FastMingwTls<CoWork *> current;
#else
	static thread_local int worker_index;
	static thread_local CoWork *current;
#endif

	ConditionVariable  waitforfinish;
	Link<MJob, 2>      jobs; // global stack and CoWork stack as double-linked lists
	int                todo;
	bool               canceled;
	std::exception_ptr exc = nullptr; // workaround for sanitizer bug(?)
	Function<void ()>  looper_fn;
	int                looper_count;

	void Do0(Function<void ()>&& fn, bool looper);

	void Cancel0();
	void Finish0();
	
	Atomic             index;

// experimental pipe support
	Mutex stepmutex;
	Array<BiVector<Function<void ()>>> step;
	Vector<bool> steprunning;
	
public:
	static bool TrySchedule(Function<void ()>&& fn);
	static bool TrySchedule(const Function<void ()>& fn)      { return TrySchedule(clone(fn)); }
	static void Schedule(Function<void ()>&& fn);
	static void Schedule(const Function<void ()>& fn)         { return Schedule(clone(fn)); }

	void     Do(Function<void ()>&& fn)                       { Do0(pick(fn), false); }
	void     Do(const Function<void ()>& fn)                  { Do(clone(fn)); }

	CoWork&  operator&(const Function<void ()>& fn)           { Do(fn); return *this; }
	CoWork&  operator&(Function<void ()>&& fn)                { Do(pick(fn)); return *this; }
	
	void     Loop(Function<void ()>&& fn);
	void     Loop(const Function<void ()>& fn)                { Loop(clone(fn)); }

	CoWork&  operator*(const Function<void ()>& fn)           { Loop(fn); return *this; }
	CoWork&  operator*(Function<void ()>&& fn)                { Loop(pick(fn)); return *this; }

	int      Next()                                           { return ++index - 1; }

	int  GetScheduledCount() const                            { return todo; }
	void Pipe(int stepi, Function<void ()>&& lambda); // experimental

	static void FinLock();
	
	void Cancel();
	static bool IsCanceled();

	void Finish();
	
	bool IsFinished();
	
	void Reset();

	static bool IsWorker()                                    { return GetWorkerIndex() >= 0; }
	static int  GetWorkerIndex();
	static int  GetPoolSize();
	static void SetPoolSize(int n);

	CoWork();
	~CoWork() noexcept(false);
};

struct CoWorkNX : CoWork {
	~CoWorkNX() noexcept(true) {}
};

inline
void CoDo(Function<void ()>&& fn)
{
	CoWork co;
	co * fn;
}

template <typename Fn>
void CoFor(int n, Fn iterator)
{
	std::atomic<int> ii;
	ii = 0;
	CoDo([&] {
		for(int i = ii++; i < n; i = ii++)
			iterator(i);
	});
}

template <class T>
class CoWorkerResources {
	int          workercount;
	Buffer<T>    res;
	
public:
	int GetCount() const  { return workercount + 1; }
	T& operator[](int i)  { return res[i]; }

	T& Get()              { int i = CoWork::GetWorkerIndex(); return res[i < 0 ? workercount : i]; }
	T& operator~()        { return Get(); }
	
	T *begin()            { return ~res; }
	T *end()              { return ~res + GetCount(); }
	
	CoWorkerResources()   { workercount = CoWork::GetPoolSize(); res.Alloc(GetCount()); }

	CoWorkerResources(Event<T&> initializer) : CoWorkerResources() {
		for(int i = 0; i < GetCount(); i++)
			initializer(res[i]);
	}
};

template <class Ret>
class AsyncWork {
	template <class Ret2>
	struct Imp {
		CoWork co;
		Ret2   ret;
	
		template<class Function, class... Args>
		void        Do(Function&& f, Args&&... args) { co.Do([=]() { ret = f(args...); }); }
		const Ret2& Get()                            { return ret; }
		Ret2        Pick()                           { return pick(ret); }
	};

	struct ImpVoid {
		CoWork co;
	
		template<class Function, class... Args>
		void        Do(Function&& f, Args&&... args) { co.Do([=]() { f(args...); }); }
		void        Get()                            {}
		void        Pick()                           {}
	};
	
	using ImpType = typename std::conditional<std::is_void<Ret>::value, ImpVoid, Imp<Ret>>::type;
	
	One<ImpType> imp;
	
public:
	template< class Function, class... Args>
	void  Do(Function&& f, Args&&... args)          { imp.Create().Do(f, args...); }

	void        Cancel()                            { if(imp) imp->co.Cancel(); }
	static bool IsCanceled()                        { return CoWork::IsCanceled(); }
	bool        IsFinished()                        { return imp && imp->co.IsFinished(); }
	Ret         Get()                               { ASSERT(imp); imp->co.Finish(); return imp->Get(); }
	Ret         operator~()                         { return Get(); }
	Ret         Pick()                              { ASSERT(imp); imp->co.Finish(); return imp->Pick(); }
	
	AsyncWork& operator=(AsyncWork&&) = default;
	AsyncWork(AsyncWork&&) = default;

	AsyncWork()                                     {}
	~AsyncWork()                                    { if(imp) imp->co.Cancel(); }
};

template< class Function, class... Args>
AsyncWork<
	typename std::result_of<
		typename std::decay<Function>::type
			(typename std::decay<Args>::type...)
	>::type
>
Async(Function&& f, Args&&... args)
{
	AsyncWork<
		typename std::result_of<
			typename std::decay<Function>::type
				(typename std::decay<Args>::type...)
		>::type
	> h;
	h.Do(f, args...);
	return pick(h);
}
