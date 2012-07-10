#include <Core/Rpc/Rpc.h>

using namespace Upp;

RPC_METHOD(compute)
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

RPC_METHOD(ping)
{
	rpc << GetSysTime();
}

CONSOLE_APP_MAIN
{
	Cout() << "Server..\n";
	LogRpcRequests();
	RpcServerLoop(1234);
}
