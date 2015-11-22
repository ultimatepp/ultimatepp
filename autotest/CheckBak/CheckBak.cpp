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
	
	for(int pass = 0; pass < 3; pass++) {
		String dir = decode(pass, 0, "/mnt/nas1/bak", 1, "/mnt/nas/bak", "/bak");
		FindFile ff(dir + "/*");
		d = Date::Low();
		int64 len = 0;
		String fn;
		
		while(ff) {
			if(ff.IsFile()) {
				Date d1 = (Date)Time(ff.GetLastWriteTime());
				if(d1 > d) {
					d = d1;
					len = ff.GetLength();
					fn = ff.GetName();
				}
			}
			ff.Next();
		}
		
		len = len >> 20;
		LOG(dir << ": " << fn << ", last date: " << d << ", size " << len << " MB");
	
		ASSERT(d >= GetSysDate() - 1);
		ASSERT(len > 500 && len < 1500);
	}

	
	s = HttpRequest("http://www.ultimatepp.org/df.info").Execute();
	LOG(s);
	q = s.Find("%");
	ASSERT(q >= 0);
	q = s.Find("%", q + 1);
	ASSERT(q >= 4);
	int n = atoi(~s + q - 3);
	LOG("Filesystem uses " << n << "%");
	ASSERT(n > 10 && n < 85);
}
