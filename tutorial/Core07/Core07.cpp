#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	int x = Null;
	int y = 120;
	Date d = Null;
	Date e = GetSysDate();

	DUMP(IsNull(x));
	DUMP(IsNull(y));
	DUMP(IsNull(d));
	DUMP(e > d);

	Value v = x;
	e = v;
	DUMP(IsNull(e));
	
	int a = Null;
	int b = 123;
	int c = 1;
	DUMP(Nvl(a, b, c));
}
