#include "TestSignals.h"

#include <Signals/Signals.h>

////////////////////////////////////////////////////////////////////////////////////
// handler for SIGHUP signal
void TestSignals::onHup(void)
{
	InfoEdit.Set(InfoEdit.Get() + "SIGHUP signal received\n");
	
} // END TestSignals::onHup()

////////////////////////////////////////////////////////////////////////////////////
// handler for SIGQUIT signal
void TestSignals::onQuit(void)
{
	InfoEdit.Set(InfoEdit.Get() + "SIGQUIT signal received\n");
	
} // END TestSignals::onQuit


////////////////////////////////////////////////////////////////////////////////////
TestSignals::TestSignals()
{
	// gets current process id to show usage
	int pid = getpid();
	UsageLabel = "Usage : open a terminal and do \"kill -HUP " + AsString(pid) +"\" or \"kill -QUIT " + AsString(pid) + "\"";
	
	// clear info box
	InfoEdit.Clear();
	
	// sets up signal handlers
	Signals().Handle(SIGHUP, THISBACK(onHup));
	Signals().Handle(SIGQUIT, THISBACK(onQuit));
	
	// sets up layout
	CtrlLayout(*this, "Posix signal handling demo");
}

GUI_APP_MAIN
{
	// first checks if another app instance is running
	if(Signals().IsOtherInstanceRunning())
	{
		// sends to other instance a SIGHUP signal
		Signals().Send(SIGHUP);
		
		// prompt and exit
		PromptOK("Another app instance is already running!!!");
		return;
	}
	
	// ok, no other instance, signals this one
	Signals().StoreInstance();
	
	// runs the application
	TestSignals().Run();
}
