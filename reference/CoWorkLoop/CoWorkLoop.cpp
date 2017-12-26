#include <Core/Core.h>

using namespace Upp;

force_inline double Sum(const String& s)
{
	CParser p(s);
	double sum = 0;
	while(!p.IsEof())
		sum += p.ReadDouble();
	return sum;
}

CONSOLE_APP_MAIN
{
	SeedRandom(0);
	Vector<String> data;
	for(int i = 0; i < 10000000; i++) {
		int n = Random(7);
		data.Add();
		for(int j = 0; j < n; j++)
			data.Top() << Random() << ' ';
	}
	
	double sum = 0;
	CoWork co;
	co * [&] {
		double m = 0;
		int i;
		while((i = co.Next()) < data.GetCount())
			m += Sum(data[i]);
		CoWork::FinLock();
		sum += m;
	};

	RDUMP(sum);
}
