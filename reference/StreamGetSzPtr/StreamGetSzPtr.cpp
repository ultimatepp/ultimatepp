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
			const byte *e4 = p + (sz & ~3);
			while(p < e4) {
				n += (p[0] == '\n' || p[1] == '\n' || p[2] == '\n' || p[3] == '\n');
				p += 4;
			}
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
	return n;
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
}
