#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String utf8 = "\xF0\x9F\x91\x8D";
	
	DUMPHEX(ToUtf8(0x1F44D));
	ASSERT(ToUtf8(0x1F44D) == utf8);
	
	CParser p("\"\\ud83d\\udc4d\"");
	p.UnicodeEscape();
	
	String h = p.ReadString();

	DUMPHEX(h);
	
	ASSERT(h == utf8);
}
