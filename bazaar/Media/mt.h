#ifndef _Media_mt_h_
#define _Media_mt_h_

// Classes thanks to U++ Forum supporters

// Safe mutex
template<class MUTEX>
class ScopedLock {
private:
	MUTEX& mutex;

private:
	// The following constructor/operators are expilictly FORBIDDEN because they have no meaning
	ScopedLock(void) {};
	ScopedLock(const ScopedLock&) {};
	ScopedLock& operator=(ScopedLock&) {return *this;};

public:
	inline ScopedLock(MUTEX& mut) : mutex(mut) 	{mutex.lock();}
	inline ~ScopedLock(void)					{mutex.unLock();}
};

/*
class MyMutex {
private:
	Mutex &mutex;

private:
	// The following constructor/operators are expilictly FORBIDDEN because they have no meaning
	//MyMutex(void) {};
	MyMutex(const MyMutex&) {};
	MyMutex& operator=(MyMutex&) {return *this;};

public:
	MyMutex(void) {
		int kk = 33;
	};
	inline MyMutex(Mutex& mut) : mutex(mut) 	{
			mutex.Enter();
		}
		inline ~MyMutex()					{
			mutex.Leave();
		}
};
*/
// Safe atomic
class AtomicVar {
	private:
		Atomic val;
	public:
		AtomicVar() {};
		
		AtomicVar(const AtomicVar& p) {AtomicWrite(val, AtomicRead(p.val));}
		template <class T>
		AtomicVar& operator=(const T& p) {AtomicWrite(val, p); return *this;}

		operator int() {return AtomicRead(val);}
};

#endif
