#ifndef _DEBUG
inline void AssertST() {}
#endif

#ifdef _MULTITHREADED

#ifdef DEPRECATED
#define thread__ thread_local
#endif

#ifdef flagPROFILEMT
class Mutex;
class StaticMutex;

struct MtInspector {
	const char *name;
	int   number;
	int   locked;
	int   blocked;
	
	static MtInspector *Dumi();

	MtInspector(const char *s, int n = -1) { name = s; number = n; locked = blocked = 0; }
	~MtInspector();
};

#define PROFILEMT(mutex) \
	{ static MtInspector MK__s(__FILE__, __LINE__); mutex.Set(MK__s); }

#define PROFILEMT_(mutex, id) \
	{ static MtInspector MK__s(id); mutex.Set(MK__s); }

#else

#define PROFILEMT(mutex)
#define PROFILEMT_(mutex, id)

#endif

template<typename Res, typename... ArgTypes>
class Function<Res(ArgTypes...)>;

class Thread : NoCopy {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
	DWORD	   thread_id;
#endif
#ifdef PLATFORM_POSIX
	pthread_t  handle;
#endif
public:
	bool       Run(Function<void ()> cb, bool noshutdown = false);
	bool       RunNice(Function<void ()> cb, bool noshutdown = false);
	bool       RunCritical(Function<void ()> cb, bool noshutdown = false);

	void       Detach();
	int        Wait();

	bool       IsOpen() const     { return handle; }

#ifdef PLATFORM_WIN32
	typedef HANDLE Handle;
	typedef DWORD  Id;

	Id          GetId() const                  { return thread_id; }
#endif
#ifdef PLATFORM_POSIX
	typedef pthread_t Handle;
	typedef pthread_t Id;

	Id          GetId() const                  { return handle; }
#endif

	Handle      GetHandle() const              { return handle; }
	
	bool        Priority(int percent); // 0 = lowest, 100 = normal
	
	void        Nice()                         { Priority(25); }
	void        Critical()                     { Priority(150); }

	static void Start(Function<void ()> cb, bool noshutdown = false);
	static void StartNice(Function<void ()> cb, bool noshutdown = false);
	static void StartCritical(Function<void ()> cb, bool noshutdown = false);

	static void Sleep(int ms);

	static bool IsST();
	static bool IsMain();
	static int  GetCount();
	static void BeginShutdownThreads();
	static void EndShutdownThreads();
	static void ShutdownThreads();
	static bool IsShutdownThreads();
	static void (*AtExit(void (*exitfn)()))();

	static void Exit();

#ifdef PLATFORM_WIN32
	static Handle GetCurrentHandle()          { return GetCurrentThread(); }
	static inline Id GetCurrentId()           { return ::GetCurrentThreadId(); };
#endif
#ifdef PLATFORM_POSIX
	static Handle GetCurrentHandle()          { return pthread_self(); }
	static inline Id GetCurrentId()           { return pthread_self(); };
#endif

	Thread();
	~Thread();

private:
	void operator=(const Thread&);
	Thread(const Thread&);
};

#ifdef _DEBUG
inline void AssertST() { ASSERT(Thread::IsST()); }
#endif

class Semaphore : NoCopy {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
#elif PLATFORM_OSX
	dispatch_semaphore_t    sem;
#else
	sem_t      sem;
#endif

public:
	void       Wait();
	void       Release();
#ifdef PLATFORM_WIN32
	void       Release(int n);
#endif

	Semaphore();
	~Semaphore();
};

struct MtInspector;

#ifdef PLATFORM_WIN32

class Mutex : NoCopy {
protected:
	CRITICAL_SECTION section;
	MtInspector        *mti;

	Mutex(int)         {}

	friend class ConditionVariable;

public:
	bool  TryEnter();
	void  Leave()                { LeaveCriticalSection(&section); }

#ifdef flagPROFILEMT
	void  Enter()                { if(!TryEnter()) { mti->blocked++; EnterCriticalSection(&section); }; mti->locked++; }
	void  Set(MtInspector& m)    { mti = &m; }

	Mutex()                      { mti = MtInspector::Dumi(); InitializeCriticalSection(&section); }
#else
	void  Enter()                { EnterCriticalSection(&section); }

	Mutex()                      { InitializeCriticalSection(&section); }
#endif

	~Mutex()                     { DeleteCriticalSection(&section); }

	class Lock;
};

/* Win32 RWMutex implementation by Chris Thomasson, cristom@comcast.net */

class RWMutex : NoCopy {
    LONG   m_count, m_rdwake;
    HANDLE m_wrwset, m_rdwset;
    CRITICAL_SECTION m_wrlock;

public:
	void EnterWrite();
	void LeaveWrite();

	void EnterRead();
	void LeaveRead();

	RWMutex();
	~RWMutex();

	class ReadLock;
	class WriteLock;
};

