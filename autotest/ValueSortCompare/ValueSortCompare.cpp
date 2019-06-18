#include <Core/Core.h>

using namespace Upp;

void Test(Vector<Value>& h)
{
	Sort(h);
	for(int i = 0; i < h.GetCount() - 1; i++)
		ASSERT(h[i] <= h[i + 1]);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<Value> vi, vf, vd, vt, vi64, vs;
	SeedRandom(0);
	for(int i = 0; i < 10000; i++) {
		vi.Add((int)Random());
		vf.Add(Randomf());
		vi64.Add((int64)Random64());
		vt.Add(GetSysTime() + (int64)Random64());
		vd.Add(GetSysDate() + Random());
		vs.Add(AsString(Random()));
	}

	Test(vi);
	Test(vf);
	Test(vi64);
	Test(vs);
	Test(vd);
	Test(vt);
	
	LOG("============ OK");
}
