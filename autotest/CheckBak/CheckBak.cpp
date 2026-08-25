#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

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
	
	DDUMP(h);

	int q = h.Find("root/forums.bak/forum_dump.sql");
	ASSERT(q >= 17);
	String s = h.Mid(q - 17, 17);
	DUMP(s);
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
	ASSERT(len > 600);

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
