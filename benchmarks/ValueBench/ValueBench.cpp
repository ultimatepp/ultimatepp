#include <Core/Core.h>

using namespace Upp;

#ifdef _DEBUG
#define N 1
#else
#define N 300000
#endif

Value h;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	Vector<int> x;
	Vector<String> xs;
	for(int i = 0; i < 100; i++) {
		x.Add(Random(1000));
		xs.Add(AsString(1000));
	}
	Value xxx = Null;
	h = xxx;
	RTIMING("Total");
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
			RTIMING("Read int with To");
			for(int i = 0; i < va.GetCount(); i++)
				sum += va[i].To<int>();
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
		{
			RTIMING("Read String with To");
			for(int i = 0; i < va.GetCount(); i++)
				sum += va[i].To<String>().GetLength();
		}
		{
			RTIMING("Read String with Get");
			for(int i = 0; i < va.GetCount(); i++)
				sum += va[i].Get<String>().GetLength();
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
			RTIMING("Add Vector<Value> String");
			for(int i = 0; i < 100; i++)
				vv.Add(xs[i]);
		}
		{
			RTIMING("Read Vector<Value> String");
			for(int i = 0; i < vv.GetCount(); i++)
				sum += String(vv[i]).GetLength();
		}
		{
			RTIMING("Read Vector<Value> String with To");
			for(int i = 0; i < vv.GetCount(); i++)
				sum += vv[i].To<String>().GetLength();
		}
	}
	Cout() << sum << "\n";
}
