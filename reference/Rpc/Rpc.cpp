#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

using namespace Upp;

RPC_METHOD(Multiply)
{
	int m, n;
	rpc >> m >> n;
	rpc << m * n;
}

RPC_METHOD(MultiplyNamed)
{
	int n = rpc["first"];
	int m = rpc["second"];
	rpc << m * n;
}

CONSOLE_APP_MAIN
{
///	LogXmlRpcRequests();
//	SetXmlRpcServerTrace(UppLog());
//  Working in "shortened" mode - without URL specified, RpcRequests are performed by methods in
//  the same process.

	DUMP(JsonRpcRequest()("Multiply", 10, 11).Execute());
	DUMP(XmlRpcRequest()("Multiply", 10, 11).Execute());
	DUMP(JsonRpcRequestNamed()("MultiplyNamed")("first", 10)("second", 11).Execute());
}
