#include <Core/Core.h>

using namespace Upp;

#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SeedRandom(0);

	Vector<int> h = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	Sort(SubRange(h, 2, 5));
	DDUMP(h);
	Sort(SubRange(h.begin() + 1, h.end()));
	DDUMP(h);
//	Sort(Range(h.begin(), 5), std::greater<int>());
	DDUMP(h);
	
	DDUMPC(FilterRange(h, [=](int x) { return x & 1; }));
	
	for(int i = 0; i < 10; i++)
		h.Add(Random(25));

	DDUMP(FilterRange(h, [=](int x) { return x & 1; }));
	DDUMP(FilterRange(h, [=](int x) { return !(x & 1); }));

	Sort(FilterRange(h, [=](int x) { return x & 1; }));
	Sort(FilterRange(h, [=](int x) { return !(x & 1); }), std::greater<int>());
	DDUMP(h);

	DDUMPC(ConstRange(12, 5));
	
	h.SetRange(5, ConstRange(123, 3));
	DDUMP(h);
	
	h.InsertRange(5, ConstRange(10, 5));
	DDUMP(h);
	
	String vs = String::GetVoid();
	DDUMP(vs.IsVoid());
	vs.Clear();
	DDUMP(vs.IsVoid());
	
	Array<int> ah;
	ah.AppendRange(h);
	DDUMP(ah);
	ah.InsertRange(4, ConstRange(54321, 2));
	DDUMP(ah);
	
	Sort(h);
	DDUMP(h);
	Reverse(SubRange(h, 5, 5));
	DDUMP(h);
	
	DDUMP(FindMin(h));
	DDUMP(FindMax(h));
	DDUMP(Min(h));
	DDUMP(Max(h));
	
	Sort(h);
	DDUMPC(h);
	DDUMP(FindUpperBound(h, 8));
	DDUMP(FindLowerBound(h, 8));

	DDUMP(FindUpperBound(h, 9));
	DDUMP(FindLowerBound(h, 9));
	
	DDUMP(FindBinary(h, 10));
	DDUMP(FindBinary(h, 20));
	
	DDUMP(Sum(h));
	
	Vector<int> empty;
	DDUMP(Min(empty, 11));
	DDUMP(Max(empty, 12));
}
