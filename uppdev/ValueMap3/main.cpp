#include "Sql/Sql.h"

using namespace Upp;

SqlId TEST("TEST");

Id ID("ID");

CONSOLE_APP_MAIN
{
	ValueMap m;
	m.Add("Ahoj", 1);
	m.Add("Kamo", "hello!");
	m.Add(TEST, "SqlId test!");
	m.Add(ID, "Id test!");
	DUMP(m);
	DUMP(m[TEST]);
	DUMP(m[ID]);
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
	n.Add("asdf", GetSysTime());
	DUMP(n);
	DUMP(n["Kamo"]);
	va = n;
	DUMP(va);
	DUMPC(n.GetKeys());
	ValueMap h = va;
	DUMP(h);
}
