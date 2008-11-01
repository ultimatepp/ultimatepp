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
	TestSignals().Run();
}

