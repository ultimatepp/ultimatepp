#include <XmlRpc/XmlRpc.h>

using namespace Upp;

XMLRPC_METHOD(compute)
{
	double a, b;
	String op;
	rpc >> a >> op >> b;
	Cout() << "Request: " << a << op << b << '\n';
	if(op.GetCount() == 1)
	switch(*op) {
	case '+':
		rpc << a + b;
		return;
	case '-':
		rpc << a - b;
		return;
	case '/':
		if(b == 0)
			rpc << ErrorValue("division by zero");
		else
			rpc << a / b;
		return;
	case '*':
		rpc << a * b;
		return;
	}
	rpc << ErrorValue("unknown operator");
}

XMLRPC_METHOD(ping)
{
	rpc << GetSysTime();
}

namespace Upp { extern bool HttpClient_Trace__; }

CONSOLE_APP_MAIN
{
	Cout() << "Server..\n";
	XmlRpcServer(1234);
}
