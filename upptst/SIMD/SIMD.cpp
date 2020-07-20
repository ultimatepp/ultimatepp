#include <Core/Core.h>
#include <Draw/Draw.h>

using namespace Upp;

#define VAL(x)     (String() << "TEST(" << #x << ", \"" << (x) << "\");")
#define TEST0(x)   LOG(VAL(x))
#define TEST(x, y) LOG(VAL(x)); ASSERT(AsString(x) == y);

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
#ifdef CPU_SIMD
	RGBA x = Color(11, 22, 33);
	RGBA *s = &x;
	
	TEST(LoadRGBAF(&x), "255 11 22 33");
	TEST(i16x8(*(dword *)s), "0 0 0 0  0 0 0 5665");
	TEST(Unpack8L(i16x8(*(dword *)s)), "0 0 0 0  0 0 22 33");
	TEST(Unpack8L(i16x8(*(dword *)s)), "0 0 0 0  0 0 22 33");

	{
		f32x4 x(1, 2, 3, 4);
		f32x4 y(1, 3, 2, 4);
		DLOG("----------------");
		TEST0(x);
		TEST0(Broadcast0(x));
		TEST0(Broadcast1(x));
		TEST0(Broadcast2(x));
		TEST0(Broadcast3(x));
		TEST0(min(x, y));
		TEST0(max(x, y));
		TEST0(x == y);
		TEST0(x != y);
		TEST0(x < y);
		TEST0(x <= y);
		TEST0(x > y);
		TEST0(x >= y);
		TEST0(AllTrue(x == y));
		TEST0(AllTrue(x == x));
		TEST0(x + y);
		TEST0(x - y);
		TEST0(x / y);
		TEST0(x * y);
	}

	{
		i16x8 x(1, 2, 3, 4, 5, 6, 7, 8);
		i16x8 y(1, 4, 2, 3, 8, 7, 5, 6);
		DLOG("----------------");
		TEST0(BroadcastLH0(x));
		TEST0(BroadcastLH1(x));
		TEST0(BroadcastLH2(x));
		TEST0(BroadcastLH3(x));
		TEST0(x == y);
		TEST0(x < y);
		TEST0(x > y);
		TEST0(AllTrue(x == y));
		TEST0(AllTrue(x == x));
		TEST0(x + y);
		TEST0(x - y);
		TEST0(x * y);
	}
	
	{
		i8x16 x(1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34);
		DLOG("----------------");
		TEST0(x);
		TEST0(Unpack8L(x));
		TEST0(Unpack8H(x));
	}
	{
		i16x8 x(1,2,3,4, 11,12,13,14);
		DLOG("----------------");
		TEST0(Unpack16L(x));
		TEST0(Unpack16H(x));
	}
	
	{
		i32x4 x(1, 2, 3, 4);
		i32x4 y(1, 4, 2, 3);
		DLOG("----------------");
		TEST0(ToFloat(x));
		TEST0(x == y);
		TEST0(x < y);
		TEST0(x > y);
		
		TEST0(AllTrue(x == y));
		TEST0(AllTrue(x == x));

		TEST0(x + y);
		TEST0(x - y);
		TEST0(x * y);
	}

	{
		f32x4 x(1.1, 2.8, 3.7, -4.2);
		DLOG("----------------");
		TEST0(x);
		TEST0(Truncate(x));
		TEST0(Round(x));
	}
	LOG("================ OK");
#else
	LOG("================ NO SIMD");
#endif
}
