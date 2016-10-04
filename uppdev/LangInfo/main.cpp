#include <Core/Core.h>

#include <winnls.h>

using namespace Upp;

/*
Vector<int> lang;

static BOOL CALLBACK sEnumLocale(char *locale_string)
{
	LCID lcid = stou(locale_string, NULL, 16);
	char buffer1[10], buffer2[10];
	::GetLocaleInfo(lcid, LOCALE_SISO639LANGNAME, buffer1, 10);
	::GetLocaleInfo(lcid, LOCALE_SISO3166CTRYNAME, buffer2, 10);
	int language = LNG_(buffer1[0], buffer1[1], buffer2[0], buffer2[1]);
	String english_name = GetLocaleInfoA(lcid, LOCALE_SENGLANGUAGE);
	String native_name = GetLocaleInfoW(lcid, LOCALE_SNATIVELANGNAME).ToString();
	String s;
	lang.Add(LNG_(buffer1[0], buffer1[1], buffer2[0], buffer2[1]));
	s << english_name << '\t' << native_name << '\t';
	s.Cat(HIBYTE(lcid));
	s.Cat(LOBYTE(lcid));
	LOG(AsCString(s, INT_MAX, NULL, ASCSTRING_OCTALHI) << ",");
//	DDUMP(FormatIntHex(lcid));
	return TRUE;
}

extern int  LanguageList[];
extern const char *LanguageInfoList[];
*/

CONSOLE_APP_MAIN
{
//	EnumSystemLocales(&sEnumLocale, LCID_SUPPORTED);
/*	for(int i = 0; i < lang.GetCount(); i++) {
		String s = LNGAsText(lang[i]);
		LOG(Format("LNG_('%c','%c','%c','%c'),", s[0], s[1], s[3], s[4]));
	}
*/
	LanguageInfo lf = GetLanguageInfo(LNG_ENGLISH);
	lf.day[0] = "Bloody monday";
	SetLanguageInfo(LNG_ENGLISH, lf);

	const int *lng = GetAllLanguages();
	int q = 0;
	while(*lng) {
		const LanguageInfo& lf = GetLanguageInfo(*lng++);
		LOG(lf.native_name << ' ' << lf.day[0]);
		DUMP(lf.Compare("Hovno", "Chleba"));
	}
}
