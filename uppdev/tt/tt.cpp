#include <Core/Core.h>

using namespace Upp;

struct tt_char {
	const char *s;

	String ToString() const       { return GetLngString(s); }
	operator const char *() const { return ToString(); }
};

CONSOLE_APP_MAIN
{
	static tt_char x[] = {
		tt_("Aborted by user."),
		tt_("Two"),
		tt_("Three")
	};
	
	SetLanguage(LNG_('I','T','I','T'));
	DDUMP(x[0]);
}

