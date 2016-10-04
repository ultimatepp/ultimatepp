#define  NDEBUG

#include <Core/Core.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>
#include <string>

using namespace std;
using namespace Upp;

namespace Upp {
NTL_MOVEABLE(std::string); // Illegal!
};

inline
unsigned GetHashValue(const std::string& x)
{
	unsigned hash = 2166136261U;
	for(size_t i = 0; i != x.size(); ++i)
		hash = 16777619 * hash + x[i];
	return hash;
}

double StopTime()
{
	static int clk;
	int q = clock();
	int n = q - clk;
	clk = q;
	return (double) n / CLOCKS_PER_SEC;
}

void PrintLine()
{
	cout << "-------------------------------------------------------\n";
}

template <class T>
void benchmark_VectorMap(const Vector<T>& key, const Vector<T>& fkey, int count)
{
	StopTime();
	cout << count << " items\n";
	{
		int i;
		VectorMap<T, int> map;
		StopTime();
		for(i = 0; i < count; i++)
			map.Add(key[i], i);
		cout << "Adding done in " << StopTime() << " s\n";
		for(int j = 0; j < 2; j++) {
			for(i = 0; i < count; i++)
				if(map.Get(fkey[i]) != i) {
					cout << "ERROR !\n";
					abort();
				}
			cout << "Search done in " << StopTime() << " s\n";
		}
		for(i = 0; i < count; i++)
			map.UnlinkKey(key[i]);
		cout << "Unlinking done in " << StopTime() << " s\n";
		for(i = 0; i < count; i++)
			map.Put(key[i], i);
		cout << "Putting done in " << StopTime() << " s\n";
	}
}

template <class T, class Map>
void benchmark_stlmap(const Vector<T>& key, const Vector<T>& fkey, int count)
{
	StopTime();
	cout << count << " items\n";
	{
		int i;
		Map map;
		StopTime();
		for(i = 0; i < count; i++)
			map.insert(make_pair(key[i], i));
		cout << "Inserting done in " << StopTime() << " s\n";
		for(i = 0; i < count; i++)
			if(map.find(fkey[i])->second != i) {
				cout << "ERROR !\n";
				abort();
			}
		cout << "Search done in " << StopTime() << " s\n";
		for(i = 0; i < count; i++)
			map.erase(key[i]);
		cout << "Removing keys done in " << StopTime() << " s\n";
	}
}

