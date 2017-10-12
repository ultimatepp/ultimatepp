#include <CtrlLib/CtrlLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	EditString a, b;
	IdCtrls idctrls;
	
	idctrls("A", a)("B", b);
	
	a <<= "Hello";
	b <<= "world";
	
	ValueMap m = ~idctrls;
	DUMP(m);
	
	ASSERT(m["A"] == "Hello");
	ASSERT(m["B"] == "world");
	
	m("A") = "Test";
	idctrls <<= m;
	DUMP(~a);
	ASSERT(~a == "Test");
	
	LOG("=============== OK");
}
