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

#define XRSFILE <XmlRpcCall/FoxRate.xrs>
#include <XmlRpc/Xrs.h>

CONSOLE_APP_MAIN
{
	FoxResult r;
	if(XmlRpcCall("http://foxrate.org/rpc")
		  .Method("foxrate.currencyConvert")
		  << "USD" << "GBP" << 120
		  >> r)
		if(r.flerror)
			Cout() << "Server reported error\n";
		else
			Cout() << r.amount << '\n';
	else 
		Cout() << "Failed.\n";

	FoxResult2 r2;
	XmlRpcCall foxrate("http://foxrate.org/rpc");
	if(foxrate("foxrate.currencyConvert", "GBP", "USD", 130.0) >> r2)
		if(r2.flerror)
			Cout() << "Server reported error\n";
		else
			Cout() << r2.amount << '\n';
	else
		Cout() << "Failed: " << foxrate.GetError() << '\n';
}
