#include <Core/Core.h>

using namespace Upp;

Vector<String> TestData()
{
	SeedRandom(0);
	Vector<String> data;
	for(int i = 1; i < 100; i++) {
		int n = 10 * (i + Random(i + 1));
		String h;
		for(int i = 0; i < n; i++)
			h << Random(99999) << ' ';
		data.Add(h);
	}
	return data;
}

int64 SumLine(const char *s)
{
	int64 sum = 0;
	int64 n = 0;
	while(*s) {
		if(IsDigit(*s))
			n = 10 * n + *s - '0';
		else {
			sum += n;
			n = 0;
		}
		s++;
	}
	return sum + n;
}

#ifdef _DEBUG
#define N 1
#else
#define N 10000
#endif

int64 gsum;

CONSOLE_APP_MAIN
{
	Vector<String> data = TestData();
	
	for(int i = 0; i < N; i++) {
		RTIMING("ThreadLoop *");
		CoWork co;
		int64 sum = 0;
		std::atomic<int> ii = 0;
		co.Loop([&] {
			int64 h = 0;
			for(int i = ii++; i < data.GetCount(); i = ii++)
				h += SumLine(data[i]);
			CoWork::FinLock();
			sum += h;
		});
		gsum = sum;
		DUMP(gsum);
	}
}
