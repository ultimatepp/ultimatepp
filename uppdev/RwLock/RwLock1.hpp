struct RwLock {
	Mutex     write_mutex;
	Semaphore writer_wait;
	
	enum { MVAL = 1000000 };

	Atomic    count;
	void DecReaders();

	void EnterRead();
	void LeaveRead();
	void EnterWrite();
	void LeaveWrite();

	RwLock() {
		count = MVAL;
	}
};

void RwLock::EnterWrite()
{
	ASSERT(count >= 0);
	LLOG("EnterWrite");
	write_mutex.Enter();
	if(AtomicXAdd(count, -MVAL) > MVAL)
		writer_wait.Wait();
	ASSERT(count >= 0);
}

void RwLock::LeaveWrite()
{
	ASSERT(count >= 0);
	LLOG("LeaveWrite");
	AtomicXAdd(count, MVAL);
	write_mutex.Leave();
	ASSERT(count >= 0);
	LLOG(".LeaveWrite");
}

void RwLock::DecReaders()
{
	ASSERT(count >= 0);
	if(AtomicDec(count) == 0)
		writer_wait.Release();
	ASSERT(count >= 0);
}

void RwLock::EnterRead()
{
	ASSERT(count >= 0);
	LLOG("EnterRead");
	if(AtomicInc(count) < MVAL) {
		DecReaders();
		write_mutex.Enter();
		AtomicInc(count);
		write_mutex.Leave();
	}
	ASSERT(count >= 0);
	LLOG(".EnterRead");
}

void RwLock::LeaveRead()
{
	LLOG("LeaveRead");
	DecReaders();
	LLOG(".LeaveRead");
}
