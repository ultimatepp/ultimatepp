#include "CtrlCore.h"

namespace Upp {

// #define LOG_QUEUE
#define LLOG(x) // LOG(x)
// #define DELAY_WATCH  1000 _DBG_

int MemoryProbeInt;

struct TimeEvent : public Link<> {
	dword      time;
	int        delay;
	Event<>    cb;
	void      *id;
	bool       rep;
};

static dword sTClick;

static StaticCriticalSection sTimerLock;

struct TimeEventsOwner__ : Link<> {
	~TimeEventsOwner__() {
		while(GetNext() != this)
			delete (TimeEvent *)GetNext();
	}
};

static Link<> *tevents() {
	static TimeEventsOwner__ t[1];
	return t;
}

static void sTimeCallback(dword time, int delay, Event<>  cb, void *id) {
	TimeEvent *e = new TimeEvent;
	e->LinkBefore(tevents());
	e->time = time;
	e->cb = cb;
	e->delay = delay;
	e->id = id;
	e->rep = false;
	LLOG("sTimeCallback " << ne->time << " " << ne->delay << " " << ne->id);
}

void SetTimeCallback(int delay_ms, Function<void ()> cb, void *id) {
	Mutex::Lock __(sTimerLock);
	ASSERT(abs(delay_ms) < 0x40000000);
	LLOG("SetTimeCallback " << delay_ms << " " << id);
	sTimeCallback(msecs() + abs(delay_ms), delay_ms, Event<> () << cb, id);
}

void KillTimeCallbacks(void *id, void *idlim) {
	Mutex::Lock __(sTimerLock);
	Link<> *list = tevents();
	Link<> *le = list->GetNext();
	while(le != list) {
		TimeEvent *e = (TimeEvent *)le;
		le = le->GetNext();
		if(e->id >= id && e->id < idlim)
			delete e;
	}
}

EXITBLOCK
{
	Mutex::Lock __(sTimerLock);
	while(tevents()->GetNext() != tevents())
		delete tevents()->GetNext();
}

bool ExistsTimeCallback(void *id) {
	Mutex::Lock __(sTimerLock);
	Link<> *list = tevents();
	for(Link<> *le = list->GetNext(); le != list; le = le->GetNext()) {
		TimeEvent *e = (TimeEvent *)le;
		if(e->id == id)
			return true;
	}
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
	Link<> *list = tevents();
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
		for(Link<> *le = list->GetNext(); le != list; le = le->GetNext()) {
			TimeEvent *e = (TimeEvent *)le;
			LLOG("TP " << e->time << " " << e->delay << " " << e->id << " " << e->rep);
		}
		LLOG("----");
	#endif

	for(;;) {
		TimeEvent *todo = NULL;
		int maxtm = -1;

		for(Link<> *le = list->GetNext(); le != list; le = le->GetNext()) {
			TimeEvent *e = (TimeEvent *)le;
			int tm = (int)(time - e->time);
			if(!e->rep && tm >= 0 && tm > maxtm) {
				maxtm = tm;
				todo = e;
			}
		}
		if(!todo)
			break;
		LLOG("Performing " << todo->time << " " << todo->delay << " " << todo->id);
		Event<>  cb = todo->cb;
		if(todo->delay < 0)
			todo->rep = true;
		else
			delete todo;
		sTimerLock.Leave();
	#if DELAY_WATCH
		int tm = msecs();
	#endif
		cb();
	#if DELAY_WATCH
		if(msecs() - tm > DELAY_WATCH)
			Panic("Long timer procedure detected!");
	#endif
		sTimerLock.Enter();
	}
	time = msecs();
	LLOG("--- Rescheduling at " << time);
	Link<> *le = list->GetNext();
	while(le != list) {
		TimeEvent *w = (TimeEvent *)le;
		le = le->GetNext();
		if(w->rep) {
			LLOG("Rescheduling " << e->id);
			sTimeCallback(time - w->delay, w->delay, w->cb, w->id);
			delete w;
		}
	}
	LLOG("----");
	sTimerLock.Leave();
	FullRefreshCleanup();
}

void  Ctrl::InitTimer()
{
	Mutex::Lock __(sTimerLock);
	tevents();
}

void  Ctrl::SetTimeCallback(int delay_ms, Function<void ()> cb, int id) {
	ASSERT(id >= 0 && (size_t)id < (int)sizeof(Ctrl));
	UPP::SetTimeCallback(delay_ms, cb, (byte *)this + id);
}

void  Ctrl::KillTimeCallback(int id) {
	ASSERT(id >= 0 && (size_t)id < sizeof(Ctrl));
	UPP::KillTimeCallback((byte *)this + id);
}

void  Ctrl::KillSetTimeCallback(int delay_ms, Function<void ()> cb, int id)
{
	KillTimeCallback(id);
	SetTimeCallback(delay_ms, cb, id);
}

void WakeUpGuiThread();

void  Ctrl::PostCallback(Function<void ()> cb, int id)
{
	SetTimeCallback(0, cb, id);
	WakeUpGuiThread();
}

void  Ctrl::KillPostCallback(Function<void ()> cb, int id)
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

}