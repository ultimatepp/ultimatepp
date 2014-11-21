#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String s = Sys("svn info svn://www.ultimatepp.org/bak");
	DDUMP(s);
	int q = s.Find("Last Changed Date: ");
	ASSERT(q >= 0);
	ScanWwwTime()
	Last Changed Date: 2014-10-30 01:01:56 +0100 (Ŕt, 30 10 2014)
}

		SQL * Update(SETTINGS)
				(VALUE,
				 Format("%04d-%02d-%02d %02d:%02d:%02d",
				        last.year, last.month, last.day, last.hour, last.minute, last.second))
		      .Where(ID == "LastKeboola");
update settings set value = '2014-11-19 00:00:00' where ID = 'LastKeboola'