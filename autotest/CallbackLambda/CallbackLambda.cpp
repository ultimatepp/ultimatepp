#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	int a = 0;

	{
		Event<> cb;
		cb << [&] { a = 1; LOG("Event"); };
		cb();
		ASSERT(a == 1);
	}

	{
		Event<> cb;
		cb << [&] { a = 1; LOG("Event"); };
		cb();
		ASSERT(a == 1);
	}

	{
		Event<int> cb;
		cb << [&](int b) { a = b; LOG("Event1"); };
		cb(123);
		ASSERT(a == 123);
	}

	{
		Event<int> cb;
		cb << [&](int b) { a = b; LOG("Event1"); };
		cb(123);
		ASSERT(a == 123);
	}

	{
		Event<int, int> cb;
		cb << [&](int b, int c) { a = b + c; LOG("Event2"); };
		cb(1, 20);
		ASSERT(a == 21);
	}

	{
		Event<int, int, int> cb;
		cb << [&](int b, int c, int d) { a = b + c + d; LOG("Event3"); };
		cb(1, 20, 300);
		ASSERT(a == 321);
	}
	
	{
		Event<int, int, int, int> cb;
		cb << [&](int b, int c, int d, int e) { a = b + c + d + e; LOG("Event4"); };
		cb(1, 20, 300, 4000);
		ASSERT(a == 4321);
	}

	{
		Gate<> cb;
		cb << [&] { a = 1; LOG("EventGate"); return true; };
		ASSERT(cb());
		ASSERT(a == 1);
	}
	
	{
		Gate<int> cb;
		cb << [&](int b) { a = b; LOG("EventGate1"); return false; };
		ASSERT(!cb(123));
		ASSERT(a == 123);
	}

	{
		Gate<int, int> cb;
		cb << [&](int b, int c) { a = b + c; LOG("EventGate2"); return true; };
		ASSERT(cb(1, 20));
		ASSERT(a == 21);
	}

	{
		Gate<int, int, int> cb;
		cb << [&](int b, int c, int d) { a = b + c + d; LOG("EventGate3"); return true; };
		ASSERT(cb(1, 20, 300));
		ASSERT(a == 321);
	}
	
	{
		Gate<int, int, int, int> cb;
		cb << [&](int b, int c, int d, int e) { a = b + c + d + e; LOG("EventGate4"); return true; };
		ASSERT(cb(1, 20, 300, 4000));
		ASSERT(a == 4321);
	}

	LOG("========== OK");
}

