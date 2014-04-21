#include <Core/Core.h>

using namespace Upp;

int DelimFn(int c)
{
	return c == ';' || c == ' ' ? c : 0;
}

const char *DelimTextFn(const char *s)
{
	bool found = false;
	while(*s == ' ' || *s == ';') {
		if(*s == ';')
			found = true;
		s++;
	}
	return found ? s : NULL;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	DUMP(Split("one,two,,three", ','));
	DUMP(Split("one,two,,three", ',', false));
	DUMP(Split("one; two; three", DelimFn));
	DUMP(Split(3, "one::two::three:x::four", "::"));
	DUMP(Split("first something   ; second;third something", DelimTextFn));
	
	Vector<String> h;  h << "1" << "2" << "3";
	
	DUMP(Join(h, ", "));
	
	String a, b, c;
	DUMP(SplitTo("1;2;3", ';', a, b, c));
	DUMP(a);
	DUMP(b);
	DUMP(c);
	DUMP(SplitTo("1;2", ';', a, b, c));
	
	DUMP(Merge("+", "hello", "", "world"));
	
	String text = "something";
	MergeWith(text, "_", "", "to", "", "merge");
	DUMP(text);
}
