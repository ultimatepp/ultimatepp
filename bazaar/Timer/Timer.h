#ifndef _Timer_Timer_h_
#define _Timer_Timer_h_

#include <Core/Core.h>

using namespace Upp;

//runs a chained list of TimerEvent stored Callbacks after sleeping
class Timer
{
public:
	typedef Timer CLASSNAME;
	Timer();
	virtual ~Timer();
	
	//API just as in Ctrl
public:
	void    SetTimeCallback(int delay_ms, Callback cb, int id = 0);
	void    KillTimeCallback(int id = 0);
	void    KillSetTimeCallback(int delay_ms, Callback cb, int id);
	bool    ExistsTimeCallback(int id = 0);
	void    PostCallback(Callback cb, int id = 0);
	void    KillPostCallback(Callback cb, int id);

	void  SetTimerGranularity(int ms);

private:
	struct TimeEvent : public Link<TimeEvent> {
		dword      time;
		int        delay;
		Callback   cb;
		void      *id;
		bool       rep;
		
		unsigned GetHashValue() const { return GetPtrHashValue(id); }
	};

	void sTimeCallback(dword time, int delay, Callback cb, void *id);
	void SetTimeCallback(int delay_ms, Callback cb, void *id);
	void KillTimeCallbacks(void *id, void *idlim);
	bool ExistsTimeCallback(void *id);
	void KillTimeCallback(void *id);
	void TimerProc(dword time, int & leftsleep);

	void KillTimeCallbacks(void *id);


	void TimerThread();
	
	//DATA
private:
	Thread t;
	bool run;
	int granularity;
	
	Mutex sTimerLock;
	dword sTClick;
	
	LinkOwner<TimeEvent> elist; //elements
	//ArrayMap<unsigned, TimeEvent*> emap;
	//LinkOwner<TimeEvent> flist; //unused elements for reuse
};

#endif
