#include <Core/Core.h>
#include <Web/Web.h>
#include <Web/ScgiServer.h>

using namespace Upp;

class App : public ScgiServer {
public:
	virtual void OnAccepted();
	virtual void OnRequest();
	virtual void OnClosed();

private:
	void HelloViaGet();
	void HelloViaPost();

public:
	App(int port = 8787) : ScgiServer(port) {}
};

void App::OnAccepted()
{
	Cout() << "Accepted connection from client " << FormatIP(clientIP) << "\n";
}

void App::HelloViaGet()
{
	Write(Format("Hello, %s!\r\n", query["NAME"]));
}

void App::HelloViaPost()
{
	Write(Format("Hello, %s!\r\n", post["NAME"]));
}

void App::OnRequest()
{
	Write("Content-Type: text/plain\r\n\r\n");
	Write("Message:\r\n");

	//
	// In a real app one should look at the server variable 'SCRIPT_NAME' to see:
	//   * /scgi/hello
	//   * /scgi/hello-form
	//
	// That method would be used for better application based
	// request dispatching vs. HasPostData()
	//
	
	if (HasPostData()) {
		HelloViaPost();

		Write("\r\nPost Data:\r\n");
		for (int i=0; i < post.GetCount(); i++)
			Write(Format("%s = '%s'\r\n", post.GetKey(i), post.GetValue(i)));
	} else
		HelloViaGet();
	
	Write("\r\nQuery String:\r\n");
	for (int i=0; i < query.GetCount(); i++)
		Write(Format("%s = '%s'\r\n", query.GetKey(i), query.GetValue(i)));
	
	Write("\r\nServer Variables:\r\n");
	const Vector<String> &keys = serverVars.GetKeys();
	for (int i=0; i < serverVars.GetCount(); i++)
		Write(Format("'%s' = '%s'\r\n", keys[i], serverVars.Get(keys[i])));
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
