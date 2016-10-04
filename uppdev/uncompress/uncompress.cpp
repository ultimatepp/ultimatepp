#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SaveFile("z:/zaci/5-2007-2.xml", ZDecompress(LoadFile("z:/zaci/5-2007-2.zml")));
}
