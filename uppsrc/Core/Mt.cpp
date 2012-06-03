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
	thread_id = 0;
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
		thread_id = 0;
	}
#elif defined(PLATFORM_POSIX)
	if(handle) {
		CHECK(!pthread_detach(handle));
		handle = 0;
	}
#endif
}

static Atomic sThreadCount;

static thread__  void (*sExit)(void);

void (*Thread::AtExit(void (*exitfn)()))()
{
	void (*prev)() = sExit;
	sExit = exitfn;
	return prev;
}

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
	if(sExit)
		(*sExit)();
#ifdef UPP_HEAP
	MemoryFreeThread();
#endif
	return 0;
}
#endif

static bool threadr; //indicates if *any* Thread instance is running (having called its Run()), upon first call of Run
#ifndef CPU_BLACKFIN
static thread__  bool sMain;
#else
#ifdef PLATFORM_POSIX
static Index<pthread_t> threadsv; //a threads id vector, sMain=true ==>> 'pthread_self() pthread_t beeing present in vector, problem, wont be cleared when thread exits
Mutex vm; //a common access synchronizer
#endif
#endif

//to sMain: an Application can start more than one thread, without having *any* one of them called Run() of any Thread instace
//when Run() is called *anytime*, it means, the term of *MainThread* has to be running anyway,
//otherwise no child threads could run. they are created by main.
//now each thread, having any Thread instace can start a first Run()


bool Thread::Run(Callback _cb)
{
	AtomicInc(sThreadCount);
	if(!threadr)
#ifndef CPU_BLACKFIN
		threadr = sMain = true;
#else
	{
		threadr = true;
		//the sMain replacement
#ifdef PLATFORM_POSIX
		pthread_t thid = pthread_self();
		vm.Enter();
		if(threadsv.Find(thid) < 0){
			//thread not yet present, mark present
			threadsv.Add(thid);
		}
		else
			RLOG("BUG: Multiple Add in Mt.cpp");
		vm.Leave();
#endif
	}
#endif
	Detach();
	Callback *cb = new Callback(_cb);
#ifdef PLATFORM_WIN32
	handle = (HANDLE)_beginthreadex(0, 0, sThreadRoutine, cb, 0, ((unsigned int *)(&thread_id)));
#endif
#ifdef PLATFORM_POSIX
	if(pthread_create(&handle, 0, sThreadRoutine, cb))
		handle = 0;
#endif
	return handle;
}

Thread::~Thread()
{
	Detach();
#ifdef CPU_BLACKFIN
#ifdef PLATFORM_POSIX
	//the static destruction replacement
	pthread_t thid = pthread_self();
	vm.Enter();
	int id = threadsv.Find(thid);
	if(id >= 0)
		threadsv.Remove(id);
	vm.Leave();
#endif
#endif
}

bool Thread::IsST() //the containing thread (of wich there may be multiple) has not run its Run() yet
{
	return !threadr;
}

bool Thread::IsMain() //the calling thread is the Main Thread or the only one in App
{
#ifndef CPU_BLACKFIN
	return !threadr || sMain;
#else
	if(!threadr) 
		return true;
#ifdef PLATFORM_POSIX
	//the sMain replacement
	pthread_t thid = pthread_self();
	vm.Enter();
	if(threadsv.Find(thid) >= 0)
	{
		vm.Leave();
		return true;
	}
	vm.Leave();
#endif
	return false;
#endif
}

int Thread::GetCount()
{
	return ReadWithBarrier(sThreadCount);
}

static volatile Atomic sShutdown = 0;

void Thread::ShutdownThreads()
{
	AtomicInc(sShutdown);
	while(AtomicRead(sThreadCount))
		Sleep(100);
	AtomicDec(sShutdown);
}

bool Thread::IsShutdownThreads()
{
	return AtomicRead(sShutdown);
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
	// ToDo
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
	::timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
#endif
}

#ifdef CPU_X86

#ifndef CPU_SSE2

static bool sSSE2 = false; //CPU_SSE2();

void ReadMemoryBarrier()
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

void Semaphore::Release(int n)
{
	ReleaseSemaphore(handle, n, NULL);
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

struct sCVWaiter_ {
	Semaphore   sem;
	sCVWaiter_ *next;
};
	
static thread__ byte sCVbuffer[sizeof(sCVWaiter_)];
static thread__ sCVWaiter_ *sCV;

void ConditionVariable::Wait(Mutex& m)
{
	{
		Mutex::Lock __(mutex);
		if(!sCV)
			sCV = new(sCVbuffer) sCVWaiter_;
		sCV->next = NULL;
		if(head)
			tail->next = sCV;
		else
			head = sCV;
		tail = sCV;
	}
	m.Leave();
	sCV->sem.Wait();
	m.Enter();
}

void ConditionVariable::Signal()
{
	Mutex::Lock __(mutex);
	if(head) {
		head->sem.Release();
		head = head->next;
	}
}

void ConditionVariable::Broadcast()
{
	Mutex::Lock __(mutex);
	while(head) {
		head->sem.Release();
		head = head->next;
	}
}

ConditionVariable::ConditionVariable()
{
	head = tail = NULL;
}

ConditionVariable::~ConditionVariable()
{
	Broadcast();
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

void StaticConditionVariable::Initialize()
{
	Mutex::Lock __(sMutexLock());
	if(!ReadWithBarrier(cv))
		BarrierWrite(cv, new(buffer) ConditionVariable);
}

void LazyUpdate::Invalidate()
{
	dirty = true;
}

bool LazyUpdate::BeginUpdate() const
{
	bool b = dirty;
	ReadMemoryBarrier();
	if(b) {
		mutex.Enter();
		if(dirty) return true;
		mutex.Leave();
	}
	return false;
}

void LazyUpdate::EndUpdate() const
{
	WriteMemoryBarrier();
	dirty = false;
	mutex.Leave();
}

LazyUpdate::LazyUpdate()
{
	dirty = true;
}

#endif

bool StartAuxThread(auxthread_t (auxthread__ *fn)(void *ptr), void *ptr)
{
#ifdef PLATFORM_WIN32
	HANDLE handle;
	handle = CreateThread(NULL, 0, fn, ptr, 0, NULL);
	if(handle) {
		CloseHandle(handle);
		return true;
	}
#endif
#ifdef PLATFORM_POSIX
	pthread_t handle;
	if(pthread_create(&handle, 0, fn, ptr) == 0) {
		pthread_detach(handle);
		return true;
	}
#endif
	return false;
}

END_UPP_NAMESPACE
