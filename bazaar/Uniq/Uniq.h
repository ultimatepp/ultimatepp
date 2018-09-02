#ifndef _Uniq_h
#define _Uniq_h

#include <CtrlLib/CtrlLib.h>

namespace Upp {

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

#ifdef PLATFORM_POSIX
		// lock file to test for pipe existence
		String lockPath;
		int lockFile;

		// send command line to callback handler
		bool SendCmdLine(int pipe);

		// polling callback -- either threaded or
		// run by timed callback
		void pollCb(void);

#else

		HANDLE pipe;
		OVERLAPPED overlapped;
		HANDLE event;
		
		// send command line to callback handler
		bool SendCmdLine(void);

		// polling callback -- either threaded or
		// run by timed callback
		void pollCb(void);

#endif

	
	protected:
	
	public:
	
	typedef Uniq CLASSNAME;
	
	// callback called when another app instance is run
	Event<const Vector<String>&> WhenInstance;
	
	Uniq();
	
	~Uniq();
	
	// checks whether we're inside main instance
	bool IsFirstInstance(void) { return isFirstInstance; }
	operator bool() { return isFirstInstance; }
};

}; // end upp namespace

#endif

