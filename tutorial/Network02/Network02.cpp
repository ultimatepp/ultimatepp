#include <Core/Core.h>

using namespace Upp;

constexpr int port = 4000;

void ProcessHttpRequest(TcpSocket& client)
{
	HttpHeader header;
	if(!header.Read(client)) {
		Cerr() << "Failed to read HttpHeader.\n";
		HttpResponse(client, false, 400, "Invalid request");
		return;
	}

	auto path = header.GetURI();
	if(header.GetMethod() == "GET") {
		if(path == "/countries") {
			JsonArray ja;
			ja << "Czech Republic"
			   << "Indonesia"
			   << "Brazil"
			   << "France";

			HttpResponse(client, false, 200, "OK", "application/json", ja.ToString());
		}
	}

	HttpResponse(client, false, 404, "Not found");
}

void RunServerLoop(TcpSocket& server)
{
	for(;;) {
		TcpSocket client;

		Cout() << "Waiting for incoming connection from the client...\n";
		if(!client.Accept(server)) {
			Cerr() << "Connection from the client not accepted.\n";
			continue;
		}

		ProcessHttpRequest(client);
	}
}

CONSOLE_APP_MAIN
{
	TcpSocket server;
	if(!server.Listen(port)) {
		Cerr() << "Cannot open server port for listening with error \"" << server.GetErrorDesc()
			   << "\".\n";
		return;
	}

	RunServerLoop(server);
}
