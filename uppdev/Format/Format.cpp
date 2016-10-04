#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DDUMP(Format("%.*f", 2, 1.22));
	return;
	Value a, b;
	String s;
	Date d = GetSysDate();
	Format("%d %d", 1, 2);
	RDUMP(Format("%d %d %'", 1, 2, d));
	{
		RTIMING("Time");
		for(int i = 0; i < 1000000; i++)
			s = Format("%d %d %'", 1, 2, d);
	}
}
