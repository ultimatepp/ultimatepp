#include <Core/Core.h>
#include <plugin/lz4/lz4.h>
#include <plugin/zstd/zstd.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	String cpath = GetHomeDirFile("test");
	
	{
		FileOut out(cpath);
		ZCompressStream z(out);
		z.Put("Compressed by zlib");
	}
	
	{
		FileIn in(cpath);
		ZDecompressStream z(in);
		RLOG(z.GetLine());
	}

	{
		FileOut out(cpath);
		LZ4CompressStream z(out);
		z.Put("Compressed by LZ4");
	}
	
	{
		FileIn in(cpath);
		LZ4DecompressStream z(in);
		RLOG(z.GetLine());
	}

	{
		FileOut out(cpath);
		ZstdCompressStream z(out);
		z.Put("Compressed by Zstd");
	}
	
	{
		FileIn in(cpath);
		ZstdDecompressStream z(in);
		RLOG(z.GetLine());
	}
	
	FileDelete(cpath);
}
