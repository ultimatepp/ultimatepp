#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Date date = GetSysDate();
	DUMP(date);
	DUMP((int)date.year);
	DUMP((int)date.month);
	DUMP((int)date.day);
	DUMP(date > Date(1970, 1, 1));
	DUMP(date + 1);
	DUMP(date - Date(1970, 1, 1));
	DUMP(Date::Low());
	DUMP(Date::High());

	Time time = GetSysTime();
	DUMP(time);
	DUMP((Date&)time);
	DUMP((int)time.hour);
	DUMP((int)time.minute);
	DUMP((int)time.second);
	DUMP(time > Time(1970, 0, 0));
	DUMP(time > date);
	DUMP(time > ToTime(date));
	DUMP(time + 1);
	DUMP(time + 24 * 3600);
	DUMP(time - date);
	DUMP(time - ToTime(date));
	DUMP(Time::Low());
	DUMP(Time::High());
}
