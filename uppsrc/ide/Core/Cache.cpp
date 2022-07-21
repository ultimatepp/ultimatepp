#include "Core.h"

String CacheDir()
{
#ifdef PLATFORM_WIN32
	return ConfigFile("cache");
#else
	return ConfigFile(".cache/upp.cache");
#endif
}

String CacheFile(const char *name)
{
	return AppendFileName(CacheDir(), name);
}

void ReduceCache(int mb_limit)
{
}