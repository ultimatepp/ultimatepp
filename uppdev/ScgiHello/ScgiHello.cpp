#include <Core/Core.h>
#include <Web/Web.h>

#include <ScgiServer/ScgiServer.h>

using namespace Upp;

class App : public ScgiServer {
private:
	typedef App CLASSNAME;
	
	void OnAccept();
	void OnRequest();
	void OnClosed();

	void HelloViaGet();
	void HelloViaPost();

public:
	App(int port = 8787);
};

App::App(int port) : ScgiServer(port)
{
	WhenAccepted = THISBACK(OnAccept);
	WhenRequest = THISBACK(OnRequest);
	WhenClosed = THISBACK(OnClosed);
}

void App::OnAccept()
{
	Cout() << "Accepted connection from client " << FormatIP(clientIP) << "\n";
}

void App::HelloViaGet()
{
	clientSock.Write(Format("Hello, %s!\r\n", query["NAME"]));
}

void App::HelloViaPost()
{
	clientSock.Write(Format("Hello, %s!\r\n", post["NAME"]));
}

void App::OnRequest()
{
	clientSock.Write("Content-Type: text/plain\r\n\r\n");
	clientSock.Write("Message:\r\n");

	//
	// Should look at the server variable 'SCRIPT_NAME' to see:
	//   * /scgi/hello
	//   * /scgi/hello-form
	//
	// That method would be used for better application based
	// request dispatching vs. HasPostData()
	//
	
	if (HasPostData()) {
		HelloViaPost();

		clientSock.Write("\r\nPost Data:\r\n");
		for (int i=0; i < post.GetCount(); i++)
			clientSock.Write(Format("%s = '%s'\r\n", post.GetKey(i), post.GetValue(i)));
	} else
		HelloViaGet();
	
	clientSock.Write("\r\nQuery String:\r\n");
	for (int i=0; i < query.GetCount(); i++)
		clientSock.Write(Format("%s = '%s'\r\n", query.GetKey(i), query.GetValue(i)));
	
	clientSock.Write("\r\nServer Variables:\r\n");
	const Vector<String> &keys = serverVars.GetKeys();
	for (int i=0; i < serverVars.GetCount(); i++)
		clientSock.Write(Format("'%s' = '%s'\r\n", keys[i], serverVars.Get(keys[i])));
}

void App::OnClosed()
{
	Cout() << "Connection with " << FormatIP(clientIP) << " closed\n";
}

CONSOLE_APP_MAIN
{
	App app;
	app.Run();
}
