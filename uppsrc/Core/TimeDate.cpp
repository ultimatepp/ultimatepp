#include "Core.h"

namespace Upp {

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
	ASSERT(m >= 1 && m <= 12);
	return s_month[m - 1] + (m == 2) * IsLeapYear(y);
}

bool Date::IsValid() const {
	return year == -32768 || month >= 1 && month <= 12 && day >= 1 && day <= GetDaysOfMonth(month, year);
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

static thread_local char s_date_format_thread[128];
static char s_date_format_main[128] = "%2:02d/%3:02d/%1:4d";

void SetDateFormat(const char *fmt)
{
	strncpy(s_date_format_thread, fmt, 127);
	if(Thread::IsMain())
		strncpy(s_date_format_main, fmt, 127);
}

#ifdef PLATFORM_WIN32 // resolve nameclash with win32 API
	#ifdef GetDateFormatA
		#undef GetDateFormatA
	#endif

	#ifdef GetDateFormatW
		#undef GetDateFormatW
	#endif
#endif

String GetDateFormat()
{
	return *s_date_format_thread ? s_date_format_thread : s_date_format_main;
}

String Format(Date date, const char *fmt)
{
	String  s;
	if(IsNull(date))
		return String();
	return Format(fmt, date.year, date.month, date.day, DayOfWeek(date));
}

String   Format(Date date) {
	String  s;
	if(IsNull(date))
		return String();
	return Format(*s_date_format_thread ? s_date_format_thread : s_date_format_main, date.year, date.month, date.day, DayOfWeek(date));
}

static thread_local char s_date_scan_thread[64];
static char s_date_scan_main[64] = "mdy";

void   SetDateScan(const char *scan)
{
	strncpy(s_date_scan_thread, scan, 63);
	if(Thread::IsMain())
		strncpy(s_date_scan_main, scan, 63);
}

const char *StrToDate(Date& d, const char *s, Date def)
{
	return StrToDate(*s_date_scan_thread ? s_date_scan_thread : s_date_scan_main, d, s, def);
}

const char *StrToDate(const char *fmt, Date& d, const char *s, Date def)
{
	if(*s == 0) {
		d = Null;
		return s;
	}
	d = Nvl(def, GetSysDate());
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
			if(d.year < 35) d.year += 2000; // Check again in 2030.... // TODO: Make this automatic
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

const char *StrToDate(Date& d, const char *s)
{
	return StrToDate(d, s, Null);
}

Date ScanDate(const char *fmt, const char *s, Date def)
{
	Date d;
	if(StrToDate(fmt, d, s, def))
		return d;
	return def;
}

Date ScanDate(const char *s, Date def)
{
	Date d;
	if(StrToDate(d, s, def))
		return d;
	return def;
}

static thread_local char s_date_seps_thread[64];
static char s_date_seps_main[64] = "A/\a .-";

void   SetDateFilter(const char *seps)
{
	strncpy(s_date_seps_thread, seps, 63);
	if(Thread::IsMain())
		strncpy(s_date_seps_main, seps, 63);
}

int  CharFilterDate(int c)
{
	if(IsDigit(c))
		return c;
	const char *s = *s_date_seps_thread ? s_date_seps_thread : s_date_seps_main;
	bool letters = false;
	bool upper = false;
	if(*s == 'a') {
		letters = true;
		s++;
	}
	else
	if(*s == 'A') {
		letters = true;
		upper = true;
		s++;
	}
	if(letters && IsLetter(c))
		return upper ? ToUpper(c) : c;
	;
	int r = 0;
	while(*s) {
		if(c == (byte)*s)
			return c;
		if(*s == '\r') {
			s++;
			while(*s != '\r') {
				if((byte)*s == c)
					return r;
				if(*s == '\0')
					return 0;
				s++;
			}
			s++;
		}
		else
			r = *s++;
	}
	return 0;
}

int Date::Get() const
{
	if(IsNull(*this))
		return Null;
	int y400 = (year / 400 ) - 2;
	int ym = year - y400 * 400;
	return y400 * (400 * 365 + 100 - 3) +
	        ym * 365 + s_month_off[month - 1] + (day - 1) +
	       (ym - 1) / 4 - (ym - 1) / 100 + (ym - 1) / 400 + 1 +
	       (month > 2) * IsLeapYear(ym);
}

void Date::Set(int d)
{
	if(IsNull(d)) {
		*this = Null;
		return;
	}
	int q, leap;
	year = 0;
	q = d / (400 * 365 + 100 - 3);
	year += 400 * q;
	d -= q * (400 * 365 + 100 - 3);
	if(d < 0) {
		year -= 400;
		d += 400 * 365 + 100 - 3;
	}
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

int Date::Compare(Date b) const
{
	int q = SgnCompare(year, b.year);
	if(q) return q;
	q = SgnCompare(month, b.month);
	return q ? q : SgnCompare(day, b.day);
}

bool operator<(Date a, Date b) {
	return a.Compare(b) < 0;
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

int DayOfYear(Date d) {
	return 1 + d - FirstDayOfYear(d);
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

int GetMonths(Date since, Date till)
{
	return 12 * till.year + till.month - (12 * since.year + since.month) + (till.day >= since.day) - 1;
}

int GetMonthsP(Date since, Date till)
{
	return 12 * till.year + till.month - (12 * since.year + since.month) + (till.day > since.day);
}

Date AddYears(Date date, int years) {
	date.year += years;
	date.day = min(date.day, LastDayOfMonth(date).day);
	return date;
}

Date GetWeekDate(int year, int week)
{
	int jan1 = Date(year, 1, 1).Get();
	int start = (jan1 - 584390) / 7 * 7 + 584390; // 584390 = 03.01.1600 Monday
	if(jan1 - start > 3)
		start += 7;
	Date d;
	d.Set(start + 7 * (week - 1));
	return d;
}

int GetWeek(Date d, int& year)
{
	year = d.year;
	Date d0 = GetWeekDate(year + 1, 1);
	if(d >= d0)
		year++;
	else {
		d0 = GetWeekDate(year, 1);
		if(d < d0)
			d0 = GetWeekDate(--year, 1);
	}
	return (d - d0) / 7 + 1;
}

Date EasterDay(int year)
{
    int a = year % 19;
    int b = year >> 2;
    int c = b / 25 + 1;
    int d = (c * 3) >> 2;
    int e = ((a * 19) - ((c * 8 + 5) / 25) + d + 15) % 30;
    e += (29578 - a - e * 32) >> 10;
    e -= ((year % 7) + b - d + e + 2) % 7;
    d = e >> 5;
    return Date(year, d + 3, e - d * 31);
}

void Time::Serialize(Stream& s)
{
	s % day % month % year % hour % minute % second;
}

#ifdef PLATFORM_WIN32
Time::Time(FileTime filetime)
{
	if(filetime) {
		SYSTEMTIME tm, tml;
		FileTime ft;
		FileTimeToSystemTime(&filetime, &tm);
		SystemTimeToTzSpecificLocalTime(NULL, &tm, &tml);
		*this = Time(tml.wYear, tml.wMonth, tml.wDay, tml.wHour, tml.wMinute, tml.wSecond);
	}
	else
		*this = Null;
}

FileTime Time::AsFileTime() const
{
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
	if(filetime) {
		struct tm time_r;
		struct tm *time = localtime_r(&filetime.ft, &time_r);
		if(time)
			*this = Time(time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
		                 time->tm_hour, time->tm_min, time->tm_sec);
	}
	else
		*this = Null;
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

int Time::Compare(Time b) const
{
	int q = Date::Compare(b);
	if(q) return q;
	q = SgnCompare(hour, b.hour);
	if(q) return q;
	q = SgnCompare(minute, b.minute);
	if(q) return q;
	return SgnCompare(second, b.second);
}

bool operator<(Time a, Time b) {
	return a.Compare(b) < 0;
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

bool Time::IsValid() const
{
	return year == -32768 ||
	       Date::IsValid() && hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60;
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

String Format(Time time, bool seconds)
{
	if(IsNull(time)) return String();
	return Format(Date(time)) + (seconds ? Format(" %02d:%02d:%02d", time.hour, time.minute, time.second)
	                                     : Format(" %02d:%02d", time.hour, time.minute));
}

const char *StrToTime(const char *datefmt, Time& d, const char *s)
{
	s = StrToDate(datefmt, d, s);
	if(!s)
		return NULL;
	d.hour = d.minute = d.second = 0;
	for(int i = 0; i < 3; i++) {
		while(*s == ' ' || *s == ':')
			s++;
		int n;
		if(IsDigit(*s)) {
			char *q;
			n = strtoul(s, &q, 10);
			s = q;
		} else
			break;
		if(n < 0 || n > (i ? 59 : 23))
			return NULL;
		(i == 0 ? d.hour : i == 1 ? d.minute : d.second) = n;
	}
	return s;
}

const char *StrToTime(Time& d, const char *s)
{
	return StrToTime(*s_date_scan_thread ? s_date_scan_thread : s_date_scan_main, d, s);
}

Time ScanTime(const char *datefmt, const char *s, Time def)
{
	Time tm;
	if(StrToTime(datefmt, tm, s))
		return tm;
	return def;
}

Time ScanTime(const char *s, Time def)
{
	Time tm;
	if(StrToTime(tm, s))
		return tm;
	return def;
}

#ifdef PLATFORM_WIN32

Time GetSysTime() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	return Time(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

Time GetUtcTime() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return Time(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

#endif

#ifdef PLATFORM_POSIX

Time GetSysTime() {
	return Time(time(NULL));
}

Time GetUtcTime()
{
	time_t gmt = time(NULL);
	struct tm timer;
	struct tm *utc = gmtime_r(&gmt, &timer);
	return Time(utc->tm_year + 1900, utc->tm_mon + 1, utc->tm_mday,
	            utc->tm_hour, utc->tm_min, utc->tm_sec);
}

#endif

Date GetSysDate() {
	return GetSysTime();
}

bool SetSysTime(Time time)
{
#ifdef PLATFORM_POSIX
	struct tm      tmp_time;
	tmp_time.tm_sec  = time.second;
	tmp_time.tm_min  = time.minute;
	tmp_time.tm_hour = time.hour;
	tmp_time.tm_mday = time.day;
	tmp_time.tm_mon  = time.month-1;
	tmp_time.tm_year = time.year-1900;
	time_t raw_time  = mktime(&tmp_time);

	struct ::timespec sys_time;
	sys_time.tv_sec  = raw_time;
	sys_time.tv_nsec = 0;

	int result = clock_settime(CLOCK_REALTIME, &sys_time);
	return (result == 0);
#endif
#ifdef PLATFORM_WIN32
	SYSTEMTIME systime;
	systime.wYear	= time.year;
	systime.wMonth	= time.month;
	systime.wDay	= time.day;
	systime.wHour	= time.hour;
	systime.wMinute	= time.minute;
	systime.wSecond	= time.second;
	systime.wDayOfWeek = 0;
	systime.wMilliseconds = 0;
	return SetLocalTime( &systime );
#endif
}

int GetTimeZone()
{
	static int zone;
	ONCELOCK {
		for(;;) { // This is somewhat ugly, but unified approach to get time zone offset
			Time t0 = GetSysTime();
			Time gmtime = GetUtcTime();
			Time ltime = GetSysTime();
			if(GetSysTime() - t0 < 1) { // Make sure that there is not much time between calls
				zone = (int)(ltime - gmtime) / 60; // Round to minutes
				break;
			}
		}
	}
	return zone;
}

String FormatTimeZone(int n)
{
	return (n < 0 ? "-" : "+") + Format("%02.2d%02.2d", abs(n) / 60, abs(n) % 60);
}

String GetTimeZoneText()
{
	return FormatTimeZone(GetTimeZone());
}

int ScanTimeZone(const char *s)
{
	int n = atoi(s);
	int hour = abs(n) / 100;
	int minutes = abs(n) % 100;
	if(hour >= 0 && hour <= 12 && minutes >= 0 && minutes < 60)
		return sgn(n) * (hour * 60 + minutes);
	return 0;
}

int GetLeapSeconds(Date dt)
{
	static Tuple<int, int> sLeapSeconds[] = {
		{ 1972,6 }, { 1972,12 }, { 1973,12 }, { 1974,12 }, { 1975,12 }, { 1976,12 }, { 1977,12 },
		{ 1978,12 }, { 1979,12 }, { 1981,6 }, { 1982,6 }, { 1983,6 }, { 1985,6 }, { 1987,12 },
		{ 1989,12 }, { 1990,12 }, { 1992,6 }, { 1993,6 }, { 1994,6 }, { 1995,12 }, { 1997,6 },
		{ 1998,12 }, { 2005,12 }, { 2008,12 }, { 2012,6 }, { 2015,6 }, { 2016,12 },
	};
	static byte ls[1200]; // 100 years of leap seconds per month
	ONCELOCK {
		for(int i = 0; i < __countof(sLeapSeconds); i++) {
			int l = (sLeapSeconds[i].a - 1970) * 12 + sLeapSeconds[i].b - 1;
			memset(ls + l, i + 1, __countof(ls) - l);
		}
	}
	return ls[minmax(12 * (dt.year - 1970) + dt.month - 1, 0, __countof(ls) - 1)];
}

int64 GetUTCSeconds(Time tm)
{
	return tm - Time(1970, 1, 1) + GetLeapSeconds(tm);
}

Time TimeFromUTC(int64 seconds)
{
	Time tm = Time(1970, 1, 1) + seconds;
	return tm - GetLeapSeconds(tm);
}

}
