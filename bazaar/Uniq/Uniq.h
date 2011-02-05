#ifndef _Uniq_Uniq_h
#define _Uniq_Uniq_h

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

class Uniq
{
	private:
	
		// flag indicating we're inside first instance
		bool isFirstInstance;
	
		// path of named pipe
		String pipePath;
		
#ifdef flagMT
		Thread pollThread;
#endif

		// send command line to callback handler
		bool SendCmdLine(int pipe);

		// polling callback -- either threaded or
		// run by timed callback
		void pollCb(void);
	
	protected:
	
	public:
	
	typedef Uniq CLASSNAME;
	
	// callback called when another app instance is run
	Callback1<Vector<String> const &> WhenInstance;
	
	Uniq();
	
	~Uniq();
	
	// checks whether we're inside main instance
	bool IsFirstInstance(void) { return isFirstInstance; }
	operator bool() { return isFirstInstance; }
};

END_UPP_NAMESPACE

#endif

