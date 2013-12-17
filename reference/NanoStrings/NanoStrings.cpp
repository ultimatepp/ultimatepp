#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	NanoStrings ns;
	Vector<dword> ws;
	
	for(int i = 0; i < 10; i++)
		ws.Add(ns.Add(AsString(i * i)));
	
	for(int i = 0; i < ws.GetCount(); i++)
		LOG(ns.Get(ws[i]));
}
