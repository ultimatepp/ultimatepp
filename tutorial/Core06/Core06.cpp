#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Value a = 1;
	Value b = 2.34;
	Value c = GetSysDate();
	Value d = "hello";

	int x = a;
	DUMP(x);
	double y = b;
	DUMP(y);
	Date z = c;
	DUMP(z);
	String s = d;
	DUMP(s);

	double i = a;
	DUMP(i);
	int j = b;
	DUMP(j);
	Time k = c;
	DUMP(k);
	WString t = d;
	DUMP(t);

	DUMP(a.Is<int>());
	DUMP(a.Is<double>());
	DUMP(b.Is<double>());
	DUMP(c.Is<int>());
	DUMP(c.Is<Date>());
	DUMP(d.Is<String>());

	DUMP(IsNumber(a));
	DUMP(IsNumber(b));
	DUMP(IsDateTime(c));
}
