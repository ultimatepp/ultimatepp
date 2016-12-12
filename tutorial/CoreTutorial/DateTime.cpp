#include "Tutorial.h"

void DateTime()
{
	/// .Date and Time

	/// To represent date and time, U++ provides `Date` and `Time` concrete types.

	Date date = GetSysDate();
	
	DUMP(date);

	/// All data members of `Date` structure are public:

	DUMP((int)date.year); // we need to cast to int because some date members
	DUMP((int)date.month); // are of unsigned character type which would log
	DUMP((int)date.day); // as characters

	/// Dates can be compared:

	DUMP(date > Date(2000, 1, 1));

	/// Adding a number to `Date` adds a number of days to it, incrementing/decrementing goes to
	/// the next/previous day:

	DUMP(date + 1);
	DUMP(--date);
	DUMP(++date);

	/// Subtraction of dates yields a number of days between them:
	
	DUMP(date - Date(2000, 1, 1));
	
	/// There are several `Date` and calendar related functions:

	DUMP(IsLeapYear(2012));
	DUMP(IsLeapYear(2014));
	DUMP(IsLeapYear(2015));
	DUMP(IsLeapYear(2016));
	DUMP(IsLeapYear(2017));
	
	///

	DUMP(GetDaysOfMonth(2, 2015));
	DUMP(GetDaysOfMonth(2, 2016));
	
	///

	DUMP(DayOfWeek(date)); // 0 is Sunday
	
	///
	
	DUMP(LastDayOfMonth(date));
	DUMP(FirstDayOfMonth(date));
	DUMP(LastDayOfYear(date));
	DUMP(FirstDayOfYear(date));
	DUMP(DayOfYear(date)); // number of days since Jan-1 + 1
	DUMP(DayOfYear(Date(2016, 1, 1)));
	
	///

	DUMP(AddMonths(date, 20));
	DUMP(GetMonths(date, date + 100)); // number of 'whole months' between two dates
	DUMP(GetMonthsP(date, date + 100)); // number of 'whole or partial months' between two dates
	DUMP(AddYears(date, 2));
	
	///

	DUMP(GetWeekDate(2015, 1));
	int year;
	DUMP(GetWeek(Date(2016, 1, 1), year)); // first day of year can belong to previous year
	DUMP(year);
	
	///

	DUMP(EasterDay(2015));
	DUMP(EasterDay(2016));
	
	/// U++ defines the beginning and the end of era, most algorithms can safely assume that as
	/// minimal and maximal values `Date` can represent:

	DUMP(Date::Low());
	DUMP(Date::High());

	/// Time is derived from `Date`, adding members to represent time:

	Time time = GetSysTime();
	DUMP(time);
	DUMP((Date)time);
	DUMP((int)time.hour);
	DUMP((int)time.minute);
	DUMP((int)time.second);

	/// Times can be compared:

	DUMP(time > Time(1970, 0, 0));

	/// Warning: As `Time` is derived from the `Date`, most operations automatically convert
	/// `Time` back to `Date`. You have to use `ToTime` conversion function to convert `Date`
	/// to `Time`:

	DUMP(time > date); // time gets converted to Date...
	DUMP(time > ToTime(date));

	/// Like `Date`, `Time` supports add and subtract operations, but numbers represent seconds
	/// (using `int64` datatype):
	
	DUMP(time + 1);
	DUMP(time + 24 * 3600);
	DUMP(time - date); // time converts to Date, so the result is in days
	DUMP(time - ToTime(date)); // Time - Time is in seconds
	
	/// `Time` defines era limits too:
	
	DUMP(Time::Low());
	DUMP(Time::High());
	
	///
}