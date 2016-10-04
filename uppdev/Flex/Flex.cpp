#include "Flex.h"
#include <set>
#include <vector>
#include <list>

void Test(int N = 400)
{
	Vector<int> a;
	Flex<int> b;
	for(int i = 0; i < N; i++) {
		int pos = i ? rand() % i : 0;
		a.Insert(pos, i);
		b.Insert(pos, i);
		for(int j = 0; j <= i; j++)
			if(a[j] != b[j]) {
				DUMPC(a);
				DUMP(j);
				for(int q = 0; q <= i; q++)
					LOG(q << ": " << b[q]);
				Panic("Panic!");
			}
		if(i % 2000 == 0)
			Cout() << i << "\n";
	}
	Cout() << "Passed!\n";
}

void TestLowerBound()
{
	Cout() << "Lower bound test\n";
	Vector<int> a;
	Flex<int> b(2);
	for(int i = 0; i < 2000; i++) {
		ASSERT(a.GetCount() == b.GetCount());
		for(int j = -i; j <= i; j++) {
			for(int k = -1; k <= 1; k++) {
				int vl = FindLowerBound(a, 2 * j + k);
				int fl = b.FindLowerBound(2 * j + k);
				if(vl != fl) {
					DUMP(2 * j);
					DUMP(k);
					DUMP(vl);
					DUMP(fl);
					DUMPC(a);
					for(int q = 0; q < b.GetCount(); q++)
						LOG(q << ": " << b[q]);
					Panic("Panic!");
				}
			}
		}
		Cout() << i << "\n";
		a.Insert(a.GetCount(), 2 * i);
		a.Insert(0, -2 * i);
		b.Insert(b.GetCount(), 2 * i);
		b.Insert(0, -2 * i);
	}
	Cout() << "Passed!\n";
}

void TestUpperBound()
{
	Vector<int> a;
	Flex<int> b(2);
	for(int i = 0; i < 20000; i++) {
		a.Insert(i, 2 * i);
		b.Insert(i, 2 * i);
		for(int j = 0; j <= i; j++) {
			for(int k = -1; k <= 1; k++) {
				int vl = FindUpperBound(a, 2 * j + k);
				int fl = b.FindUpperBound(2 * j + k);
				if(vl != fl) {
					DUMP(2 * j);
					DUMP(k);
					DUMP(vl);
					DUMP(fl);
					DUMPC(a);
					for(int q = 0; q <= i; q++)
						LOG(q << ": " << b[q]);
					Panic("Panic!");
				}
			}
		}
		if(i % 2000 == 0)
			Cout() << i << "\n";
	}
	Cout() << "Passed!\n";
}

void TestMap()
{
	Order<int> a;
	Vector<int> b;
	for(int i = 0; i < 100000; i++) {
		int q = rand();
		a.FindAdd(q);
		int p = FindLowerBound(b, q);
		if(p >= b.GetCount() || b[p] != q)
			b.Insert(p, q);
		LOG(a.GetCount() << " " << b.GetCount());
		if(a.GetCount() != b.GetCount()) {
			DUMP(q);
			DUMPC(b);
			for(int q = 0; q < a.GetCount(); q++)
				LOG(q << ": " << a[q]);
			Panic("Failed count!");
		}
		for(int j = 0; j < b.GetCount(); j++) {
			if(a[j] != b[j]) {
				DUMP(q);
				DUMPC(b);
				for(int q = 0; q < a.GetCount(); q++)
					LOG(q << ": " << a[q]);
				Panic("Failed!");
			}
		}
		if(i % 1000 == 0)
			Cout() << i << "\n";
	}
}

static Buffer<int> rndpos(100000000);

void ScaleBench()
{
	for(int N = 10; N < 20000000; N += N) {
		int M = max(1, 2000000 / N);
		Cout() << N << " elements\r\n";
		RLOG(N << " elements");
		for(int shift = 3; shift < 16; shift++)
		{
			TimeStop tm;
			for(int q = 0; q < M; q++) {
				Flex<int> f(shift);
				for(int i = 0; i < N; i++)
					f.Insert(rndpos[i], i);
			}
			Cout() << " shift " << shift << ":" << tm << "\n";
			RLOG(" shift " << shift << ":" << tm);
		}
		LOG("-----------------");
	}
	return;
}

