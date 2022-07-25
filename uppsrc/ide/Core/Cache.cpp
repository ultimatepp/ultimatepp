#include "Core.h"

String CacheDir()
{
	String dir;
#ifdef PLATFORM_WIN32
	dir = ConfigFile("cache");
#else
	dir = ConfigFile(".cache/upp.cache");
#endif
	ONCELOCK {
		RealizeDirectory(dir);
	}
	return dir;
}

String CacheFile(const char *name)
{
	return AppendFileName(CacheDir(), name);
}

void ReduceCache(int mb_limit)
{
	// TODO
}