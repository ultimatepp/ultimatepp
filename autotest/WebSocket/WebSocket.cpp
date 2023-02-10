#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	HttpRequest::Trace();
	WebSocket::Trace();
#if 0
	for(int ssl = 0; ssl < 2; ssl++)
		for(int blocking = 0; blocking < 2; blocking++) {
			DDUMP(ssl);
			DDUMP(blocking);
			
			WebSocket ws;

			ws.Connect((ssl ? "wss" : "ws") + String("://echo.websocket.events"));
		
			ws.NonBlocking(!blocking);
			
			ws.SendText("Test");
			
			while(ws.IsOpen()) {
				if(ws.IsError()) {
					DDUMP(ws.GetError());
					ASSERT(0);
					break;
				}
				String s = ws.Receive();
				if(s.GetCount()) {
					DLOG(s);
					ASSERT(s == "Test");
					ws.Close();
				}
			}
			
			DLOG("... ok");
		}

	DLOG("============= OK");
#endif
}
