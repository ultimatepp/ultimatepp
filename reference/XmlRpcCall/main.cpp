#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	LogRpcRequests();
	XmlRpcRequest xr("http://foxrate.org/rpc");
	xr.Method("foxrate.currencyConvert") << "USD" << "GBP" << 120;
	Value v = xr.Execute();
	if(v.IsError())
		LOG("Error: " << v);
	else
		if(v["flerror"] == 0)
			LOG(v["amount"]);
		else
			LOG("Failed.");
		
	v = XmlRpcRequest("http://foxrate.org/rpc")("foxrate.currencyConvert", "EUR", "USD", 10)
	    .Execute();
	if(v.IsError())
		LOG("Error: " << v);
	else
		if(v["flerror"] == 0)
			LOG(v["amount"]);
		else
			LOG("Failed.");
}
