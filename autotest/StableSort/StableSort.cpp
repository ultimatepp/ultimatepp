#include <Core/Core.h>

#include <algorithm>
#include <vector>

using namespace Upp;

#define LLOG(x)

struct Item : Comparable<Item> {
	String v;
	int    i;

	int Compare(const Item& b) const { return SgnCompare(v, b.v); }
};

void Benchmark();

void Check(int N, int M)
{
	LOG("LESS");
	int i;
	Vector<String> k;
	Vector<int> v;
	StableSort(k);
	LOG("Testing " << N << ' ' << M);
	for(i = 0; i < N; i++) {
		k.Add(AsString(rand() % M));
		v.Add(i);
	}
	LOG(" GetStableSortOrder...");
	Vector<int> o = GetStableSortOrder(k);
	i = 0;
	while(i < o.GetCount()) {
		LLOG(k[o[i]].v);
		if(i > 0)
			ASSERT(k[o[i]] >= k[o[i - 1]]);
		int j = i;
		while(++j < o.GetCount() && k[o[j]] == k[o[i]])
			ASSERT(o[j] > o[j - 1]);
		i = j;
	}
	LOG(" StableIndexSort...");
	StableIndexSort(k, v);
	LOG(" Testing...");
	for(i = 0; i < k.GetCount(); i++)
		LLOG(k[i] << ' ' << v[i]);
	LLOG("----------");
	i = 0;
	while(i < k.GetCount()) {
		LLOG(k[i] << ' ' << v[i]);
		if(i > 0)
			ASSERT(k[i] >= k[i - 1]);
		int j = i;
		while(++j < k.GetCount() && k[j] == k[i])
			ASSERT(v[j] > v[j - 1]);
		i = j;
	}
}

CONSOLE_APP_MAIN
{
	TimeStop tm;
	StdLogSetup(LOG_COUT|LOG_FILE);

	Check(1000, 100);
	Check(1000000, 100);
	Check(1000000, 10000);
	Check(1000000, 1);
	for(int i = 1; i < 30000; i += i)
		Check(2000000, i);
	
	LOG("=========== OK " << tm << " s");
}

void CompileCheck()
{
	Vector<String> x;
	IndexSort(x, x);
	IndexSort2(x, x, x);
	IndexSort3(x, x, x, x);
	StableIndexSort(x, x);
	StableIndexSort2(x, x, x);
	StableIndexSort3(x, x, x, x);
}
