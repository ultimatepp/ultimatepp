#include "SvoValue.h"

void CheckRawValue()
{
	typedef Tuple2<int, int> T;
	T x = MakeTuple(11, 22);
	Value v = RawToValue(x);
	const T& x2 = v.To<T>();
	ASSERT(x2 == x);
}

void CheckRawPickValue()
{
	Vector<int> x;
	x.Add(123);
	Value v = RawPickToValue(pick(x));
	ASSERT(v.Is< Vector<int> >());
	const Vector<int>& xx = v.To< Vector<int> >();
	ASSERT(xx.GetCount() == 1);
	ASSERT(xx[0] == 123);

	x.Clear();
	x.Add(321);
	v = RawDeepToValue(x);
	const Vector<int>& x2 = v.To< Vector<int> >();
	ASSERT(x2.GetCount() == 1);
	ASSERT(x2[0] == 321);
	ASSERT(x.GetCount() == 1);
	ASSERT(x[0] == 321);
}

void CheckCreateRawValue()
{
	Value v;
	Vector<int>& x = CreateRawValue< Vector<int> >(v);
	x.Add(123);
	ASSERT(v.Is< Vector<int> >());
	const Vector<int>& xx = v.To< Vector<int> >();
	ASSERT(xx.GetCount() == 1);
	ASSERT(xx[0] == 123);
}
