#include <Core/Core.h>
#include <deque>

using namespace Upp;
using namespace std;

#define N 500000

int dummy;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	{
		TimeStop tm;
		Vector<int> d;
		for(int n = 0; n < N; n++) {
			d.clear();
			for(int i = 0; i < 200; i++) {
				d.Add(i);
				d.Add(i);
			}
		}
		RLOG("Vector add " << tm);
		tm.Reset();
		for(int n = 0; n < N; n++)
			for(int i = 0; i < (int)d.size(); i++)
				dummy += d[i];
		RLOG("Vector read " << tm);
	}
	{
		TimeStop tm;
		deque<int> d;
		for(int n = 0; n < N; n++) {
			d.clear();
			for(int i = 0; i < 200; i++) {
				d.push_back(i);
				d.push_front(i);
			}
		}
		RLOG("deqeue add " << tm);
		tm.Reset();
		for(int n = 0; n < N; n++)
			for(int i = 0; i < (int)d.size(); i++)
				dummy += d[i];
		RLOG("deqeue index read " << tm);
		tm.Reset();
		for(int n = 0; n < N; n++)
			for(deque<int>::iterator i = d.begin(); i != d.end(); i++)
				dummy += *i;
		RLOG("deqeue iterator read " << tm);
	}
	{
		TimeStop tm;
		BiVector<int> d;
		for(int n = 0; n < N; n++) {
			d.Clear();
			for(int i = 0; i < 200; i++) {
				d.AddTail(i);
				d.AddHead(i);
			}
		}
		RLOG("BiVector add " << tm);
		tm.Reset();
		for(int n = 0; n < N; n++)
			for(int i = 0; i < d.GetCount(); i++) {
				dummy += d[i];
			}
		RLOG("BiVector read " << tm);
	}
}
