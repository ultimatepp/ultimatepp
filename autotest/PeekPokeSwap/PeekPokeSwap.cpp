#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	byte h[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
	word v16 = Peek16le(h + 1);
	RDUMP(Format64Hex(v16));

	ASSERT(v16 == 0x3322);
	v16 = SwapEndian16(v16);
	RDUMP(Format64Hex(v16));
	ASSERT(v16 == 0x2233);
	dword v32 = Peek32le(h + 1);
	RDUMP(Format64Hex(v32));
	ASSERT(v32 == 0x55443322);
	v32 = SwapEndian32(v32);
	RDUMP(Format64Hex(v32));
	ASSERT(v32 == 0x22334455);
	uint64 v64 = Peek64le(h + 1);
	RDUMP(Format64Hex(v64));
	ASSERT(v64 == 0x9988776655443322);
	v64 = SwapEndian64(v64);
	RDUMP(Format64Hex(v64));
	ASSERT(v64 == 0x2233445566778899);
	
	RLOG("================ OK");
}
