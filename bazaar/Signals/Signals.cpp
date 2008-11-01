#include "Signals.h"


////////////////////////////////////////////////////////////////////////////////////
// global Signals object
static One<TSignals> _Signals;

////////////////////////////////////////////////////////////////////////////////////
// global signal handler
void _SignalHandler(int sig, siginfo_t *info, void *v)
{
	Signals().Dispatch(sig, info, v);
	
} // END ::_SignalHandler()

////////////////////////////////////////////////////////////////////////////////////
// constructor
TSignals::TSignals()
{
	
} // END Constructor TSignals
		
////////////////////////////////////////////////////////////////////////////////////
// destructor
TSignals::~TSignals()
{
	// removes all handlers
	for(int i = 0; i < CallbackMap.GetCount(); i++)
	{
		int sig = CallbackMap.GetKey(i);
		struct sigaction act;
		act.sa_flags = SA_SIGINFO;
		act.sa_sigaction = (void(*)(int, siginfo_t *, void *))SIG_DFL;
		sigaction(sig, &act,(struct sigaction *)NULL);
	}
	
	// empties the map
	CallbackMap.Clear();
	
} // END Destructor class TSignals
	
////////////////////////////////////////////////////////////////////////////////////
// dispatch signals
void TSignals::Dispatch(int sig, siginfo_t *info, void *v)
{
	// gets callback, returns if none found (should not happen)
	int i = CallbackMap.Find(sig);
	if(i < 0)
		return;
	
	// posts the callback to event loop
	Ctrl::GetActiveWindow()->PostCallback(CallbackMap[i]);
	
} // END TSignals::Dispatcher()
		
////////////////////////////////////////////////////////////////////////////////////
// add a signal handler
bool TSignals::Handle(int sig, Callback const &cb)
{
	// adds/replaces the handler
	int i;
	if( (i = CallbackMap.Find(sig)) >= 0)
		CallbackMap[sig] = cb;
	else
		CallbackMap.Add(sig, cb);
	
	// if needed, hooks the signal to global signal handler
	if(i < 0)
	{
		struct sigaction act;
		act.sa_flags = SA_SIGINFO;
		act.sa_sigaction = _SignalHandler;
		if(sigaction(sig, &act,(struct sigaction *)NULL))
		{
			// an error occurred, just remove the handler
			// from map and return false
			CallbackMap.RemoveKey(sig);
			return false;
		}
		else
			return true;
	}
	
	return true;
	
} // END TSignals::Handle()

////////////////////////////////////////////////////////////////////////////////////
// removes a signal handler
bool TSignals::Unhandle(int sig)
{
	// locates signal in map, error if not found
	int i = CallbackMap.Find(sig);
	if(i < 0)
		return false;
	
	// unhooks signal from global signal handler
	struct sigaction act;
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = (void(*)(int, siginfo_t *, void *))SIG_DFL;
	sigaction(sig, &act,(struct sigaction *)NULL);
	
	// removes signal from map
	CallbackMap.Remove(i);
	
	return true;
	
} // END TSignals::Unhandle()

////////////////////////////////////////////////////////////////////////////////////
// Global signals access
TSignals &Signals(void)
{
	if(_Signals.IsEmpty())
		_Signals = new TSignals;
	
	return *_Signals;

} // END Signals()
