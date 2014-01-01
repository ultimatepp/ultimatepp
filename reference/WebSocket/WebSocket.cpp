#include "Core/Core.h"

using namespace Upp;

// Open demo.html in browser, run this app, push the button...

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TcpSocket server;
	if(!server.Listen(8888)) {
		LOG("Failed to start listening on 8888..");
		return;
	}

	for(;;) {
		TcpSocket socket;
		WebSocket ws;
		if(socket.Accept(server) && ws.WebAccept(socket)) {
			LOG("Accepted connection");
			LOG(ws.Recieve());
			ws.SendText("Hello browser!");
		}
		if(ws.IsError())
			LOG("ERROR: " << ws.GetErrorDesc());
	}
}
