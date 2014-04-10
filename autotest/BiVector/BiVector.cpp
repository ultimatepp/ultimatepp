#include <Core/Core.h>

using namespace Upp;

#ifdef flagLONG
#define N 100000000
#else
#define N 3000000
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TimeStop tm;

	Vector<int> x;
	BiVector<int> y;
	for(int i = 0; i < N; i++) {
		if(i % 10000 == 0)
			Cout() << i << ' ' << x.GetCount() << '\n';
		switch(rand() % 6) {
		case 0:
		case 1:
			x.Add(i);
			y.AddTail(i);
			break;
		case 2:
		case 3:
			x.Insert(0, i);
			y.AddHead(i);
			break;
		case 4:
			if(x.GetCount()) {
				x.Drop();
				y.DropTail();
			}
			break;
		case 5:
			if(x.GetCount()) {
				x.Remove(0, 1);
				y.DropHead();
			}
			break;
		}
		ASSERT(x.GetCount() == y.GetCount());
		for(int i = 0; i < x.GetCount(); i++)
			ASSERT(x[i] == y[i]);
		if(x.GetCount() > 10000) {
			x.Clear();
			y.Clear();
		}
	}

	LOG("========== OK " << tm);
}
