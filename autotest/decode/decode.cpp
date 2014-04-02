#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ASSERT(decode(0, 1, "one", 2, "two", 3, "three", "unknown") == String("unknown"));
	ASSERT(decode(1, 1, "one", 2, "two", 3, "three", "unknown") == String("one"));
	ASSERT(decode(2, 1, "one", 2, "two", 3, "three", "unknown") == String("two"));
	ASSERT(decode(3, 1, "one", 2, "two", 3, "three", "unknown") == String("three"));
	ASSERT(decode(3, 1, "one", 2, "two", 3.0, "three", "unknown") == String("three"));
	ASSERT(decode(4, 1, "one", 2, "two", 3, "three", "unknown") == String("unknown"));

	String s = "3";
	ASSERT(findarg(s, "0", "1", "2", "3") == 3);
	
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
}
