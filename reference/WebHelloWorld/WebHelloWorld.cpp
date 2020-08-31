#ifdef flagTURTLEGUI
#include <Turtle/Turtle.h>
#else
#include <CtrlLib/CtrlLib.h>
#endif

// Try connecting to "localhost:8888" via your web browser.

using namespace Upp;

void AppMainLoop()
{
	PromptOK("Hello, world!");
}

#ifdef flagTURTLEGUI

CONSOLE_APP_MAIN
{

#ifdef _DEBUG
	TurtleServer::DebugMode();
#endif

	// MemoryLimitKb(100000000); // Can aid preventing DDoS attacks.

	TurtleServer guiserver;
	guiserver.Host("localhost");
	guiserver.Port(8888);
	guiserver.MaxConnections(100);
	RunTurtleGui(guiserver, AppMainLoop);
}

#else

GUI_APP_MAIN
{
	AppMainLoop();
}

#endif

