#include <Core/Core.h>

#include <string>

using namespace Upp;
using namespace std;

CONSOLE_APP_MAIN
{
	String z1((char *)NULL);
	WString z2((char *)NULL);
	WString z3((wchar *)NULL);
	
	DUMP(z1.GetLength());
	DUMP(z2.GetLength());
	DUMP(z3.GetLength());

	String x = AsString(12345) + "AsString(12345)AsString(12345)AsString(12345)";
	String y = AsString(12345) + "AsString(12345)AsString(12345)AsString(12345)";
//	__BREAK__;
	memcmp(~x, ~y, x.GetLength());
	if(x == y)
		RLOG("Match");
/*
	String a = "Hello world!";
	DUMP(a.StartsWith("Hello"));
	DUMP(a.StartsWith("Hella"));
	DUMP(a.StartsWith(x));
	DUMP(a.EndsWith("ld!"));
	DUMP(a.EndsWith("."));
	DUMP(a.EndsWith(x));


	std::string sx = a;
	x = sx;
	DUMP(x);
	sx = std::string();
	sx = y.operator std::string();
	sx = y;
	x = sx;
	DUMP(x);

	WString w = x.ToWString();
	std::wstring ww = std::wstring(w);
	WString q = ww;
	DUMP(q);
*/
//	DUMP(q.ReverseFind('x', q.GetLength() + 1));
}
