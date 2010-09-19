#include "Protect.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#include <unistd.h>

static bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access)
{
	// round start and size to page size -- needed for mprotect
	long pSize = sysconf(_SC_PAGESIZE );
	long delta =  (long)start % pSize;
	if(delta)
	{
		start -= delta;
		size += delta;
	}
	return mprotect(start, size, access ? PROT_READ | PROT_WRITE | PROT_EXEC : PROT_READ | PROT_EXEC) == 0;
}
#else

static bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access)
{
	dword oldProt;
	bool res = VirtualProtect(start, size, access ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ, &oldProt);
	
	return res;
}
#endif

bool PROTECT_DECRYPT(byte *start, size_t size, String const &key)
{
	RC4 rc4(key);

	if(!PROTECT_WRITE_ACCESS(start, size, true))
		return false;
	rc4.Crypt(start, start, size);
	return PROTECT_WRITE_ACCESS(start, size, false);
}
