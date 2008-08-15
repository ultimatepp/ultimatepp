#include <CppBase/CppBase.h>

using namespace Upp;

#define NCOUNT      50
#define MAX_THREADS 100

String text[MAX_THREADS];

void Work(int i = 0)
{
	CppBase base;
	MemReadStream in(text[i], text[i].GetLength());
	Parse(in, Vector<String>(), base, "", Callback2<int, const String&>());
}

void ThreadWork(int ii)
{
	for(int i = 0; i < NCOUNT; i++)
		Work(ii);
}

void ChrisTest();

void sLarge(String& text, int *large, int count, const char *txt)
{
	int n = min(1024, count);
	Sort(large, large + n, StdLess<int>());
	int i = 0;
	while(i < n) {
		int q = large[i];
		int nn = i++;
		while(i < n && large[i] == q) i++;
		nn = i - nn;
		if(q < 10000)
			text << Format("%4d B, %5d %s (%6d KB)\r\n", q, nn, txt, (nn * q) >> 10);
		else
			text << Format("%4d`KB, %5d %s (%6d KB)\r\n", q >> 10, nn, txt, (nn * q) >> 10);
	}
}

String sProfile(MemoryProfile& mem)
{
	String text;
	int acount = 0;
	size_t asize = 0;
	int fcount = 0;
	size_t fsize = 0;
	for(int i = 0; i < 1024; i++)
		if(mem.allocated[i]) {
			int sz = 4 * i;
			text << Format("%4d B, %6d allocated (%5d KB), %6d fragmented (%5d KB)\n",
			              sz, mem.allocated[i], (mem.allocated[i] * sz) >> 10,
			              mem.fragmented[i], (mem.fragmented[i] * sz) >> 10);
			acount += mem.allocated[i];
			asize += mem.allocated[i] * sz;
			fcount += mem.fragmented[i];
			fsize += mem.fragmented[i] * sz;
		}
	text << Format(" TOTAL, %6d allocated (%5d KB), %6d fragmented (%5d KB)\n",
	              acount, int(asize >> 10), fcount, int(fsize >> 10));
	text << "Free pages " << mem.freepages << " (" << mem.freepages * 4 << " KB)\n";
	text << "Large block count " << mem.large_count
	    << ", total size " << (mem.large_total >> 10) << " KB\n";
	sLarge(text, mem.large_size, mem.large_count, "allocated");
	text << "Large fragments count " << mem.large_free_count
	    << ", total size " << (mem.large_free_total >> 10) << " KB\n";
	sLarge(text, mem.large_free_size, mem.large_free_count, "fragments");
	return text;
}

CONSOLE_APP_MAIN
{
//	PeakMemoryProfile();
#if 0
	ChrisTest();
#else
	StdLogSetup(LOG_FILE|LOG_COUT);
	String fn = GetDataFile("x.cpp");
	fn = AppendFileName(GetFileFolder(GetFileFolder(GetFileFolder(fn))),
	                    "uppsrc/CtrlLib/ArrayCtrl.cpp");
	String file = LoadFile(fn);
	for(int i = 0; i < MAX_THREADS; i++)
		text[i] = file;
#ifdef _MULTITHREADED
	Thread x[MAX_THREADS];
	for(int n = 64; n < MAX_THREADS; n = 2 * n) {
		TimeStop tm;
		for(int i = 0; i < n; i++)
			x[i].Run(callback1(ThreadWork, i));
		for(int i = 0; i < n; i++)
			x[i].Wait();
		RLOG(n << " thread(s) " << tm.Elapsed() << " ms");
	}
	TimeStop tm;
	{
		CoWork co;
		for(int i = 0; i < 10000; i++)
			co & callback1(Work, 0);
	}
	RLOG("CoWork (1000 runs): " << tm.Elapsed() << " ms");
#else
	TimeStop tm;
	for(int i = 0; i < 1000; i++)
		Work();
	RLOG("Single thread: " << tm.Elapsed() << " ms");
#endif
#endif
//	RLOG(sProfile(*PeakMemoryProfile()));
}
