#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	String s = HttpRequest("http://www.ultimatepp.org/df.info").Execute();
	LOG(s);
	int q = s.Find("%");
	ASSERT(q >= 0);
	q = s.Find("%", q + 1);
	ASSERT(q >= 4);
	int n = atoi(~s + q - 3);
	LOG("Filesystem uses " << n << "%");
	ASSERT(n > 10 && n < 80);

	FindFile ff("/bak/*");
	Time tm = Time::Low();
	int64 len = 0;
	String fn;
	
	while(ff) {
		if(ff.IsFile()) {
			Time tm1 = Time(ff.GetLastWriteTime());
			if(tm1 > tm) {
				tm = tm1;
				len = ff.GetLength();
				fn = ff.GetPath();
			}
		}
		ff.Next();
	}

	LOG(fn << ", last time: " << tm << ", size " << (len >> 20) << " MB");

	String h = Sys("tar -tvf " + fn);
	q = h.Find("root/website.bak/root/bak/forum.fud");
	ASSERT(q >= 17);
	s = h.Mid(q - 17, 17);
	Date d;
	// 2014-10-30 01:01:56
	// 0123456789012345678
	d.year = atoi(s);
	d.month = atoi(~s + 5);
	d.day = atoi(~s + 8);
	LOG("forum.fud last date: " << d);
	ASSERT(d >= GetSysDate() - 1);
	ASSERT(tm >= GetSysDate() - 1);
	len = len >> 20;
	LOG("backup len (MB): " << len);
	ASSERT(len > 1500 && len < 5800);

	for(auto dir : Split("/big/bak", ';')) { // /net/nas1/bak;/net/nas/bak;
		FileIn in1(fn);
		
		String p = dir + '/' + GetFileName(fn);
		LOG(p);
		FileIn in2(p);
		
		for(;;) {
			if(in1.IsEof()) {
				ASSERT(in2.IsEof());
				break;
			}
			ASSERT(!in2.IsEof());
			ASSERT(in1.Get(1 << 20) == in2.Get(1 << 20));
		}
	}
	
	LOG("------------------- OK");
}
