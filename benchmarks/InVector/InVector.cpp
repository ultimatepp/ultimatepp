#include <Core/Core.h>

#include <set>

using namespace Upp;

#define SN 10000000
#define IN 100

int sSum = 0;

template <class T>
void ScanBenchmark()
{
	T x;
	for(int i = 0; i < SN; i++)
		x.Insert(x.GetCount()) = i;
	
	{
		TimeStop tm;
		for(int j = 0; j < IN; j++)
			for(int i = 0; i < SN; i++)
				sSum += x[i];
		RLOG(typeid(T).name() << " index scan " << tm);
	}
	{
		TimeStop tm;
		typename T::Iterator end = x.End();
		for(int j = 0; j < IN; j++)
			for(typename T::Iterator it = x.Begin(); it != end; ++it)
				sSum += *it;
		RLOG(typeid(T).name() << " iterator scan " << tm);
	}
}

void InVectorScanBenchmark()
{
	ScanBenchmark< Vector<int> >();
	ScanBenchmark< Array<int> >();
	ScanBenchmark< InVector<int> >();
}

#define N 10000000


void InVectorBenchmark()
{

	std::multiset<int> s;
	SeedRandom();
	{
		RTIMING("std::set INSERT");
		for(int i = 0; i < N; i++) {
			int x = Random();
			s.insert(x);
		}
	}

	SeedRandom();
	{
		InVector<int> a;
		RTIMING("InVector INSERT UB");
		for(int i = 0; i < N; i++) {
			int x = Random();
			a.Insert(FindLowerBound(a, x)) = x;
		}
	}
	InVector<int> a;
	{
		RTIMING("InVector INSERT");
		a.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			a.Insert(Random(a.GetCount())) = i;
		}
	}
	{
		InVector<int> a;
		RTIMING("InVector INSERT 0");
		a.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			a.Insert(0) = Random(100);
		}
	}
	{
		InVector<int> a;
		RTIMING("InVector INSERT N/2");
		a.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			a.Insert(i / 2) = Random(100);
		}
	}
	return;
	{
		RTIMING("InVector SCAN");
		int sum = 0;
		for(int i = 0; i < a.GetCount(); i++)
			sum += a[i];
		RDUMP(sum);
	}
	{
		RTIMING("InVector iterator SCAN");
		int sum = 0;
		for(InVector<int>::Iterator i = a.Begin(); i != a.End(); i++)
			sum += *i;
		RDUMP(sum);
	}
	{
		RTIMING("InVector Sort");
		Sort(a);
	}
	RDUMP(a.GetCount());

	SeedRandom();
	Vector<int> b;
	{
		RTIMING("Vector INSERT");
		b.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			b.Add(Random());
//			b.Insert(Random(b.GetCount())) = Random(100);
		}
	}
	{
		RTIMING("Vector SCAN");
		int sum = 0;
		for(int i = 0; i < b.GetCount(); i++)
			sum += b[i];
		RDUMP(sum);
	}
	{
		RTIMING("Vector iterator SCAN");
		int sum = 0;
		for(Vector<int>::Iterator i = b.Begin(); i != b.End(); i++)
			sum += *i;
		RDUMP(sum);
	}
	{
		RTIMING("Vector Sort");
		Sort(b);
	}
	return;
	SeedRandom();
	Array<int> c;
	{
		RTIMING("Array INSERT");
		c.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			c.Add(Random(100));
//			c.Insert(Random(c.GetCount())) = Random(100);
		}
	}
	{
		RTIMING("Array SCAN");
		int sum = 0;
		for(int i = 0; i < c.GetCount(); i++)
			sum += c[i];
		RDUMP(sum);
	}
	{
		RTIMING("Array Sort");
		Sort(c);
	}

	SeedRandom();
	BiVector<int> d;
	{
		RTIMING("BiVector INSERT");
		d.AddTail() = 0;
		for(int i = 0; i < N; i++) {
			d.AddTail(Random(100));
//			d.Insert(Random(d.GetCount())) = Random(100);
		}
	}
	{
		RTIMING("BiVector SCAN");
		int sum = 0;
		for(int i = 0; i < d.GetCount(); i++)
			sum += d[i];
		RDUMP(sum);
	}
	{
		RTIMING("BiVector Sort");
		Sort(d);
	}
}

