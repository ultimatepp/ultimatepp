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

void Compute(double a, String op, double b)
{
	double result;
	Cout() << a << op << b << '=';
	XmlRpcCall call(NULL);
	if(call("compute", a, op, b) >> result)
	   	Cout() << result;
	else
		Cout() << " error: " << call.GetError();
	Cout() << '\n';
}

CONSOLE_APP_MAIN
{
	Time tm;
	XmlRpcCall(NULL)("ping") >> tm;
	Cout() << tm << '\n';	

	Compute(12, "+", 12);
	Compute(12, "*", 12);
	Compute(12, "+56", 12);
	Compute(12, "/", 0);
}
