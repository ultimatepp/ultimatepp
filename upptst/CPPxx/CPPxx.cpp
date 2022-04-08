#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef CPP_20
	DLOG("CPP20");
#endif
#ifdef CPP_17
	DLOG("CPP17");
#endif
#ifdef CPP_14
	DLOG("CPP14");
#endif
#ifdef CPP_11
	DLOG("CPP11");
#endif
}
