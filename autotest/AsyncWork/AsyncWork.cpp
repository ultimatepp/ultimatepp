#include <Core/Core.h>

using namespace Upp;

template <class Range>
ValueTypeOf<Range> ASum(const Range& rng)
{
	int n = rng.GetCount();
	if(n == 1)
		return rng[0];
	if(n == 0)
		return 0;
	auto l = Async([&] { return ASum(SubRange(rng, 0, n / 2)); });
	auto r = Async([&] { return ASum(SubRange(rng, n / 2, n - n / 2)); });
	return l.Get() + r.Get();
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<int> h;
	for(int i = 0; i < 1000000; i++)
		h.Add(Random());
	
	DDUMP(CoSum(h));
	DDUMP(Sum(h));
	DDUMP(ASum(h));
	ASSERT(ASum(h) == Sum(h));
	ASSERT(ASum(h) == CoSum(h));
	
	LOG("------------- OK");
}
