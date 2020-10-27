#include <Core/Core.h>
#include <plugin/pcre/Pcre.h>

using namespace Upp;

// http://isocpp.org/blog/2014/12/myths-3
// section 6.1

CONSOLE_APP_MAIN
{
	String s = ToCharset(CHARSET_UTF8, HttpRequest("http://www.stroustrup.com/C++.html").Execute(),
	                     CHARSET_ISO8859_1);
	RegExp x("href *= *\"(.*?)\"");
	while(x.GlobalMatch(s))
		Cout() << x.GetStrings()[0] << "\n";
}
