#include <Core/Core.h>

using namespace Upp;

template <class T, class W, int n>
T Test()
{
	T c;
	for(int i = 0; i < n; i++) {
		W h[n], g[n], e[n];
		for(int j = 0; j < n; j++) {
			h[j] = j < i ? 0 : 9;
			g[j] = j < i ? 9 : 0;
			e[j] = 9;
		}
		c.Load(e);
		T a(h);
		T b(g);
		DDUMP(a);
		DDUMP(c);
		DDUMP(AllTrue(c == a));
		DDUMP(AnyTrue(c == a));
		DDUMP(CountTrue(c == a));
		if(AnyTrue(c == a))
			DDUMP(FirstTrue(c == a));
		if(!AllTrue(c == a))
			DDUMP(FirstFalse(c == a));
		String matches;
		for(int j = 0; j < n; j++)
			if(IsTrue(c == a, j))
				matches << j << ' ';
		DDUMP(matches);
		DDUMP(b);
		DDUMP(AllTrue(c == b));
		DDUMP(AnyTrue(c == b));
		DDUMP(CountTrue(c == b));
		if(AnyTrue(c == b))
			DDUMP(FirstTrue(c == b));
		if(!AllTrue(c == b))
			DDUMP(FirstFalse(c == b));
		matches.Clear();
		for(int j = 0; j < n; j++)
			if(IsTrue(c == b, j))
				matches << j << ' ';
		DDUMP(matches);
		DLOG("---");
	}
	return c;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	// CheckCode();

	// DoTest2(); return;

	DLOG("=============== f32x4");
	{
		f32x4 c = Test<f32x4, float, 4>();

		f32x4 a(1, 2, 3, 4);
		f32x4 b(1, 2, 3, 9);

		DDUMP(a);
		DDUMP(b);
		DDUMP(c);
		DDUMP(AllTrue(a == b));
		DDUMP(AllTrue(a == a));
		DDUMP(AnyTrue(a == b));
		DDUMP(AnyTrue(a == c));
		DDUMP(CountTrue(a == b));
		DDUMP(FirstTrue(a == b));
		DDUMP(FirstFalse(a == b));
	}
	DLOG("=============== i16x8");
	{
		i16x8 c = Test<i16x8, word, 8>();

		i16x8 a(1, 2, 3, 4, 5, 6, 7, 8);
		i16x8 b(1, 4, 2, 4, 5, 7, 7, 9);
		
		DDUMP(a);
		DDUMP(b);
		DDUMP(c);
		DDUMP(AllTrue(a == b));
		DDUMP(AllTrue(a == a));
		DDUMP(AnyTrue(a == b));
		DDUMP(AnyTrue(a == c));
		DDUMP(CountTrue(a == b));
		DDUMP(FirstTrue(a == b));
		DDUMP(FirstFalse(a == b));
	}
	DLOG("=============== i32x4");
	{
		i32x4 c = Test<i32x4, dword, 4>();

		i32x4 a(1, 2, 3, 4);
		i32x4 b(1, 2, 3, 9);
		
		DDUMP(a);
		DDUMP(b);
		DDUMP(c);
		DDUMP(AllTrue(a == b));
		DDUMP(AllTrue(a == a));
		DDUMP(AnyTrue(a == b));
		DDUMP(AnyTrue(a == c));
		DDUMP(CountTrue(a == b));
		DDUMP(FirstTrue(a == b));
		DDUMP(FirstFalse(a == b));
	}
	DLOG("=============== i8x16");
	{
		i8x16 c = Test<i8x16, byte, 16>();

		i8x16 a(1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
		i8x16 b(1, 4, 2, 4, 5, 7, 7, 0, 1, 4, 2, 4, 5, 7, 7, 9);
		
		DDUMP(a);
		DDUMP(b);
		DDUMP(c);
		DDUMP(AllTrue(a == b));
		DDUMP(AllTrue(a == a));
		DDUMP(AnyTrue(a == b));
		DDUMP(AnyTrue(a == c));
		DDUMP(CountTrue(a == b));
		DDUMP(FirstTrue(a == b));
		DDUMP(FirstFalse(a == b));
	}
	CheckLogEtalon();
}
