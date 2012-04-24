#include <XmlRpc/XmlRpc.h>

using namespace Upp;

struct FoxResult {
	bool   flerror;
	double amount;
	String message;
	
	void Map(ValueMapper& m) {
		m("flerror", flerror)("amount", amount)("message", message);
	}
};

XMLRPC_STRUCT(FoxResult)

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
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

	FoxResult r;
	if(XmlRpcRequest("http://foxrate.org/rpc").Method("foxrate.currencyConvert")
		  << "USD" << "GBP" << 120
		  >> r)
		if(r.flerror)
			Cout() << "Server reported error\n";
		else
			Cout() << r.amount << '\n';
	else 
		Cout() << "Failed.\n";
}
