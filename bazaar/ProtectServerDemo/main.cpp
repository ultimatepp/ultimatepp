#include <Protect/ProtectServer.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ProtectServer server;
	
	// setup key
	server.SetKey(ScanHexString("aabbccddeeff00112233445566778899"));
	
	// setup cypher
	server.SetCypher(new Snow2);

	server.Run();
}
