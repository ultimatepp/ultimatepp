#include <Core/Core.h>

using namespace Upp;

dword qqq;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String s;
	for(int i = 0; i < 1000000; i++)
		s.Cat(i);

	for(int i = 0; i < 1000; i++) {
		StringStream ss(s);
		RTIMING("Time");
		int q = 0;
		while(!ss.IsEof())
			q += ss.Get32();
		qqq += q;
	}
}
