#ifndef _Signals_Signals_h_
#define _Signals_Signals_h_

#include <CtrlCore/CtrlCore.h>

using namespace Upp;

class TSignals
{
	friend TSignals &Signals(void);
	friend void ::_SignalHandler(int, siginfo_t *, void *);
	
	private:
		// signal/handlers map
		ArrayMap<int, Callback> CallbackMap;
		
		// costruttore
		TSignals();
		
		// dispatch signals
		void Dispatch(int sig, siginfo_t *info, void *v);
		
	public:

		// distruttore
		~TSignals();
	
		// add a signal handler
		bool Handle(int sig, Callback const &cb);

		// removes a signal handler
		bool Unhandle(int sig);

}; // END Class TSignals

////////////////////////////////////////////////////////////////////////////////////
// Global signals access
TSignals &Signals(void);


#endif
