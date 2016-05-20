#include <Core/Core.h>
// Note: Core.h needs to be included before Rpc.h because of Win32 rpc.h name clash problem
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

RPC_METHOD(end_test)
{
	rpc << "OK";
	Cout() << "Immediate reply\n";
	rpc.EndRpc();
	Sleep(2000);
	Cout() << "Method ended now\n";
}

CONSOLE_APP_MAIN
{
	Cout() << "Server..\n";
	LogRpcRequests();
	RpcServerLoop(1234);
}