class ConditionVariable {
	static VOID (WINAPI *InitializeConditionVariable)(PCONDITION_VARIABLE ConditionVariable);
	static VOID (WINAPI *WakeConditionVariable)(PCONDITION_VARIABLE ConditionVariable);
	static VOID (WINAPI *WakeAllConditionVariable)(PCONDITION_VARIABLE ConditionVariable);
	static BOOL (WINAPI *SleepConditionVariableCS)(PCONDITION_VARIABLE ConditionVariable, PCRITICAL_SECTION CriticalSection, DWORD dwMilliseconds);
	
	CONDITION_VARIABLE cv[1];

	struct WaitingThread { // Windows XP does not provide ConditionVariable, implement using Semaphores
		Semaphore      sem;
		WaitingThread *next;
	};
	Mutex          mutex;
	WaitingThread *head, *tail;
	
public:
	void Wait(Mutex& m);
	void Signal();
	void Broadcast();
	
	ConditionVariable();
	~ConditionVariable();
};

#endif

#ifdef PLATFORM_POSIX

class Mutex : NoCopy {
	pthread_mutex_t  mutex[1];
#ifdef flagPROFILEMT
	MtInspector     *mti;
#endif
	friend class ConditionVariable;

public:
#ifdef flagPROFILEMT
	bool  TryEnter()          { bool b = pthread_mutex_trylock(mutex) == 0; mti->locked += b; return b; }
	void  Enter()             { if(pthread_mutex_trylock(mutex) != 0) { mti->blocked++; pthread_mutex_lock(mutex); } mti->locked++; }
	void  Set(MtInspector& m) { mti = &m; }
#else
	bool  TryEnter()          { return pthread_mutex_trylock(mutex) == 0; }
	void  Enter()             { pthread_mutex_lock(mutex); }
#endif
	void  Leave()             { pthread_mutex_unlock(mutex); }

	class Lock;

	Mutex();
	~Mutex()           { pthread_mutex_destroy(mutex); }
};

class RWMutex : NoCopy {
	pthread_rwlock_t rwlock[1];

public:
	void EnterWrite()  { pthread_rwlock_wrlock(rwlock); }
	void LeaveWrite()  { pthread_rwlock_unlock(rwlock); }
	void EnterRead()   { pthread_rwlock_rdlock(rwlock); }
	void LeaveRead()   { pthread_rwlock_unlock(rwlock); }

	RWMutex();
	~RWMutex();

	class ReadLock;
	class WriteLock;
};

class ConditionVariable {
	pthread_cond_t cv[1];
	
public:
	void Wait(Mutex& m)  { pthread_cond_wait(cv, m.mutex); }

	void Signal()        { pthread_cond_signal(cv); }
	void Broadcast()     { pthread_cond_broadcast(cv); }
	
	ConditionVariable()  { pthread_cond_init(cv, NULL); }
	~ConditionVariable() { pthread_cond_destroy(cv); }
};

#endif

typedef std::atomic<bool> OnceFlag;

#define ONCELOCK_(o_b_) \
for(static ::Upp::Mutex o_ss_; !o_b_.load(std::memory_order_acquire);) \
	for(::Upp::Mutex::Lock o_ss_lock__(o_ss_); !o_b_.load(std::memory_order_acquire); o_b_.store(true, std::memory_order_release))

#define ONCELOCK \
for(static ::Upp::OnceFlag o_b_; !o_b_.load(std::memory_order_acquire);) ONCELOCK_(o_b_)


class Mutex::Lock : NoCopy {
	Mutex& s;

public:
	Lock(Mutex& s) : s(s) { s.Enter(); }
	~Lock()               { s.Leave(); }
};

class RWMutex::ReadLock : NoCopy {
	RWMutex& s;

public:
	ReadLock(RWMutex& s) : s(s) { s.EnterRead(); }
	~ReadLock()                 { s.LeaveRead(); }
};

class RWMutex::WriteLock : NoCopy {
	RWMutex& s;

public:
	WriteLock(RWMutex& s) : s(s) { s.EnterWrite(); }
	~WriteLock()                 { s.LeaveWrite(); }
};

template <class Primitive>
class StaticPrimitive_ : NoCopy {
	Primitive *primitive;
	byte       buffer[sizeof(Primitive)];
	OnceFlag   once;
	
	void Initialize() { primitive = new(buffer) Primitive; }

public:
	Primitive& Get()  { ONCELOCK_(once) Initialize(); return *primitive; }
};

class StaticMutex : StaticPrimitive_<Mutex> {
public:
	operator Mutex&()          { return Get(); }
	bool TryEnter()            { return Get().TryEnter();}
	void Enter()               { Get().Enter();}
	void Leave()               { Get().Leave(); }
#ifdef flagPROFILEMT
	void Set(MtInspector& mti) { Get().Set(mti); }
#endif
};

class StaticSemaphore : StaticPrimitive_<Semaphore> {
public:
	operator Semaphore&()        { return Get(); }
	void Wait()                  { Get().Wait(); }
	void Release()               { Get().Release(); }
};

