#include <Core/Core.h>

using namespace Upp;

void TestTrimLeft(String what, String s, String r)
{
	for(int i = 0; i < 3; i++) {
		ASSERT(TrimLeft(what, s) == r);
		ASSERT(TrimLeft(~what, s) == r);
		ASSERT(TrimLeft(~what, what.GetLength(), s) == r);
		r = s;
		what = i == 1 ? "<notfound\1>" : "";
	}
}

void TestTrimRight(String what, String s, String r)
{
	for(int i = 0; i < 3; i++) {
		ASSERT(TrimRight(what, s) == r);
		ASSERT(TrimRight(~what, s) == r);
		ASSERT(TrimRight(~what, what.GetLength(), s) == r);
		r = s;
		what = i == 1 ? "<notfound\1>" : "";
	}
}

void TestMultiReplace()
{
	VectorMap<String, String> r;
	r("hell", "hello")("hello", "hell");
	String h = Replace(String("TEXT: hell hello!"), r);
	DDUMP(h);
	ASSERT(h == "TEXT: hello hell!");
}

CONSOLE_APP_MAIN
{
	String h = "http://www.website.org";
	DDUMP(TrimLeft("test", h));
	DDUMP(TrimLeft("http://", h));
	DDUMP(TrimRight("test", h));
	DDUMP(TrimRight(".org", h));

	TestTrimLeft("http://", "http://www.website.org", "www.website.org");
	TestTrimRight(".org", "www.website.org", "www.website");
	
	TestMultiReplace();
}