void InVectorInsertBenchmark()
{
	RTIMING("InVectorInsertBenchmark");
	InVector<int> iv;
	for(int i = 0; i < 10000000; i++)
		iv.Insert(iv.GetCount() / 2);
}

void SetBenchmark()
{
	const int rep = 100;
	const int count = 1000;
	SeedRandom();
	Buffer<int> rnd(count);
	for(int i = 0; i < count; i++)
		rnd[i] = Random();

	{	
		std::multiset<int> s;
		{
			RTIMING("std::set<int> INSERT");
			for(int j = 0; j < rep; j++) {
				s.clear();
				for(int i = 0; i < count; i++) {
					s.insert(rnd[i]);
				}
			}
		}
		typedef std::multiset<int>::iterator It;
		{
			RTIMING("std::set<int> SCAN");
			It e = s.end();
			for(int j = 0; j < rep; j++)
				for(It i = s.begin(); i != e; i++) {
					sSum += *i;
				}
		}
	}
	{
		InVector<int> s;
		{
			RTIMING("InVector<int> INSERT");
			for(int j = 0; j < rep; j++) {
				s.Clear();
				for(int i = 0; i < count; i++)
					s.InsertUpperBound(rnd[i]);
			}
			RDUMP(s.GetCount());
	//				s.Insert(s.FindUpperBound(rnd[i])) = rnd[i];
		}
		{
			RTIMING("InVector<int> SCAN");
			for(int j = 0; j < rep; j++)
				for(int i = 0; i < count; i++)
					sSum += s[i];
		}
	}
}

void MemoryInfo()
{
	RLOG(MemoryUsedKb() << " KB");
	MemoryProfile mem;
	RLOG(mem);
}

void SetBenchmark2()
{
	const int rep = 1;
	const int count = 10000 * 1000;
	SeedRandom();
	Buffer<String> rnd(count);
	for(int i = 0; i < count; i++)
		rnd[i] = AsString(Random());

	if(1) {	
		std::multiset<String> s;
		{
			RTIMING("std::set<String> INSERT");
			for(int j = 0; j < rep ; j++) {
				s.clear();
				for(int i = 0; i < count; i++) {
					s.insert(rnd[i]);
				}
			}
		}
		typedef std::multiset<String>::iterator It;
		{
			RTIMING("std::set<String> SCAN");
			It e = s.end();
			for(int j = 0; j < rep ; j++)
				for(It i = s.begin(); i != e; i++) {
					sSum += i->GetCount();
				}
		}
		{
			RTIMING("std::set<String> FIND");
			int n = 0;
			for(int j = 0; j < rep ; j++)
				for(int i = 0; i < count; i++)
					if(s.find(rnd[i]) != s.end())
					   n++;
			RDUMP(n);
		}
		MemoryInfo();
	}
	if(1) {
		InVector<String> s;
		{
			RTIMING("InVector<String> INSERT");
			for(int j = 0; j < rep ; j++) {
				s.Clear();
				for(int i = 0; i < count; i++)
					s.InsertUpperBound(rnd[i]);
			}
//				s.Insert(s.FindUpperBound(rnd[i])) = rnd[i];
		}
		{
			RTIMING("InVector<String> SCAN");
			for(int j = 0; j < rep ; j++)
				for(int i = 0; i < count; i++)
					sSum += s[i].GetCount();
		}
		{
			RTIMING("InVector<String> FIND");
			int n = 0;
			for(int j = 0; j < rep ; j++) {
				for(int i = 0; i < count; i++)
					if(s.Find(rnd[i]) >= 0)
						n++;
			}
			RDUMP(n);
		}
		MemoryInfo();
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	SeedRandom();

	SetBenchmark();
	InVectorInsertBenchmark();
	SetBenchmark2();
	InVectorScanBenchmark();
	InVectorBenchmark();
}
