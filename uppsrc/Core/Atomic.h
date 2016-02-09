#ifdef _MULTITHREADED

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
#elif defined(COMPILER_GCC)
#ifndef PLATFORM_WIN32
inline void ReadMemoryBarrier()
{
	__sync_synchronize();
}

inline void WriteMemoryBarrier()
{
	__sync_synchronize();
}
#endif
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

struct MtInspector;

#ifdef PLATFORM_WIN32

typedef LONG Atomic;

inline int  AtomicInc(volatile Atomic& t)             { return InterlockedIncrement((Atomic *)&t); }
inline int  AtomicDec(volatile Atomic& t)             { return InterlockedDecrement((Atomic *)&t); }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { return InterlockedExchangeAdd((Atomic *)&t, incr); }

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

#endif

inline int  AtomicRead(const volatile Atomic& t)      { return ReadWithBarrier(t); }
inline void AtomicWrite(volatile Atomic& t, int val)  { BarrierWrite(t, val); }

#else

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

#endif
