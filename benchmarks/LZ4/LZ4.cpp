#include <Core/Core.h>
#include <plugin/lz4/lz4.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TimeStop tm;
	String s = LoadFile(GetHomeDirFile("testdata/c12_OCOE_env.blb"));
	
	RLOG("Loaded in " << tm << " s");
	
	s.Cat(s);
	
	RLOG("Original size " << s.GetCount() / 1024 / 1024 << " MB");
	
	{
		TimeStop tm;
		String c = LZ4Compress(s);
		RLOG("Compresss " << s.GetLength() / tm.Seconds() / 1024/1024 << " MB / s");
		RLOG("Compressed size " << c.GetCount() / 1024 / 1024 << " MB (" << (100.0 * c.GetCount() / s.GetCount()) << "%)");
		tm.Reset();
		String d = LZ4Decompress(c);
		RLOG("DeCompresss " << s.GetLength() / tm.Seconds() / 1024/1024 << " MB / s");
		ASSERT(s == d);
	}
	{
		TimeStop tm;
		String c = CoLZ4Compress(s);
		RLOG("CoCompresss " << s.GetLength() / tm.Seconds() / 1024/1024 << " MB / s");
		RLOG("Compressed size " << c.GetCount() / 1024 / 1024 << " MB (" << (100.0 * c.GetCount() / s.GetCount()) << "%)");
		tm.Reset();
		String d = CoLZ4Decompress(c);
		RLOG("CoDeCompresss " << s.GetLength() / tm.Seconds() / 1024/1024 << " MB / s");
		ASSERT(s == d);
	}
}
