#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TcpSocket socket;
	
	if(!socket.Connect("pop.gmail.com", 995)) {
		LOG("Socket error encountered: " << socket.GetErrorDesc());
		Exit(1);
	}
	if(!socket.StartSSL()) {
		LOG("Couldn't start SSL session.");
		Exit(1);
	}

	// 5 secs.
	socket.Timeout(5000);

	String server_hello = socket.GetLine();
	
	DUMP(server_hello);
	DUMPHEX(server_hello);
	
	ASSERT(server_hello.StartsWith("+OK Gpop ready for requests from"));
	
	LOG("=========== OK");
}
