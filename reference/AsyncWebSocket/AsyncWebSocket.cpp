#include <Core/Core.h>

using namespace Upp;

struct Worker {
	WebSocket ws;
	
	void Do();
	
	Worker();
};

void Worker::Do()
{
	String s = ws.Receive();
	if(s.GetCount()) {
		LOG("Received request " << s);
		ws.SendText(AsString(sqrt(atof(s))));
	}
}

Worker::Worker()
{
	ws.NonBlocking();
}

void ClientEmulation();

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	WebSocket::Trace();

	TcpSocket server;
	server.Timeout(0);

	if(!server.Listen(12321, 5, false, true)) {
		LOG("Cannot open server socket for listening!");
		Exit(1);
	}

	ClientEmulation();

	Array<Worker> worker;

	LOG("Starting to listen");
	for(;;) {
		if(server.IsError()) {
			LOG("Server error: " << server.GetErrorDesc());
			server.ClearError();
		}
		SocketWaitEvent sel;
		sel.Add(server, WAIT_READ);
		for(auto& w : worker)
			w.ws.AddTo(sel);
		sel.Wait(1000);
		LOG("Waiting ended at " << GetSysTime());
		for(int i = worker.GetCount() - 1; i >= 0; i--)
			if(sel[i + 1]) {
				LOG("Event at worker " << i);
				Worker& w = worker[i];
				w.Do();
				if(w.ws.IsClosed() || w.ws.IsError()) {
					worker.Remove(i);
					LOG("WebSocket closed, current count: " << worker.GetCount());
				}
			}
		if(sel[0]) {
			if(!worker.Add().ws.NonBlocking().Accept(server))
				worker.Drop();
			else
				LOG("WebSocket accepted, current count: " << worker.GetCount());
		}
	}
}
