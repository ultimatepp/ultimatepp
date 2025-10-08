#include "SSL.h"

#define LLOG(x) // DLOG(x)

namespace Upp {

#ifdef UPP_HEAP

static int64 UPP_SSL_alloc = 0;

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static void *SslAlloc(size_t size, const char *file, int line)
#else
static void *SslAlloc(size_t size)
#endif
{
	size_t alloc = size + sizeof(int64);
	int64 *aptr = (int64 *)MemoryAllocSz(alloc);
	*aptr++ = (int64)alloc;
	UPP_SSL_alloc += alloc;
	LLOG("UPP_SSL_MALLOC(" << (int64)size << ", size " << size
         << ", " << file << " " << line
	     << ") -> " << aptr << ", MemorySize: " << GetMemoryBlockSize(aptr)
	     << ", total = " << UPP_SSL_alloc << ", thread: " << Thread::GetCurrentId());
	return aptr;
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static void SslFree(void *ptr, const char *file, int line)
#else
static void SslFree(void *ptr)
#endif
{
	if(!ptr)
		return;
	int64 *aptr = (int64 *)ptr - 1;
	UPP_SSL_alloc -= *aptr;
	LLOG("UPP_SSL_FREE(" << ptr << ", size " << *aptr
	                     << ", " << file << " " << line
	                     << "), MemorySize: " << GetMemoryBlockSize(aptr) << ", total = " << UPP_SSL_alloc << ", thread: " << Thread::GetCurrentId());
	if(*aptr !=  GetMemoryBlockSize(aptr))
		Panic("SslFree corrupted");
	MemoryFree(aptr);
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static void *SslRealloc(void *ptr, size_t size, const char *file, int line)
#else
static void *SslRealloc(void *ptr, size_t size)
#endif
{
	LLOG("SslRealloc " << ptr << ", " << size);
	if(!ptr) {
		LLOG("UPP_SSL_REALLOC by Alloc:");
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		return SslAlloc(size, file, line);
#else
		return SslAlloc(size);
#endif
	}
	int64 *aptr = (int64 *)ptr - 1;
	if((int64)(size + sizeof(int64)) <= *aptr) { // TODO: Do we really want this?
		LLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << *aptr << ") -> keep same block" << ", thread: " << Thread::GetCurrentId());
		return ptr;
	}
	size_t newalloc = size + sizeof(int64);
	int64 *newaptr = (int64 *)MemoryAllocSz(newalloc);
	if(!newaptr) {
		LLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << newalloc << ") -> fail" << ", thread: " << Thread::GetCurrentId());
		return NULL;
	}
	*newaptr++ = newalloc;
	memcpy(newaptr, ptr, min<int>((int)(*aptr - sizeof(int64)), (int)size));
	UPP_SSL_alloc += newalloc - *aptr;
	LLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int64)size << ", alloc " << newalloc
	     << ", " << file << " " << line
	     << ") -> " << newaptr << ", total = " << UPP_SSL_alloc << ", thread: " << Thread::GetCurrentId());
	MemoryFree(aptr);
	return newaptr;
}

#endif

void TcpSocketInit();

INITIALIZER(SSL)
{
	MemoryIgnoreLeaksBlock __;
	LLOG("SslInit");
	TcpSocketInit();
#ifdef UPP_HEAP
	CRYPTO_set_mem_functions(SslAlloc, SslRealloc, SslFree);
#endif
	SSL_library_init();
	SSL_load_error_strings();
}

EXITBLOCK
{
	MemoryIgnoreLeaksBlock __;
	CONF_modules_unload(1);
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
#if OPENSSL_VERSION_NUMBER < 0x10100000L
	STACK_OF(SSL_COMP) *pCOMP = SSL_COMP_get_compression_methods();
	if(pCOMP)
		sk_SSL_COMP_free( pCOMP );
	ERR_remove_state(0);
#else
//	ERR_remove_thread_state(NULL);
	OPENSSL_thread_stop();
#endif
	ERR_free_strings();
}


static thread_local bool sThreadInit;
static thread_local void (*sPrevExit)();

static void sslExitThread()
{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
	ERR_remove_state(0);
#else
//	ERR_remove_thread_state(NULL);
	OPENSSL_thread_stop();
#endif
	if(sPrevExit)
		(*sPrevExit)();
}

void SslInitThread()
{
	MemoryIgnoreLeaksBlock __;
	if(sThreadInit || Thread::IsMain())
		return;
	sThreadInit = true;
	sPrevExit = Thread::AtExit(sslExitThread);
}

}
