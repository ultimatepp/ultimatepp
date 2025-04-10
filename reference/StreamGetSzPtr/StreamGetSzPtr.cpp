#include <Core/Core.h>

using namespace Upp;

int CountLines(Stream& s)
{
	int n = 0;
	int c;
	while((c = s.Get()) >= 0)
		n += c == '\n';
	return n;
}

int CountLinesOptimized(Stream& s)
{
	int n = 0;
	for(;;) {
		int sz;
		const byte *p = s.GetSzPtr(sz);
		if(sz) {
			const byte *e = p + sz;
			while(p < e)
				n += *p++ == '\n';
		}
		else {
			int c = s.Get();
			if(c < 0)
				return n;
			n += c == '\n';
		}
	}
}

int CountLinesOptimizedSIMD(Stream& s)
{
	int n = 0;

	for(;;) {
		int sz;
		const byte* p = s.GetSzPtr(sz);

		if(sz)
			n += memcnt8(p, '\n', sz); // this is using SIMD..
		else {
			int c = s.Get();
			if(c < 0)
				return n;
			n += (c == '\n');
		}
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	const Vector<String>& arg = CommandLine();
	if(arg.GetCount() == 0) {
		RLOG("Please provide testing file path as argument, preferably >500MB");
		return;
	}
	
	{
		RTIMESTOP("Simple");
		FileIn in(arg[0]);
		RDUMP(CountLines(in));
	}

	{
		RTIMESTOP("Optimized");
		FileIn in(arg[0]);
		RDUMP(CountLinesOptimized(in));
	}

	{
		RTIMESTOP("Optimized SIMD (using memcnt8)");
		FileIn in(arg[0]);
		RDUMP(CountLinesOptimizedSIMD(in));
	}
}
