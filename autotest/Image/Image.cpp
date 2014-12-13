#include <Draw/Draw.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ASSERT(IsNull(StreamRaster::LoadFileAny("invalid_file_name.asldfasdf")));
}