class StaticRWMutex : StaticPrimitive_<RWMutex> {
public:
	operator RWMutex&()  { return Get(); }
	void EnterRead()     { Get().EnterRead();}
	void LeaveRead()     { Get().LeaveRead(); }
	void EnterWrite()    { Get().EnterWrite();}
	void LeaveWrite()    { Get().LeaveWrite(); }
};

class StaticConditionVariable : StaticPrimitive_<ConditionVariable> {
public:
	operator ConditionVariable&() { return Get(); }
	void Wait(Mutex& m)  { Get().Wait(m); }
	void Signal()        { Get().Signal(); }
	void Broadcast()     { Get().Broadcast(); }
};

class LazyUpdate {
	mutable Mutex              mutex;
	mutable std::atomic<bool>  dirty;

public:
	void Invalidate();
	bool BeginUpdate() const;
	void EndUpdate() const;

	LazyUpdate();
};

inline bool IsMainThread() { return Thread::IsMain(); }

struct SpinLock : Moveable<SpinLock> {
#ifdef COMPILER_MSC
	volatile LONG locked;

	bool TryEnter() { return InterlockedCompareExchange(&locked, 1, 0) == 0; }
	void Leave()    { _ReadWriteBarrier(); locked = 0; }
#else
	volatile int locked;
	
	bool TryEnter() { return  __sync_bool_compare_and_swap (&locked, 0, 1); }
	void Leave()    { __sync_lock_release(&locked); }
#endif

	void Enter()    { while(!TryEnter()) Wait(); }
	
	void Wait();
	
	class Lock;

	SpinLock()         { locked = 0; }
};

class SpinLock::Lock : NoCopy {
	SpinLock& s;

public:
	Lock(SpinLock& s) : s(s) { s.Enter(); }
	~Lock()                  { s.Leave(); }
};

#define INTERLOCKED \
for(bool i_b_ = true; i_b_;) \
	for(static UPP::Mutex i_ss_; i_b_;) \
		for(UPP::Mutex::Lock i_ss_lock__(i_ss_); i_b_; i_b_ = false)

struct H_l_ : Mutex::Lock {
	bool b;
	H_l_(Mutex& cs) : Mutex::Lock(cs) { b = true; }
};

#define INTERLOCKED_(cs) \
for(UPP::H_l_ i_ss_lock__(cs); i_ss_lock__.b; i_ss_lock__.b = false)
	
#else

inline bool IsMainThread() { return true; }

#define thread__

#define PROFILEMT(mutex)
#define PROFILEMT_(mutex, id)

class Mutex : NoCopy {
public:
	bool  TryEnter()             { return true; }
	void  Enter()                {}
	void  Leave()                {}

	class Lock;
};

typedef Mutex StaticMutex;

class Mutex::Lock : NoCopy {
public:
	Lock(Mutex&) {}
	~Lock()                {}
};

class RWMutex : NoCopy {
public:
	void EnterWrite() {}
	void LeaveWrite() {}

	void EnterRead() {}
	void LeaveRead() {}

	class ReadLock;
	class WriteLock;
};

class RWMutex::ReadLock : NoCopy {
public:
	ReadLock(RWMutex&) {}
	~ReadLock()        {}
};

class RWMutex::WriteLock : NoCopy {
public:
	WriteLock(RWMutex&) {}
	~WriteLock()        {}
};

typedef RWMutex StaticRWMutex;

class LazyUpdate {
	mutable bool  dirty;

public:
	void Invalidate()              { dirty = true; }
	bool BeginUpdate() const       { return dirty; }
	void EndUpdate() const         { dirty = false; }

	LazyUpdate()                   { dirty = true; }
};

struct SpinLock {
	bool TryEnter() { return true; }
	void Leave()    {}
	void Enter()    {}
	
	class Lock;

	SpinLock()      {}
};

class SpinLock::Lock : NoCopy {
public:
	Lock(SpinLock& s) {}
	~Lock()           {}
};

#ifdef _DEBUG
inline void AssertST() {}
#endif

#define INTERLOCKED
#define INTERLOCKED_(x) { x.Enter(); }

#define ONCELOCK \
for(static bool o_b_; !o_b_; o_b_ = true)

#define ONCELOCK_(o_b_) \
for(; !o_b_; o_b_ = true) \

#define ONCELOCK_PTR(ptr, init) \
if(!ptr) ptr = init;

inline void ReadMemoryBarrier() {}
inline void WriteMemoryBarrier() {}

#endif

#ifdef DEPRECATED
typedef Mutex CriticalSection;
typedef StaticMutex StaticCriticalSection;
#endif

// Auxiliary multithreading - this is not using/cannot use U++ heap, so does not need cleanup.
// Used to resolve some host platform issues.

#ifdef PLATFORM_WIN32
#define auxthread_t DWORD
#define auxthread__ WINAPI
#else
#define auxthread_t void *
#define auxthread__
#endif

bool StartAuxThread(auxthread_t (auxthread__ *fn)(void *ptr), void *ptr);
