typedef std::atomic<int> Atomic;

inline int  AtomicInc(volatile Atomic& t)             { return ++t; }
inline int  AtomicDec(volatile Atomic& t)             { return --t; }
