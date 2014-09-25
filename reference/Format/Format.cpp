#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SetLanguage(LNG_ENGLISH);

	DUMP(Format("%d, %s", 123, "TEXT"));
	DUMP(Format("%2:s, %1:d", 123, "TEXT"));
	DUMP(Format("%010d", 123));
	DUMP(Format("%0*d", 11, 123));
	DUMP(Format("|%20<d|", 123));
	DUMP(Format("|%20>d|", 123));
	DUMP(Format("|%20=d|", 123));
	DUMP(Format("%d`pt", 123));
	DUMP(Format("%[empty]~d, %[empty]~d", 123, Null));

	DUMP(Format("%`", 123));

	DUMP(Format("%c", 65));
	DUMP(Format("%d", 123));
	DUMP(Format("%i", 123));
	DUMP(Format("%o", 123));
	DUMP(Format("%x", 123));
	DUMP(Format("%X", 123));

	DUMP(Format("%e", 1234567.89));
	DUMP(Format("%E", 1234567.89));
	DUMP(Format("%f", 1234567.89));
	DUMP(Format("%g", 1234567.89));
	DUMP(Format("%G", 1234567.89));

	DUMP(Format("%n", 1234567.89));
	DUMP(Format("%,n", 1234567.89));
	DUMP(Format("%ne", 1234567.89));
	DUMP(Format("%nf", 1234567.89e30));
	DUMP(Format("%nl", 1234567.89));
	DUMP(Format("%@nl", 1234567.89));
	DUMP(Format("%@,nl", 1234567.89));
	DUMP(Format("%v", 1234567.89));
	DUMP(Format("%ve", 1234567.89));
	DUMP(Format("%vf", 1234567.89e30));
	DUMP(Format("%vl", 1234567.89));

	DUMP(Format("%[1:one;2:two;3:three;another]s", 2));
	DUMP(Format("%[1:one;2:two;3:three;another]s", 20));
	DUMP(Format("%[3%1:one;2:two;3:three;another]s", 20));

	DUMP(Format("%month", 6));
	DUMP(Format("%Month", 6));
	DUMP(Format("%MONTH", 6));
	DUMP(Format("%mon", 6));
	DUMP(Format("%Mon", 6));
	DUMP(Format("%MON", 6));
	DUMP(Format("%day", 6));
	DUMP(Format("%Day", 6));
	DUMP(Format("%DAY", 6));
	DUMP(Format("%dy", 6));
	DUMP(Format("%Dy", 6));
	DUMP(Format("%DY", 6));
	DUMP(Format("%tw", 0));
	DUMP(Format("%tw", 5));
	DUMP(Format("%tw", 15));
	DUMP(Format("%0tw", 15));

	DUMP(Format("%a", 1));
	DUMP(Format("%a", 123));
	DUMP(Format("%A", 1));
	DUMP(Format("%A", 123));
	DUMP(Format("%r", 8));
	DUMP(Format("%R", 1231));

	DUMP(Format("%`", GetSysDate()));
	DUMP(Format("%`", GetSysTime()));
	DUMP(Format("%`", "text"));

	SetLanguage("cs-cz");
	DUMP(Format("%`", GetSysDate()));
}
