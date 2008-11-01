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
	// not(yet) stored instance on file
	StoredInstance = false;
	
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
	
	// if process had stored instance on file, removes it
	if(StoredInstance)
		FileDelete(CreateLockName());
	StoredInstance = false;
	
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
// creates lock file name based on user name, app name and display
String TSignals::CreateLockName(void)
{
	if(LockName == "")
		LockName = "/tmp/" + Environment().Get("USER") + "_" + GetExeTitle() + "_" + Environment().Get("DISPLAY") + ".lck";
	
	return LockName;

} // END TSignals::CreateLockName()

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
// stores process info on filesystem
// allowing other process to communicate with it
bool TSignals::StoreInstance(void)
{
	// if already stored, does nothing
	if(StoredInstance)
		return true;
	
	// first, checks if another instance is running
	// if yes returns false (can't store more than 1 instance)
	if(IsOtherInstanceRunning())
		return false;
	
	// creates lock file
	FileOut f(CreateLockName());
	
	// stores current process id on lock file
	f.PutLine(AsString(getpid()));
	f.Close();
	
	// marks instance as already stored
	StoredInstance = true;
	
	// all ok
	return true;
	
} // END TSignals::StoreInstance()

////////////////////////////////////////////////////////////////////////////////////
// checks whether another instance of this process is running
bool TSignals::IsOtherInstanceRunning(void)
{
	// if we stored THIS instance, no other is running
	if(StoredInstance)
		return false;
	
	// otherwise, check the lock file
	if(FileExists(CreateLockName()))
		return true;
	return false;
	
} // END TSignals::IsOtherInstanceRunning()

////////////////////////////////////////////////////////////////////////////////////
// sends a signal to another process
bool TSignals::Send(int sig, int procId)
{
	return (kill(procId, sig) == 0);
	
} // END TSignals::Send()

////////////////////////////////////////////////////////////////////////////////////
// sends a signal to another running instance of this app
bool TSignals::Send(int sig)
{
	// if no other instance running, returns false
	if(!IsOtherInstanceRunning())
		return false;
	
	// gets pid of other instance
	FileIn f(CreateLockName());
	int pid = atoi(f.GetLine());
	f.Close();
	
	// sends the signal to process
	return Send(sig, pid); 
	
} // END TSignals::Send()

////////////////////////////////////////////////////////////////////////////////////
// Global signals access
TSignals &Signals(void)
{
	if(_Signals.IsEmpty())
		_Signals = new TSignals;
	
	return *_Signals;

} // END Signals()
