#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

using namespace Upp;

void GetClientList(int i)
{
	INTERLOCKED { LOG("=>> GetClientList(" << i << ")"); }
	
	ValueArray result;
	XmlRpcRequest call("127.0.0.1:1234");

	if(call("GetClientList") >> result)
	{
		INTERLOCKED	{ LOG("Message received " << i); }
	}
	else
	{
		INTERLOCKED { LOG("Error: " << call.GetError()); }
	}
	
	INTERLOCKED { LOG("<<= GetClientList(" << i << ")"); }
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	CoWork co;
	for(int i = 0; i < 20; i++)
	{
		co & callback1(GetClientList, i);
	}
	
}
