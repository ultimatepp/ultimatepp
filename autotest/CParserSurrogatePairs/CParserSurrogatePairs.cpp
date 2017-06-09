#include <Core/Core.h>

using namespace Upp;

void CheckError(const char *err)
{
	try {
		CParser p(err);
		p.ReadString();
	}
	catch(CParser::Error error) {
		DUMP(error);
		return;
	}
	
	DUMP(err);
	ASSERT(0);
}

CONSOLE_APP_MAIN
{
	String utf8 = "\xF0\x9F\x91\x8D";
	
	DUMPHEX(ToUtf8(0x1F44D));
	ASSERT(ToUtf8(0x1F44D) == utf8);
	
	CParser p("\"\\ud83d\\udc4d\\U0001F44D\"");
	
	String h = p.ReadString();

	DUMPHEX(h);
	
	ASSERT(h == utf8 + utf8);
	
	CheckError("\"\\ud83d1\\udc4d\\U0001F44D\"");
	CheckError("\"\\ud83d\\u0000\\U0001F44D\"");
	CheckError("\"\\ud83d\\U0001F44D\"");
	CheckError("\"\\ud83d\\udc4d\\U000F44D\"");
	CheckError("\"\\u83d");
	CheckError("\"\\U83d");
}
