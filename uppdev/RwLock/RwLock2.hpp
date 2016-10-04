struct RwLock {
	Mutex     lock_mutex;
	Mutex     write_mutex;
	Semaphore writer_wait;

	int       readers;
	bool      waiting_writer;

	void EnterRead();
	void LeaveRead();
	void EnterWrite();
	void LeaveWrite();

	RwLock() {
		readers = 0;
		waiting_writer = false;
	}
};

void RwLock::EnterWrite()
{
	LLOG("EnterWrite");
	write_mutex.Enter();
	LLOG("EnterWrite:write_mutex.Enter();");
	lock_mutex.Enter();
	ASSERT(!waiting_writer);
	waiting_writer = true;
	if(readers) {
		LLOG("EnterWrite:if(readers)");
		lock_mutex.Leave();
		writer_wait.Wait();
		lock_mutex.Enter();
		LLOG("EnterWrite:writer_wait.Wait();");
	}
	waiting_writer = false;
	lock_mutex.Leave();
	LLOG(".EnterWrite");
}

void RwLock::LeaveWrite()
{
	LLOG("LeaveWrite");
	lock_mutex.Enter();
	lock_mutex.Leave();
	write_mutex.Leave();
	LLOG(".LeaveWrite");
}

void RwLock::EnterRead()
{
	LLOG("EnterRead");
	write_mutex.Enter();
	lock_mutex.Enter();
	readers++;
	lock_mutex.Leave();
	write_mutex.Leave();
	LLOG(".EnterRead");
}

void RwLock::LeaveRead()
{
	LLOG("LeaveRead");
	lock_mutex.Enter();
	LLOG("LeaveRead:lock_mutex.Enter();");
	if(--readers == 0 && waiting_writer) {
		LLOG("LeaveRead:if(--readers == 0 && writer)");
		ASSERT(readers == 0);
		writer_wait.Release();
		LLOG("LeaveRead:writer_wait.Release();");
	}
	LLOG("LeaveRead:--readers");
	lock_mutex.Leave();
	LLOG(".LeaveRead");
}
