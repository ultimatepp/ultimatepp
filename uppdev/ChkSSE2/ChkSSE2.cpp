#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(CPU_MMX());
	DUMP(CPU_SSE());
	DUMP(CPU_SSE2());
	DUMP(CPU_SSE3());

	ReadMemoryBarrier();
}
