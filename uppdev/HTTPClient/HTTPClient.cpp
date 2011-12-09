#include <Web/Web.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Cout() << HttpClient("www.rosettacode.org").ExecuteRedirect();
}
