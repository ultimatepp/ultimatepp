class Nuller;
class Stream;
struct FileTime;

struct Date : RelOps< Date, Moveable<Date> > {
	byte   day;
	byte   month;
	int16  year;

	void     Serialize(Stream& s);

	bool     IsValid() const;
	void     Set(int scalar);
	int      Get() const;

	static Date Low()                    { return Date(-4000, 1, 1); }
	static Date High()                   { return Date(4000, 1, 1); }
	
	int      Compare(Date b) const;

	Date& operator++()                   { if(day < 28) day++; else Set(Get() + 1); return *this; }
	Date& operator--()                   { if(day > 1) day--; else Set(Get() - 1); return *this; }

	Date  operator++(int)                { Date d = *this; operator++(); return d; }
	Date  operator--(int)                { Date d = *this; operator--(); return d; }

	Date()                               { year = -32768; day = month = 0; }
	Date(const Nuller&)                  { year = -32768; day = month = 0; }
	Date(int y, int m, int d)            { day = d; month = m; year = y; }
};

inline hash_t GetHashValue(Date t) {
	return 512 * t.year + 32 * t.month + t.day;
}

inline bool operator==(Date a, Date b) {
	return a.day == b.day && a.month == b.month && a.year == b.year;
}

template<> inline bool  IsNull(const Date& d)    { return d.year == -32768; }

bool operator<(Date a, Date b);

int   operator-(Date a, Date b);
Date  operator+(Date a, int b);
Date  operator+(int a, Date b);
Date  operator-(Date a, int b);
Date& operator+=(Date& a, int b);
Date& operator-=(Date& a, int b);

bool IsLeapYear(int year);

int  GetDaysOfMonth(int month, int year);

int  DayOfWeek(Date date);
Date LastDayOfMonth(Date d);
Date FirstDayOfMonth(Date d);
Date LastDayOfYear(Date d);
Date FirstDayOfYear(Date d);
int  DayOfYear(Date d);

Date AddMonths(Date date, int months);
int  GetMonths(Date since, Date till);
int  GetMonthsP(Date since, Date till);
Date AddYears(Date date, int years);

Date GetWeekDate(int year, int week);
int  GetWeek(Date d, int& year);

Date EasterDay(int year);

Date GetSysDate();

String DayName(int i, int lang = 0);
String DyName(int i, int lang = 0);
String MonthName(int i, int lang = 0);
String MonName(int i, int lang = 0);

void   SetDateFormat(const char *fmt);
void   SetDateScan(const char *scan);
void   SetDateFilter(const char *seps);

#ifdef PLATFORM_WIN32 // resolve nameclash with win32 API
	#ifdef GetDateFormatA
		#undef GetDateFormatA
	#endif

	#ifdef GetDateFormatW
		#undef GetDateFormatW
	#endif
#endif

String GetDateFormat();

const char *StrToDate(const char *fmt, Date& d, const char *s, Date def = Null);
const char *StrToDate(Date& d, const char *s, Date def);
const char *StrToDate(Date& d, const char *s);
Date        ScanDate(const char *fmt, const char *s, Date def = Null);
Date        ScanDate(const char *s, Date def = Null);
String      Format(Date date, const char *fmt);
String      Format(Date date);
int         CharFilterDate(int c);

template<>
inline String AsString(const Date& date) { return Format(date); }

struct Time : Date, RelOps< Time, Moveable<Time> > {
	byte   hour;
	byte   minute;
	byte   second;

	void     Serialize(Stream& s);

	static Time High()       { return Time(4000, 1, 1); }
	static Time Low()        { return Time(-4000, 1, 1); }

	void   Set(int64 scalar);
	int64  Get() const;

	bool   IsValid() const;

	int    Compare(Time b) const;

	Time()                   { hour = minute = second = 0; }
	Time(const Nuller&)      { hour = minute = second = 0; }
	Time(int y, int m, int d, int h = 0, int n = 0, int s = 0)
		{ day = d; month = m; year = y; hour = h; minute = n; second = s; }

	Time(FileTime filetime);
	FileTime AsFileTime() const;
};

inline Time ToTime(const Date& d) {
	return Time(d.year, d.month, d.day);
}

inline hash_t GetHashValue(Time t) {
	return t.second +
		   32 * (t.minute + 32 * (t.hour + 16 * (t.day + 32 * (t.month + 8 * t.year))));
}

template<> inline bool  IsNull(const Time& t)    { return t.year == -32768; }

bool operator==(Time a, Time b);
bool operator<(Time a, Time b);

int64  operator-(Time a, Time b);
Time   operator+(Time a, int64 seconds);
Time   operator+(int64 seconds, Time a);
Time   operator-(Time a, int64 secs);
Time&  operator+=(Time& a, int64 secs);
Time&  operator-=(Time& a, int64 secs);

inline Time   operator+(Time a, int i)   { return a +  int64(i); }
inline Time   operator-(Time a, int i)   { return a -  int64(i); }
inline Time&  operator+=(Time& a, int i) { return a += int64(i); }
inline Time&  operator-=(Time& a, int i) { return a -= int64(i); }

inline Time   operator+(Time a, double i)   { return a +  int64(i); }
inline Time   operator-(Time a, double i)   { return a -  int64(i); }
inline Time&  operator+=(Time& a, double i) { return a += int64(i); }
inline Time&  operator-=(Time& a, double i) { return a -= int64(i); }

Time  GetSysTime();
Time  GetUtcTime();

String Format(Time time, bool seconds = true);
const char *StrToTime(const char *datefmt, Time& d, const char *s);
const char *StrToTime(Time& d, const char *s);
Time        ScanTime(const char *datefmt, const char *s, Time def = Null);
Time        ScanTime(const char *s, Time def = Null);

template<>
inline String AsString(const Time& time) { return Format(time); }

bool SetSysTime(Time time); // only root/sysadmin can do this...

int    GetTimeZone();
String GetTimeZoneText();
int    ScanTimeZoneText(const char *s);
int    ScanTimeZone(const char *s);

int   GetLeapSeconds(Date dt);
int64 GetUTCSeconds(Time tm);
Time  TimeFromUTC(int64 seconds);
