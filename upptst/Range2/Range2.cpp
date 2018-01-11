#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(MakeIota(10));
	DUMP(MakeIota(10, 1, 3));

	// Testing r-value argument
	Sort(SubRange(MakeIota(10), 1, 4));
	Sort(ReverseRange(MakeIota(10)));
	Sort(ViewRange(MakeIota(10), MakeIota(3)));
	Sort(FilterRange(MakeIota(10), [=](int x) { return x > 4; }));
	Sort(SortedRange(MakeIota(10)));

	// Testing const argument
	auto h = MakeIota(20);
	const auto& x = h;

	DUMP(SubRange(x, 1, 4));
	DUMP(SortedRange(x));
	DUMP(ReverseRange(x));
	DUMP(ViewRange(x, MakeIota(3)));
	DUMP(FilterRange(x, [=](int x) { return x > 4; }));
	DUMP(SortedRange(x));
	DUMP(FilterRange(ReverseRange(SortedRange(x)), [] (int x) { return x & 1; }));
}
