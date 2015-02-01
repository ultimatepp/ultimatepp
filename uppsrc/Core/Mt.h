#ifndef _DEBUG
inline void AssertST() {}
#endif

#ifdef _MULTITHREADED

#ifdef COMPILER_MSC
	#ifdef flagDLL
	#define thread__
	#else
	#define thread__ __declspec(thread)
	#endif
#else
#define thread__ __thread
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

class Callback;

class Thread : NoCopy {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
	DWORD	   thread_id;
#endif
#ifdef PLATFORM_POSIX
	pthread_t  handle;
#endif
public:
	bool       Run(Callback cb);

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

	static void Start(Callback cb);

	static void Sleep(int ms);

	static bool IsST();
	static bool IsMain();
	static int  GetCount();
	static void BeginShutdownThreads();
	static void EndShutdownThreads();
	static void ShutdownThreads();
	static bool IsShutdownThreads();
	static void (*AtExit(void (*exitfn)()))();

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

void ReadMemoryBarrier();
void WriteMemoryBarrier();

#ifdef CPU_SSE2
inline void ReadMemoryBarrier()
{
#ifdef CPU_AMD64
	#ifdef COMPILER_MSC
		_mm_lfence();
	#else
		__asm__("lfence");
	#endif
#else	
	#ifdef COMPILER_MSC
		__asm lfence;
	#else
		__asm__("lfence");
	#endif
#endif
}

inline void WriteMemoryBarrier() {
#ifdef CPU_AMD64
	#ifdef COMPILER_MSC
		_mm_sfence();
	#else
		__asm__("sfence");
	#endif
#else	
	#ifdef COMPILER_MSC
		__asm sfence;
	#else
		__asm__("sfence");
	#endif
#endif
}
#endif

#ifdef CPU_BLACKFIN
inline void ReadMemoryBarrier() {} //placing in Mt.cpp somehow yields 'undefined reference'
inline void WriteMemoryBarrier() {}
#endif

template <class U>
inline U ReadWithBarrier(const volatile U& b)
{
	/*volatile*/ U tmp = b;
	ReadMemoryBarrier();
	return tmp;
}

template <class U, class V>
inline void BarrierWrite(volatile U& dest, V data)
{
	WriteMemoryBarrier();
	dest = data;
}

class Semaphore : NoCopy {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
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

class StaticSemaphore : NoCopy {
	volatile Semaphore *semaphore;
	byte                buffer[sizeof(Semaphore)];

	void Initialize();

public:
	Semaphore& Get()             { if(!ReadWithBarrier(semaphore)) Initialize(); return *const_cast<Semaphore *>(semaphore); }
	operator Semaphore&()        { return Get(); }
	void Wait()                  { Get().Wait(); }
	void Release()               { Get().Release(); }
};

struct MtInspector;

#ifdef PLATFORM_WIN32

typedef LONG Atomic;

inline int  AtomicInc(volatile Atomic& t)             { return InterlockedIncrement((Atomic *)&t); }
inline int  AtomicDec(volatile Atomic& t)             { return InterlockedDecrement((Atomic *)&t); }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { return InterlockedExchangeAdd((Atomic *)&t, incr); }

class Mutex : NoCopy {
protected:
	CRITICAL_SECTION section;
	MtInspector        *mti;

	Mutex(int)         {}

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
	Mutex                 mutex;
	friend struct sCVWaiter_;
	
	struct sCVWaiter_ *head, *tail;
	
public:
	void Wait(Mutex& m);
	void Signal();
	void Broadcast();
	
	ConditionVariable();
	~ConditionVariable();
};

#endif

#ifdef PLATFORM_POSIX

#if !(defined(PLATFORM_BSD) && defined(__clang__))
typedef _Atomic_word Atomic;
#else
typedef int Atomic;
#endif

inline int  AtomicXAdd(volatile Atomic& t, int incr)
{
#if !(defined(PLATFORM_BSD) && defined(__clang__))
	using namespace __gnu_cxx;
	return __exchange_and_add(&t, incr);
#else
	return __sync_fetch_and_add(&t, incr);
#endif
}

inline int  AtomicInc(volatile Atomic& t)             { return AtomicXAdd(t, +1) + 1; }
inline int  AtomicDec(volatile Atomic& t)             { return AtomicXAdd(t, -1) - 1; }

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

inline int  AtomicRead(const volatile Atomic& t)      { return ReadWithBarrier(t); }
inline void AtomicWrite(volatile Atomic& t, int val)  { BarrierWrite(t, val); }

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

class StaticMutex : NoCopy {
	volatile Mutex *section;
	byte            buffer[sizeof(Mutex)];

	void Initialize();

public:
	Mutex& Get()               { if(!ReadWithBarrier(section)) Initialize(); return *const_cast<Mutex *>(section); }
	operator Mutex&()          { return Get(); }
	bool TryEnter()            { return Get().TryEnter();}
	void Enter()               { Get().Enter();}
	void Leave()               { Get().Leave(); }
#ifdef flagPROFILEMT
	void Set(MtInspector& mti) { Get().Set(mti); }
#endif
};

class StaticRWMutex : NoCopy {
	volatile RWMutex *rw;
	byte              buffer[sizeof(RWMutex)];

	void Initialize();

public:
	RWMutex& Get()       { if(!ReadWithBarrier(rw)) Initialize(); return *const_cast<RWMutex *>(rw); }
	operator RWMutex&()  { return Get(); }
	void EnterRead()     { Get().EnterRead();}
	void LeaveRead()     { Get().LeaveRead(); }
	void EnterWrite()    { Get().EnterWrite();}
	void LeaveWrite()    { Get().LeaveWrite(); }
};

class StaticConditionVariable : NoCopy {
	volatile ConditionVariable *cv;
	byte                        buffer[sizeof(ConditionVariable)];
	
