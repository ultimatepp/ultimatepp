#include <Core/Core.h>
#include <plugin/lz4/lz4.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	String h = LZ4Compress(String());
	h = LZ4Decompress(h);
	ASSERT(h.GetCount() == 0);
	LOG("=============== OK");
}
