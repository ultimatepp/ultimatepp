#include "Core.h"

#ifdef PLATFORM_WIN32
#include <wingdi.h>
#include <winnls.h>
#endif
#ifdef PLATFORM_POSIX
#include <locale.h>
	#ifndef PLATFORM_ANDROID
	#include <langinfo.h>
	#endif
#endif

namespace Upp {

#define LLOG(x) // LOG(x)

extern int  LanguageList[];
extern const char *LanguageInfoList[];

const int *GetAllLanguages()
{
	return LanguageList;
}

WString DefaultGetIndexLetter(const wchar *text, int)
{
	return IsLetter(*text) ? WString(text, 1) : WString(Null);
}

static int LangCompareDigits(const wchar *&a, const wchar *&b, const wchar *e1, const wchar *e2)
{
	const wchar *p1 = a, *p2 = b;
	while(p1 < e1 && *p1 == '0')
		p1++;
	while(p2 < e2 && *p2 == '0')
		p2++;
	const wchar *x1 = p1, *x2 = p2;
	while(p1 < e1 && IsDigit(*p1))
		p1++;
	while(p2 < e2 && IsDigit(*p2))
		p2++;
	if(p1 - x1 != p2 - x2)
		return cmp(p1 - x1, p2 - x2);
	for(; x1 != p1; x1++, x2++)
		if(*x1 != *x2)
			return *x1 > *x2 ? 1 : -1;
	a = p1;
	b = p2;
	return 0;
}

int DefaultLanguageCompare(const wchar *a, int a_length, const wchar *b, int b_length, int)
{
	int  little = 0, middle = 0;
	const wchar *p1 = a, *e1 = a + a_length, *p2 = b, *e2 = b + b_length;

	while(p1 < e1 && p2 < e2)
	{
		wchar c1 = *p1++;
		wchar c2 = *p2++;

		int level1 = (IsLetter(c1) ? 3 : IsDigit(c1) ? 2 : c1 == ' ' ? 0 : 1);
		int level2 = (IsLetter(c2) ? 3 : IsDigit(c2) ? 2 : c2 == ' ' ? 0 : 1);
		if(level1 != level2)
			return cmp(level1, level2);
		if(level1 <= 1)
		{
			if(c1 != c2)
				return cmp(c1, c2);
			continue;
		}
		if(level1 == 2)
		{ // digits
			const wchar *dp1 = --p1, *dp2 = --p2;
			int res = LangCompareDigits(dp1, dp2, e1, e2);
			if(res)
				return res;
			p1 = dp1;
			p2 = dp2;
			continue;
		}

		int u1, u2, i1, i2;

		i1 = ToAscii(u1 = ToUpper(c1));
		i2 = ToAscii(u2 = ToUpper(c2));

		if(i1 != i2)
			return i1 >= i2 ? 1 : -1;

		if(u1 != u2 && middle == 0) // different diacritics
			middle = u1 - u2;

		if(c1 != c2 && little == 0) // different case
			little = (u1 != c1) - (u2 != c2);
	}
	return a_length != b_length ? cmp(a_length, b_length) : sgn(middle ? middle : little);
}

WString CSCZGetIndexLetter(const wchar *s, int)
{
	wchar temp[3];
	temp[0] = temp[1] = temp[2] = 0;
	if(*s <= 2047 && IsLetter(*s)) // IsLetter
	{
		temp[0] = (wchar)ToUpper(*s);
		if(s[1] <= 2047 && IsLetter(s[1]))
			temp[1] = (wchar)ToLower(s[1]);
		if(temp[0] != 'C' || temp[1] != 'h')
			temp[1] = 0;
		switch(ToUpper(ToAscii(*s)))
		{
		case 'A': case 'E': case 'I': case 'N':
		case 'O': case 'T': case 'U': case 'Y':
			temp[0] = (wchar)ToAscii(temp[0]);
			break;
		}
	}
	return temp;
}

int CSCZLanguageCompare(const wchar *a, int a_length, const wchar *b, int b_length, int)
{
	int  little = 0, middle = 0;
	const wchar *p1 = a, *e1 = a + a_length, *p2 = b, *e2 = b + b_length;

	while(p1 < e1 && p2 < e2)
	{
		wchar c1 = *p1++;
		wchar c2 = *p2++;

		int level1 = (IsLetter(c1) ? 3 : IsDigit(c1) ? 2 : c1 == ' ' ? 0 : 1);
		int level2 = (IsLetter(c2) ? 3 : IsDigit(c2) ? 2 : c2 == ' ' ? 0 : 1);
		if(level1 != level2)
			return cmp(level1, level2);
		if(level1 <= 1)
		{
			if(c1 != c2)
				return cmp(c1, c2);
			continue;
		}
		if(level1 == 2)
		{ // digits
			const wchar *dp1 = --p1, *dp2 = --p2;
			int res = LangCompareDigits(dp1, dp2, e1, e2);
			if(res)
				return res;
			p1 = dp1;
			p2 = dp2;
			continue;
		}

		int u1, u2, i1, i2;

		if((c1 == 'C' || c1 == 'c') && (p1 < e1 && (*p1 == 'H' || *p1 == 'h')))
		{ // CH
			i1 = 'H';
			u1 = 65535;
			p1++;
		}
		else
			i1 = ToAscii(u1 = ToUpper(c1));
		if((c2 == 'C' || c2 == 'c') && (p2 < e2 && (*p2 == 'H' || *p2 == 'h')))
		{ // CH
			i2 = 'H';
			u2 = 65535;
			p2++;
		}
		else
			i2 = ToAscii(u2 = ToUpper(c2));

		if(i1 != i2)
			return i1 >= i2 ? 1 : -1;

		if(u1 != u2) // different diacritics
			switch(i1) {
			case 'A': case 'E': case 'I': case 'N':
			case 'O': case 'T': case 'U': case 'Y':
				if(middle == 0)
					middle = u1 - u2;
				continue;
			default:
				return u1 >= u2 ? 1 : -1;
			}
		if(c1 != c2 && little == 0) // different case
			little = (u1 != c1) - (u2 != c2);
	}
	return a_length != b_length ? cmp(a_length, b_length) : sgn(middle ? middle : little);
}

#ifdef PLATFORM_WIN32

String GetLocaleInfoA(LCID lcid, LCTYPE lctype)
{
	WCHAR cbuf[1000];
	::GetLocaleInfoW(lcid, lctype, cbuf, __countof(cbuf));
	return cbuf;
}

WString GetLocaleInfoW(LCID lcid, LCTYPE lctype)
{
	WCHAR wbuf[1000];
	Zero(wbuf);
	if(::GetLocaleInfoW(lcid, lctype, (WCHAR *)wbuf, __countof(wbuf)))
		return wbuf;
	return Null;
}

#endif

static dword sGetLanguageDetails(int language, String *english_name, String *native_name)
{
	int q = 0;
	byte cs = GetLNGCharset(language);
	language &= ~LNGC_(0, 0, 0, 0, ~0);
	for(const int *ptr = LanguageList; *ptr; ptr++, q++)
		if(*ptr == language) {
			const char *f = LanguageInfoList[q];
			const char *a = strchr(f, '\t');
			const char *b = strchr(a + 1, '\t');
			if(english_name)
				*english_name = String(f, a);
			if(native_name)
				*native_name = ToCharset(cs, String(a + 1, b), CHARSET_UTF8);
			return MAKEWORD(b[2], b[1]);
		}
	return 0;
}

#ifdef PLATFORM_WIN32
LCID GetLanguageLCID(int language)
{
	return sGetLanguageDetails(language, NULL, NULL);
}
#endif

LanguageInfo::LanguageInfo()
{
	getindexletter = DefaultGetIndexLetter;
	compare = DefaultLanguageCompare;
}

void LanguageInfo::Set(int lang_)
{
	language = lang_;
	dword q = sGetLanguageDetails(language, &english_name, &native_name);
	if(!q)
		return;

	getindexletter = DefaultGetIndexLetter;
	compare = DefaultLanguageCompare;
	if(language == LNG_('C','S','C','Z')) {
		getindexletter = CSCZGetIndexLetter;
		compare = CSCZLanguageCompare;
	}

	decimal_point = ".";
	thousand_separator = ",";

#ifdef PLATFORM_WIN32
	LCID lcid = q;
	thousand_separator = GetLocaleInfoA(lcid, LOCALE_STHOUSAND);
	decimal_point = GetLocaleInfoA(lcid, LOCALE_SDECIMAL);
	static const LCTYPE months[] =
	{
		LOCALE_SMONTHNAME1,  LOCALE_SMONTHNAME2,  LOCALE_SMONTHNAME3,
		LOCALE_SMONTHNAME4,  LOCALE_SMONTHNAME5,  LOCALE_SMONTHNAME6,
		LOCALE_SMONTHNAME7,  LOCALE_SMONTHNAME8,  LOCALE_SMONTHNAME9,
		LOCALE_SMONTHNAME10, LOCALE_SMONTHNAME11, LOCALE_SMONTHNAME12,
	};
	static const LCTYPE smonths[] =
	{
		LOCALE_SABBREVMONTHNAME1,  LOCALE_SABBREVMONTHNAME2,  LOCALE_SABBREVMONTHNAME3,
		LOCALE_SABBREVMONTHNAME4,  LOCALE_SABBREVMONTHNAME5,  LOCALE_SABBREVMONTHNAME6,
		LOCALE_SABBREVMONTHNAME7,  LOCALE_SABBREVMONTHNAME8,  LOCALE_SABBREVMONTHNAME9,
		LOCALE_SABBREVMONTHNAME10, LOCALE_SABBREVMONTHNAME11, LOCALE_SABBREVMONTHNAME12,
	};
	ASSERT(__countof(months) == __countof(month_names));
	int i;
	for(i = 0; i < __countof(months); i++)
	{
		month_names[i] = GetLocaleInfoW(lcid, months[i]);
		short_month_names[i] = GetLocaleInfoW(lcid, smonths[i]);
	}
	static const LCTYPE days[] =
	{
		LOCALE_SDAYNAME1, LOCALE_SDAYNAME2, LOCALE_SDAYNAME3, LOCALE_SDAYNAME4,
		LOCALE_SDAYNAME5, LOCALE_SDAYNAME6, LOCALE_SDAYNAME7,
	};
	static const LCTYPE sdays[] =
	{
		LOCALE_SABBREVDAYNAME1, LOCALE_SABBREVDAYNAME2, LOCALE_SABBREVDAYNAME3, LOCALE_SABBREVDAYNAME4,
		LOCALE_SABBREVDAYNAME5, LOCALE_SABBREVDAYNAME6, LOCALE_SABBREVDAYNAME7,
	};
	ASSERT(__countof(days) == __countof(day_names));
	for(i = 0; i < __countof(days); i++)
	{
		day_names[i] = GetLocaleInfoW(lcid, days[i]);
		short_day_names[i] = GetLocaleInfoW(lcid, sdays[i]);
	}
#endif

#if defined(PLATFORM_POSIX) && !defined(PLATFORM_ANDROID)
	String langtext = LNGAsText(language);
	String ltext = Format("%c%c_%c%c", ToLower(langtext[0]), ToLower(langtext[1]), ToUpper(langtext[3]), ToUpper(langtext[4]));
	String oldloc = setlocale(LC_ALL, NULL);
	if(setlocale(LC_ALL, ltext + ".utf8") || setlocale(LC_ALL, ltext + ".UTF-8") || setlocale(LC_ALL, ltext)) {
		const struct lconv *lc = localeconv();
		decimal_point = lc->decimal_point;
		thousand_separator = lc->thousands_sep;
		static const int months[] =
		{
			MON_1, MON_2, MON_3, MON_4, MON_5, MON_6, MON_7, MON_8, MON_9, MON_10, MON_11, MON_12,
		};
		static const int smonths[] =
		{
			ABMON_1, ABMON_2, ABMON_3, ABMON_4, ABMON_5, ABMON_6, ABMON_7, ABMON_8, ABMON_9, ABMON_10, ABMON_11, ABMON_12,
		};
		ASSERT(__countof(months) == __countof(month_names) && __countof(smonths) == __countof(month_names));
		int i;
		for(i = 0; i < __countof(months); i++)
		{
			month_names[i] = nl_langinfo(months[i]);
			short_month_names[i] = nl_langinfo(smonths[i]);
		}
		static const int days[] =
		{ // Linux locale starts with Sunday
			DAY_2, DAY_3, DAY_4, DAY_5, DAY_6, DAY_7, DAY_1,
		};
		static const int sdays[] =
		{
			ABDAY_2, ABDAY_3, ABDAY_4, ABDAY_5, ABDAY_6, ABDAY_7, ABDAY_1,
		};
		ASSERT(__countof(days) == __countof(day_names) && __countof(sdays) == __countof(day_names));
		for(i = 0; i < __countof(days); i++)
		{
			day_names[i] = nl_langinfo(days[i]);
			short_day_names[i] = nl_langinfo(sdays[i]);
		}

		setlocale(LC_ALL, oldloc);
	}
	
#endif
	for(int i = 0; i < 12; i++) {
		month[i] = month_names[i].ToString();
		smonth[i] = short_month_names[i].ToString();
	}
	for(int i = 0; i < 7; i++) {
		day[i] = day_names[i].ToString();
		sday[i] = short_day_names[i].ToString();
	}
}

String LanguageInfo::ToString() const
{
	String out;
	out << "LANGUAGE={" << LNGAsText(language) << "}\n"
		<< "ENGLISH_NAME={" << english_name << "}\n"
		"NATIVE_NAME={" << native_name << "}\n"
		"THOUSAND_SEPARATOR={" << thousand_separator << "}\n"
		"DECIMAL_POINT={" << decimal_point << "}\n"
		"MONTH_NAMES={\n";
	int i;
	for(i = 0; i < __countof(month_names); i++)
		out << "    {" << FromUnicode(month_names[i]) << "} / {" << FromUnicode(short_month_names[i]) << "}\n";
	out << "}\n"
		"DAY_NAMES={\n";
	for(i = 0; i < __countof(day_names); i++)
		out << "    {" << FromUnicode(day_names[i]) << "} / {" << FromUnicode(short_day_names[i]) << "}\n";
	out << "}\n";
	return out;
}

static const char *NlsFindDigits(const char *src, String& dest)
{
	if(*src && !IsDigit(*src))
	{
		const char *start = src;
		while(*++src && !IsDigit(*src))
			;
		dest.Cat(start, (int)(src - start));
	}
	return src;
}

static const char *NlsCopyDigits(const char *src, String& dest, String thousands)
{
	if(IsDigit(*(src = NlsFindDigits(src, dest))))
	{
		const char *p = src;
		while(IsDigit(*++src))
			;
		int first = ((int)(src - p) + 2) % 3 + 1;
		while(p < src)
		{
			dest.Cat(p, first);
			if((p += first) < src)
			{
				dest.Cat(thousands);
				first = 3;
			}
		}
	}
	return src;
}

static String NlsFormatRaw(const char *n, String thousands, String decimals)
{
	if(*n == 0)
		return Null;
	String result;
	n = NlsCopyDigits(n, result, thousands);
	if(*n == (wchar)'.')
	{ // decimal separator
		n++;
		result.Cat(decimals);
		const char *s = n;
		while(IsDigit(*n))
			n++;
		result.Cat(s, (int)(n - s));
	}
	if(*(n = NlsCopyDigits(n, result, thousands)))
		result.Cat(n);
	return result;
}

String LanguageInfo::FormatInt(int value) const
{
	if(IsNull(value))
		return Null;
	String dest;
	String is = UPP::FormatInt(value);
	const char *p = NlsCopyDigits(is, dest, thousand_separator);
	if(*p)
		dest.Cat(p);
	return dest;
}

String LanguageInfo::FormatDouble(double value, int digits, int FD_flags, int fill_exp) const
{
	if(IsNull(value))
		return Null;
	return NlsFormatRaw(UPP::FormatDouble(value, digits, FD_flags),
	                    FD_flags & FD_NOTHSEPS ? String() : thousand_separator,
	                    FD_flags & FD_COMMA ? String(",") : decimal_point);
}

String LanguageInfo::FormatDate(Date date) const
{
	return UPP::FormatDate(date, date_format, language);
}

String LanguageInfo::FormatTime(Time time) const
{
	return UPP::FormatTime(time, time_format, language);
}

ArrayMap<int, LanguageInfo>& LangMap()
{
	static ArrayMap<int, LanguageInfo> x;
	return x;
}

StaticMutex sLanguageInfoMutex;

static std::atomic<const LanguageInfo *> sCurrentLangInfo;

const LanguageInfo& GetLanguageInfo(int lang)
{
	Mutex::Lock __(sLanguageInfoMutex);
	if(!lang)
		lang = GetCurrentLanguage();
	lang = SetLNGCharset(lang, 0);
	ArrayMap<int, LanguageInfo>& m = LangMap();
	int q = m.Find(lang);
	if(q >= 0)
		return m[q];
	LanguageInfo& f = m.Add(lang);
	f.Set(lang);
	return f;
}

void SyncLngInfo__()
{
	sCurrentLangInfo = &GetLanguageInfo(GetCurrentLanguage());
}

const LanguageInfo& GetLanguageInfo()
{
	if(!sCurrentLangInfo.load())
		SyncLngInfo__();
	return *sCurrentLangInfo;
}

void SetLanguageInfo(int lang, const LanguageInfo& lf)
{
	Mutex::Lock __(sLanguageInfoMutex);
	if(!lang)
		lang = GetCurrentLanguage();
	ArrayMap<int, LanguageInfo>& m = LangMap();
	m.GetAdd(lang) = lf;
}

int LanguageList[] = {
	LNG_('A','R','S','A'),
	LNG_('B','G','B','G'),
	LNG_('C','A','E','S'),
	LNG_('Z','H','T','W'),
	LNG_('C','S','C','Z'),
	LNG_('D','A','D','K'),
	LNG_('D','E','D','E'),
	LNG_('E','L','G','R'),
	LNG_('E','N','U','S'),
	LNG_('E','S','E','S'),
	LNG_('F','I','F','I'),
	LNG_('F','R','F','R'),
	LNG_('H','E','I','L'),
	LNG_('H','U','H','U'),
	LNG_('I','S','I','S'),
	LNG_('I','T','I','T'),
	LNG_('J','A','J','P'),
	LNG_('K','O','K','R'),
	LNG_('N','L','N','L'),
	LNG_('N','B','N','O'),
	LNG_('P','L','P','L'),
	LNG_('P','T','B','R'),
	LNG_('R','O','R','O'),
	LNG_('R','U','R','U'),
	LNG_('H','R','H','R'),
	LNG_('S','K','S','K'),
	LNG_('S','Q','A','L'),
	LNG_('S','V','S','E'),
	LNG_('T','H','T','H'),
	LNG_('T','R','T','R'),
	LNG_('U','R','P','K'),
	LNG_('I','D','I','D'),
	LNG_('U','K','U','A'),
	LNG_('B','E','B','Y'),
	LNG_('S','L','S','I'),
	LNG_('E','T','E','E'),
	LNG_('L','V','L','V'),
	LNG_('L','T','L','T'),
	LNG_('F','A','I','R'),
	LNG_('V','I','V','N'),
	LNG_('H','Y','A','M'),
	LNG_('A','Z','A','Z'),
	LNG_('E','U','E','S'),
	LNG_('M','K','M','K'),
	LNG_('A','F','Z','A'),
	LNG_('K','A','G','E'),
	LNG_('F','O','F','O'),
	LNG_('H','I','I','N'),
	LNG_('M','S','M','Y'),
	LNG_('K','K','K','Z'),
	LNG_('K','Y','K','G'),
	LNG_('S','W','K','E'),
	LNG_('U','Z','U','Z'),
	LNG_('T','T','R','U'),
	LNG_('P','A','I','N'),
	LNG_('G','U','I','N'),
	LNG_('T','A','I','N'),
	LNG_('T','E','I','N'),
	LNG_('K','N','I','N'),
	LNG_('M','R','I','N'),
	LNG_('S','A','I','N'),
	LNG_('M','N','M','N'),
	LNG_('G','L','E','S'),
	LNG_('K','O','I','N'),
	LNG_('S','Y','S','Y'),
	LNG_('D','I','M','V'),
	LNG_('A','R','I','Q'),
	LNG_('Z','H','C','N'),
	LNG_('D','E','C','H'),
	LNG_('E','N','G','B'),
	LNG_('E','S','M','X'),
	LNG_('F','R','B','E'),
	LNG_('I','T','C','H'),
	LNG_('N','L','B','E'),
	LNG_('N','N','N','O'),
	LNG_('P','T','P','T'),
	LNG_('S','R','S','P'),
	LNG_('S','V','F','I'),
	LNG_('A','Z','A','Z'),
	LNG_('M','S','B','N'),
	LNG_('U','Z','U','Z'),
	LNG_('A','R','E','G'),
	LNG_('Z','H','H','K'),
	LNG_('D','E','A','T'),
	LNG_('E','N','A','U'),
	LNG_('E','S','E','S'),
	LNG_('F','R','C','A'),
	LNG_('S','R','S','P'),
	LNG_('A','R','L','Y'),
	LNG_('Z','H','S','G'),
	LNG_('D','E','L','U'),
	LNG_('E','N','C','A'),
	LNG_('E','S','G','T'),
	LNG_('F','R','C','H'),
	LNG_('A','R','D','Z'),
	LNG_('Z','H','M','O'),
	LNG_('D','E','L','I'),
	LNG_('E','N','N','Z'),
	LNG_('E','S','C','R'),
	LNG_('F','R','L','U'),
	LNG_('A','R','M','A'),
	LNG_('E','N','I','E'),
	LNG_('E','S','P','A'),
	LNG_('F','R','M','C'),
	LNG_('A','R','T','N'),
	LNG_('E','N','Z','A'),
	LNG_('E','S','D','O'),
	LNG_('A','R','O','M'),
	LNG_('E','N','J','M'),
	LNG_('E','S','V','E'),
	LNG_('A','R','Y','E'),
	LNG_('E','N','C','B'),
	LNG_('E','S','C','O'),
	LNG_('A','R','S','Y'),
	LNG_('E','N','B','Z'),
	LNG_('E','S','P','E'),
	LNG_('A','R','J','O'),
	LNG_('E','N','T','T'),
	LNG_('E','S','A','R'),
	LNG_('A','R','L','B'),
	LNG_('E','N','Z','W'),
	LNG_('E','S','E','C'),
	LNG_('A','R','K','W'),
	LNG_('E','N','P','H'),
	LNG_('E','S','C','L'),
	LNG_('A','R','A','E'),
	LNG_('E','S','U','Y'),
	LNG_('A','R','B','H'),
	LNG_('E','S','P','Y'),
	LNG_('A','R','Q','A'),
	LNG_('E','S','B','O'),
	LNG_('E','S','S','V'),
	LNG_('E','S','H','N'),
	LNG_('E','S','N','I'),
	LNG_('E','S','P','R'),
	LNG_('H','R','B','A'),
	LNG_('B','S','B','A'),
	LNG_('S','R','B','A'),
	LNG_('C','Y','G','B'),
	LNG_('M','I','N','Z'),
	LNG_('M','T','M','T'),
	LNG_('Q','U','B','O'),
	LNG_('Q','U','E','C'),
	LNG_('Q','U','P','E'),
	LNG_('T','N','Z','A'),
	LNG_('X','H','Z','A'),
	LNG_('Z','U','Z','A'),
	LNG_('N','S','Z','A'),
	LNG_('S','E','N','O'),
	LNG_('S','E','S','E'),
	LNG_('S','E','F','I'),
	LNG_('S','E','N','O'),
	LNG_('S','E','S','E'),
	LNG_('S','E','N','O'),
	LNG_('S','E','S','E'),
	LNG_('S','E','F','I'),
	LNG_('S','E','F','I'),
	LNG_('S','R','B','A'),
	LNG_('B','S','B','A'),
	LNG_('F','I','P','H'),
	LNG_('L','B','L','U'),
	LNG_('P','S','A','F'),
	LNG_('F','Y','N','L'),
	LNG_('A','R','C','L'),
	LNG_('N','E','N','P'),
	LNG_('I','U','C','A'),
	LNG_('G','A','I','E'),
	LNG_('M','O','C','A'),
	LNG_('R','M','C','H'),
	LNG_('B','N','I','N'),
	LNG_('M','L','I','N'),
	0
};

const char *LanguageInfoList[] = {
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t\004\001",
	"Bulgarian\t\320\261\321\212\320\273\320\263\320\260\321\200\321\201\320\272\320\270\t\004\002",
	"Catalan\tcatal\303\240\t\004\003",
	"Chinese\t\344\270\255\346\226\207(\347\271\201\351\253\224)\t\004\004",
	"Czech\t\304\215e\305\241tina\t\004\005",
	"Danish\tdansk\t\004\006",
	"German\tDeutsch\t\004\a",
	"Greek\t\316\265\316\273\316\273\316\267\316\275\316\271\316\272\316\254\t\004\b",
	"English\tEnglish\t\004\t",
	"Spanish\tespa\303\261ol\t\004\n",
	"Finnish\tsuomi\t\004\v",
	"French\tfran\303\247ais\t\004\f",
	"Hebrew\t\327\242\327\221\327\250\327\231\327\252\t\004\r",
	"Hungarian\tMagyar\t\004\016",
	"Icelandic\t\303\255slenska\t\004\017",
	"Italian\titaliano\t\004\020",
	"Japanese\t\346\227\245\346\234\254\350\252\236\t\004\021",
	"Korean\t\355\225\234\352\265\255\354\226\264\t\004\022",
	"Dutch\tNederlands\t\004\023",
	"Norwegian (Bokm\303\245l)\tnorsk (bokm\303\245l)\t\004\024",
	"Polish\tpolski\t\004\025",
	"Portuguese\tPortugu\303\252s\t\004\026",
	"Romanian\trom\303\242n\304\203\t\004\030",
	"Russian\t\321\200\321\203\321\201\321\201\320\272\320\270\320\271\t\004\031",
	"Croatian\thrvatski\t\004\032",
	"Slovak\tsloven\304\215ina\t\004\033",
	"Albanian\tshqipe\t\004\034",
	"Swedish\tsvenska\t\004\035",
	"Thai\t\340\271\204\340\270\227\340\270\242\t\004\036",
	"Turkish\tT\303\274rk\303\247e\t\004\037",
	"Urdu\t\330\247\331\217\330\261\330\257\331\210\t\004 ",
	"Indonesian\tBahasa Indonesia\t\004!",
	"Ukrainian\t\321\203\320\272\321\200\320\260\321\227\320\275\321\214\321\201\320\272\320\260\t\004\"",
	"Belarusian\t\320\221\320\265\320\273\320\260\321\200\321\203\321\201\320\272\321\226\t\004#",
	"Slovenian\tslovenski\t\004$",
	"Estonian\teesti\t\004%",
	"Latvian\tlatvie\305\241u\t\004&",
	"Lithuanian\tlietuvi\305\263\t\004'",
	"Farsi\t\331\201\330\247\330\261\330\263\331\211\t\004)",
	"Vietnamese\tTi\303\252\314\201ng Vi\303\252\314\243t Nam\t\004*",
	"Armenian\t\325\200\325\241\325\265\325\245\326\200\325\245\325\266\t\004+",
	"Azeri (Latin)\tAz\311\231rbaycan\302\255\304\261l\304\261\t\004,",
	"Basque\teuskara\t\004-",
	"FYRO Macedonian\t\320\274\320\260\320\272\320\265\320\264\320\276\320\275\321\201\320\272\320\270 \321\230\320\260\320\267\320\270\320\272\t\004/",
	"Afrikaans\tAfrikaans\t\0046",
	"Georgian\t\341\203\245\341\203\220\341\203\240\341\203\227\341\203\243\341\203\232\341\203\230\t\0047",
	"Faroese\tf\303\270royskt\t\0048",
	"Hindi\t\340\244\271\340\244\277\340\244\202\340\244\246\340\245\200\t\0049",
	"Malay\tBahasa Malaysia\t\004>",
	"Kazakh\t\322\232\320\260\320\267\320\260\322\233\t\004?",
	"Kyrgyz\t\320\232\321\213\321\200\320\263\321\213\320\267\t\004@",
	"Swahili\tKiswahili\t\004A",
	"Uzbek (Latin)\tU'zbek\t\004C",
	"Tatar\t\320\242\320\260\321\202\320\260\321\200\t\004D",
	"Punjabi\t\340\250\252\340\251\260\340\250\234\340\250\276\340\250\254\340\251\200\t\004F",
	"Gujarati\t\340\252\227\340\253\201\340\252\234\340\252\260\340\252\276\340\252\244\340\253\200\t\004G",
	"Tamil\t\340\256\244\340\256\256\340\256\277\340\256\264\340\257\215\t\004I",
	"Telugu\t\340\260\244\340\261\206\340\260\262\340\261\201\340\260\227\340\261\201\t\004J",
	"Kannada\t\340\262\225\340\262\250\340\263\215\340\262\250\340\262\241\t\004K",
	"Marathi\t\340\244\256\340\244\260\340\244\276\340\244\240\340\245\200\t\004N",
	"Sanskrit\t\340\244\270\340\244\202\340\244\270\340\245\215\340\244\225\340\245\203\340\244\244\t\004O",
	"Mongolian\t\320\234\320\276\320\275\320\263\320\276\320\273\302\240\321\205\321\215\320\273\t\004P",
	"Galician\tgalego\t\004V",
	"Konkani\t\340\244\225\340\245\213\340\244\202\340\244\225\340\244\243\340\245\200\t\004W",
	"Syriac\t\334\243\334\230\334\252\334\235\334\235\334\220\t\004Z",
	"Divehi\t\336\213\336\250\336\210\336\254\336\200\336\250\336\204\336\246\336\220\336\260\t\004e",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t\b\001",
	"Chinese\t\344\270\255\346\226\207(\347\256\200\344\275\223)\t\b\004",
	"German\tDeutsch\t\b\a",
	"English\tEnglish\t\b\t",
	"Spanish\tEspa\303\261ol\t\b\n",
	"French\tfran\303\247ais\t\b\f",
	"Italian\titaliano\t\b\020",
	"Dutch\tNederlands\t\b\023",
	"Norwegian (Nynorsk)\tnorsk (nynorsk)\t\b\024",
	"Portuguese\tportugu\303\252s\t\b\026",
	"Serbian (Latin)\tsrpski\t\b\032",
	"Swedish\tsvenska\t\b\035",
	"Azeri (Cyrillic)\t\320\220\320\267\323\231\321\200\320\261\320\260\321\230\322\271\320\260\320\275\t\b,",
	"Malay\tBahasa Malaysia\t\b>",
	"Uzbek (Cyrillic)\t\320\216\320\267\320\261\320\265\320\272\t\bC",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t\f\001",
	"Chinese\t\344\270\255\346\226\207(\347\271\201\351\253\224)\t\f\004",
	"German\tDeutsch\t\f\a",
	"English\tEnglish\t\f\t",
	"Spanish\tespa\303\261ol\t\f\n",
	"French\tfran\303\247ais\t\f\f",
	"Serbian (Cyrillic)\t\321\201\321\200\320\277\321\201\320\272\320\270\t\f\032",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t\020\001",
	"Chinese\t\344\270\255\346\226\207(\347\256\200\344\275\223)\t\020\004",
	"German\tDeutsch\t\020\a",
	"English\tEnglish\t\020\t",
	"Spanish\tEspa\303\261ol\t\020\n",
	"French\tfran\303\247ais\t\020\f",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t\024\001",
	"Chinese\t\344\270\255\346\226\207(\347\256\200\344\275\223)\t\024\004",
	"German\tDeutsch\t\024\a",
	"English\tEnglish\t\024\t",
	"Spanish\tEspa\303\261ol\t\024\n",
	"French\tfran\303\247ais\t\024\f",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t\030\001",
	"English\tEnglish\t\030\t",
	"Spanish\tEspa\303\261ol\t\030\n",
	"French\tfran\303\247ais\t\030\f",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t\034\001",
	"English\tEnglish\t\034\t",
	"Spanish\tEspa\303\261ol\t\034\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t \001",
	"English\tEnglish\t \t",
	"Spanish\tEspa\303\261ol\t \n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t$\001",
	"English\tEnglish\t$\t",
	"Spanish\tEspa\303\261ol\t$\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t(\001",
	"English\tEnglish\t(\t",
	"Spanish\tEspa\303\261ol\t(\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t,\001",
	"English\tEnglish\t,\t",
	"Spanish\tEspa\303\261ol\t,\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t0\001",
	"English\tEnglish\t0\t",
	"Spanish\tEspa\303\261ol\t0\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t4\001",
	"English\tEnglish\t4\t",
	"Spanish\tEspa\303\261ol\t4\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t8\001",
	"Spanish\tEspa\303\261ol\t8\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t<\001",
	"Spanish\tEspa\303\261ol\t<\n",
	"Arabic\t\330\247\331\204\330\271\330\261\330\250\331\212\330\251\t@\001",
	"Spanish\tEspa\303\261ol\t@\n",
	"Spanish\tEspa\303\261ol\tD\n",
	"Spanish\tEspa\303\261ol\tH\n",
	"Spanish\tEspa\303\261ol\tL\n",
	"Spanish\tEspa\303\261ol\tP\n",
	"Croatian\thrvatski\t\020\032",
	"Bosnian\tbosanski\t\024\032",
	"Serbian (Latin)\tsrpski\t\030\032",
	"Welsh\tCymraeg\t\004R",
	"Maori\tReo M\304\201ori\t\004\201",
	"Maltese\tMalti\t\004:",
	"Quechua\trunasimi\t\004k",
	"Quechua\trunasimi\t\bk",
	"Quechua\trunasimi\t\fk",
	"Tswana\tSetswana\t\0042",
	"Xhosa\tisiXhosa\t\0044",
	"Zulu\tisiZulu\t\0045",
	"Northern Sotho\tSesotho sa Leboa\t\004l",
	"Sami (Northern)\tdavvis\303\241megiella\t\004;",
	"Sami (Northern)\tdavvis\303\241megiella\t\b;",
	"Sami (Northern)\tdavvis\303\241megiella\t\f;",
	"Sami (Lule)\tjulevus\303\241megiella\t\020;",
	"Sami (Lule)\tjulevus\303\241megiella\t\024;",
	"Sami (Southern)\t\303\245arjelsaemiengiele\t\030;",
	"Sami (Southern)\t\303\245arjelsaemiengiele\t\034;",
	"Sami (Skolt)\ts\303\244\303\244m\302\264\307\251i\303\265ll\t ;",
	"Sami (Inari)\ts\303\244mikiel\303\242\t$;",
	"Serbian (Cyrillic)\t\321\201\321\200\320\277\321\201\320\272\320\270\t\034\032",
	"Bosnian (Cyrillic)\t\320\261\320\276\321\201\320\260\320\275\321\201\320\272\320\270\t \032",
	"Filipino\tFilipino\t\004d",
	"Luxembourgish\tL\303\253tzebuergesch\t\004n",
	"Pashto\t\331\276\332\232\330\252\331\210\t\004c",
	"Frisian\tFrysk\t\004b",
	"Mapudungun\tMapudungun\t\004z",
	"Nepali\t\340\244\250\340\245\207\340\244\252\340\244\276\340\244\262\340\245\200\t\004a",
	"Inuktitut (Latin)\tInuktitut (Kanatami)\t\b]",
	"Irish\tGaeilge\t\b<",
	"Mohawk\tKanien'k\303\251ha\t\004|",
	"Romansh\tRumantsch\t\004\027",
	"Bengali\t\340\246\254\340\246\276\340\246\242\340\246\262\340\246\276\t\004E",
	"Malayalam\t\340\264\256\340\264\262\340\264\257\340\264\276\340\264\263\340\264\202\t\004L",
};

};