	void Initialize();

public:
	ConditionVariable& Get()      { if(!ReadWithBarrier(cv)) Initialize(); return *const_cast<ConditionVariable *>(cv); }
	operator ConditionVariable&() { return Get(); }

	void Wait(Mutex& m)  { Get().Wait(m); }

	void Signal()        { Get().Signal(); }
	void Broadcast()     { Get().Broadcast(); }
};

#define INTERLOCKED \
for(bool i_b_ = true; i_b_;) \
	for(static UPP::StaticMutex i_ss_; i_b_;) \
		for(UPP::Mutex::Lock i_ss_lock__(i_ss_); i_b_; i_b_ = false)

struct H_l_ : Mutex::Lock {
	bool b;
	H_l_(Mutex& cs) : Mutex::Lock(cs) { b = true; }
};

#define INTERLOCKED_(cs) \
for(UPP::H_l_ i_ss_lock__(cs); i_ss_lock__.b; i_ss_lock__.b = false)

void Set__(volatile bool& b);

#define ONCELOCK \
for(static volatile bool o_b_; !ReadWithBarrier(o_b_);) \
	for(static StaticMutex o_ss_; !o_b_;) \
		for(Mutex::Lock o_ss_lock__(o_ss_); !o_b_; BarrierWrite(o_b_, true))

#define ONCELOCK_(o_b_) \
for(static StaticMutex o_ss_; !ReadWithBarrier(o_b_);) \
	for(Mutex::Lock o_ss_lock__(o_ss_); !o_b_; BarrierWrite(o_b_, true))

#define ONCELOCK_PTR(ptr, init) \
if(!ReadWithBarrier(ptr)) { \
	static StaticMutex cs; \
	cs.Enter(); \
	if(!ptr) \
		BarrierWrite(ptr, init); \
	cs.Leave(); \
}

class LazyUpdate {
	mutable Mutex mutex;
	mutable bool  dirty;

public:
	void Invalidate();
	bool BeginUpdate() const;
	void EndUpdate() const;

	LazyUpdate();
};

inline bool IsMainThread() { return Thread::IsMain(); }

struct SpinLock {
#ifdef PLATFORM_WIN32
	volatile LONG locked;
	
	bool TryEnter() { return InterlockedExchange(&locked, 1) == 0; }
	void Leave()    { InterlockedExchange(&locked, 0); }
#endif
#ifdef PLATFORM_POSIX
	volatile int locked;
	
	bool TryEnter() { return __sync_lock_test_and_set(&locked, 1) == 0; }
	void Leave()    { __sync_lock_release(&locked); }
#endif
	
	void Enter()    { while(!TryEnter()); }
	
	class Lock;

	SpinLock()      { locked = 0; }
};

class SpinLock::Lock : NoCopy {
	SpinLock& s;

public:
	Lock(SpinLock& s) : s(s) { s.Enter(); }
	~Lock()                  { s.Leave(); }
};


#else

inline bool IsMainThread() { return true; }

#define thread__

#define PROFILEMT(mutex)
#define PROFILEMT_(mutex, id)

typedef int Atomic;

template <class U>
inline U ReadWithBarrier(const U& b)
{
	/*volatile*/ U tmp = b;
	return tmp;
}

template <class U, class V>
inline void BarrierWrite(U& dest, V data)
{
	dest = data;
}

inline int  AtomicRead(const volatile Atomic& t)      { return t; }
inline void AtomicWrite(volatile Atomic& t, int data) { t = data; }

inline int  AtomicInc(volatile Atomic& t)             { ++t; return t; }
inline int  AtomicDec(volatile Atomic& t)             { --t; return t; }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { Atomic x = t; t += incr; return x; }

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

#ifdef _DEBUG
inline void AssertST() {}
#endif

#endif

typedef Mutex CriticalSection; // deprecated
typedef StaticMutex StaticCriticalSection; // deprecated

// Auxiliary multithreading intended for use even in single-threaded applications
// to resolve some host platform issues. Raw threads cannot use U++ heap (nor indirectly)

#ifdef PLATFORM_WIN32
#define auxthread_t DWORD
#define auxthread__ WINAPI
#else
#define auxthread_t void *
#define auxthread__
#endif

#ifdef PLATFORM_WIN32
struct AuxMutex {
	CRITICAL_SECTION cs;
	
	void Enter() { EnterCriticalSection(&cs); }
	void Leave() { LeaveCriticalSection(&cs); }
	
	AuxMutex()   { InitializeCriticalSection(&cs); }
	~AuxMutex()  { DeleteCriticalSection(&cs); }
};
#endif

#ifdef PLATFORM_POSIX
struct AuxMutex {
	pthread_mutex_t  mutex[1];
	
	void Enter() { pthread_mutex_lock(mutex); }
	void Leave() { pthread_mutex_unlock(mutex); }
	
	AuxMutex()   {
		pthread_mutexattr_t mutex_attr[1];
		pthread_mutexattr_init(mutex_attr);
		pthread_mutexattr_settype(mutex_attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(mutex, mutex_attr);
 	}
	~AuxMutex()  { pthread_mutex_destroy(mutex); }
};
#endif

bool StartAuxThread(auxthread_t (auxthread__ *fn)(void *ptr), void *ptr);
