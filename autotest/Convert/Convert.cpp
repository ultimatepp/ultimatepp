#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	SetLanguage(LNG_('C','S','C','Z'));

	ConvertTime cv;

	DUMP(cv.Format(Date(2013, 1, 2)));
	ASSERT(cv.Format(Date(2013, 1, 2)) == "02.01.2013");

	DUMP(cv.Format(Time(2013, 1, 2)));
	ASSERT(cv.Format(Time(2013, 1, 2)) == "02.01.2013");

	DUMP(cv.Format(Time(2013, 1, 2, 11, 12, 13)));
	ASSERT(cv.Format(Time(2013, 1, 2, 11, 12, 13)) == "02.01.2013 11:12:13");
	
	cv.TimeAlways();

	DUMP(cv.Format(Date(2013, 1, 2)));
	ASSERT(cv.Format(Date(2013, 1, 2)) == "02.01.2013 00:00:00");

	DUMP(cv.Format(Time(2013, 1, 2, 11, 12, 13)));
	ASSERT(cv.Format(Time(2013, 1, 2, 11, 12, 13)) == "02.01.2013 11:12:13");
	
	DUMP(cv.Scan("15.6.2010"));
	ASSERT(cv.Scan("15.6.2010") == Time(2010, 6, 15, 0, 0, 0));

	cv.DayEnd();

	DUMP(cv.Scan("15.6.2010"));
	ASSERT(cv.Scan("15.6.2010") == Time(2010, 6, 15, 23, 59, 59));
}
