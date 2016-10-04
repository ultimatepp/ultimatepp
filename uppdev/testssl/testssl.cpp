#include <Web/SSL/SSL.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	HttpClient::Trace();
	HttpsClient client;
	client.URL("www.google.cz");
	DDUMP(client.Execute());
	
}

