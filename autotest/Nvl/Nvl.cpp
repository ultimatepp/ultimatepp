#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	int x = Null;
	
	ASSERT(Nvl(1, 2, 3, 4, 5) == 1);
	ASSERT(Nvl(x, 2, 3, 4, 5) == 2);
	ASSERT(Nvl(x, x, 3, 4, 5) == 3);
	ASSERT(Nvl(x, x, x, 4, 5) == 4);
	ASSERT(Nvl(x, x, x, x, 5) == 5);
	ASSERT(Nvl(1, x, 3, x, 5) == 1);
	ASSERT(Nvl(x, x, 3, x, 5) == 3);
	ASSERT(IsNull(Nvl(x, x, x, x, x)));

	ASSERT(Nvl(1, 2, 3, 4) == 1);
	ASSERT(Nvl(x, 2, 3, 4) == 2);
	ASSERT(Nvl(x, x, 3, 4) == 3);
	ASSERT(Nvl(x, x, x, 4) == 4);
	ASSERT(Nvl(1, x, 3, x) == 1);
	ASSERT(Nvl(x, x, 3, x) == 3);
	ASSERT(IsNull(Nvl(x, x, x, x)));

	ASSERT(Nvl(1, 2, 3) == 1);
	ASSERT(Nvl(x, 2, 3) == 2);
	ASSERT(Nvl(x, x, 3) == 3);
	ASSERT(Nvl(1, x, 3) == 1);
	ASSERT(Nvl(x, x, 3) == 3);
	ASSERT(IsNull(Nvl(x, x, x)));
}
