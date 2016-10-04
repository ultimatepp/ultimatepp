#include <Core/Core.h>

#include <algorithm>
#include <vector>

using namespace Upp;

#include "StableSort.h"
#include "QStableSort.h"

struct Item : Comparable<Item> {
	String v;
	int    i;

	int Compare(const Item& b) const { return SgnCompare(v, b.v); }
};

void Benchmark();

CONSOLE_APP_MAIN
{
	Benchmark(); return;
	int i;
/*
	Vector<int> x;
	for(i = 0; i < 100; i++)
		x.Add(rand() % 200);
	QStableSort(x);
	DUMPC(x);
	return;
*/
	Array<Item> a;
	QStableSort(a);
	for(i = 0; i < 100000; i++) {
		Item& m = a.Add();
		m.v = AsString(rand() % 10000);
		m.i = i;
	}
	Cout() << "Sorting...\r\n";
	QStableSortCmp(a);
	Cout() << "Testing...\r\n";
	for(i = 0; i < a.GetCount(); i++)
		LOG(a[i].v << ' ' << a[i].i);
	LOG("----------");
	i = 0;
	while(i < a.GetCount()) {
		LOG(a[i].v << ' ' << a[i].i);
		if(i > 0)
			ASSERT(a[i].v >= a[i - 1].v);
		int j = i;
		while(++j < a.GetCount() && a[j].v == a[i].v)
			ASSERT(a[j].i > a[j - 1].i);
		i = j;
	}
}

void Benchmark()
{
	for(int q = 0; q < 100; q++) {
		Vector<String> d, d1;
		Array<String> a, a1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 10000; i++) {
			String x = AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand()) + AsString(rand());
			d.Add(x);
			s.push_back(to_string(x));
			a.Add(x);
		}
		d1 <<= d;
		{
			RTIMING("M Sort");
			Sort(d1);
		}
		d1 <<= d;
		{
			RTIMING("M GetSortOrder");
			GetSortOrder(d1);
		}
		d1 <<= d;
		{
			RTIMING("M StableSort");
			StableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("M std::sort String");
			std::sort(d1.Begin(), d1.End());
		}
		d1 <<= d;
		{
			RTIMING("M std::stable_sort String");
			std::stable_sort(d1.Begin(), d1.End());
		}
		s1 = s;
		{
			RTIMING("M std::sort std::string");
			std::sort(s1.begin(), s1.end());
		}
		s1 = s;
		{
			RTIMING("M std::stable_sort std::string");
			std::stable_sort(s1.begin(), s1.end());
		}
	}

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
		d1 <<= d;
		{
			RTIMING("Sort");
			Sort(d1);
		}
		d1 <<= d;
		{
			RTIMING("GetSortOrder");
			GetSortOrder(d1);
		}
		d1 <<= d;
		{
			RTIMING("StableSort");
			StableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("NewStableSort");
			NewStableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("QStableSort");
			QStableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("QStableSortCmp");
			QStableSort(d1);
		}
		a1 <<= a;
		{
			RTIMING("Array StableSort");
			StableSort(a1);
		}
		a1 <<= a;
		{
			RTIMING("Array NewStableSort");
			NewStableSort(a1);
		}
		a1 <<= a;
		{
			RTIMING("Array QStableSort");
			QStableSort(a1);
		}
		a1 <<= a;
		{
			RTIMING("Array QStableSortCmp");
			QStableSort(a1);
		}
		d1 <<= d;
		{
			RTIMING("std::sort String");
			std::sort(d1.Begin(), d1.End());
		}
		d1 <<= d;
		{
			RTIMING("std::stable_sort String");
			std::stable_sort(d1.Begin(), d1.End());
		}
		s1 = s;
		{
			RTIMING("std::sort std::string");
			std::sort(s1.begin(), s1.end());
		}
		s1 = s;
		{
			RTIMING("std::stable_sort std::string");
			std::stable_sort(s1.begin(), s1.end());
		}
	}

	for(int q = 0; q < 100; q++) {
		Vector<String> d, d1;
		Array<String> a, a1;
		std::vector<std::string> s, s1;
		for(int i = 0; i < 10000; i++) {
			String x = AsString(rand() % 10);
			d.Add(x);
			a.Add(x);
			s.push_back(to_string(x));
		}
		d1 <<= d;
		{
			RTIMING("X Sort");
			Sort(d1);
		}
		d1 <<= d;
		{
			RTIMING("X GetSortOrder");
			GetSortOrder(d1);
		}
		d1 <<= d;
		{
			RTIMING("X StableSort");
			StableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("X NewStableSort");
			NewStableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("X QStableSort");
			QStableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("X QStableSortCmp");
			QStableSort(d1);
		}
		a1 <<= a;
		{
			RTIMING("X Array StableSort");
			StableSort(a1);
		}
		a1 <<= a;
		{
			RTIMING("X Array NewStableSort");
			NewStableSort(a1);
		}
		a1 <<= a;
		{
			RTIMING("X Array QStableSort");
			QStableSort(a1);
		}
		a1 <<= a;
		{
			RTIMING("X Array QStableSortCmp");
			QStableSort(a1);
		}
		d1 <<= d;
		{
			RTIMING("X std::sort String");
			std::sort(d1.Begin(), d1.End());
		}
		d1 <<= d;
		{
			RTIMING("X std::stable_sort String");
			std::stable_sort(d1.Begin(), d1.End());
		}
		s1 = s;
		{
			RTIMING("X std::sort std::string");
			std::sort(s1.begin(), s1.end());
		}
		s1 = s;
		{
			RTIMING("X std::stable_sort std::string");
			std::stable_sort(s1.begin(), s1.end());
		}
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
		d1 <<= d;
		{
			RTIMING("L Sort");
			Sort(d1);
		}
		d1 <<= d;
		{
			RTIMING("L GetSortOrder");
			GetSortOrder(d1);
		}
		d1 <<= d;
		{
			RTIMING("L StableSort");
			StableSort(d1);
		}
		d1 <<= d;
		{
			RTIMING("L std::sort String");
			std::sort(d1.Begin(), d1.End());
		}
		d1 <<= d;
		{
			RTIMING("L std::stable_sort String");
			std::stable_sort(d1.Begin(), d1.End());
		}
		s1 = s;
		{
			RTIMING("L std::sort std::string");
			std::sort(s1.begin(), s1.end());
		}
		s1 = s;
		{
			RTIMING("L std::stable_sort std::string");
			std::stable_sort(s1.begin(), s1.end());
		}
	}
}
