#include <Core/Core.h>

using namespace Upp;

#ifdef flagLONG
#define N 100
#else
#define N 20
#endif

#ifdef CPU_ARM
#undef N
#define N 1
#endif

template <class C1, class C2>
void Compare(C1& a, C2& b)
{
	ASSERT(a.GetCount() == b.GetCount());

	for(int i = 0; i < a.GetCount(); i++)
		ASSERT(a[i] == b[i]);
	
	typename C1::Iterator ia = a.Begin();
	typename C2::Iterator ib = b.Begin();
	while(ib != b.End()) {
		ASSERT(*ia == *ib);
		ia++;
		ib++;
	}
}

void InArrayTest()
{
	SeedRandom();
	Vector<int> q;
	InArray<int> iv;
	Compare(q, iv);
	iv.Insert(0) = 0;
	q.Insert(0) = 0;
	iv.Insert(1) = -1;
	q.Insert(1) = -1;
	for(int j = 0; j < 100 * N; j++) {
		if(j % 1000 == 0)
			LOG(j);
		int i = Random(iv.GetCount());
		iv.Insert(i) = i;
		q.Insert(i) = i;
		Compare(q, iv);
		ASSERT(iv.End() - iv.Begin() == iv.GetCount());
	}

	for(int i = 0; i < N; i++) {
		int n = Random(100) + 20;
		InArray<int>::Iterator it2, it = iv.Begin();
		it += n;
		ASSERT(it - iv.Begin() == n);
		it2 = it;
		for(int j = 0; j < 10; j++) {
			ASSERT(it2 - iv.Begin() == n + j);
			++it2;
		}
		it2 = it;
		for(int j = 0; j < 10; j++) {
			ASSERT(it2 - iv.Begin() == n - j);
			--it2;
		}
	}

	StableSort(q);
	StableSort(iv);
	Compare(q, iv);
}

void TestUpperBound()
{
	{
		InArray<int> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			v.Insert(i) = i;
			ASSERT(v.FindUpperBound(i) == i + 1);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindUpperBound(j) == j + 1);
		}
	}
	{
		InArray<int> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			for(int j = 0; j < 7; j++)
				v.Insert(7 * i) = i;
			ASSERT(v.FindUpperBound(i) == 7 * i + 7);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindUpperBound(j) == 7 * j + 7);
		}
	}
}

void TestLowerBound()
{
	{
		InArray<int> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			v.Insert(i) = i;
			ASSERT(v.FindLowerBound(i) == i);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindLowerBound(j) == j);
		}
	}
	{
		InArray<int> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			for(int j = 0; j < 7; j++)
				v.Insert(7 * i) = i;
			ASSERT(v.FindLowerBound(i) == 7 * i);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindLowerBound(j) == 7 * j);
		}
	}
}

void SetTest()
{
	for(int j = 0; j < 100; j++) {
		LOG(j);
		Vector<int> va;
		InArray<int> ia;
		for(int i = 0; i < 10 * N; i++) {
			int q = Random(100);
			int ii = FindUpperBound(va, q);
			va.Insert(ii) = q;
			ia.InsertUpperBound(q);
			Compare(va, ia);
			
			ii = ia.Find(q);
			ASSERT(ia[ii] == q);
			ASSERT(ia.Find(200) < 0);
		}
	}
}

void RemoveTest()
{
	SeedRandom();
	Vector<int> q;
	InArray<int> iv;
	Compare(q, iv);
	iv.Insert(0) = 0;
	q.Insert(0) = 0;
	iv.Insert(1) = -1;
	q.Insert(1) = -1;
	for(int j = 0; j < 100000 * N; j++) {
		if(j % 1000 == 0)
			LOG(j);
		if(iv.GetCount() > 200 && Random(4) == 1) {
			int i = Random(iv.GetCount() - 21);
			int n = Random(20);
			iv.Remove(i, n);
			q.Remove(i, n);
		}
		else {
			int i = Random(iv.GetCount());
			iv.Insert(i) = i;
			q.Insert(i) = i;
		}
		Compare(q, iv);
		ASSERT(iv.End() - iv.Begin() == iv.GetCount());
	}
}

void InsertNTest()
{
	SeedRandom();
	Vector<int> av;
	InArray<int> iv;
	for(int i = 0; i < 1000 * N; i++) {
		if(i % 1000 == 0)
			LOG(i);
		if(av.GetCount() > 2000) {
			av.Clear();
			iv.Clear();
		}
		int pos = av.GetCount() ? Random(av.GetCount()) : 0;
		int n = Random(30);
		av.InsertN(pos, n);
		iv.InsertN(pos, n);
		for(int j = 0; j < n; j++) {
			int r = Random();
			av[pos + j] = r;
			iv[pos + j] = r;
		}
		Compare(av, iv);
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	TimeStop tm;
	SeedRandom();

	RemoveTest();
	InsertNTest();
	SetTest();
	TestLowerBound();
	TestUpperBound();
	InArrayTest();
	
	LOG("============== OK " << tm);
}
