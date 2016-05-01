#ifdef _MULTITHREADED

typedef std::atomic<int> Atomic;

inline int  AtomicInc(volatile Atomic& t)             { return ++t; }
inline int  AtomicDec(volatile Atomic& t)             { return --t; }

#else

typedef int Atomic;

inline int  AtomicInc(volatile Atomic& t)             { ++t; return t; }
inline int  AtomicDec(volatile Atomic& t)             { --t; return t; }

#endif
