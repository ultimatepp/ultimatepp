#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String path = GetHomeDirFile("test-filetime-get-set.test");
	
	SaveFile(path, "xxx");
	
	FileTime tm1;
	tm1 = GetFileTime(path);
	DDUMP(Time(tm1));
	SetFileTime(path, tm1);
	FileTime tm2 = GetFileTime(path);
	DDUMP(Time(tm2));
	ASSERT(tm1 == tm2);

	Time m1 = FileGetTime(path);
	DDUMP(m1);
	DDUMP(FileGetTime(path));
	FileSetTime(path, FileGetTime(path));
	Time m2 = FileGetTime(path);
	DDUMP(m2);
	ASSERT(m1 == m2);

	String path2 = GetHomeDirFile("test-filetime-get-set-tgt.test");
	
	FileCopy(path, path2);

	FileTime tm3 = GetFileTime(path2);

	DDUMP(Time(tm3));
	
	ASSERT(tm3 == tm1);
	
	FileDelete(path);
	FileDelete(path2);
	
	DLOG("============= OK");
}
