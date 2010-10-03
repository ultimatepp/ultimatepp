#include "Protect.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#include <unistd.h>

bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access)
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

bool PROTECT_WRITE_ACCESS(byte *start, size_t size, bool access)
{
	dword oldProt;
	bool res = VirtualProtect(start, size, access ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ, &oldProt);
	
	return res;
}
#endif

void PROTECT_DECRYPT(byte *start, size_t size, String const &key, byte const *nonce, size_t nonceLen)
{
	Snow2 snow2((byte const *)~key, key.GetCount(), nonce, nonceLen);

	snow2(start, size);
}

void PROTECT_OBFUSCATE(byte *start, size_t len, byte *key, size_t keyLen)
{
	String k;
	for(unsigned i = 0; i < keyLen; i++)
		k += *key++;
	if(!PROTECT_WRITE_ACCESS(start, len, true))
		return;
	Snow2 snow2(k, "12345678");
	snow2(start, len);
	PROTECT_WRITE_ACCESS(start, len, false);
}
