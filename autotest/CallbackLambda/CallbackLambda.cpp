#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef CPP_11
	StdLogSetup(LOG_COUT|LOG_FILE);
	int a = 0;

	{
		Callback cb;
		cb = lambda([&] { a = 1; LOG("Callback"); });
		cb();
		ASSERT(a == 1);
	}

	{
		Callback cb;
		cb = LAMBDA { a = 1; LOG("Callback"); };
		cb();
		ASSERT(a == 1);
	}

	{
		Callback1<int> cb;
		cb = lambda([&](int b) { a = b; LOG("Callback1"); });
		cb(123);
		ASSERT(a == 123);
	}

	{
		Callback1<int> cb;
		cb = LAMBDA(int b) { a = b; LOG("Callback1"); };
		cb(123);
		ASSERT(a == 123);
	}

	{
		Callback2<int, int> cb;
		cb = lambda([&](int b, int c) { a = b + c; LOG("Callback2"); });
		cb(1, 20);
		ASSERT(a == 21);
	}

	{
		Callback3<int, int, int> cb;
		cb = lambda([&](int b, int c, int d) { a = b + c + d; LOG("Callback3"); });
		cb(1, 20, 300);
		ASSERT(a == 321);
	}
	
	{
		Callback4<int, int, int, int> cb;
		cb = lambda([&](int b, int c, int d, int e) { a = b + c + d + e; LOG("Callback4"); });
		cb(1, 20, 300, 4000);
		ASSERT(a == 4321);
	}

	{
		Gate cb;
		cb = lambda([&] { a = 1; LOG("Gate"); return true; });
		ASSERT(cb());
		ASSERT(a == 1);
	}
	
	{
		Gate1<int> cb;
		cb = lambda([&](int b) { a = b; LOG("Gate1"); return false; });
		ASSERT(!cb(123));
		ASSERT(a == 123);
	}

	{
		Gate2<int, int> cb;
		cb = lambda([&](int b, int c) { a = b + c; LOG("Gate2"); return true; });
		ASSERT(cb(1, 20));
		ASSERT(a == 21);
	}

	{
		Gate3<int, int, int> cb;
		cb = lambda([&](int b, int c, int d) { a = b + c + d; LOG("Gate3"); return true; });
		ASSERT(cb(1, 20, 300));
		ASSERT(a == 321);
	}
	
	{
		Gate4<int, int, int, int> cb;
		cb = lambda([&](int b, int c, int d, int e) { a = b + c + d + e; LOG("Gate4"); return true; });
		ASSERT(cb(1, 20, 300, 4000));
		ASSERT(a == 4321);
	}

	LOG("========== OK");
#endif
}

