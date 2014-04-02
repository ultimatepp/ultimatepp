#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Vector<String> h = Split("alfa,beta,,gamma", ',', false);
//	DUMP(Join(h, ","));
//	DUMP(Join(h, ",", true));
	
	String a, b, c;
	
	SplitTo("one", ',', a, b, c);
	
	DUMP(SplitTo("one", ',', a, b, c));
	DUMP(SplitTo("one,two,,three", ',', a, b, c));
	DUMP(MakeTuple(a, b, c));
	DUMP(SplitTo("one,two,,three", ',', false, a, b, c));
	DUMP(MakeTuple(a, b, c));
	DUMP(SplitTo("one,two,,three", ",", a, b, c));
	DUMP(MakeTuple(a, b, c));
	DUMP(SplitTo("one,two,,three", ",", false, a, b, c));
	DUMP(MakeTuple(a, b, c));
}
