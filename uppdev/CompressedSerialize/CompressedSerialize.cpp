#include <Core/Core.h>
#include <plugin/lz4/lz4.h>

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
		Lz4 zlib;
		OutFilterStream out(fout, zlib);
		zlib.Compress();
		out % h;
		int ob = out.GetPos();
		out.Close();
		fout.Seek(pos);
		fout.Put64le(out.GetPos());
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
		Lz4 zlib;
		InFilterStream in(fin, zlib);
		in.SetSize(sz);
		zlib.Decompress();
		in % h2;
		DUMP(h2);
	}
}
