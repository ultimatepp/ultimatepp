String         FormatIntBase(int i, int base, int width = 0, char lpad = ' ', int sign = 0, bool upper = false);
String         FormatInt(int i);
String         FormatIntDec(int i, int width, char lpad = ' ', bool always_sign = false);
String         FormatIntHex(int i, int width = 8, char lpad = '0');
String         FormatIntHexUpper(int i, int width = 8, char lpad = '0');
String         FormatIntOct(int i, int width = 12, char lpad = '0');
String         FormatIntRoman(int i, bool upper = false);
String         FormatIntAlpha(int i, bool upper = true);
String         Format64(uint64 a);
String         Format64Hex(uint64 a);

#ifdef CPU_64
inline String  FormatIntHex(const void *ptr) { return Format64Hex((int64)(uintptr_t)ptr); }
inline String  FormatHex(const void *ptr)    { return Format64Hex((int64)(uintptr_t)ptr); }
#else
inline String  FormatIntHex(const void *ptr) { return FormatIntHex((int)(uintptr_t)ptr); }
inline String  FormatHex(const void *ptr)    { return FormatIntHex((int)(uintptr_t)ptr); }
#endif

String         FormatInteger(int a);
String         FormatUnsigned(unsigned long a);
String         FormatDouble(double a);
String         FormatBool(bool a);
String         FormatInt64(int64 a);

template<> inline String AsString(const short& a)           { return FormatInteger(a); }
template<> inline String AsString(const unsigned short& a)  { return FormatUnsigned(a); }
template<> inline String AsString(const int& a)             { return FormatInteger(a); }
template<> inline String AsString(const unsigned int& a)    { return FormatUnsigned(a); }
template<> inline String AsString(const long& a)            { return FormatInt64(a); }
template<> inline String AsString(const unsigned long& a)   { return Format64(a); }
template<> inline String AsString(const double& a)          { return FormatDouble(a); }
template<> inline String AsString(const float& a)           { return FormatDouble(a); }
template<> inline String AsString(const int64& a)           { return FormatInt64(a); }
template<> inline String AsString(const uint64& a)          { return Format64(a); }

enum
{
	FD_SIGN     = 0x01,  // always prepend sign (+10)
	FD_REL      = 0x02,  // relative decimal places (valid digits)
	FD_SIGN_EXP = 0x04,  // always prepend sign to exponent (1e+2)
	FD_CAP_E    = 0x08,  // capital E for exponent (1E10)
	FD_ZEROS    = 0x10,  // keep trailing zeros (1.25000)
	FD_FIX      = 0x20,  // always use fixed notation (FormatDouble)
	FD_EXP      = 0x40,  // always use exponential notation (FormatDouble)
	FD_COMMA    = 0x80,  // use ',' instead of '.'
	FD_NOTHSEPS = 0x100, // In i18n, do not use thousands separators
};

String         FormatDoubleDigits(double d, int digits, int flags, int& exponent);
String         FormatDouble(double d, int digits, int flags = 0, int fill_exp = 0);
String         FormatDoubleFix(double d, int digits, int flags = 0);
String         FormatDoubleExp(double d, int digits, int flags = 0, int fill_exp = 0);

String         FormatDate(Date date, const char *format, int language = 0);
String         FormatTime(Time time, const char *format, int language = 0);

inline String  IntStr(int i)         { return FormatInt(i); }
inline String  IntStr64(int64 i)     { return FormatInt64(i); }
inline String  DblStr(double d)      { return FormatDouble(d, 10); }

/*
Date        ScanDate(const char *text, const char **endptr, const char *format, int language, Date base_date);
Time        ScanTime(const char *text, const char **endptr, const char *format, int language, Time base_time);
*/

struct Formatting
{
	int    language;
	Value  arg;
	String format;
	String id;
};

typedef String (*Formatter)(const Formatting& fmt);

void RegisterFormatter(int type, const char *id, Formatter f) init_;
void RegisterNullFormatter(const char *id, Formatter f) init_;
void RegisterNumberFormatter(const char *id, Formatter f) init_;
void RegisterStringFormatter(const char *id, Formatter f) init_;
void RegisterDateTimeFormatter(const char *id, Formatter f) init_;
void RegisterValueFormatter(const char *id, Formatter f) init_;

#define E__NFValue(I)  const Value& COMBINE(p, I)
#define E__NFBody(I) \
String NFormat(const char *fmt, __List##I(E__NFValue)); \
String NFormat(int language, const char *fmt, __List##I(E__NFValue));

//$-String NFormat(const char *fmt, Value p1, ...);
//$ String NFormat(int language, const char *fmt, Value p1, ...);
__Expand20(E__NFBody)
//$+

#undef E__NFBody
#undef E__NFValue

String NFormat(const char *s, const Vector<Value>& v);
String NFormat(int language, const char *s, const Vector<Value>& v);

String VFormat(const char *fmt, va_list args);
String Sprintf(const char *fmt, ...);

//$-

#define E__NFValue(I)  const Value& COMBINE(p, I)
#define E__NFBody(I) \
String Format(const char *fmt, __List##I(E__NFValue)); \
String Format(int language, const char *fmt, __List##I(E__NFValue));

__Expand20(E__NFBody)

#undef E__NFBody
#undef E__NFValue

String Format(const char *s, const Vector<Value>& v);
String Format(int language, const char *s, const Vector<Value>& v);

String DeFormat(const char *text);
