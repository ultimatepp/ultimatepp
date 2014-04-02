#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ASSERT(DecodeQuotedPrintable("=3DTest=\r\n=3D") == "Test");
}
