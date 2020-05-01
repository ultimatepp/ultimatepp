#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SeedRandom(0);
	Vector<String> data;
	for(int i = 0; i < 1000; i++) {
		int n = Random(7);
		data.Add();
		for(int j = 0; j < n; j++)
			data.Top() << Random() << ' ';
	}
	
	double sum = 0;
	std::atomic<int> ii(0);
	CoDo([&] {
		double m = 0;
		for(int i = ii++; i < data.GetCount(); i = ii++) {
			CParser p(data[i]);
			while(!p.IsEof())
				m += p.ReadDouble();
		}
		CoWork::FinLock();
		sum += m;
	});

	RDUMP(sum);
}
