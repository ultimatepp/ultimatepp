#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String s = Sys("svn info svn://www.ultimatepp.org/bak");
	LOG("SVN info:");
	LOG(s);
	String key = "Last Changed Date: ";
	int q = s.Find(key);
	ASSERT(q >= 0);
	s = s.Mid(q + key.GetCount());
	ASSERT(s.GetCount() > 18);
	Date d;
	// 2014-10-30 01:01:56
	// 0123456789012345678
	d.year = atoi(s);
	d.month = atoi(~s + 5);
	d.day = atoi(~s + 8);
	LOG("Bak repository last date: " << d);
	ASSERT(d >= GetSysDate() - 1);
	
	FindFile ff("/mnt/nas/bak/*.*");
	d = Date::Low();
	
	while(ff) {
		d = max(d, (Date)Time(ff.GetLastWriteTime()));
		ff.Next();
	}
	
	LOG("mnt/nas/bak last date: " << d);

	ASSERT(d >= GetSysDate() - 1);
}
