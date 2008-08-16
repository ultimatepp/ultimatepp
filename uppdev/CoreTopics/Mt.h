#ifndef _DEBUG
inline void AssertST() {}
#endif

#ifdef _MULTITHREADED

#ifdef COMPILER_MSC
#define thread__ __declspec(thread)
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

class Thread {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
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
	HANDLE      GetHandle() const { return handle; }
#endif
#ifdef PLATFORM_POSIX
	pthread_t   GetHandle() const { return handle; }
#endif

	void        Priority(int percent); // 0 = lowest, 100 = normal

	static void Start(Callback cb);

	static void Sleep(int ms);

	static bool IsST();
	static int  GetCount();
	static void ShutdownThreads();
	static bool IsShutdownThreads();


	Thread();
	~Thread()   { Detach(); }

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

template <class U>
inline U ReadWithBarrier(const volatile U& b)
{
	/*volatile*/ U tmp = b;
	ReadMemoryBarrier();
	return tmp;
}

template <class U, class V>
inline void BarrierWrite(volatile U& ptr, V data)
{
	WriteMemoryBarrier();
	ptr = data;
}

class Semaphore {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
#else
	sem_t      sem;
#endif

public:
	void       Wait();
	void       Release();

	Semaphore();
	~Semaphore();
};

class StaticSemaphore {
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

class Mutex {
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

	struct Lock;
};

/* Win32 RWMutex implementation by Chris Thomasson, cristom@comcast.net */

class RWMutex {
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

	struct ReadLock;
	struct WriteLock;
};

#endif

#ifdef PLATFORM_POSIX

typedef _Atomic_word Atomic;

inline int  AtomicXAdd(volatile Atomic& t, int incr)  { using namespace __gnu_cxx; return __exchange_and_add(&t, incr); }

inline int  AtomicInc(volatile Atomic& t)             { return AtomicXAdd(t, +1) + 1; }
inline int  AtomicDec(volatile Atomic& t)             { return AtomicXAdd(t, -1) - 1; }

class Mutex {
protected:
	pthread_mutex_t  mutex[1];
	MtInspector     *mti;

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

	struct Lock;

	Mutex();
	~Mutex()           { pthread_mutex_destroy(mutex); }
};

class RWMutex {
	pthread_rwlock_t rwlock[1];

public:
	void EnterWrite()  { pthread_rwlock_wrlock(rwlock); }
	void LeaveWrite()  { pthread_rwlock_unlock(rwlock); }
	void EnterRead()   { pthread_rwlock_rdlock(rwlock); }
	void LeaveRead()   { pthread_rwlock_unlock(rwlock); }

	RWMutex();
	~RWMutex();

	struct ReadLock;
	struct WriteLock;
};

#endif

inline int  AtomicRead(const volatile Atomic& t)      { return ReadWithBarrier(t); }
inline void AtomicWrite(volatile Atomic& t, int val)  { BarrierWrite(t, val); }

struct Mutex::Lock {
	Mutex& s;
	Lock(Mutex& s) : s(s) { s.Enter(); }
	~Lock()               { s.Leave(); }
};

struct RWMutex::ReadLock {
	RWMutex& s;
	ReadLock(RWMutex& s) : s(s) { s.EnterRead(); }
	~ReadLock()                 { s.LeaveRead(); }
};

struct RWMutex::WriteLock {
	RWMutex& s;
	WriteLock(RWMutex& s) : s(s) { s.EnterWrite(); }
	~WriteLock()                 { s.LeaveWrite(); }
};

class StaticMutex {
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

class StaticRWMutex {
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

#else

#define thread__

#define PROFILEMT(mutex)
#define PROFILEMT_(mutex, id)

typedef int Atomic;

inline int  AtomicRead(const volatile Atomic& t)      { return t; }
inline void AtomicWrite(volatile Atomic& t, int data) { t = data; }

inline int  AtomicInc(volatile Atomic& t)             { ++t; return t; }
inline int  AtomicDec(volatile Atomic& t)             { --t; return t; }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { Atomic x = t; t += incr; return x; }

struct Mutex {
	bool  TryEnter()             { return true; }
	void  Enter()                {}
	void  Leave()                {}

	struct Lock;
};

typedef Mutex StaticMutex;

struct Mutex::Lock {
	Lock(Mutex&) {}
	~Lock()                {}
};

struct RWMutex {
	void EnterWrite() {}
	void LeaveWrite() {}

	void EnterRead() {}
	void LeaveRead() {}

	struct ReadLock;
	struct WriteLock;
};

struct RWMutex::ReadLock {
	ReadLock(RWMutex&) {}
	~ReadLock()        {}
};

struct RWMutex::WriteLock {
	WriteLock(RWMutex&) {}
	~WriteLock()        {}
};

typedef RWMutex StaticRWMutex;

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
