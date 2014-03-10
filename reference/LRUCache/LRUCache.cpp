#include <Core/Core.h>

using namespace Upp;

struct MakeFactorial : LRUCache<double, int>::Maker {
	int n;

	virtual int    Key() const {
		return n;
	}
	virtual int    Make(double& object) const {
		LOG("Computing factorial of " << n); // only to show that values are cached
		object = 1;
		for(int i = 1; i <= n; i++)
			object *= i;
		return 1; // 'Size' of created object
	}
};

double GetFactorialCached(int num)
{
#ifdef MULTITHREADED  // need to serialize access to cache
	static StaticMutex cm;
	Mutex::Lock __(cm);
#endif
	static LRUCache<double, int> cache;
	MakeFactorial f;
	f.n = num;
	double result = cache.Get(f);
	cache.Shrink(20); // cache only 20 values
	return result;
}

CONSOLE_APP_MAIN
{
	DUMP(GetFactorialCached(20));
	DUMP(GetFactorialCached(30));
	DUMP(GetFactorialCached(20)); // this one should be retrieved from the cache
}
