#include "telupp.h"

#define IMAGECLASS TeltestImg
#define IMAGEFILE <telupp/teltest.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TeltestImg
#define IMAGEFILE <telupp/teltest.iml>
#include <Draw/iml_source.h>

int mouse_x, mouse_y;

void DrawSomething(Draw& w)
{
	w.DrawRect(0, 0, 1000, 1000, White);
	ParseQTF(LoadFile(GetDataFile("test.qtf"))).Paint(Zoom(2, 10), w, 20, 20, 500);
	w.DrawText(10, 10, AsString(mouse_x) + ' ' + AsString(mouse_y));
	w.DrawImage(mouse_x, mouse_y, TeltestImg::Test());
}

void ProcessEventQueue(const String& event_queue)
{
	StringStream ss(event_queue);
	while(!ss.IsEof()) {
		String s = ss.GetLine();
		CParser p(s);
		try {
			if(p.Id("RI"))
				TelDraw::ResetI();
			else
			if(p.Id("MM")) {
				mouse_x = p.ReadInt();
				mouse_y = p.ReadInt();
			}
		}
		catch(CParser::Error) {}
	}
}

TcpSocket   server;
StaticMutex ServerMutex;

String content;

void Server()
{
	for(;;) {
		TcpSocket socket;
//		LOG("Waiting...");
		ServerMutex.Enter();
		bool b = socket.Accept(server);
		ServerMutex.Leave();
		if(b) {
//			LOG("Connection accepted");
			HttpHeader http;
			TimeStop tm;
			http.Read(socket);
//			DDUMP(http.GetURI());
//			DDUMP(http.GetContentLength());
			String event_queue = socket.Get((int)http.GetContentLength());
			DDUMP(event_queue);
			
			ProcessEventQueue(event_queue);
			
			TelDraw draw;
			draw.Init(Size(1000, 1000));
			DrawSomething(draw);

			String content = draw.result;
			
			DDUMP(content.GetLength());
			DDUMP(ZCompress(content).GetLength());

			if(http.GetURI().GetCount() < 2)
				HttpResponse(socket, http.scgi, 200, "OK", "text/html", LoadFile(GetDataFile("telupp.html")));
			else	
				HttpResponse(socket, http.scgi, 200, "OK", "text/plain; charset=x-user-defined", content);
			DDUMP(tm);
		}
	}
}


CONSOLE_APP_MAIN
{
//	StdLogSetup(LOG_COUT|LOG_FILE);
	
	if(!server.Listen(8000, 10)) {
		LOG("Cannot open server port for listening\r\n");
		return;
	}
	Server();
}
