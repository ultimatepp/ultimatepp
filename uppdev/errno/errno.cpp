#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	FileIn in("34523");
	DUMP(errno);
	DUMP(strerror(errno));
	char h[100];
	DUMP(strerror_r(errno, h, 100));
	DUMP(h);
	DUMP(GetLastErrorMessage());
}

