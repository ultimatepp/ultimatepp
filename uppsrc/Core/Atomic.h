#ifdef _MULTITHREADED

void ReadMemoryBarrier();
void WriteMemoryBarrier();

#ifdef CPU_SSE2
inline void ReadMemoryBarrier()
{
	_mm_lfence();
}

inline void WriteMemoryBarrier() {
	_mm_sfence();
}
#else
inline void ReadMemoryBarrier()
{
	__sync_synchronize();
}

inline void WriteMemoryBarrier()
{
	__sync_synchronize();
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

struct MtInspector;
/*
typedef LONG Atomic;

inline int  AtomicInc(volatile Atomic& t)             { return InterlockedIncrement((Atomic *)&t); }
inline int  AtomicDec(volatile Atomic& t)             { return InterlockedDecrement((Atomic *)&t); }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { return InterlockedExchangeAdd((Atomic *)&t, incr); }
*/

typedef std::atomic<int> Atomic;

inline int  AtomicInc(volatile Atomic& t)             { return ++t; }
inline int  AtomicDec(volatile Atomic& t)             { return --t; }
/*
inline int  AtomicRead(const volatile Atomic& t)      { return ReadWithBarrier(t); }
inline void AtomicWrite(volatile Atomic& t, int val)  { BarrierWrite(t, val); }
*/
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
