#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String n = " 2";
	enum { A = 0, B = 12 };
	
	ASSERT(decode(0, 1, "one", 2, "two", 3, "three", "unknown") == String("unknown"));
	ASSERT(decode(1, 1, "one", 2, "two", 3, "three", "unknown") == String("one"));
	ASSERT(decode(2, 1, "one", 2, "two", 3, "three", "unknown") == String("two"));
	ASSERT(decode(3, 1, "one", 2, "two", 3, "three", "unknown") == String("three"));
	ASSERT(decode(3, 1, "one", 2, "two", 3.0, "three", "unknown") == String("three"));
	ASSERT(decode(4, 1, "one", 2, "two", 3, "three", "unknown") == String("unknown"));

	ASSERT(decode(1, 1, "one" + n, 2, "two", 3, "three", "unknown" + n) == String("one 2"));
	ASSERT(decode(2, 1, "one", 2, "two" + n, 3, "three", "unknown" + n) == String("two 2"));
	ASSERT(decode(3, 1, "one", 2, "two", 3, "three" + n, "unknown" + n) == String("three 2"));
	ASSERT(decode(4, 1, "one", 2, "two", 3, "three", "unknown" + n) == String("unknown 2"));

	ASSERT(decode(2, 1, "one" + n, 2, "two", 3, "three", "unknown") == String("two"));
	ASSERT(decode(3, 1, "one", 2, "two" + n, 3, "three", "unknown") == String("three"));
	ASSERT(decode(4, 1, "one", 2, "two", 3, "three" + n, "unknown") == String("unknown"));
	ASSERT(decode(1, 1, "one", 2, "two", 3, "three", "unknown") == String("one"));
	
	ASSERT(decode(0, 0, A, 1, 33, B) == 0);
	ASSERT(decode(A, 0, A, 1, 33, B) == 0);
	ASSERT(decode(200, 0, A, 1, 33, B) == 12);

	ASSERT(decode(0, 1, 11, 2, 12, 3, 13, 14) == 14);
	ASSERT(decode(1, 1, 11, 2, 12, 3, 13, 14) == 11);
	ASSERT(decode(2, 1, 11, 2, 12, 3, 13, 14) == 12);
	ASSERT(decode(3, 1, 11, 2, 12, 3, 13, 14) == 13);
	ASSERT(decode(3, 1, 11, 2, 12, 3.0, 13, 14) == 13);
	ASSERT(decode(4, 1, 11, 2, 12, 3, 13, 14) == 14);

	ASSERT(get_i(-1, "one", "two", "three") == String("one"));
	ASSERT(get_i(0, "one", "two", "three") == String("one"));
	ASSERT(get_i(1, "one", "two", "three") == String("two"));
	ASSERT(get_i(2, "one", "two", "three") == String("three"));
	ASSERT(get_i(3, "one", "two", "three") == String("three"));
	ASSERT(get_i(30, "one", "two", "three") == String("three"));

	ASSERT(get_i(-1, "one", "two", "three") == String("one"));
	ASSERT(get_i(0, "one", "two", "three") == String("one"));
	ASSERT(get_i(1, "one", "two", "three") == String("two"));
	ASSERT(get_i(2, "one", "two", "three") == String("three"));
	ASSERT(get_i(3, "one", "two", "three") == String("three"));
	ASSERT(get_i(30, "one", "two", "three") == String("three"));

	ASSERT(get_i(0, "one" + n, "two") == String("one 2"));
	ASSERT(get_i(1, "one" + n, "two") == String("two"));

	ASSERT(get_i(0, "one", "two" + n) == String("one"));
	ASSERT(get_i(1, "one", "two" + n) == String("two 2"));
	
	ASSERT(get_i(0, A, B, 44) == 0);
	ASSERT(get_i(0, A, B, 44) == 0);

	ASSERT(get_i(-1, 1, 2, 3) == 1);
	ASSERT(get_i(0, 1, 2, 3) == 1);
	ASSERT(get_i(1, 1, 2, 3) == 2);
	ASSERT(get_i(2, 1, 2, 3) == 3);
	ASSERT(get_i(3, 1, 2, 3) == 3);
	ASSERT(get_i(30, 1, 2, 3) == 3);

	String s = "3";
	ASSERT(findarg(s, "0", "1", "2", "3") == 3);
	ASSERT(findarg("a", "0", "1", "2", "3") == -1);
	
	ASSERT(decode(s, "3", 3, -1) == 3);
	ASSERT(decode(s, "4", 3, -1) == -1);
	
	ASSERT(min(1, 5) == 1);
	ASSERT(min(5, 1) == 1);

	ASSERT(min(1, 5, 5) == 1);
	ASSERT(min(5, 1, 5) == 1);
	ASSERT(min(5, 5, 1) == 1);

	ASSERT(min(1, 5, 5, 5) == 1);
	ASSERT(min(5, 1, 5, 5) == 1);
	ASSERT(min(5, 5, 1, 5) == 1);
	ASSERT(min(5, 5, 5, 1) == 1);

	ASSERT(max(1, 0) == 1);
	ASSERT(max(0, 1) == 1);

	ASSERT(max(1, 0, 0) == 1);
	ASSERT(max(0, 1, 0) == 1);
	ASSERT(max(0, 0, 1) == 1);

	ASSERT(max(1, 0, 0, 0) == 1);
	ASSERT(max(0, 1, 0, 0) == 1);
	ASSERT(max(0, 0, 1, 0) == 1);
	ASSERT(max(0, 0, 0, 1) == 1);
	
	ASSERT(gather<Vector<String>>("1", "2", "3") == Vector<String>({ "1", "2", "3" }));
	
	int a, b, c;
	scatter(Vector<int>({11, 22, 33}), a, b, c);
	ASSERT(a == 11);
	ASSERT(b == 22);
	ASSERT(c == 33);
	
	LOG("=============== OK");
}
