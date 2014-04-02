#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ASSERT(QPDecode("=3DTest=\r\n=3D") == "=Test=");
}
