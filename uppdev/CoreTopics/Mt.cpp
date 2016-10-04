#include "Core.h"

NAMESPACE_UPP

#ifdef _MULTITHREADED

static Mutex& sMutexLock()
{
	static Mutex *section;
	if(!section) {
		static byte b[sizeof(Mutex)];
		section = new(b) Mutex;
	}
	return *section;
}

INITBLOCK {
	sMutexLock();
}

Thread::Thread()
{
	sMutexLock();
#ifdef PLATFORM_WIN32
	handle = 0;
#endif
#ifdef PLATFORM_POSIX
	handle = 0;
#endif
}

void Thread::Detach()
{
#if defined(PLATFORM_WIN32)
	if(handle) {
		CloseHandle(handle);
		handle = 0;
	}
#elif defined(PLATFORM_POSIX)
	if(handle) {
		CHECK(!pthread_detach(handle));
		handle = 0;
	}
#endif
}

static Atomic sThreadCount;

#if defined(PLATFORM_WIN32) || defined(PLATFORM_POSIX)
static
#ifdef PLATFORM_WIN32
#ifdef CPU_64
unsigned int
#else
uintptr_t __stdcall
#endif
#else
void *
#endif
sThreadRoutine(void *arg)
{
	Callback *cb = (Callback *)arg;
	(*cb)();
	AtomicDec(sThreadCount);
	delete cb;
#ifdef UPP_HEAP
	MemoryFreeThread();
#endif
	return 0;
}
#endif

static bool threadr;

bool Thread::IsST()
{
	return !threadr;
}

bool Thread::Run(Callback _cb)
{
	AtomicInc(sThreadCount);
	threadr = true;
	Detach();
	Callback *cb = new Callback(_cb);
#ifdef PLATFORM_WIN32
	unsigned thread_id;
	handle = (HANDLE)_beginthreadex(0, 0, sThreadRoutine, cb, 0, &thread_id);
#endif
#ifdef PLATFORM_POSIX
	if(pthread_create(&handle, 0, sThreadRoutine, cb))
		handle = 0;
#endif
	return handle;
}

int Thread::GetCount()
{
	return ReadWithBarrier(sThreadCount);
}

static Atomic  sShutdown;

void Thread::ShutdownThreads()
{
	AtomicInc(sShutdown);
	while(sThreadCount)
		Sleep(100);
	AtomicDec(sShutdown);
}

bool Thread::IsShutdownThreads()
{
	return sShutdown;
}

