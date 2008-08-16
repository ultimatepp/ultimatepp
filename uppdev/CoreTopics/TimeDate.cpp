#include "Core.h"

NAMESPACE_UPP

static int s_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int s_month_off[] = {
	  0,  31,  59,  90, 120, 151,
	181, 212, 243, 273, 304, 334
};

bool IsLeapYear(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

void Date::Serialize(Stream& s)
{
	s % day % month % year;
}

int  GetDaysOfMonth(int m, int y) {
	return s_month[m - 1] + (m == 2) * IsLeapYear(y);
}

bool Date::IsValid() const {
	return year == -32768 /* TRC fix 2007/06/17:was == 0 */ || month >= 1 && month <= 12 &&
		                day >= 1 && day <= GetDaysOfMonth(month, year);
}

String DayName(int i, int lang)
{
	static const char *day[] = {
		tt_("date\vSunday"), tt_("date\vMonday"), tt_("date\vTuesday"),
		tt_("date\vWednesday"), tt_("date\vThursday"), tt_("date\vFriday"), tt_("date\vSaturday")
	};
	return i >= 0 && i < 7 ? Nvl(GetLngString(lang, day[i]), GetENUS(day[i])) : String();
}

String DyName(int i, int lang)
{
	static const char *day[] = {
		tt_("date\vSu"), tt_("date\vMo"), tt_("date\vTu"),
		tt_("date\vWe"), tt_("date\vTh"), tt_("date\vFr"), tt_("date\vSa")
	};
	return i >= 0 && i < 7 ? Nvl(GetLngString(lang, day[i]), GetENUS(day[i])) : String();
}

String MonthName(int i, int lang)
{
	static const char *month[] = {
		tt_("date\vJanuary"), tt_("date\vFebruary"), tt_("date\vMarch"), tt_("date\vApril"), tt_("date\vMay"),
		tt_("date\vJune"), tt_("date\vJuly"), tt_("date\vAugust"), tt_("date\vSeptember"), tt_("date\vOctober"),
		tt_("date\vNovember"), tt_("date\vDecember")
	};
	return i >= 0 && i < 12 ? Nvl(GetLngString(lang, month[i]), GetENUS(month[i])) : String();
}

String MonName(int i, int lang)
{
	static const char *month[] = {
		tt_("sdate\vJan"), tt_("sdate\vFeb"), tt_("sdate\vMar"), tt_("sdate\vApr"), tt_("sdate\vMay"),
		tt_("sdate\vJun"), tt_("sdate\vJul"), tt_("sdate\vAug"), tt_("sdate\vSep"), tt_("sdate\vOct"),
		tt_("sdate\vNov"), tt_("sdate\vDec")
	};
	return i >= 0 && i < 12 ? Nvl(GetLngString(lang, month[i]), GetENUS(month[i])) : String();
}

static char s_date_format[64] = "%2:02d/%3:02d/%1:4d";

void   SetDateFormat(const char *fmt)
{
	strncpy(s_date_format, fmt, 63);
}

String   Format(Date date) {
	String  s;
	if(IsNull(date))
		return String();
	return Format(s_date_format, date.year, date.month, date.day, DayOfWeek(date));
}

static char s_date_scan[64] = "mdy";

void   SetDateScan(const char *scan)
{
	strncpy(s_date_scan, scan, 63);
}

const char *StrToDate(Date& d, const char *s)
{
	const char *fmt = s_date_scan;
	if(*s == 0) {
		d = Null;
		return s;
	}
	d = GetSysDate();
	while(*fmt) {
		while(*s && !IsDigit(*s) && !IsAlpha(*s) && (byte)*s < 128)
			s++;
		int n;
		if(IsDigit(*s)) {
			char *q;
			n = strtoul(s, &q, 10);
			s = q;
		}
		else
		if(IsAlpha(*s) || (byte)*s >= 128) {
			if(*fmt != 'm')
				return NULL;
			String m;
			while(IsAlpha(*s) || (byte)*s >= 128)
				m.Cat(*s++);
			m = ToUpper(m);
			for(int i = 0; i < 12; i++)
				if(m == ToUpper(MonthName(i)) || m == ToUpper(MonName(i))) {
					n = i + 1;
					goto found;
				}
			return NULL;
		found:
			;
		}
		else
			break;

		switch(*fmt) {
		case 'd':
			if(n < 1 || n > 31)
				return NULL;
			d.day = n;
			break;
		case 'm':
			if(n < 1 || n > 12)
				return NULL;
			d.month = n;
			break;
		case 'y':
			d.year = n;
			if(d.year < 20) d.year += 2000; // Check again in 2015....
			else
			if(d.year < 100) d.year += 1900;
			break;
		default:
			NEVER();
		}
		fmt++;
	}
	return d.IsValid() ? s : NULL;
}

static bool s_date_letters = true, s_date_upper = true;
static char s_date_seps[64] = "A/\a .-";

void   SetDateFilter(const char *seps)
{
	s_date_letters = false;
	s_date_upper = false;
	if(*seps == 'a') {
		s_date_letters = true;
		seps++;
	}
	else
	if(*seps == 'A') {
		s_date_letters = true;
		s_date_upper = true;
		seps++;
	}
	strncpy(s_date_seps, seps, 63);
}

int  CharFilterDate(int c)
{
	if(IsDigit(c))
		return c;
	if(s_date_letters && IsLetter(c))
		return s_date_upper ? ToUpper(c) : c;
	;
	int a = 0;
	for(const char *s = s_date_seps; *s; s++) {
		if(c == (byte)*s)
			return a ? a : *s;
		if(s[1] == '\a')
			a = *s++;
	}
	return 0;
}

int Date::Get() const {
	return year * 365 + s_month_off[month - 1] + (day - 1) +
	       (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + 1 +
	       (month > 2) * IsLeapYear(year);
}

void Date::Set(int d) {
	int q, leap;
	year = 0;
	q = d / (400 * 365 + 100 - 3);
	year += 400 * q;
	d -= q * (400 * 365 + 100 - 3);
	leap = 1;
	if(d >= 100 * 365 + 24 + 1) {
		d--;
		q = d / (100 * 365 + 24);
		year += 100 * q;
		d -= q * (100 * 365 + 24);
		leap = 0;
	}
	if(d >= 365 * 4 + leap) {
		q = (d + 1 - leap) / (365 * 4 + 1);
		year += 4 * q;
		d -= q * (365 * 4 + 1) - 1 + leap;
		leap = 1;
	}
	if(d >= 365 + leap) {
		q = (d - leap) / 365;
		year += q;
		d -= q * 365 + leap;
		leap = 0;
	}
	int i;
	for(i = 0; i < 12; i++) {
		int q = s_month[i] + (i == 1) * leap;
		if(q > d) break;
		d -= q;
	}
	month = i + 1;
	day = d + 1;
}

bool operator<(Date a, Date b) {
	if(a.year < b.year) return true;
	if(a.year > b.year) return false;
	if(a.month < b.month) return true;
	if(a.month > b.month) return false;
	return a.day < b.day;
}

int   operator-(Date a, Date b)    { return a.Get() - b.Get(); }
Date  operator+(Date a, int b)     { Date q; q.Set(a.Get() + b); return q; }
Date  operator+(int a, Date b)     { Date q; q.Set(b.Get() + a); return q; }
Date  operator-(Date a, int b)     { Date q; q.Set(a.Get() - b); return q; }
Date& operator+=(Date& a, int b)   { a.Set(a.Get() + b); return a; }
Date& operator-=(Date& a, int b)   { a.Set(a.Get() - b); return a; }

int DayOfWeek(Date date) {
	return (date.Get() + 6) % 7;
}

Date LastDayOfMonth(Date d) {
	d.day = GetDaysOfMonth(d.month, d.year);
	return d;
}

Date FirstDayOfMonth(Date d) {
	d.day = 1;
	return d;
}

Date LastDayOfYear(Date d)
{
	d.day = 31;
	d.month = 12;
	return d;
}

Date FirstDayOfYear(Date d)
{
	d.day = 1;
	d.month = 1;
	return d;
}

Date AddMonths(Date date, int months) {
	months += date.month - 1;
	int year = idivfloor(months, 12);
	months -= 12 * year;
	year += date.year;
	date.year = year;
	date.month = months + 1;
	date.day = min(date.day, LastDayOfMonth(date).day);
	return date;
}

Date AddYears(Date date, int years) {
	date.year += years;
	date.day = min(date.day, LastDayOfMonth(date).day);
	return date;
}

void Time::Serialize(Stream& s)
{
	s % day % month % year % hour % minute % second;
}

#ifdef PLATFORM_WIN32
Time::Time(FileTime filetime) {
	SYSTEMTIME tm;
	FileTime ft;
	FileTimeToLocalFileTime(&filetime, &ft);
	FileTimeToSystemTime(&ft, &tm);
	*this = Time(tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
}

FileTime Time::AsFileTime() const {
	SYSTEMTIME tm;
	tm.wYear = year;
	tm.wMonth = month;
	tm.wDay = day;
	tm.wHour = hour;
	tm.wMinute = minute;
	tm.wSecond = second;
	tm.wMilliseconds = 0;
	FileTime ft, filetime;
	SystemTimeToFileTime(&tm, &ft);
	LocalFileTimeToFileTime(&ft, &filetime);
	return filetime;
}
#endif

#ifdef PLATFORM_POSIX
Time::Time(FileTime filetime) {
	struct tm *time = localtime(&filetime.ft);
	*this = Time(time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
                 time->tm_hour, time->tm_min, time->tm_sec);
}

FileTime Time::AsFileTime() const {
	struct tm time;
	time.tm_year = year - 1900;
	time.tm_mon = month - 1;
	time.tm_mday = day;
	time.tm_hour = hour;
	time.tm_min = minute;
	time.tm_sec = second;
	time.tm_isdst = -1;
	return mktime(&time);
}
#endif

bool operator==(Time a, Time b) {
	return a.day == b.day && a.month == b.month && a.year == b.year &&
		   a.hour == b.hour && a.minute == b.minute && a.second == b.second;
}

bool operator<(Time a, Time b) {
	if(a.year < b.year) return true;
	if(a.year > b.year) return false;
	if(a.month < b.month) return true;
	if(a.month > b.month) return false;
	if(a.day < b.day) return true;
	if(a.day > b.day) return false;
	if(a.hour < b.hour) return true;
	if(a.hour > b.hour) return false;
	if(a.minute < b.minute) return true;
	if(a.minute > b.minute) return false;
	return a.second < b.second;
}

void Time::Set(int64 scalar)
{
	int q = (int)(scalar / (24 * 3600));
	Date::Set(q);
	int n = int(scalar - (int64)q * 24 * 3600);
	hour = n / 3600;
	n %= 3600;
	minute = n / 60;
	second = n % 60;
}

int64 Time::Get() const
{
	return Date::Get() * (int64)24 * 3600 + hour * 3600 + minute * 60 + second;
}

int64 operator-(Time a, Time b) {
	return a.Get() - b.Get();
}

Time operator+(Time time, int64 secs) {
	time.Set(time.Get() + secs);
	return time;
}

Time  operator+(int64 seconds, Time a)          { return a + seconds; }
Time  operator-(Time a, int64 secs)             { return a + (-secs); }
Time& operator+=(Time& a, int64 secs)           { a = a + secs; return a; }
Time& operator-=(Time& a, int64 secs)           { a = a - secs; return a; }

String Format(Time time, bool seconds) {
	if(IsNull(time)) return String();
	String s = Format(Date(time));
	if(time.hour == 0 && time.minute == 0 && time.second == 0)
		return s;
	else
		return s + (seconds ? Format(" %02d:%02d:%02d", time.hour, time.minute, time.second)
	                        : Format(" %02d:%02d", time.hour, time.minute));
}

#ifdef PLATFORM_WIN32
Time GetSysTime() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	return Time(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}
#endif

#ifdef PLATFORM_POSIX
Time GetSysTime() {
	return Time(time(NULL));
}
#endif

Date GetSysDate() {
	return GetSysTime();
}

END_UPP_NAMESPACE
