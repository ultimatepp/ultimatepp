#include <Core/Core.h>

using namespace Upp;

#undef CHECK

#define CHECK(a, b) AsString(a) == b

// #define CHECK(a, b) LOG("CHECK(" << #a << ", " << AsCString(AsString(a)) << ");");

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	CHECK(MakeIota(10), "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
	CHECK(MakeIota(10, 1, 3), "[1, 4, 7]");
	CHECK(ConstRange(7, 5), "[7, 7, 7, 7, 7]");

	LOG("----- r-value argument");
	CHECK(SubRange(MakeIota(20), 1, 4), "[1, 2, 3, 4]");
	CHECK(SortedRange(MakeIota(20)), "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]");
	CHECK(ReverseRange(MakeIota(20)), "[19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0]");
	CHECK(ViewRange(MakeIota(20), MakeIota(3)), "[0, 1, 2]");
	CHECK(FilterRange(MakeIota(20), [](int a) { return a > 4; }), "[5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]");
	CHECK(FilterRange(ReverseRange(SortedRange(MakeIota(20))), [] (int a) { return a & 1; }), "[19, 17, 15, 13, 11, 9, 7, 5, 3, 1]");

	auto h = MakeIota(20);

	LOG("----- l-value argument");
	CHECK(SubRange(h, 1, 4), "[1, 2, 3, 4]");
	CHECK(SortedRange(h), "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]");
	CHECK(ReverseRange(h), "[19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0]");
	CHECK(ViewRange(h, MakeIota(3)), "[0, 1, 2]");
	CHECK(FilterRange(h, [](int h) { return h > 4; }), "[5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]");
	CHECK(FilterRange(ReverseRange(SortedRange(h)), [] (int h) { return h & 1; }), "[19, 17, 15, 13, 11, 9, 7, 5, 3, 1]");

	LOG("----- const argument");
	const auto& x = h;

	CHECK(SubRange(x, 1, 4), "[1, 2, 3, 4]");
	CHECK(SortedRange(x), "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]");
	CHECK(ReverseRange(x), "[19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0]");
	CHECK(ViewRange(x, MakeIota(3)), "[0, 1, 2]");
	CHECK(FilterRange(x, [](int x) { return x > 4; }), "[5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]");
	CHECK(FilterRange(ReverseRange(SortedRange(x)), [] (int x) { return x & 1; }), "[19, 17, 15, 13, 11, 9, 7, 5, 3, 1]");
	
	LOG("============ OK");
}
