#include <Core/Core.h>

using namespace Upp;

// To run, OpenSSL libraries must be installed and accessible
// After starting, type "https://localhost:4000" into your browser

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TcpSocket   server;

	if(!server.Listen(4000, 10)) {
		LOG("Cannot open server port for listening\r\n");
		return;
	}

	for(;;) {
		TcpSocket socket;
		LOG("Waiting...");
		if(socket.Accept(server)) {
			socket.SSLCertificate(LoadFile(GetDataFile("server.crt")),
			                      LoadFile(GetDataFile("server.key")),
			                      false);
			LOG("Connection accepted");
			socket.StartSSL();
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
