#include <Web/Web.h>

using namespace Upp;


CONSOLE_APP_MAIN
{
	DDUMP(HttpClient("www.idnes.cz").ExecuteRedirect());
	DDUMP(HttpClient("http://g.idnes.cz/js/sph/2010.js?rr=033").ExecuteRedirect());
	DDUMP(HttpClient("http://www.ultimatepp.org/src$CtrlLib$ArrayCtrl$en-us.html").ExecuteRedirect());
}

