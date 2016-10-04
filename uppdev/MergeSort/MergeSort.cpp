#include <Core/Core.h>

void Reorder(const Vector<int>& order, Vector<int>& data)
{
	LOG("----------------------");
	ASSERT(data.GetCount() == order.GetCount());
	Vector<int> reorder, ndx;
	reorder.SetCount(order.GetCount());
	ndx.SetCount(order.GetCount());
	for(int q = reorder.GetCount(); --q >= 0;)
		reorder[q] = q;
	for(int i = 0; i < order.GetCount(); i++) {
		int pi = reorder[order[i]];
		DUMP(order[i]);
		DUMP(pi);
		Swap(data[i], data[pi]);
		reorder[ ndx[i] ] = pi;
		DUMPC(reorder);
		DUMPC(data);
		LOG("------------");
	}
}

struct VectorLess {
	bool operator()(const Vector<int>& a, const Vector<int>& b) const
	{
		return a[0] < b[0];
	}
};

bool int_less(int a, int b)
{
	return a < b;
}

bool vector_less(const Vector<int>& a, const Vector<int>& b)
{
	return a[0] < b[0];
}

CONSOLE_APP_MAIN
{
	Vector< Vector<int> > x;
	for(int i = 0; i < 20; i++) {
		x.Add().Add() = rand();
	}
	Sort(x, &vector_less);
	DUMPCC(x);
}
