#include <Core/Core.h>

#include <algorithm>
#include <vector>

using namespace Upp;

template <class I, class T>
void Sort__(I begin, I end, const T&)
{
	Sort(begin, end, StdLess<T>());
}

template <class I>
void Sort(I begin, I end)
{
	Sort__(begin, end, *begin);
}

#define BM_(k, x1, x, sort) { d1 <<= d; RTIMING(k); sort(d1); }

#define BV(k, sort) BM_(k " "  #sort " Vector<String>", d1, d, sort)
#define BA(k, sort) BM_(k " "  #sort " Array<String>", a1, a, sort)
#define Bv(k, sort) { s1 = s; RTIMING(k " " #sort " vector<string>"); sort(s1.begin(), s1.end()); }
#define BS(k, sort) { d1 <<= d; RTIMING(k " " #sort " Vector<String>"); sort(d1.Begin(), d1.End()); }

#define NN 10

#define Bm(k)\
for(int i = 0; i < NN; i++) { \
	Bv(k, std::sort)\
	Bv(k, Sort)\
	BS(k, std::sort)\
	BV(k, Sort)\
}

/*
BV(k, GetStableSortOrderCmp)\
BV(k, StableSortCmp)\
BV(k, Sort)\
BV(k, GetSortOrder)\
BV(k, StableSort)\
BV(k, GetStableSortOrder)\
BA(k, Sort)\
BA(k, StableSort)\
BA(k, StableSortCmp)\
Bv(k, std::sort)\
Bv(k, std::stable_sort)\
Bv(k, Sort)\
BS(k, std::stable_sort)\
*/

#define N 20

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	for(int q = 1; q < N; q++) {
		Vector<String> d, d1;
		Array<String> a, a1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 2000 * q; i++) {
			String x = AsString(rand());
			d.Add(x);
			a.Add(x);
			s.push_back(to_string(x));
		}
		RLOG("S" << q);
		Bm("S");
	}

	for(int q = 1; q < N; q++) {
		Vector<String> d, d1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 2000 * q; i++) {
			String x = AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand());
			d.Add(x);
			s.push_back(to_string(x));
		}
		RLOG("M" << q);
		Bm("M");
	}
	
	for(int q = 1; q < N; q++) {
		Vector<String> d, d1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 2000 * q; i++) {
			String x = AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand())
			         + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand())
			         + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand())
			         + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand());
			d.Add(x);
			s.push_back(to_string(x));
		}
		RLOG("L" << q);
		Bm("L");
	}
}
