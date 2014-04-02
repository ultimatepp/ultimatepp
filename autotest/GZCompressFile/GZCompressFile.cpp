#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String dir1 = GetHomeDirFile("gztest1");
	DeleteFolderDeep(dir1);
	RealizeDirectory(dir1);
	String file1 = AppendFileName(dir1, "test.cpp");
	String gzfile1 = AppendFileName(dir1, "test.cpp.gz");
	
	String dir2 = GetHomeDirFile("gztest2");
	DeleteFolderDeep(dir2);
	RealizeDirectory(dir2);
	String file2 = AppendFileName(dir2, "test.cpp");
	String gzfile2 = AppendFileName(dir2, "test.cpp.gz");
	
	SaveFile(file1, LoadFile(GetDataFile("GZCompressFile.cpp")));
	GZCompressFile(file1);
	
	SaveFile(gzfile2, LoadFile(gzfile1));
	GZDecompressFile(gzfile2);
	
	ASSERT(LoadFile(file1) == LoadFile(file2));
}
