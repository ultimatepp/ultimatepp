#include "Timer.h"

//Timer

Timer::Timer()
{
	granularity = 10;
	run = true;
	t.Run(THISBACK(TimerThread));
}

Timer::~Timer()
{
	run = false;
	t.Wait();
	
	Mutex::Lock __(sTimerLock);
	while(elist.GetNext() != elist.GetPtr())
		delete elist.GetNext();
}

void Timer::sTimeCallback(dword time, int delay, Callback cb, void *id) {
	TimeEvent *list = elist.GetPtr();
	TimeEvent *e;
	for(e = list->GetNext(); e != list && ((int)(time - e->time) >= 0); e = e->GetNext());
	TimeEvent *ne = e->InsertPrev();
	ne->time = time;
	ne->cb = cb;
	ne->delay = delay;
	ne->id = id;
	ne->rep = false;
}

void Timer::SetTimeCallback(int delay_ms, Callback cb, void *id) {
	Mutex::Lock __(sTimerLock);
	ASSERT(abs(delay_ms) < 0x40000000);
	sTimeCallback(GetTickCount() + abs(delay_ms), delay_ms, cb, id);
}

void Timer::KillTimeCallbacks(void *id, void *idlim) {
	Mutex::Lock __(sTimerLock);
	TimeEvent *list = elist.GetPtr();
	for(TimeEvent *e = list->GetNext(); e != list;)
		if(e->id >= id && e->id < idlim) {
			e = e->GetNext();
			delete e->GetPrev();
		}
		else
			e = e->GetNext();
}
void Timer::KillTimeCallbacks(void *id) {
	Mutex::Lock __(sTimerLock);
	TimeEvent *list = elist.GetPtr();
	for(TimeEvent *e = list->GetNext(); e != list;)
		if(e->id == id) {
			e = e->GetNext();
			delete e->GetPrev();
		}
		else
			e = e->GetNext();
}

bool Timer::ExistsTimeCallback(void *id) {
	Mutex::Lock __(sTimerLock);
	TimeEvent *list = elist.GetPtr();
	for(TimeEvent *e = list->GetNext(); e != list; e = e->GetNext())
		if(e->id == id)
			return true;
	return false;
}

void Timer::KillTimeCallback(void *id) {
//	KillTimeCallbacks(id, (byte *)id + 1);
	KillTimeCallbacks(id);
}

#if 0
void Timer::TimerProc(dword time, int& leftsleep)
{
	if(IsPanicMode())
		return;
	sTimerLock.Enter();
	TimeEvent *list = elist.GetPtr();
	if(sTClick > time)
		for(TimeEvent *e = list->GetNext(); e != list; e = e->GetNext())
			if(e->time > 0x80000000)
				e->time = 0;
	sTClick = time;
//	sTimerLock.Leave();
//	//***
//	sTimerLock.Enter();
	while(list->GetNext() != list && ((leftsleep = (list->GetNext()->time - time)) <= 0)) {
		TimeEvent *e = list->GetNext();
		e->Unlink();
		sTimerLock.Leave();
		e->cb();
		sTimerLock.Enter();
		if(e->delay < 0) {
			dword t = GetTickCount();
			sTimeCallback(t + (t == time) - e->delay, e->delay, e->cb, e->id);
		}
		delete e;
	}
	sTimerLock.Leave();
	if(leftsleep < 0) leftsleep = granularity; //if last done has been processed and no more in queue, ensure good sleep
}
#else
void Timer::TimerProc(dword time, int& leftsleep)
{
	if(IsPanicMode())
		return;
	sTimerLock.Enter();
	TimeEvent *list = elist.GetPtr();;
	sTClick = time;

//	sTimerLock.Leave();
//	//***
//	sTimerLock.Enter();

	TimeEvent *e = list->GetNext();
	while(e != list && ((leftsleep = (e->time - (int)time)) <= 0) && !e->rep) {
//	while(e != list && ((int)(time - e->time)) >= 0 && !e->rep) {
		TimeEvent *w = e;
		e = e->GetNext();
		w->Unlink();
		//eventid++;
		Callback cb = w->cb;
		if(w->delay < 0) {
			w->rep = true;
			w->LinkBefore(elist.GetPtr());
		}
		else
			delete w;
		sTimerLock.Leave();
		cb();
		sTimerLock.Enter();
	}
	time = GetTickCount();
	e = list->GetPrev();
	while(e != list && e->rep) {
		TimeEvent *w = e;
		e = e->GetPrev();
		w->Unlink();
		sTimeCallback(time - w->delay, w->delay, w->cb, w->id);
		if((-w->delay) < leftsleep) leftsleep = -w->delay;
		delete w;
	}
	sTimerLock.Leave();
	if(leftsleep < 0) leftsleep = granularity; //if last done has been processed and no more in queue, ensure good sleep
}
#endif

//SAME API AS IN Ctrl
void  Timer::SetTimeCallback(int delay_ms, Callback cb, int id) {
//	ASSERT(id >= 0 && (size_t)id < (int)sizeof(Timer));
//	SetTimeCallback(delay_ms, cb, (byte *)this + id);
	SetTimeCallback(delay_ms, cb, (byte *)id);
}

void  Timer::KillTimeCallback(int id) {
//	ASSERT(id >= 0 && (size_t)id < sizeof(Timer));
//	KillTimeCallback((byte *)this + id);
	KillTimeCallback((byte *)id);
}

void  Timer::KillSetTimeCallback(int delay_ms, Callback cb, int id)
{
	KillTimeCallback(id);
	SetTimeCallback(delay_ms, cb, id);
}

void  Timer::PostCallback(Callback cb, int id)
{
	SetTimeCallback(0, cb, id);
}

void  Timer::KillPostCallback(Callback cb, int id)
{
	KillSetTimeCallback(0, cb, id);
}

bool  Timer::ExistsTimeCallback(int id) {
//	ASSERT(id >= 0 && (size_t)id < sizeof(Timer));
//	return ExistsTimeCallback((byte *)this + id);
	return ExistsTimeCallback((byte *)id);
}

void Timer::SetTimerGranularity(int ms)
{
	if(ms > 0)
		granularity = ms;
}

///

void Timer::TimerThread()
{
	int leftsleep;
	while(run)
	{
		leftsleep = granularity;
		TimerProc(GetTickCount(), leftsleep);
		Sleep(min(granularity, leftsleep));
	}
}