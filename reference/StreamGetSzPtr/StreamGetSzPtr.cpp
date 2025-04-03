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
			__BREAK__;
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

#ifdef CPU_SIMD
int CountLinesOptimizedSIMD(Stream& s)
{
	int n = 0;

	for(;;) {
		int sz;
		const byte* p = s.GetSzPtr(sz);

		if (sz) {
			const byte* e = p + sz;
			const byte* e16 = p + (sz & ~15);  // Process in 16-byte chunks
			i8x16 q = i8all('\n');
			while(p < e16) {
				n += CountTrue(i8x16(p) == q);
				p += 16;
			}
			
			while (p < e) { // Process remaining bytes (less than 16)
				n += (*p++ == '\n');
			}
		}
		else {
			int c = s.Get();
			if(c < 0)
				return n;
			n += (c == '\n');
		}
	}
}
#endif

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

#ifdef CPU_SIMD
	{
		RTIMESTOP("Optimized SIMD");
		FileIn in(arg[0]);
		RDUMP(CountLinesOptimizedSIMD(in));
	}
#endif
}
