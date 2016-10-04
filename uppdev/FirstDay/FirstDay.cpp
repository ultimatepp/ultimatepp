#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 200; i++) {
		Date d = GetSysDate() + i;
		int q = DayOfWeek(d);
		int n = (d.day - 1) / 7;
		bool even = ((d - Date::Low() - 1) / 7) & 1;
		LOG(d << " " << DayName(q) << " " << n << ' ' << even);
	}
}

