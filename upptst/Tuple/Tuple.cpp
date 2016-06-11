#include <Core/Core.h>

using namespace Upp;

struct NonVal {
	Tuple<int, double> h;

	int x;
	int y;
};

CONSOLE_APP_MAIN
{
	Tuple<int, int, int, int> i4;
	DDUMP(sizeof(i4));
	
	Vector<Tuple<int, String>> testv;
	testv.Add(MakeTuple(13, "Hello"));
	DDUMP(testv);
	
	static Tuple<int, String> h[] = { { 1, "Ahoj" } };
	
	DDUMP(h[0]);

	Tuple<int, String> x;
	x.a = 171;
	x.b = "HHH";
	
	DDUMP(x.Get<int>());
	DDUMP(x.Get<String>());
	
	x.Get<0>() = 666;
	DDUMP(x.Get<int>());

	x.Get<int>() = 777;
	DDUMP(x.Get<0>());

	Tuple<int, Point, String> z;
	z.a = 0;
	z.b = Point(1, 2);
	z.c = "HHH";
	
	DDUMP(z.c);
	
	DDUMP(z.Get(0));
	DDUMP(z.Get(1));
	DDUMP(z.Get(2));
	
	DDUMP(z.Get<0>());
	DDUMP(z.Get<1>());
	DDUMP(z.Get<2>());
	
	DDUMP(z.GetArray());
	
	{
		ValueArray h = z.GetArray();
		h.Set(1, Point(5, 6));
		z.SetArray(h);
		DDUMP(z);
	}
	
	Tuple<NonVal, int> y;
	y.a.x = 5;
	
	Tuple<int, int> it;
	it.a = 10;
	it.b = 11;
	Tuple<double, double> ft = it;
	DDUMP(ft.a / 7);
	DDUMP(ft.b);
}