int Thread::Wait()
{
	if(!IsOpen())
		return -1;
	int out;
#ifdef PLATFORM_WIN32
	dword exit;
	if(!GetExitCodeThread(handle, &exit))
		return -1;
	if(exit != STILL_ACTIVE)
		out = (int)exit;
	else
	{
		if(WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
			return Null;
		out = GetExitCodeThread(handle, &exit) ? int(exit) : int(Null);
	}
	Detach();
#endif
#ifdef PLATFORM_POSIX
	void *thread_return;
	if(pthread_join(handle, &thread_return))
		out = Null;
	else
		out = (int)(intptr_t)thread_return;
	handle = 0;
#endif
	return out;
}

void Thread::Priority(int percent)
{
	ASSERT(IsOpen());
#ifdef PLATFORM_WIN32
	int prior;
	if(percent <= 25)
		prior = THREAD_PRIORITY_LOWEST;
	else if(percent <= 75)
		prior = THREAD_PRIORITY_BELOW_NORMAL;
	else if(percent <= 125)
		prior = THREAD_PRIORITY_NORMAL;
	else if(percent <= 175)
		prior = THREAD_PRIORITY_ABOVE_NORMAL;
	else
		prior = THREAD_PRIORITY_HIGHEST;
	SetThreadPriority(handle, prior);
#endif
#ifdef PLATFORM_POSIX
	//!! todo
#endif
}

void Thread::Start(Callback cb)
{
	Thread t;
	t.Run(cb);
	t.Detach();
}

void Thread::Sleep(int msec)
{
#ifdef PLATFORM_WIN32
	::Sleep(msec);
#endif
#ifdef PLATFORM_POSIX
	timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
#endif
}

#ifdef CPU_X86

#ifndef CPU_SSE2

static bool sSSE2 = false; //CPU_SSE2();

inline void ReadMemoryBarrier()
{
#ifdef CPU_AMD64
	#ifdef COMPILER_MSC
		_mm_lfence();
	#else
		__asm__("lfence");
	#endif
#else
	if(sSSE2)
	#ifdef COMPILER_MSC
		__asm lfence;
	#else
		__asm__("lfence");
	#endif
	else {
		static Atomic x;
		AtomicInc(x);
	}
#endif
}

void WriteMemoryBarrier() {
#ifdef CPU_AMD64
	#ifdef COMPILER_MSC
		_mm_sfence();
	#else
		__asm__("sfence");
	#endif
#else
	if(sSSE2)
	#ifdef COMPILER_MSC
		__asm sfence;
	#else
		__asm__("sfence");
	#endif
	else {
		static Atomic x;
		AtomicInc(x);
	}
#endif
}
#endif

#endif

#ifdef flagPROFILEMT
MtInspector *MtInspector::Dumi()
{
	static MtInspector h(NULL);
	return &h;
}

MtInspector::~MtInspector()
{
	if(name)
		RLOG("Mutex " << name << '(' << number << ") " << blocked << "/" << locked <<
		     " = " << Sprintf("%.4f", locked ? (double)blocked / locked : 0) << " blocked/locked times");
}
#endif

#ifdef PLATFORM_WIN32

void Semaphore::Release()
{
	ReleaseSemaphore(handle, 1, NULL);
}

void Semaphore::Wait()
{
	WaitForSingleObject(handle, INFINITE);
}

Semaphore::Semaphore()
{
	handle = CreateSemaphore(NULL, 0, INT_MAX, NULL);
}

Semaphore::~Semaphore()
{
	CloseHandle(handle);
}

Mutex& sMutexLock();


typedef BOOL (WINAPI *TEC)(LPCRITICAL_SECTION lpCriticalSection);

static TEC sTec;

bool Mutex::TryEnter()
{
	if(!sTec) {
		if(HMODULE hDLL = LoadLibrary("Kernel32"))
			sTec = (TEC) GetProcAddress(hDLL, "TryEnterCriticalSection");
	}
/* TODO! TryEntery0
#ifdef flagPROFILEMT
	bool b = (*sTec)(&section);
	mti->blocked += b;
	return b;
#else
*/
	return (*sTec)(&section);
//#endif
}

/* Win32 RWMutex implementation by Chris Thomasson, cristom@comcast.net */

void RWMutex::EnterWrite()
{
	EnterCriticalSection ( &m_wrlock );
	LONG count = InterlockedExchangeAdd(&m_count, -LONG_MAX);
	if(count < LONG_MAX)
		if(InterlockedExchangeAdd ( &m_rdwake, LONG_MAX - count ) + LONG_MAX - count )
			WaitForSingleObject ( m_wrwset, INFINITE );
}

void RWMutex::LeaveWrite()
{
	LONG count = InterlockedExchangeAdd ( &m_count, LONG_MAX );
	if (count < 0)
	    ReleaseSemaphore ( m_rdwset, count * -1, 0 );
	LeaveCriticalSection ( &m_wrlock );
}

void RWMutex::EnterRead()
{
	LONG count = InterlockedDecrement ( &m_count );
	if(count < 0)
		WaitForSingleObject ( m_rdwset, INFINITE );
}

void RWMutex::LeaveRead()
{
	LONG count = InterlockedIncrement ( &m_count );
	if ( count < 1 )
		if ( ! InterlockedDecrement ( &m_rdwake ) )
			SetEvent ( m_wrwset );
}

RWMutex::RWMutex()
:	m_count ( LONG_MAX ),
	m_rdwake ( 0 ),
	m_wrwset ( CreateEvent ( 0, FALSE, FALSE, 0 ) ),
	m_rdwset ( CreateSemaphore ( 0, 0, LONG_MAX, 0 ) )
{
	InitializeCriticalSection ( &m_wrlock );
}

RWMutex::~RWMutex()
{
	DeleteCriticalSection ( &m_wrlock );
	CloseHandle ( m_rdwset );
	CloseHandle ( m_wrwset );
}

#endif

#ifdef PLATFORM_POSIX

Mutex::Mutex()
{
	pthread_mutexattr_t mutex_attr[1];
	pthread_mutexattr_init(mutex_attr);
	pthread_mutexattr_settype(mutex_attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(mutex, mutex_attr);
#ifdef flagPROFILEMT
	mti = MtInspector::Dumi();
#endif
}

RWMutex::RWMutex()
{
	pthread_rwlock_init(rwlock, NULL);
}

RWMutex::~RWMutex()
{
	pthread_rwlock_destroy(rwlock);
}

/*
Event::Event()
{
	pthread_mutex_init(mutex, NULL);
	pthread_cond_init(cond, NULL);
}

Event::~Event()
{
	pthread_mutex_destroy(mutex);
	pthread_cond_destroy(cond);
}

void Event::Wait()
{
	pthread_mutex_lock(mutex);
    pthread_cond_wait(cond, mutex);
	pthread_mutex_unlock(mutex);
}

void Event::Go()
{
	pthread_cond_signal(cond);
}
*/
void Semaphore::Release()
{
	sem_post(&sem);
}

void Semaphore::Wait()
{
	sem_wait(&sem);
}

Semaphore::Semaphore()
{
	sem_init(&sem, 0, 0);
}

Semaphore::~Semaphore()
{
	sem_destroy(&sem);
}

#endif

void StaticMutex::Initialize()
{
	Mutex::Lock __(sMutexLock());
	if(!ReadWithBarrier(section))
		BarrierWrite(section, new(buffer) Mutex);
}

void StaticRWMutex::Initialize()
{
	Mutex::Lock __(sMutexLock());
	if(!ReadWithBarrier(rw))
		BarrierWrite(rw, new(buffer) RWMutex);
}

void StaticSemaphore::Initialize()
{
	Mutex::Lock __(sMutexLock());
	if(!ReadWithBarrier(semaphore))
		BarrierWrite(semaphore, new(buffer) Semaphore);
}

#endif

END_UPP_NAMESPACE
