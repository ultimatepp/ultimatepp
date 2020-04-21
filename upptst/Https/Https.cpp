#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	HttpRequest::Trace();
	LOG(HttpRequest("https://www.ultimatepp.org").Execute());
}
