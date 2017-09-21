#include <Core/Core.h>

using namespace Upp;

// Clients are run in blocking mode

void ClientEmulation()
{
	Thread::Start([] {
	#if 0 // change to 1 to do single request
		Sleep(500);
		WebSocket ws;
		ws.Connect("ws://127.0.0.1:12321");
		if(ws.IsError()) {
			LOG("Failed to connect");
			return;
		}
		LOG("Client Request");
		ws.SendText(AsString(Random()));
		LOG("Response: " << ws.Receive());
		ws.Close();
	#else
		for(;;) {
			Sleep(Random(2000));
			Thread::Start([] {
				LOG("Started a new client");
				WebSocket ws;
				ws.Connect("ws://127.0.0.1:12321");
				if(ws.IsError()) {
					LOG("Failed to connect");
					return;
				}
				LOG("WebSocket connected to the server");
				while(Random(20) && ws.IsOpen()) {
					String request = AsString(Random());
					LOG("Sending request " << request);
					ws.SendText(request);
					String reply = ws.Receive();
					LOG(request << " -> " << reply);
					Sleep(Random(2000));
				}
				ws.Close();
			});
		}
	#endif
	});
}