void VectorBench()
{
	for(int N = 200; N < 200000; N += N) {
		int M = max(1, 2000000 / N);
		Cout() << N << " elements ( " << M << "x):\n";
		LOG(N << " elements ( " << M << "x):");
		{
			TimeStop tm;
			for(int q = 0; q < M; q++) {
				Flex<int> f;
				for(int i = 0; i < N; i++)
					f.Insert(rndpos[i], i);
			}
			Cout() << " Flex: " <<  tm << "\n";
			LOG(" Flex: " <<  tm);
		}
		{
			TimeStop tm;
			for(int q = 0; q < M; q++) {
				Vector<int> f;
				for(int i = 0; i < N; i++)
					f.Insert(rndpos[i], i);
			}
			Cout() << " Vector: " << tm << "\n";
			LOG(" Vector: " <<  tm);
		}
	}
}

template <class T>
void SetBench(T *rndpos) {
	for(int elements = 10; elements < 10000000; elements += 3 * elements) {
	#ifdef _DEBUG
		int repeat = 1;
	#else
		int repeat = max(1, 10000000 / elements);
	#endif
		int count1, count2;
		RLOG("------ " << elements << " elements");
		{
			TimeStop tm;
			String info;
			for(int q = 0; q < repeat; q++) {
				Order<T> set;
				for(int i = 0; i < elements; i++)
					set.FindAdd(rndpos[i]);
				if(q == 0)
					info = set.GetInfo();
			}
			RLOG("Order FindAdd: " << tm.Elapsed() << "ms, info: " << info);
			Order<T> set;
			for(int i = 0; i < elements; i++)
				set.FindAdd(rndpos[i]);
			count1 = set.GetCount();
			for(int i = 0; i < set.GetCount() - 1; i++)
				if(set[i] > set[i + 1])
					Panic("BUG!");
			TimeStop tm2;
			int xx;
			for(int q = 0; q < repeat; q++) {
				xx = 0;
				for(int i = 0; i < elements; i++) {
					if(set.Find(rndpos[i]) >= 0) xx++;
				}
			}
			RLOG("Order Find: " << tm2.Elapsed() << "ms, matches: " << xx);
		}
		{
			TimeStop tm;
			for(int q = 0; q < repeat; q++) {
				Index<T> set;
				for(int i = 0; i < elements; i++)
					set.FindAdd(rndpos[i]);
			}
			RLOG("Index FindAdd: " << tm.Elapsed());
			Index<T> set;
			for(int i = 0; i < elements; i++)
				set.FindAdd(rndpos[i]);
			TimeStop tm2;
			int xx;
			for(int q = 0; q < repeat; q++) {
				xx = 0;
				for(int i = 0; i < elements; i++) {
					if(set.Find(rndpos[i]) >= 0) xx++;
				}
			}
			RLOG("Index Find: " << tm2.Elapsed() << "ms, matches: " << xx);
		}
		{
			TimeStop tm;
			for(int q = 0; q < repeat; q++) {
				std::set<T> sset;
				for(int i = 0; i < elements; i++)
					sset.insert(rndpos[i]);
			}
			RLOG("tree  insert: " << tm.Elapsed() << "ms");
			std::set<T> sset;
			for(int i = 0; i < elements; i++)
				sset.insert(rndpos[i]);
			count2 = sset.size();
			TimeStop tm2;
			int xx;
			for(int q = 0; q < repeat; q++) {
				xx = 0;
				for(int i = 0; i < elements; i++)
					if(sset.find(rndpos[i]) != sset.end()) xx++;
			}
			RLOG("tree  find: " << tm2.Elapsed() << "ms, matches: " << xx);
		}
/*		if(count1 != count2) {
			RDUMP(count1);
			RDUMP(count2);
			Panic("Failure!");
		}*/
	}
}

void SetBenchmark()
{
	RLOG("========= int");
	SetBench<int>(rndpos);
	Buffer<String> rnd(10000000);
	for(int i = 0; i < 10000000; i++)
		rnd[i] = AsString(rndpos[i]);
	RLOG("========= String");
	SetBench<String>(rnd);
}

