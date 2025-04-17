#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	for(int i = 0; i < 32; i++)
		DLOG(i << " " << CountTrailingZeroBits(1 << i));
	DDUMP(CountTrailingZeroBits(0x78));
	DDUMP(CountTrailingZeroBits(0xf000));
	DDUMP(CountTrailingZeroBits(0xf0000000));

	for(int i = 0; i < 64; i++)
		DLOG(i << " " << CountTrailingZeroBits64((uint64)1 << i));
	DDUMP(CountTrailingZeroBits64(0x78));
	DDUMP(CountTrailingZeroBits64(0xf000));
	DDUMP(CountTrailingZeroBits64(0xf0000000));
	DDUMP(CountTrailingZeroBits64(0xf0000000f0000000));
	DDUMP(CountTrailingZeroBits64(0xf000000000000000));
	
	CheckLogEtalon();
}
