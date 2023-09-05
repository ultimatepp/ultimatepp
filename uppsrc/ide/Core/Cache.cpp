#include "Core.h"

String CacheDir()
{
	String dir;
#ifdef _DEBUG
	#ifdef PLATFORM_WIN32
		dir = ConfigFile("cache.debug");
	#else
		dir = ConfigFile(".cache/upp.cache.debug");
	#endif
#else
	#ifdef PLATFORM_WIN32
		dir = ConfigFile("cache");
	#else
		dir = ConfigFile(".cache/upp.cache");
	#endif
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

void ReduceCacheFolder(const char *path, int64 max_total)
{
	struct RCB_FileInfo {
		String path;
		Time   time;
		int64  length;
		
		bool operator<(const RCB_FileInfo& a) const { return time > a.time; }
	};
	
	Array<RCB_FileInfo> file;
	FindFile ff(AppendFileName(path, "*.*"));
	int64 total = 0;
	while(ff) {
		if(ff.IsFile()) {
			RCB_FileInfo& m = file.Add();
			m.path = ff.GetPath();
			m.time = ff.GetLastAccessTime();
			m.length = ff.GetLength();
			total += m.length;
		}
		ff.Next();
	}
	Sort(file);
	while(total > max_total && file.GetCount()) {
		if(DeleteFile(file.Top().path))
			total -= file.Top().length;
		file.Drop();
	}
}

void ReduceCache()
{
	ReduceCacheFolder(CacheDir(), (int64)4096 * 1024 * 1024);
}
