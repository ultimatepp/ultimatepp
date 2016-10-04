#include <Web/SSL/SSL.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	
	HttpClient::Trace();
	HttpsClient h;
	h.URL("www.google.com");
	h.Proxy("90.182.182.154", 8080);
	DDUMP(h.ExecuteRedirect());
}
