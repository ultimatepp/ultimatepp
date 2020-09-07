#include "SSH.h"

namespace Upp {
	
#define MLOG(x)  //  LOG(x)

#ifdef UPP_HEAP

static std::atomic<int64> UPP_SSH_alloc;

static void *ssh_malloc(size_t size, void **abstract)
{
	MLOG("Alloc " << size);
	size_t alloc = size + sizeof(int64);
	int64 *aptr = (int64 *)MemoryAllocSz(alloc);
	*aptr++ = (int64)alloc;
	UPP_SSH_alloc += alloc;
	MLOG("UPP_SSH_MALLOC(" << (int64)size << ", alloc " << alloc << ") -> " << FormatIntHex(aptr) << ", total = " << (int64) UPP_SSH_alloc);
	return aptr;
}

static void ssh_free(void *ptr, void **abstract)
{
	if(!ptr)
		return;
	int64 *aptr = (int64 *)ptr - 1;
	UPP_SSH_alloc -= *aptr;
	MLOG("UPP_SSH_FREE(" << ptr << ", alloc " << *aptr << "), total = " << (int64) UPP_SSH_alloc);
	MemoryFree(aptr);
}

static void *ssh_realloc(void *ptr, size_t size, void** abstract)
{
	if(!ptr)
		return NULL;
	int64 *aptr = (int64 *)ptr - 1;
	if((int64)(size + sizeof(int64)) <= *aptr) {
		MLOG("UPP_SSH_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << *aptr << ") -> keep same block");
		return ptr;
	}
	size_t newalloc = size + sizeof(int64);
	int64 *newaptr = (int64 *)MemoryAllocSz(newalloc);
	if(!newaptr) {
		MLOG("UPP_SSH_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << newalloc << ") -> fail");
		return NULL;
	}
	*newaptr++ = newalloc;
	memcpy(newaptr, ptr, min<int>((int)(*aptr - sizeof(int64)), (int)size));
	UPP_SSH_alloc += newalloc - *aptr;
	MLOG("UPP_SSH_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << newalloc << ") -> "
		<< FormatIntHex(newaptr) << ", total = " << (int64) UPP_SSH_alloc);
	MemoryFree(aptr);
	return newaptr;
}

#endif
}