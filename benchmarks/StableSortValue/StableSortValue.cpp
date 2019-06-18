#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<Value> vi, vf, vd, vt, vi64, vs;
	SeedRandom(0);
	for(int i = 0; i < 1000000; i++) {
		vi.Add((int)Random());
		vf.Add(Randomf());
		vi64.Add((int64)Random64());
		vt.Add(GetSysTime() + (int64)Random64());
		vd.Add(GetSysDate() + Random());
		vs.Add(AsString(Random()));
	}
	
	for(int i = 0; i < 3; i++) {
		{
			Vector<Value> v2 = clone(vi);
			RTIMING("StableSort int");
			StableSort(v2);
		}
		{
			Vector<Value> v2 = clone(vf);
			RTIMING("StableSort double");
			StableSort(v2);
		}
		{
			Vector<Value> v2 = clone(vi64);
			RTIMING("StableSort int64");
			StableSort(v2);
		}
		{
			Vector<Value> v2 = clone(vs);
			RTIMING("StableSort String");
			StableSort(v2);
		}
		{
			Vector<Value> v2 = clone(vd);
			RTIMING("StableSort Date");
			StableSort(v2);
		}
		{
			Vector<Value> v2 = clone(vt);
			RTIMING("StableSort Time");
			StableSort(v2);
		}
	}
}
