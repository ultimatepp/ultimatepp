#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

using namespace Upp;

RPC_METHOD(Multiply)
{
	int m, n;
	rpc >> m >> n;
	rpc << m * n;
}

RPC_METHOD(Multiply2)
{
	int m = rpc++;
	int n = rpc++;
	rpc = m * n;
}

RPC_METHOD(Divide)
{
	int m = rpc++;
	int n = rpc++;
	if(n == 0)
		ThrowRpcError("divide by zero");
	rpc = m / n;
}

RPC_METHOD(MultiplyNamed)
{
	int n = rpc["first"];
	int m = rpc["second"];
	rpc = m * n;
}

RPC_METHOD(JsonMultiply)
{
	Value v = rpc++;
	rpc = Json("result", (int)v["first"] * (int)v["second"]);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
//	LogXmlRpcRequests();
//	SetXmlRpcServerTrace(UppLog());

//  Working in "shortened" mode - without URL specified, RpcRequests are performed by methods in
//  the same process.

	DUMP(JsonRpcRequest()("Multiply", 2, 3).Execute());
	DUMP(JsonRpcRequest()("Multiply2", 4, 5).Execute());
	DUMP(XmlRpcRequest()("Multiply2", 6, 7).Execute());
	DUMP(JsonRpcRequestNamed()("MultiplyNamed")("first", 8)("second", 9).Execute());
	DUMP(JsonRpcRequest()("Divide", 2, 0).Execute());
	DUMP(JsonRpcRequest()("Divide", 20, 4).Execute());
	DUMP(JsonRpcRequest()("JsonMultiply", Json("first", 4)("second", 10)).Execute());
}
