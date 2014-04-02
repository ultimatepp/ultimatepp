#include <Core/Core.h>

using namespace Upp;

#define CHECK_OVERFLOW(s, cls, overflow) { cls x; Value v = x.Scan(s); LOG(s << " -> " << v); ASSERT(v.IsError() == overflow); }

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	CHECK_OVERFLOW("2147483647", ConvertInt, false);
	CHECK_OVERFLOW("2147483648", ConvertInt, true);

	CHECK_OVERFLOW("-2147483647", ConvertInt, false);
	CHECK_OVERFLOW("-2147483648", ConvertInt, true);
}
