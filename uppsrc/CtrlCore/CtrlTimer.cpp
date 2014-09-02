#include "CtrlCore.h"

NAMESPACE_UPP

// #define LOG_QUEUE
#define LLOG(x) // LOG(x)

int MemoryProbeInt;

struct TimeEvent : public Link<TimeEvent> {
	dword      time;
	int        delay;
	Callback   cb;
	void      *id;
	bool       rep;
};

static dword sTClick;

static StaticCriticalSection sTimerLock;

struct CtrlTimerOwner__ : public LinkOwner<TimeEvent> {
	CtrlTimerOwner__();
	~CtrlTimerOwner__();
};

static TimeEvent *tevents() {
	static LinkOwner<TimeEvent> t;
	return t.GetPtr();
}

static void sTimeCallback(dword time, int delay, Callback cb, void *id) {
	TimeEvent *ne = tevents()->InsertPrev();
	ne->time = time;
	ne->cb = cb;
	ne->delay = delay;
	ne->id = id;
	ne->rep = false;
	LLOG("sTimeCalllback " << ne->time << " " << ne->delay << " " << ne->id);
}

void SetTimeCallback(int delay_ms, Callback cb, void *id) {
	Mutex::Lock __(sTimerLock);
	ASSERT(abs(delay_ms) < 0x40000000);
	LLOG("SetTimeCallback " << delay_ms << " " << id);
	sTimeCallback(GetTickCount() + abs(delay_ms), delay_ms, cb, id);
}

void KillTimeCallbacks(void *id, void *idlim) {
	Mutex::Lock __(sTimerLock);
	TimeEvent *list = tevents();
	for(TimeEvent *e = list->GetNext(); e != list;)
		if(e->id >= id && e->id < idlim) {
			e = e->GetNext();
			delete e->GetPrev();
		}
		else
			e = e->GetNext();
}

EXITBLOCK
{
	Mutex::Lock __(sTimerLock);
	while(tevents()->GetNext() != tevents())
		delete tevents()->GetNext();
}

bool ExistsTimeCallback(void *id) {
	Mutex::Lock __(sTimerLock);
	TimeEvent *list = tevents();
	for(TimeEvent *e = list->GetNext(); e != list; e = e->GetNext())
		if(e->id == id)
			return true;
	return false;
}

void KillTimeCallback(void *id) {
	KillTimeCallbacks(id, (byte *)id + 1);
}

void Ctrl::TimerProc(dword time)
{
	if(IsPanicMode())
		return;
	sTimerLock.Enter();
	TimeEvent *list = tevents();
	if(time == sTClick) {
		sTimerLock.Leave();
		return;
	}
	sTClick = time;
	sTimerLock.Leave();
	Ctrl::CheckMouseCtrl();
	Ctrl::SyncCaret();
	sTimerLock.Enter();

	#ifdef LOG_QUEUE
		LLOG("--- Timer queue at " << time);
		for(TimeEvent *e = list->GetNext(); e != list; e = e->GetNext())
			LLOG("TP " << e->time << " " << e->delay << " " << e->id << " " << e->rep);
		LLOG("----");
	#endif

	for(;;) {
		TimeEvent *todo = NULL;
		int maxtm = -1;
		for(TimeEvent *e = list->GetNext(); e != list; e = e->GetNext()) {
			int tm = (int)(time - e->time);
			if(!e->rep && tm >= 0 && tm > maxtm) {
				maxtm = tm;
				todo = e;
			}
		}
		if(!todo)
			break;
		LLOG("Performing " << todo->time << " " << todo->delay << " " << todo->id);
		Callback cb = todo->cb;
		if(todo->delay < 0)
			todo->rep = true;
		else
			delete todo;
		sTimerLock.Leave();
		cb();
		sTimerLock.Enter();
	}
	time = GetTickCount();
	LLOG("--- Rescheduling at " << time);
	TimeEvent *e = list->GetNext();
	while(e != list) {
		TimeEvent *w = e;
		e = e->GetNext();
		if(w->rep) {
			LLOG("Rescheduling " << e->id);
			sTimeCallback(time - w->delay, w->delay, w->cb, w->id);
			delete w;
		}
	}
	LLOG("----");
	sTimerLock.Leave();
}

void  Ctrl::InitTimer()
{
	Mutex::Lock __(sTimerLock);
	tevents();
}

void  Ctrl::SetTimeCallback(int delay_ms, Callback cb, int id) {
	ASSERT(id >= 0 && (size_t)id < (int)sizeof(Ctrl));
	UPP::SetTimeCallback(delay_ms, cb, (byte *)this + id);
}

void  Ctrl::KillTimeCallback(int id) {
	ASSERT(id >= 0 && (size_t)id < sizeof(Ctrl));
	UPP::KillTimeCallback((byte *)this + id);
}

void  Ctrl::KillSetTimeCallback(int delay_ms, Callback cb, int id)
{
	KillTimeCallback(id);
	SetTimeCallback(delay_ms, cb, id);
}

void  Ctrl::PostCallback(Callback cb, int id)
{
	SetTimeCallback(0, cb, id);
}

void  Ctrl::KillPostCallback(Callback cb, int id)
{
	KillSetTimeCallback(0, cb, id);
}

bool  Ctrl::ExistsTimeCallback(int id) const {
	ASSERT(id >= 0 && (size_t)id < sizeof(Ctrl));
	return UPP::ExistsTimeCallback((byte *)this + id);
}

dword GetTimeClick()
{
	return sTClick;
}

END_UPP_NAMESPACE
