#include "Core/Core.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	{
		HttpRequest http("www.ultimatepp.org");
		InFilterStream in;
		http.WhenContent = callback(&in, &InFilterStream::Out);
		in.More = callback(&http, &HttpRequest::Do);
		http.Blocking();
		ASSERT(!in.IsEof());
		String h;
		h = in.GetLine();
		ASSERT(h.StartsWith("<!DOCTYPE"));
		while(!in.IsEof())
			h = in.GetLine();
		ASSERT(h.EndsWith("BODY>"));
	}
	String path = GetHomeDirFile("test.gz");
	{
		FileOut fout(path);
		Zlib zlib;
		OutFilterStream out(fout, zlib);
		zlib.GZip().Compress();
		for(int i = 0; i < 100000; i++)
			out.Put(FormatIntBase(i, 27));
		out.Close();
	}

	String data;
	for(int i = 0; i < 100000; i++)
		data.Cat(FormatIntBase(i, 27));
	SaveFile(path + ".1", GZCompress(data));
#if 0
	SaveFile(path, GZCompress(data));
#endif
	for(int pass = 0; pass < 2; pass++) {	
		FileIn fin(path);
		Zlib zlib;
		InFilterStream in(fin, zlib);
		zlib.GZip().Decompress();
		if(pass)
			for(int i = 0; i < data.GetCount(); i++) {
				char c = in.Get();
				ASSERT(c == data[i]);
			}
		else
			for(int i = 0; i < data.GetCount(); i += 17) {
				int n = min(data.GetCount() - i, 17);
				String h = in.Get(n);
				ASSERT(h == data.Mid(i, n));
			}
		ASSERT(in.Get() < 0);
		ASSERT(in.IsEof());	
	}
}
