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

#define LLOG(x)  LOG(x)

String LNGAsText(int d)
{
	String result;
	int c = (d >> 15) & 31;
	if(c) {
		result.Cat(c + 'A' - 1);
		c = (d >> 10) & 31;
		if(c) {
			result.Cat(c + 'A' - 1);
			c = (d >> 5) & 31;
			if(c) {
				result.Cat('-');
				result.Cat(c + 'A' - 1);
				c = d & 31;
				if(c) result.Cat(c + 'A' - 1);
			}
		}
	}
	c = (d >> 20) & 255;
	if(c)
		result << ' ' << CharsetName(c);
	return result;
}

byte GetLNGCharset(int d)
{
	byte cs = byte(d >> 20);
	return cs ? cs : CHARSET_UTF8;
}

int  SetLNGCharset(int lng, byte chrset)
{
	return (lng & ~(0xffffffff << 20)) | (chrset << 20);
}

int LNGFromText(const char *s)
{
	int l = 0;
	if(IsAlpha(*s)) {
		l = (ToUpper(*s++) - 'A' + 1) << 15;
		if(IsAlpha(*s)) {
			l |= (ToUpper(*s++) - 'A' + 1) << 10;
			if(*s && !IsAlpha(*s))
				s++;
			if(IsAlpha(*s)) {
				l |= (ToUpper(*s++) - 'A' + 1) << 5;
				if(IsAlpha(*s)) {
					l |= (ToUpper(*s++) - 'A' + 1);
					while(*s && *s != ' ')
						s++;
					if(*s == ' ') {
						s++;
						int cs = CharsetByName(s);
						if(cs > 0)
							l |= (cs << 20);
						else
							return 0;
					}
					return l;
				}
			}
		}
	}
	return 0;
}

#ifdef PLATFORM_WIN32

String GetUserLocale(dword type)
{
#ifdef PLATFORM_WINCE
	wchar h[256];
	int n = ::GetLocaleInfo(GetUserDefaultLCID(), type, h, 256);
	return n ? WString(h, n - 1).ToString() : String();
#else
	char h[256];
	int n =:: GetLocaleInfo(GetUserDefaultLCID(), type, h, 256);
	return n ? String(h, n - 1) : String();
#endif
}

int GetSystemLNG()
{
	static int lang;
	ONCELOCK {
		lang = LNGFromText(GetUserLocale(LOCALE_SISO639LANGNAME) + GetUserLocale(LOCALE_SISO3166CTRYNAME));
		if(!lang)
			lang = LNG_('E', 'N', 'U', 'S');
		int cs = atoi(GetUserLocale(LOCALE_IDEFAULTANSICODEPAGE));
		if(cs >= 1250 && cs <= 1258)
			lang = SetLNGCharset(lang, CHARSET_WIN1250 + cs - 1250);
	}
	return lang;
}
#endif

#ifdef PLATFORM_POSIX
int GetSystemLNG() {
	static int lang;
	ONCELOCK {
		String s = Environment().Get("LANG", Null);
		lang = LNGFromText(s);
		if(!lang)
			lang = LNG_ENGLISH;
		const char *q = strchr(s, '.');
		if(q)
			lang = SetLNGCharset(lang, CharsetByName(q + 1));
	};
	return lang;
};

#endif

class LangConvertClass : public Convert {
	virtual Value  Format(const Value& q) const {
		return LNGAsText((int)q);
	}

	virtual Value  Scan(const Value& text) const {
		if(IsNull(text)) return 0;
		int q = LNGFromText((String)text);
		if(!q) return ErrorValue(t_("Invalid language specification."));
		return (int) q;
	}

	virtual int    Filter(int chr) const {
		return chr == ' ' || chr == '-' || IsDigit(chr) ? chr : IsAlpha(chr) ? ToUpper(chr) : 0;
	}
};

Convert& LNGConvert()
{
	return Single<LangConvertClass>();
}

void SetLanguage(int lang) {
	if(lang != LNG_CURRENT)
		SetDefaultCharset(GetLNGCharset(lang));
	SetCurrentLanguage(lang);
}

INITBLOCK {
	SetLanguage(LNG_('E', 'N', 'U', 'S'));
}

void SetLanguage(const char *s)
{
	SetLanguage(LNGFromText(s));
}

String GetCurrentLanguageString()
{
	return LNGAsText(GetCurrentLanguage());
}

String GetLangName(int language)
{
	return GetLanguageInfo(language).english_name;
}

String GetNativeLangName(int language) {
	return GetLanguageInfo(language).native_name.ToString();
}

}