String FillLeft(const String& text, int n)
{
	int q = n - text.GetLength();
	return q > 0 ? String(' ', q) + text : text;
}

String FillRight(const String& text, int n)
{
	int q = n - text.GetLength();
	return q > 0 ? text + String(' ', q) : text;
}

template <class T>
void InsertBench(const T& data)
{
	VppLog() << "\n" << FillRight("size (times)", 20)
	         << FillLeft("std::vector", 12)
	         << FillLeft("Vector", 12)
	         << FillLeft("Flex", 12) << "\n";
	for(int n = 10; n < 500000; n += n) {
		Buffer<int> rnd(n);
		for(int q = 0; q < n; q++)
			rnd[q] = q ? (rand() + (rand() << 16)) % q : 0;
		int N = 1000000 / n;
		VppLog() << FillLeft(AsString(n), 7) << FillRight(" (" + AsString(N) + "x)", 13);
		T x = data;
		{
			TimeStop tm;
			for(int q = 0; q < N; q++) {
				std::vector<T> v;
				for(int i = 0; i < n; i++)
					v.insert(v.begin() + rnd[i], x);
			}
			VppLog() << FillLeft(tm.ToString() + " s", 12);
		}
		{
			TimeStop tm;
			for(int q = 0; q < N; q++) {
				Vector<T> v;
				for(int i = 0; i < n; i++)
					v.Insert(rnd[i], x);
			}
			VppLog() << FillLeft(tm.ToString() + " s", 12);
		}
		{
			TimeStop tm;
			for(int q = 0; q < N; q++) {
				Flex<T> v;
				for(int i = 0; i < n; i++)
					v.Insert(rnd[i], x);
			}
			VppLog() << FillLeft(tm.ToString() + " s", 12);
		}
		VppLog() << "\n";
	}
}

void InsertBenchmark()
{
	VppLog() << "int\n";
	InsertBench<int>(0);
	VppLog() << "\n\nString";
	InsertBench<String>(String("01234567890123456789"));
}

void IterateBenchmark()
{
	int elements = 10000000;
	int N = 10;
	while(elements > 0) {
		RLOG(elements << "(" << N << "x)");
		Flex<int> a;
		Vector<int> b;
		Array<int> c;
		std::list<int> d;
		for(int i = 0; i < elements; i++) {
			a.Insert(a.GetCount(), i);
			b.Add(i);
			c.Add(i);
			d.push_back(i);
		}
		{
			TimeStop tm;
			unsigned sum = 0;
			for(int n = 0; n < N; n++) {
				sum = 0;
				for(int i = 0; i < a.GetCount(); i++)
					sum += a[i];
			}
			RLOG("Flex " << tm << ", sum = " << sum);
		}
		{
			TimeStop tm;
			unsigned sum = 0;
			for(int n = 0; n < N; n++) {
				sum = 0;
				for(int i = 0; i < b.GetCount(); i++)
					sum += b[i];
			}
			RLOG("Vector " << tm << ", sum = " << sum);
		}
		{
			TimeStop tm;
			unsigned sum = 0;
			for(int n = 0; n < N; n++) {
				sum = 0;
				for(int i = 0; i < c.GetCount(); i++)
					sum += c[i];
			}
			RLOG("Array " << tm << ", sum = " << sum);
		}
		{
			TimeStop tm;
			unsigned sum = 0;
			for(int n = 0; n < N; n++) {
				sum = 0;
				for(std::list<int>::iterator i = d.begin(); i != d.end(); i++)
					sum += *i;
			}
			RLOG("std::list " << tm << ", sum = " << sum);
		}
		elements /= 100;
		N *= 100;
	}
}

CONSOLE_APP_MAIN
{
//	TestMap(); return;
//	Test(10000); return;
//	TestUpperBound(); return;
//	Test(400); TestLowerBound(); return;


	StdLogSetup(LOG_COUT|LOG_FILE);
	InsertBenchmark(); return;
//	IterateBenchmark(); return;
	for(int i = 0; i < 100000000; i++) {
		rndpos[i] = i ? (rand() + (rand() << 15)) : 0;
	}
//	SetBenchmark();
//	Test();	return;
//	ScaleBench(); return;
	return;
}
