#include <CtrlLib/CtrlLib.h>

using namespace Upp;

int alpha;

void SecondThread()
{
	for(;;)
		alpha++;
}

GUI_APP_MAIN
{
	String x = "asdfasdfkjasdfkl alsjd fl hdasflk hdasfkl;jahs dlfkjh alsfhj la flf";
	Thread d;
	d.Run(callback(SecondThread));
	PromptOK("test");
	__BREAK__;
	PromptOK("test");
}
