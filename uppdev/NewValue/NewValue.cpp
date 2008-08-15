#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(sizeof(RichValueRep<int>));
	DUMP(sizeof(RichValueRep<double>));
	DUMP(sizeof(RichValueRep<Date>));
	DUMP(sizeof(RichValueRep<Time>));
}
