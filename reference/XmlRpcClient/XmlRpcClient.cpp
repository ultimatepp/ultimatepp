#include <Core/Core.h>
// Note: Core.h needs to be included before Rpc.h because of Win32 rpc.h name clash problem
#include <Core/Rpc/Rpc.h>

using namespace Upp;

void Compute(double a, String op, double b)
{
	double result;
	Cout() << a << op << b << '=';
	XmlRpcRequest call("127.0.0.1:1234");
	if(call("compute", a, op, b) >> result)
		Cout() << result;
	else
		Cout() << " error: " << call.GetError();
	Cout() << '\n';
}

CONSOLE_APP_MAIN
{
	Time tm;
	XmlRpcRequest("127.0.0.1:1234")("ping") >> tm;
	Cout() << tm << '\n';

	Compute(12, "+", 12);
	Compute(12, "*", 12);
	Compute(12, "+56", 12);
	Compute(12, "/", 0);
	
	String res;
	JsonRpcRequest("127.0.0.1:1234")("end_test") >> res;
	Cout() << res << '\n';
}
