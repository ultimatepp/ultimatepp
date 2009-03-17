#include <Core/Core.h>

using namespace Upp;

struct DblSum {
	Vector<double> d;
	mutable double sum;
	LazyUpdate dirty;

public:
	void Add(double x) {
		d.Add(x);
		dirty.Invalidate();
	}

	double Sum() const {
		if(dirty.BeginUpdate()) {
			Vector<int> o = GetSortOrder(d);
			sum = 0;
			for(int i = 0; i < o.GetCount(); i++)
				sum += d[o[i]];
			dirty.EndUpdate();
		}
		return sum;
	}
};

CONSOLE_APP_MAIN
{
	DblSum x;
	x.Add(100);
	for(int i = 0; i < 1000*1000; i++)
		x.Add(1e-10);
	Cout() << x.Sum() << "\n";
}
