#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SetDefaultCharset(CHARSET_UTF8);

	WString x = "characters 280-300: ";
	for(int i = 280; i < 300; i++)
		x.Cat(i);
	DUMP(x);

	String y = x.ToString();
	DUMP(y);
	y.Cat(" (appended)");
	x = y.ToWString();

	DUMP(x);
}
