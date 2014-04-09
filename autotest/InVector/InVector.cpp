#include <Core/Core.h>

using namespace Upp;

#ifdef flagLONG
#define N 100
#else
#define N 8
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

template <class T>
void Check(const InVector<T>& iv)
{
	ASSERT(iv.GetCount() == 0 || iv.FindUpperBound(iv.Top()) == iv.GetCount());
}

template <class T>
T ToType(int);

template <>
String ToType(int i)
{
	return Format("%010d", i);
}

template <>
int ToType(int i)
{
	return i;
}

template <class T>
void InVectorTest()
{
	SeedRandom();
	Vector<T> q;
	InVector<T> iv;
	Compare(q, iv);
	iv.Insert(0) = 0;
	q.Insert(0) = 0;
	iv.Insert(1) = ToType<T>(-1);
	q.Insert(1) = ToType<T>(-1);
	for(int j = 0; j < 100 * N; j++) {
		if(j % 1000 == 0)
			LOG(j);
		int i = Random(iv.GetCount());
		iv.Insert(i) = ToType<T>(i);
		q.Insert(i) = ToType<T>(i);
		Compare(q, iv);
	}

	for(int i = 0; i < N; i++) {
		int n = Random(100) + 20;
		typename InVector<T>::Iterator it2, it = iv.Begin();
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

template <class T>
void TestUpperBound()
{
	{
		InVector<T> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			v.Insert(i) = ToType<T>(i);
			ASSERT(v.FindUpperBound(ToType<T>(i)) == i + 1);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindUpperBound(ToType<T>(j)) == j + 1);
			Check(v);
		}
	}
	{
		InVector<T> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			for(int j = 0; j < 7; j++)
				v.Insert(7 * i) = ToType<T>(i);
			ASSERT(v.FindUpperBound(ToType<T>(i)) == 7 * i + 7);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindUpperBound(ToType<T>(j)) == 7 * j + 7);
			Check(v);
		}
	}
}

template <class T>
void TestLowerBound()
{
	{
		InVector<T> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			v.Insert(i) = ToType<T>(i);
			ASSERT(v.FindLowerBound(ToType<T>(i)) == i);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindLowerBound(ToType<T>(j)) == j);
			Check(v);
		}
	}
	{
		InVector<T> v;
		for(int i = 0; i < 30 * N; i++) {
			if(i % 1000 == 0)
				LOG(i);
			for(int j = 0; j < 7; j++)
				v.Insert(7 * i) = ToType<T>(i);
			ASSERT(v.FindLowerBound(ToType<T>(i)) == 7 * i);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindLowerBound(ToType<T>(j)) == 7 * j);
			Check(v);
		}
	}
}

template <class T>
void SetTest()
{
	for(int j = 0; j < 100; j++) {
		LOG(j);
		Vector<T> va;
		InVector<T> ia;
		for(int i = 0; i < 10 * N; i++) {
			int q = Random(100);
			int ii = FindUpperBound(va, ToType<T>(q));
			T val = ToType<T>(q);
			va.Insert(ii) = val;
			ia.InsertUpperBound(val);
			Compare(va, ia);
			Check(ia);
			
			ii = ia.Find(val);
			ASSERT(ia[ii] == val);
			ASSERT(ia.Find(ToType<T>(20000)) < 0);
		}
	}
}

template <class T>
void RemoveTest()
{
	SeedRandom();
	Vector<T> q;
	InVector<T> iv;
	Compare(q, iv);
	iv.Insert(0) = 0;
	q.Insert(0) = 0;
	iv.Insert(1) = ToType<T>(-1);
	q.Insert(1) = ToType<T>(-1);
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
			iv.Insert(i) = ToType<T>(i);
			q.Insert(i) = ToType<T>(i);
		}
		Compare(q, iv);
		ASSERT(iv.End() - iv.Begin() == iv.GetCount());
	}
}

template <class T>
void InsertNTest()
{
	SeedRandom();
	Vector<T> av;
	InVector<T> iv;
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
			av[pos + j] = ToType<T>(r);
			iv[pos + j] = ToType<T>(r);
		}
		Compare(av, iv);
	}
}

struct TestType : Moveable<TestType> {
	int    x;
	String y;
	
	bool operator<(const TestType& b) const  { return CombineCompare(x, b.x)(y, b.y) < 0; }
	bool operator==(const TestType& b) const { return x == b.x && y == b.y; }
	
	TestType(int i) { x = i; y = AsString(i ^ 7); }
	TestType() {}
};

template <>
TestType ToType(int i)
{
	return TestType(i);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	TimeStop tm;

	int time0 = msecs();
	
	SeedRandom();

	SetTest<int>();
	TestLowerBound<int>();
	TestUpperBound<int>();
	RemoveTest<int>();
	InsertNTest<int>();
	InVectorTest<int>();

	SetTest<String>();
	TestLowerBound<String>();
	TestUpperBound<String>();
	RemoveTest<String>();
	InsertNTest<String>();
	InVectorTest<String>();

	SetTest<TestType>();
	TestLowerBound<TestType>();
	TestUpperBound<TestType>();
	RemoveTest<TestType>();
	InsertNTest<TestType>();
	InVectorTest<TestType>();
	
	LOG("========= OK " << tm);
}
