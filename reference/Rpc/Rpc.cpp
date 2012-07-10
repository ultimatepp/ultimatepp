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

RPC_METHOD(MultiplyNamed)
{
	int n = rpc["first"];
	int m = rpc["second"];
	rpc = m * n;
}

CONSOLE_APP_MAIN
{
//	LogXmlRpcRequests();
//	SetXmlRpcServerTrace(UppLog());
//  Working in "shortened" mode - without URL specified, RpcRequests are performed by methods in
//  the same process.

	DUMP(JsonRpcRequest()("Multiply", 2, 3).Execute());
	DUMP(JsonRpcRequest()("Multiply2", 4, 5).Execute());
	DUMP(XmlRpcRequest()("Multiply2", 6, 7).Execute());
	DUMP(JsonRpcRequestNamed()("MultiplyNamed")("first", 8)("second", 9).Execute());
}
