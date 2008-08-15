#include <Core/Core.h>

using namespace Upp;

namespace Upp {
extern word CHRTAB_CP850[];
};

CONSOLE_APP_MAIN
{
	String ss = "Spag. Süß";
	String s = "ß";
	for(int i = 0; i < 128; i++) {
		DUMP(i + 128);
		LOG(CHRTAB_CP850[i]);
	}
	DUMP(FormatIntHex(s.ToWString()[0]));
	DUMP(ToCharset(CHARSET_CP850, s, CHARSET_UTF8));
	return;
	DUMP(ToCharset(CHARSET_WIN1250, s, CHARSET_UTF8));
	DUMP(ToCharset(CHARSET_WIN1252, s, CHARSET_UTF8));
}

