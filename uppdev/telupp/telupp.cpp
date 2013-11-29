#include "telupp.h"

void DrawSomething(Draw& w)
{
	w.DrawRect(0, 0, 100, 100, LtGray);
	w.DrawRect(30, 30, 25, 50, Red);
}

TcpSocket   server;
StaticMutex ServerMutex;

String content;

void Server()
{
	for(;;) {
		TcpSocket socket;
		LOG("Waiting...");
		ServerMutex.Enter();
		bool b = socket.Accept(server);
		ServerMutex.Leave();
		if(b) {
			LOG("Connection accepted");
			HttpHeader http;
			http.Read(socket);
			DDUMP(http.GetURI());
			if(http.GetURI().GetCount() < 2)
				HttpResponse(socket, http.scgi, 200, "OK", "text/html", LoadFile(GetDataFile("telupp.html")));
			else	
				HttpResponse(socket, http.scgi, 200, "OK", "text/plain; charset=x-user-defined", content);
		}
	}
}


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TelDraw draw;
	draw.Init(Size(400, 400));
	DrawSomething(draw);
	
	content = draw.result;
	
	DUMPHEX(content);

	if(!server.Listen(80, 10)) {
		LOG("Cannot open server port for listening\r\n");
		return;
	}
	Server();
}
