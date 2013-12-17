#include <Core/Core.h>

using namespace Upp;

TcpSocket   server;
StaticMutex ServerMutex;

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
			String html;
			html << "<html>"
			     << "<b>Method:</b> " << http.GetMethod() << "<br>"
			     << "<b>URI:</b> " << http.GetURI() << "<br>";
			for(int i = 0; i < http.fields.GetCount(); i++)
				html << "<b>" << http.fields.GetKey(i) << ":</b> " << http.fields[i] << "<br>";
			int len = (int)http.GetContentLength();
			if(len > 0)
				socket.GetAll(len);
			html << "<b><i>Current time:</i></b> " << GetSysTime() << "</html>";
			HttpResponse(socket, http.scgi, 200, "OK", "text/html", html);
		}
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	if(!server.Listen(4000, 10)) {
		LOG("Cannot open server port for listening\r\n");
		return;
	}
#ifdef _MULTITHREADED
	const int NTHREADS = 10;
	for(int i = 0; i < NTHREADS; i++)
		Thread::Start(callback(Server));
#endif
	Server();
}
