#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	void *ptr = MemoryAlloc(400);
	DDUMP(TryRealloc(ptr, 420));
	DDUMP(TryRealloc(ptr, 1000));
	
	MemoryFree(ptr);
	
	ptr = MemoryAlloc(2000);
	DDUMP(TryRealloc(ptr, 4000));
	DDUMP(TryRealloc(ptr, 200));
	DDUMP(TryRealloc(ptr, 200000));
	
	MemoryFree(ptr);
}
