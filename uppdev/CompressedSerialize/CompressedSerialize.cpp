#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String h = "This is some line that is to be compressed";
	h << h;
	h << h;
	h << h;
	h << h;
	DUMP(h);
	String path = "c:/xxx/test.zc";

	{
		FileOut fout(path);
		fout.Put("COMPRESSED");
		int64 pos = fout.GetPos();
		
		fout.Put64le(pos);
		Zlib zlib;
		OutFilterStream out(fout, zlib);
		zlib.Compress();
		out % h;
		fout.Seek(pos);
		DDUMP(out.GetInCount());
		DDUMP(out.GetCount());
		fout.Put64le(out.GetInCount());
		out.Close();
		DDUMP(out.GetCount());
		DDUMP(out.GetInCount());
	}
	

	{
		String h2;
		FileIn fin(path);
		if(fin.GetSize() < 14)
			return;
		String hdr = fin.Get(10);
		if(hdr != "COMPRESSED")
			return;
		int sz = fin.Get64le();
		DDUMP(sz);
		Zlib zlib;
		InFilterStream in(fin, zlib);
		in.SetSize(sz);
		zlib.Decompress();
		in % h2;
		DUMP(h2);
	}
}
