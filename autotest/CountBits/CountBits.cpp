#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	DUMP(CountBits(0x0001));
	DUMP(CountBits(0x000f));
	DUMP(CountBits(0x00ff));
	DUMP(CountBits(0x0fff));
	DUMP(CountBits(0xffff));
	DUMP(CountBits(0x000fffff));
	DUMP(CountBits(0x00ffffff));
	DUMP(CountBits(0x0fffffff));
	DUMP(CountBits(0xffffffff));
	
	
	LOG("====================");
	
	DUMP(CountBits64(0x00000001ffffffff));
	DUMP(CountBits64(0x0000000fffffffff));
	DUMP(CountBits64(0x000000ffffffffff));
	DUMP(CountBits64(0x00000fffffffffff));
	DUMP(CountBits64(0x0000ffffffffffff));
	DUMP(CountBits64(0x000fffffffffffff));
	DUMP(CountBits64(0x00ffffffffffffff));
	DUMP(CountBits64(0x0fffffffffffffff));
	DUMP(CountBits64(0xffffffffffffffff));
	
	CheckLogEtalon();
}
