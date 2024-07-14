int utoa32(dword value, char *buffer);
int utoa64(uint64 value, char *buffer);

String FormatUnsigned(dword w);
String FormatInt(int i);
String FormatUInt64(uint64 w);
String FormatInt64(int64 i);

String         FormatIntBase(int i, int base, int width = 0, char lpad = ' ', int sign = 0, bool upper = false);
String         FormatIntDec(int i, int width, char lpad = ' ', bool always_sign = false);
String         FormatIntHex(int i, int width = 8, char lpad = '0');
String         FormatIntHexUpper(int i, int width = 8, char lpad = '0');
String         FormatIntOct(int i, int width = 12, char lpad = '0');
String         FormatIntRoman(int i, bool upper = false);
String         FormatIntAlpha(int i, bool upper = true);
String         Format64Hex(uint64 a);

#ifdef CPU_64
inline String  FormatIntHex(const void *ptr) { return Format64Hex((int64)(uintptr_t)ptr); }
inline String  FormatHex(const void *ptr)    { return Format64Hex((int64)(uintptr_t)ptr); }
#else
inline String  FormatIntHex(const void *ptr) { return FormatIntHex((int)(uintptr_t)ptr); }
inline String  FormatHex(const void *ptr)    { return FormatIntHex((int)(uintptr_t)ptr); }
#endif

enum
{
	FD_SIGN     = 0x01,  // always prepend sign (+10)
	FD_MINUS0   = 0x02,  // show sign for negative zero (-0)
	FD_SIGN_EXP = 0x04,  // always prepend sign to exponent (1e+2)
	FD_CAP_E    = 0x08,  // capital E for exponent (1E10)
	FD_ZEROS    = 0x10,  // keep trailing zeros (1.25000)
	FD_FIX      = 0x20,  // always use fixed notation (FormatF)
	FD_EXP      = 0x40,  // always use exponential notation (FormatG)
	FD_COMMA       = 0x80,  // use ',' instead of '.'
	FD_NOTHSEPS    = 0x100, // In i18n, do not use thousands separators
	FD_MINIMAL_EXP = 0x1000, // Use minimal exp (1e5 instead 1e+05)
	FD_SPECIAL     = 0x2000, // Print nan, inf (if not specified output is empty for nan/inf)
	FD_SIGN_SPACE  = 0x4000, // Add space in place of sign for positive numbers ( 10)
	FD_POINT       = 0x8000, // Always add decimal point
};

inline constexpr dword
FD_TOLERANCE(int x) // how many zeroes can be on the right side of '.' until changing to E format
{ return x << 16; }

char  *FormatE(char *t, double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP);
String FormatE(double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP);

char *FormatF(char *t, double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_ZEROS);
String FormatF(double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_ZEROS);

char *FormatG(char *t, double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_TOLERANCE(3));
String FormatG(double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_TOLERANCE(3));

char *FormatDouble(char *t, double x, int precision, dword flags = FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL);
String FormatDouble(double x, int precision, dword flags = FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL);

char  *FormatDouble(char *t, double x);
String FormatDouble(double x);

String FormatDoubleN(double x);

String FormatFloat(float x);
String FormatFloatN(float x);

String         FormatDate(Date date, const char *format, int language = 0);
String         FormatTime(Time time, const char *format, int language = 0);

template<> inline String AsString(const short& a)           { return FormatInt(a); }
template<> inline String AsString(const unsigned short& a)  { return FormatUnsigned(a); }
template<> inline String AsString(const int& a)             { return FormatInt(a); }
template<> inline String AsString(const unsigned int& a)    { return FormatUnsigned(a); }
template<> inline String AsString(const long& a)            { return FormatInt64(a); }
template<> inline String AsString(const unsigned long& a)   { return FormatUInt64(a); }
template<> inline String AsString(const int64& a)           { return FormatInt64(a); }
template<> inline String AsString(const uint64& a)          { return FormatUInt64(a); }
template<> inline String AsString(const double& a)          { return FormatDoubleN(a); }
template<> inline String AsString(const float& a)           { return FormatFloatN(a); }

struct Formatting
{
	int    language;
	Value  arg;
	String format;
	String id;
};

typedef String (*Formatter)(const Formatting& fmt);

void RegisterFormatter(int type, const char *id, Formatter f);
void RegisterNullFormatter(const char *id, Formatter f);
void RegisterNumberFormatter(const char *id, Formatter f);
void RegisterStringFormatter(const char *id, Formatter f);
void RegisterDateTimeFormatter(const char *id, Formatter f);
void RegisterValueFormatter(const char *id, Formatter f);

String Format(const char *s, const Vector<Value>& v);
String Format(int language, const char *s, const Vector<Value>& v);

template <typename... Args>
String Format(int language, const char *s, const Args& ...args)
{
	return Format(language, s, gather<Vector<Value>>(args...));
}

template <typename... Args>
String Format(const char *s, const Args& ...args)
{
	return Format(s, gather<Vector<Value>>(args...));
}

String VFormat(const char *fmt, va_list args);
String Sprintf(const char *fmt, ...);

String DeFormat(const char *text);

// DEPRECATED

template <typename... Args>
String NFormat(int language, const char *s, const Args& ...args)
{
	return Format(language, s, gather<Vector<Value>>(args...));
}

template <typename... Args>
String NFormat(const char *s, const Args& ...args)
{
	return Format(s, gather<Vector<Value>>(args...));
}

inline String         FormatInteger(int a)                  { return FormatInt(a); }
inline String         Format64(uint64 a)                    { return FormatUInt64(a); }

inline String         FormatDoubleFix(double x, int digits) { return FormatF(x, digits); }
inline String         FormatDoubleExp(double x, int digits) { return FormatE(x, digits); }
