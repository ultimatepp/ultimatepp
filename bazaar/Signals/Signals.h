#ifndef _Signals_Signals_h_
#define _Signals_Signals_h_

#include <CtrlCore/CtrlCore.h>

using namespace Upp;

void _SignalHandler(int sig, siginfo_t *info, void *v);

class TSignals
{
	friend TSignals &Signals(void);
	friend void ::_SignalHandler(int, siginfo_t *, void *);
	
	private:
	
		// flag indicating that the process instance has been stored on file
		bool StoredInstance;
		String LockName;
		
		// signal/handlers map
		ArrayMap<int, Callback> CallbackMap;
		
		// costruttore
		TSignals();
		
		// dispatch signals
		void Dispatch(int sig, siginfo_t *info, void *v);
		
		// creates lock file name based on user name, app name and display
		String CreateLockName(void);
		
	public:

		// distruttore
		~TSignals();
	
		// add a signal handler
		bool Handle(int sig, Callback const &cb);

		// removes a signal handler
		bool Unhandle(int sig);
		
		// stores process info on filesystem
		// allowing other process to communicate with it
		bool StoreInstance(void);
		
		// checks whether another instance of this process is running
		bool IsOtherInstanceRunning(void); 

		// sends a signal to another process
		bool Send(int sig, int procId);
		
		// sends a signal to another running instance of this app
		bool Send(int sig);
		
}; // END Class TSignals

////////////////////////////////////////////////////////////////////////////////////
// Global signals access
TSignals &Signals(void);


#endif
