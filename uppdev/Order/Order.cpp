#include "Order.h"
#include <set>

#define LTIMING(x) // RTIMING(x)

template <class T>
void Test(T *test, int elements) {
	int repeat = 10000000 / elements;
	RLOG("------ " << elements << " elements");
	{
		TimeStop tm;
		for(int q = 0; q < repeat; q++) {
			Order<T> set;
			for(int i = 0; i < elements; i++)
				set.FindAdd(test[i]);
		}
		RLOG("Order FindAdd: " << tm.Elapsed() << "ms");
		Order<T> set;
		for(int i = 0; i < elements; i++)
			set.FindAdd(test[i]);
		TimeStop tm2;
		int xx;
		for(int q = 0; q < repeat; q++) {
			xx = 0;
			for(int i = 0; i < elements; i++)
				if(set.Find(test[i]) >= 0) xx++;
		}
		RLOG("Order Find: " << tm2.Elapsed() << "ms, matches: " << xx);
	}

	{
		TimeStop tm;
		for(int q = 0; q < repeat; q++) {
			std::set<T> sset;
			for(int i = 0; i < elements; i++)
				sset.insert(test[i]);
		}
		RLOG("tree  insert: " << tm.Elapsed() << "ms");
		std::set<T> sset;
		for(int i = 0; i < elements; i++)
			sset.insert(test[i]);
		TimeStop tm2;
		int xx;
		for(int q = 0; q < repeat; q++) {
			xx = 0;
			for(int i = 0; i < elements; i++)
				if(sset.find(test[i]) != sset.end()) xx++;
		}
		RLOG("tree  find: " << tm2.Elapsed() << "ms, matches: " << xx);
	}
/*
	{
		TimeStop tm;
		for(int q = 0; q < repeat; q++) {
			Index<T> sset;
			for(int i = 0; i < elements; i++)
				sset.FindAdd(test[i]);
		}
		RDUMP(tm.Elapsed());
	}*/
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	static int test[100000];
	RLOG("=========== int");
	for(int q = 10; q < 50000; q = 4 * q) {
		for(int i = 0; i < q; i++)
			test[i] = i ? (rand() + (rand() << 15)) : 0;
		Test(test, i);
	}

	RLOG("=========== String");
	static String stst[100000];
	for(int q = 10; q < 50000; q = 2 * q) {
		for(int i = 0; i < q; i++)
			stst[i] = AsString(i ? (rand() + (rand() << 15)) : 0);
		Test(stst, i);
	}
}
