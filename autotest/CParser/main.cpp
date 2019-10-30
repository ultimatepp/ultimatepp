#include <Core/Core.h>

using namespace Upp;

#define CHECK_OVERFLOW(s, method, overflow) \
{ \
	DLOG(s << ' ' << #method); \
	bool isoverflow = false; \
	try { \
		CParser p(s); \
		DDUMP(p.method); \
	} \
	catch(CParser::Error e) { \
		DLOG("Overflow: " << e); \
		isoverflow = true; \
	} \
	ASSERT(isoverflow == overflow); \
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	{
		String s = "UTF8 ěščřžýá";
		String txt = AsCString(s) + "\n" + AsCString(s);
		CParser p(txt);
		ASSERT(p.ReadString() == s + s);
	}
	
	try {
		CParser p("\"terminated string\"");
		ASSERT(p.ReadString() == "terminated string");
		CParser p2("\"unterminated string\n");
		p2.ReadString();
		NEVER();
	}
	catch(CParser::Error) {
	}
	
	{
		CParser p2("\"unterminated string\n");
		ASSERT(p2.ReadString(false) == "unterminated string\n");
	}

	CParser p("i if while 12345 alfa");
	ASSERT(p.Id("i"));
	ASSERT(p.Id("if"));
	ASSERT(p.Id("while"));
	ASSERT(p.IsInt());
	ASSERT(p.ReadInt() == 12345);
	ASSERT(p.IsId());
	ASSERT(p.ReadId() == "alfa");
	
	CHECK_OVERFLOW("2147483647", ReadInt(), false);
	CHECK_OVERFLOW("2147483648", ReadInt(), true);

	CHECK_OVERFLOW("-2147483648", ReadInt(), false);
	CHECK_OVERFLOW("-2147483649", ReadInt(), true);

	CHECK_OVERFLOW("ffffffff", ReadNumber(16), false);
	CHECK_OVERFLOW("100000000", ReadNumber(16), true);	
	
	CHECK_OVERFLOW("9223372036854775807", ReadInt64(), false);
	CHECK_OVERFLOW("9223372036854775808", ReadInt64(), true);

	CHECK_OVERFLOW("-9223372036854775808", ReadInt64(), false);
	CHECK_OVERFLOW("-9223372036854775809", ReadInt64(), true);

	CHECK_OVERFLOW("ffffffffffffffff", ReadNumber64(16), false);
	CHECK_OVERFLOW("10000000000000000", ReadNumber64(16), true);

	DDUMP(1 * pow(10.0, 500));
	
	CHECK_OVERFLOW("1e300", ReadDouble(), false);
	CHECK_OVERFLOW("1e500", ReadDouble(), true);
	CHECK_OVERFLOW("-1e300", ReadDouble(), false);
	CHECK_OVERFLOW("-1e500", ReadDouble(), true);
	CHECK_OVERFLOW(String('0', 500), ReadDouble(), true);
	
	{
		CParser p("+ /* \n  */ a");
		p.Char('+');
		ASSERT(p.GetLine() == 2);
		ASSERT(p.GetColumn() == 6);
		ASSERT(p.ReadId() == "a");
	}
	
	LOG("=========== OK");
}
