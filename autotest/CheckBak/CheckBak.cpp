#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String s = Sys("svn info svn://www.ultimatepp.org/bak");
	DUMP(s);
	int q = s.Find("Last Changed Date: ");
	ASSERT(q >= 0);
	s = s.Mid(q);
	DUMP(s);
	ASSERT(s.GetCount() > 18);
	Date d;
	// 2014-10-30 01:01:56
	// 0123456789012345678
	d.year = atoi(s);
	d.month = atoi(~s + 5);
	d.day = atoi(~s + 8);
	DUMP(d);
	ASSERT(d >= GetSysDate() - 1);
}
