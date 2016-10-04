#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	{
		bool b = false;
		Callback x;
		x << [&] { b = true; };
		x();
		ASSERT(b);
	}
	{
		bool b = false;
		Callback1<bool> x;
		x << [&](bool x) { b = x; };
		x(true);
		ASSERT(b);
		x(false);
		ASSERT(!b);
	}
	{
		bool b = false;
		int  n = 0;
		Callback2<bool, int> x;
		x << [&](bool x, int y) { b = x; n = y; };
		x(true, 12);
		ASSERT(b);
		ASSERT(n == 12);
		x(false, 13);
		ASSERT(!b);
		ASSERT(n == 13);
	}
	{
		bool b = false;
		int  n = 0;
		String s;
		Callback3<bool, int, String> x;
		x << [&](bool x, int y, String z) { b = x; n = y; s = z; };
		x(true, 12, "one");
		ASSERT(b);
		ASSERT(n == 12);
		ASSERT(s == "one");
		x(false, 13, "two");
		ASSERT(!b);
		ASSERT(n == 13);
		ASSERT(s == "two");
	}
	{
		bool b = false;
		int  n = 0;
		String s;
		Color c = Blue;
		Callback4<bool, int, String, Color> x;
		x << [&](bool x, int y, String z, Color q) { b = x; n = y; s = z; c = q; };
		x(true, 12, "one", Blue());
		ASSERT(b);
		ASSERT(n == 12);
		ASSERT(s == "one");
		ASSERT(c == Blue());
		x(false, 13, "two", Green());
		ASSERT(!b);
		ASSERT(n == 13);
		ASSERT(s == "two");
		ASSERT(c == Green());
	}

	{
		bool b = false;
		Gate<> x;
		x << [&]() -> bool { b = true; return b; };
		ASSERT(x());
		ASSERT(b);
	}
	{
		bool b = false;
		Gate1<bool> x;
		x << [&](bool x) -> bool { b = x; return b; };
		ASSERT(x(true));
		ASSERT(b);
		ASSERT(!x(false));
		ASSERT(!b);
	}
	{
		bool b = false;
		int  n = 0;
		Gate2<bool, int> x;
		x << [&](bool x, int y) -> bool { b = x; n = y; return b; };
		ASSERT(x(true, 12));
		ASSERT(b);
		ASSERT(n == 12);
		ASSERT(!x(false, 13));
		ASSERT(!b);
		ASSERT(n == 13);
	}
	{
		bool b = false;
		int  n = 0;
		String s;
		Gate3<bool, int, String> x;
		x << [&](bool x, int y, String z) -> bool { b = x; n = y; s = z; return b; };
		ASSERT(x(true, 12, "one"));
		ASSERT(b);
		ASSERT(n == 12);
		ASSERT(s == "one");
		ASSERT(!x(false, 13, "two"));
		ASSERT(!b);
		ASSERT(n == 13);
		ASSERT(s == "two");
	}
	{
		bool b = false;
		int  n = 0;
		String s;
		Color c = Blue;
		Gate4<bool, int, String, Color> x;
		x << [&](bool x, int y, String z, Color q) -> bool { b = x; n = y; s = z; c = q; return b; };
		ASSERT(x(true, 12, "one", Blue()));
		ASSERT(b);
		ASSERT(n == 12);
		ASSERT(s == "one");
		ASSERT(c == Blue());
		ASSERT(!x(false, 13, "two", Green()));
		ASSERT(!b);
		ASSERT(n == 13);
		ASSERT(s == "two");
		ASSERT(c == Green());
	}

	DLOG("----- Everything OK");
}
