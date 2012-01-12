#include <Core/Core.h>

using namespace Upp;

#ifdef _DEBUG
#define N 1
#else
#define N 100000
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	Vector<int> x;
	Vector<String> xs;
	for(int i = 0; i < 100; i++) {
		x.Add(Random(1000));
		xs.Add(AsString(1000));
	}
	int sum;
	for(int i = 0; i < N; i++) {
		ValueArray va;
		{
			va.Clear();
			RTIMING("Add int");
			for(int i = 0; i < 100; i++)
				va.Add(x[i]);
		}
		{
			RTIMING("Read int");
			for(int i = 0; i < va.GetCount(); i++)
				sum += (int)va[i];
		}
		{
			va.Clear();
			RTIMING("Add String");
			for(int i = 0; i < 100; i++)
				va.Add(xs[i]);
		}
		{
			RTIMING("Read String");
			for(int i = 0; i < va.GetCount(); i++)
				sum += String(va[i]).GetLength();
		}
		Vector<String> vs;
		{
			vs.Clear();
			RTIMING("Add Vector<String>");
			for(int i = 0; i < 100; i++)
				vs.Add(xs[i]);
		}
		{
			RTIMING("Read Vector<String>");
			for(int i = 0; i < vs.GetCount(); i++)
				sum += vs[i].GetLength();
		}
		Vector<Value> vv;
		{
			vv.Clear();
			RTIMING("Add Vector<Value>");
			for(int i = 0; i < 100; i++)
				vv.Add(xs[i]);
		}
		{
			RTIMING("Read Vector<Value>");
			for(int i = 0; i < vv.GetCount(); i++)
				sum += String(vv[i]).GetLength();
		}
	}
	Cout() << sum << "\n";
}
