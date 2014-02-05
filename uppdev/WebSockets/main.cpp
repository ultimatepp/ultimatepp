#include "WebSockets.h"

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TcpSocket server;
	if(!server.Listen(9998)) {
		LOG("Failed to listen..");
		return;
	}

	for(;;) {
		WebSocket ws;
		if(ws.WebAccept(server)) {
			LOG("Accepted, trying to handshake");
			LOG("Handshake successfull, trying to recieve");
			LOG(ws.Recieve());
			ws.SendText("This is some text...");
		}
		if(ws.IsError())
			LOG("ERROR: " << ws.GetErrorDesc());
	}
}
