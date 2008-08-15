#include <Core/Core.h>

#include <algorithm>
#include <vector>

using namespace Upp;

#define BM_(k, x1, x, sort) { d1 <<= d; RTIMING(k " " #sort); sort(d1); }

#define BV(k, sort) BM_(k, d1, d, sort)
#define BA(k, sort) BM_(k " Array", a1, a, sort)
#define Bv(k, sort) { s1 = s; RTIMING(k " " #sort " vector<string>"); sort(s1.begin(), s1.end()); }
#define BS(k, sort) { d1 <<= d; RTIMING(k " " #sort " String"); sort(d1.Begin(), d1.End()); }

#define Bm(k)\
BV(k, Sort)\
BV(k, GetSortOrder)\
BV(k, StableSort)\
BV(k, GetStableSortOrder)\
BV(k, StableSortCmp)\
BV(k, GetStableSortOrderCmp)\
BS(k, std::sort)\
BS(k, std::stable_sort)\
Bv(k, std::sort)\
Bv(k, std::stable_sort)\
BA(k, Sort)\
BA(k, StableSort)\
BA(k, StableSortCmp)\

CONSOLE_APP_MAIN
{
	for(int q = 0; q < 100; q++) {
		Vector<String> d, d1;
		Array<String> a, a1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 10000; i++) {
			String x = AsString(rand());
			d.Add(x);
			a.Add(x);
			s.push_back(to_string(x));
		}
		Bm("S");
	}

	for(int q = 0; q < 100; q++) {
		Vector<String> d, d1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 10000; i++) {
			String x = AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand());
			d.Add(x);
			s.push_back(to_string(x));
		}
		Bm("M");
	}
	
	for(int q = 0; q < 100; q++) {
		Vector<String> d, d1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 10000; i++) {
			String x = AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand())
			         + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand())
			         + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand())
			         + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand());
			d.Add(x);
			s.push_back(to_string(x));
		}
		Bm("L");
	}
}
