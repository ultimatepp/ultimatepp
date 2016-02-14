#include <Core/Core.h>

using namespace Upp;

const int N = 1000000;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	for(int n = -1; n <= 10; n++) {
		Vector<String> h;
		for(int i = 0; i < N; i++)
			if(n < 0)
				h.Add(AsString(N - i));
			else
			if(n)
				h.Add(AsString(Random(n)));
			else
				h.Add(AsString(Random()));
		TimeStop tm;
		Sort(h);
		double t = tm.Seconds();
		RLOG(n << " " << t << " s");
	}
	for(int n = 2; n <= 10; n++) {
		Vector<String> h;
		int r;
		for(int i = 0; i < N; i++) {
			if(i % n == 0)
				r = Random();
			h.Add(AsString(r));
		}
		TimeStop tm;
		Sort(h);
		double t = tm.Seconds();
		RLOG("A " << n << " " << t << " s");
	}
	for(int n = 2; n <= 10; n++) {
		Vector<String> h;
		int r = Random();
		int n0 = 0;
		for(int i = 0; i < N; i++) {
			if(i / n != n0) {
				r = Random();
				n0 = i / n;
			}
			h.Add(AsString(r));
		}
		TimeStop tm;
		Sort(h);
		double t = tm.Seconds();
		RLOG("B " << n << " " << t << " s");
	}
}