CONSOLE_APP_MAIN
{
	int n;
	string h = "tst";

	Vector<string> key;
	for(int i = 0; i < 1000000; i++) {
		char h[50];
//		sprintf(h, "%.10f", (double) i / 7);
		key.Add(FormatIntHex(i));
	}
	std::random_shuffle(key.Begin(), key.End());
	
	for(int i = 0; i < 5; i++)
		cout << key[i] << '\n';

	Vector<String> skey, skey2;
	for(int i = 0; i < 1000000; i++) {
		skey.Add(key[i]);
		skey2.Add(key[i]);
	}

/*	cout << "ADDING ELEMENTS TO VECTOR\n\n";

	
	StopTime();
	for(n = 0; n < 100; n++) {
		vector<int> v;
		for(int i = 0; i < 50000; i++)
			v.push_back(i);
	}
	cout << "Adding 50000 elements to vector<int> done in " << StopTime() / 100 << " s\n";
	StopTime();
	for(n = 0; n < 100; n++) {
		Vector<int> v;
		for(int i = 0; i < 50000; i++)
			v.Add(i);
	}
	cout << "Adding 50000 elements to Vector<int> done in " << StopTime() / 100 << " s\n";
	PrintLine();
	StopTime();
	for(n = 0; n < 10; n++) {
		vector<string> v;
		for(int i = 0; i < 50000; i++)
			v.push_back(key[i]);
	}
	cout << "Adding 50000 elements to vector<string> done in " << StopTime() / 10 << " s\n";
	StopTime();
	for(n = 0; n < 10; n++) {
		Vector<string> v;
		for(int i = 0; i < 50000; i++)
			v.Add(key[i]);
	}
	cout << "Adding 50000 elements to Vector<string> done in " << StopTime() / 10 << " s\n";
	PrintLine();
	
	StopTime();
	for(n = 0; n < 1000; n++)
	{
		vector< vector<int> > v;
		for(int i = 0; i < 100; i++) {
			vector<int> x;
			for(int q = 0; q < 100; q++)
				x.push_back(q);
			v.push_back(x);
		}
	}
	cout << "Adding 100x100 elements to vector< vector<int> > done in " << StopTime() << " ms\n";
	StopTime();
	for(n = 0; n < 1000; n++)
	{
		vector< vector<int> > v;
		for(int i = 0; i < 100; i++) {
			vector<int> x;
			v.push_back(x);
			vector<int>& w = v.back();
			for(int q = 0; q < 100; q++)
				w.push_back(q);
		}
	}
	cout << "Adding 100x100 elements to vector< vector<int> > (smart version) done in " << StopTime() << " ms\n";
	StopTime();
	for(n = 0; n < 1000; n++)
	{
		Vector< Vector<int> > v;
		for(int i = 0; i < 100; i++) {
			Vector<int>& w = v.Add();
			for(int q = 0; q < 100; q++)
				w.Add(q);
		}
	}
	cout << "Adding 100x100 elements to Vector< Vector<int> > done in " << StopTime() << " ms\n";
	PrintLine();

	StopTime();
	for(n = 0; n < 1000; n++)
	{
		vector< vector<string> > v;
		for(int i = 0; i < 100; i++) {
			vector<string> x;
			for(int q = 0; q < 100; q++)
				x.push_back(key[q]);
			v.push_back(x);
		}
	}
	cout << "Adding 100x100 elements to vector< vector<string> > done in " << StopTime() << " ms\n";
	StopTime();
	for(n = 0; n < 1000; n++)
	{
		vector< vector<string> > v;
		for(int i = 0; i < 100; i++) {
			vector<string> x;
			v.push_back(x);
			vector<string>& w = v.back();
			for(int q = 0; q < 100; q++)
				w.push_back(key[q]);
		}
	}
	cout << "Adding 100x100 elements to vector< vector<string> > (smart version) done in " << StopTime() << " ms\n";
	StopTime();
	for(n = 0; n < 1000; n++)
	{
		Vector< Vector<string> > v;
		for(int i = 0; i < 100; i++) {
			Vector<string>& w = v.Add();
			for(int q = 0; q < 100; q++)
				w.Add(key[q]);
		}
	}
	cout << "Adding 100x100 elements to Vector< Vector<string> > done in " << StopTime() << " ms\n";
	PrintLine();
*/
	
//	return;
/*	
	StopTime();
	{
		for(int i = 0; i < 1000; i++) {
			int i;
			deque<int> d;
			for(i = 0; i < 50000; i++)
				d.push_front(i);
			for(i = 0; i < 50000; i++) {
				d.pop_back();
				d.push_front(i);
			}
			for(i = 0; i < 50000; i++)
				d.pop_back();
		}
	}
	cout << "Passing 150000 elements through deque<int> done in " << StopTime() << " ms\n";
	
	StopTime();
	{
		for(int i = 0; i < 1000; i++) {
			int i;
			BiVector<int> d;
			for(i = 0; i < 50000; i++)
				d.AddHead(i);
			for(i = 0; i < 50000; i++) {
				d.DropTail();
				d.AddHead(i);
			}
			for(i = 0; i < 50000; i++)
				d.DropTail();
		}
	}
	cout << "Passing 150000 elements through BiVector<int> done in " << StopTime() << " ms\n";
*/
	cout << "\n\nMAP BENCHMARKS\n\n";
/*	{
		Vector<int> key;
		for(int i = 0; i < 1000000; i++)
			key.Add(i);
		std::random_shuffle(key.Begin(), key.End());
		
		cout << "\nVectorMap<int, int> benchmark ";
		benchmark_VectorMap(key, 1000000);
		cout << "\nmap<int, int> benchmark ";
		benchmark_map(key, 1000000);

		PrintLine();
		
		cout << "\nVectorMap<int, int> benchmark ";
		benchmark_VectorMap(key, 100000);
		cout << "\nmap<int, int> benchmark ";
		benchmark_map(key, 100000);
	}
	PrintLine();
*/
	{
		cout << "\nVectorMap<String, int> benchmark ";
		benchmark_VectorMap(skey, skey2, 1000000);
		cout << "\nmap<string, int> benchmark ";
		benchmark_stlmap< string, map<string, int> >(key, 1000000);
		cout << "\nmap<String, int> benchmark ";
		benchmark_stlmap< String, map<String, int> >(skey, 1000000);

		PrintLine();
		
		cout << "\nVectorMap<String, int> benchmark ";
		benchmark_VectorMap(skey, skey2, 100000);
		cout << "\nmap<string, int> benchmark ";
		benchmark_stlmap< string, map<string, int> >(key, 100000);
		cout << "\nmap<String, int> benchmark ";
		benchmark_stlmap< string, map<string, int> >(skey, 100000);
	}
}
