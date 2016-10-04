#include "ValueMap.h"

SqlId TEST("TEST");

CONSOLE_APP_MAIN
{
	ValueMap m;
	m.Put("Ahoj", 1);
	m.Put("Kamo", "hello!");
	DUMP(m);
	DUMP(m[TEST]);
	DUMP(m["Ahoj"]);
	DUMP(m["Ahoj1"]);
	DUMP(m["Kamo"]);
	LOG("-----------");
	Value v = m;
//	ValueArray va = v;
	ValueArray va = m;
	DUMP(va);
	ValueMap n = v;
	va = n;
	DUMP(va);
	n.Put("asdf", GetSysTime());
	n.Remove("Kamo");
	DUMP(n);
	DUMP(n["Kamo"]);
	va = n;
	DUMP(va);
	DUMP(n.GetKeys());
}
