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
	
//	for(int i = 0; i < 10000; i++)
	{
		{
			double sum = 0;
			{
				RTIMING("Single thread");
				for(const String& s : data)
					sum += Sum(s);
			}
//			RDUMP(sum);
		}
		if(0) {
			double sum = 0;
			{
				RTIMING("CoWork");
				CoWork co;
				for(const String& s : data)
					co & [=, &sum] {
						double m = Sum(s);
						CoWork::FinLock();
						sum += m;
					};
			}
//			RDUMP(sum);
		}
		{
			double sum = 0;
			{
				RTIMING("CoPartition");
				CoWork co;
				CoPartition(0, data.GetCount(),
				            [&](int l, int h) {
					double m = 0;
				    for(int j = l; j < h; j++)
				        m += Sum(data[j]);
					CoWork::FinLock();
					sum += m;
				});
			}
//			RDUMP(sum);
		}
		{
			double sum = 0;
	
			{
				RTIMING("CoIndex");
	
				CoWork co;
				co * [&] {
					double m = 0;
					int i;
					while((i = co.Next()) < data.GetCount())
						m += Sum(data[i]);
					CoWork::FinLock();
					sum += m;
				};
			}
//			RDUMP(sum);
		}
	}
}
