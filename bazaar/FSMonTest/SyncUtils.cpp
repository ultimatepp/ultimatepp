#include "SyncUtils.h"

// synchronize a source file with a dest one
// checks filestamp, size and attributes
bool SyncFile(String const &src, String const &dst)
{
	Time srcTime;
	
	if(src == dst)
		return false;
	if(!FileExists(src))
	{
		if(FileExists(dst))
		{
			FileDelete(dst);
			return true;
		}
		return false;
	}
	bool shallCopy = false;
	if(!FileExists(dst))
		shallCopy = true;
	else
	{
		srcTime = FileGetTime(src);
		int64 srcLen = GetFileLength(src);
	
		Time dstTime = FileGetTime(dst);
		int64 dstLen = GetFileLength(dst);
		
		if(srcTime != dstTime || srcLen != dstLen)
			shallCopy = true;
	}
	if(shallCopy)
	{
		if(!RealizePath(dst))
			return false;
		if(FileCopy(src, dst))
			return FileSetTime(dst, srcTime);
		else
			return false;
	}
	return true;
}

// synchronizes a whole source folder with a dest one
// uses SyncFile for single files synchronization
bool SyncFolder(String const &src, String const &dst)
{
	bool res = true;
	
#ifdef PLATFORM_POSIX
	FindFile ff(AppendFileName(src, "*"));
#else
	FindFile ff(AppendFileName(src, "*.*"));
#endif
	
	if(!DirectoryExists(dst))
	{
		if(!RealizeDirectory(dst))
			return false;
	}
	while(ff)
	{
		// skip . and .. folders...
		if(ff.GetName() == "." || ff.GetName() == "..")
		{
			ff.Next();
			continue;
		}
		String srcPath = AppendFileName(src, ff.GetName());
		String dstPath = AppendFileName(dst, ff.GetName());
		if(ff.IsFolder())
			res &= SyncFolder(srcPath, dstPath);
		else
			res &= SyncFile(srcPath, dstPath);
		ff.Next();
	}
	return res;
}
