#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = -1; i < 64; i++) {
		uint64 v = i >= 0 ? (uint64)1 << i : 0;
		LOG("===============");
		DUMPHEX(v);
		DUMP(SignificantBits64(v));
		DUMP(SignificantBits(v));
		if(i < 32)
			ASSERT(SignificantBits((dword)v) == i + 1);
		ASSERT(SignificantBits64(v) == i + 1);
	}
	
	LOG("================= OK");
}
