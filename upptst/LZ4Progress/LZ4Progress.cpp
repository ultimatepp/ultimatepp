#include <Core/Core.h>
#include <plugin/lz4/lz4.h>
#include <plugin/zstd/zstd.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	String data = LoadFile("C:/xxx/xxx/upp-x11-src-9041.tar");
	
	Gate<int64, int64> cp, dp;
	cp << [](int64 pos, int64 size) -> bool { DLOG("Compress: " << pos << " " << size); return false; };
	dp << [](int64 pos, int64 size) -> bool { DLOG("DeCompress: " << pos << " " << size); return false; };

	{
		String c = ZstdCompress(data, cp);
		DDUMP(c.GetCount());
		String d = ZstdDecompress(c, dp);
		ASSERT(data == d);
	}

	{
		String c = CoZstdCompress(data, cp);
		DDUMP(c.GetCount());
		String d = CoZstdDecompress(c, dp);
		ASSERT(data == d);
	}
	
	{
		String c = LZ4Compress(data, cp);
		DDUMP(c.GetCount());
		String d = LZ4Decompress(c, dp);
		ASSERT(data == d);
	}

	{
		String c = CoLZ4Compress(data, cp);
		DDUMP(c.GetCount());
		String d = CoLZ4Decompress(c, dp);
		ASSERT(data == d);
	}
}
