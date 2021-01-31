#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	HttpRequest r("https://raw.githubusercontent.com/ultimatepp/UppHub/main/README.md");
		
	String s = r.Execute();
	
	int begin = s.FindAfter("UPPHUB_BEGIN");
	int end = s.Find("UPPHUB_END");
	
	if(begin >= 0 && end >= 0)
		s = s.Mid(begin, end - begin);

	Value v = ParseJSON(s);
	if(v.IsError()) {
		s.Replace("&quot;", "\"");
		s.Replace("&amp;", "&");
		v = ParseJSON(s);
	}
	
	ASSERT(!v.IsError());
}
