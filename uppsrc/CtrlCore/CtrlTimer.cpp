#include "CtrlCore.h"

NAMESPACE_UPP

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
	TimeEvent *list = tevents();
	TimeEvent *e;
	for(e = list->GetNext(); e != list && ((int)(time - e->time) >= 0); e = e->GetNext());
	TimeEvent *ne = e->InsertPrev();
	ne->time = time;
	ne->cb = cb;
	ne->delay = delay;
	ne->id = id;
	ne->rep = false;
}

void SetTimeCallback(int delay_ms, Callback cb, void *id) {
	Mutex::Lock __(sTimerLock);
	ASSERT(abs(delay_ms) < 0x40000000);
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
	sTClick = time;
	sTimerLock.Leave();
	Ctrl::CheckMouseCtrl();
	Ctrl::SyncCaret();
	sTimerLock.Enter();
	TimeEvent *e = list->GetNext();
	while(e != list && ((int)(time - e->time)) > 0 && !e->rep) {
		TimeEvent *w = e;
		e = e->GetNext();
		w->Unlink();
		eventid++;
		Callback cb = w->cb;
		if(w->delay < 0) {
			w->rep = true;
			w->LinkBefore(tevents());
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
		delete w;
	}
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

void   Ctrl::EndLoop()
{
	GuiLock __;
	inloop = false;
}

void   Ctrl::EndLoop(int code)
{
	GuiLock __;
	ASSERT(!parent);
	exitcode = code;
	EndLoop();
}

bool   Ctrl::InLoop() const
{
	GuiLock __;
	return inloop;
}

bool   Ctrl::InCurrentLoop() const
{
	GuiLock __;
	return GetLoopCtrl() == this;
}

int    Ctrl::GetExitCode() const
{
	GuiLock __;
	return exitcode;
}

END_UPP_NAMESPACE
