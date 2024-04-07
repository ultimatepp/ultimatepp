#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	String testdir = GetHomeDirFile("path_test_dir__");
	DirectoryCreate(testdir);
	ASSERT(DirectoryExists(testdir));
	ASSERT(!FileExists(testdir));
	ASSERT(!DirectoryExists(testdir + "q24312"));
	
	String path = AppendFileName(testdir, "testík.txt");
	Time tm0 = GetSysTime();
	SaveFile(path, "test");
	ASSERT(FileExists(path));
	ASSERT(!DirectoryExists(path));
	ASSERT(!FileExists(path + "1"));
	
	ASSERT(GetFileLength(path) == 4);
	ASSERT(GetFileLength(path + "1") < 0);
	
	Time tm = GetFileTime(path);
	DDUMP(tm);
	ASSERT(tm == FileGetTime(path));
	
	ASSERT(tm >= tm0 && tm <= GetSysTime());

	String newpath = AppendFileName(testdir, "test2.txt");
	ASSERT(FileMove(path, newpath));
	ASSERT(FileExists(newpath));
	ASSERT(!FileExists(path));
	ASSERT(!DirectoryExists(newpath));
	
	DeleteFolderDeep(testdir);
	ASSERT(!DirectoryExists(testdir));
	
	LOG("================== OK");
}
