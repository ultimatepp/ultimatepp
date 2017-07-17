#include "Core/Core.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

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
			h << in.GetLine();
		ASSERT(h.EndsWith("BODY>"));
	}
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
			h << in.GetLine();
		ASSERT(h.EndsWith("BODY>"));
		ASSERT(Filter(HttpRequest("www.ultimatepp.org").Execute(), [](int c) { return c == '\n' || c == '\r' ? 0 : c; }) == h);
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

	{	
		String data;
		for(int i = 0; i < 100000; i++)
			data << AsString(i) << ": " << AsString(Uuid::Create()) << '\n';
	
		String path = GetHomeDirFile("test.z");
		{
			FileOut fout(path);
			Zlib zlib;
			OutFilterStream out(fout, zlib);
			zlib.Compress();
			for(int i = 0; i < data.GetCount(); i++) {
				ASSERT(i == out.GetPos());
				out.Put(data[i]);
			}
			out.Close();
		}
	
		String data2;
		{
			FileIn fin(path);
			Zlib zlib;
			InFilterStream in(fin, zlib);
			zlib.Decompress();
			int i = 0;
			while(!in.IsEof()) {
				ASSERT(i == in.GetPos());
				data2.Cat(in.Get());
				i++;
			}
		}
		
		ASSERT(data == data2);
	}
	
	{
		String data;
		for(int i = 0; i < 10000; i++)
			data << AsString(i) << ": " << AsString(Uuid::Create()) << '\n';
	
		String path = GetHomeDirFile("test.z");
		{
			FileOut fout(path);
			int64 sz_pos = fout.GetPos();
			fout.Put64(0);
			Zlib zlib;
			OutFilterStream out(fout, zlib);
			zlib.Compress();
			out % data;
			int64 sz = out.GetPos();
			out.Close();
			fout.Seek(sz_pos);
			fout.Put64(sz);
		}
	
		String data2;
		{
			FileIn fin(path);
			fin.Get64();
			Zlib zlib;
			InFilterStream in(fin, zlib);
			zlib.Decompress();
			in % data2;
		}
		
		ASSERT(data == data2);
	}
	
	LOG("=========== OK");
}
