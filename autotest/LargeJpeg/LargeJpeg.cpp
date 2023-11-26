#include <Core/Core.h>
#include <plugin/jpg/jpg.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Image m = StreamRaster::LoadFileAny(GetDataFile("sample.jpg"));
	DDUMP(m.GetSize());
	ASSERT(m.GetSize() == Size(1920, 1080));

	// if this does not work, set #define DEFAULT_MAX_MEM 100000000L
}
