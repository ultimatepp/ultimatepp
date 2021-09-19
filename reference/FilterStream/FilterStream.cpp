#include <Core/Core.h>

using namespace Upp;

String data;

void AnythingPut(const void *ptr, int count)
{
	data.Cat((const char *)ptr, count);	
}

void AnythingEnd()
{
	LOG("----- STREAM CLOSED");
}

struct ProgressFilter { // Simple filter example just prints amount of data processed
	Event<const void *, int> WhenOut;
	void Put(const void *ptr, int size) {
		DLOG("#### Loaded " << size << " bytes");
		WhenOut(ptr, size);
	}
	void End() {}
};

CONSOLE_APP_MAIN
{
	String path = GetHomeDirFile("test.gz");
	{
		FileOut fout(path);
		Zlib zlib;
		OutFilterStream out(fout, zlib);
		zlib.GZip().Compress();
		out.Put("Hello world!");
		out.Close();
	}

	{
		FileIn fin(path);
		Zlib zlib;
		InFilterStream in(fin, zlib);
		zlib.GZip().Decompress();
		LOG(in.GetLine());
	}
	
	{
		FileIn fin(GetDataFile("FilterStream.cpp"));
		ProgressFilter pf;
		InFilterStream in(fin, pf);
		in.SetBufferSize(64); // Set it small to demonstrate the example
		LOG("==================================");
		while(!in.IsEof())
			LOG(in.GetLine());
	}

	{ // In this case we are using InFilterStream without input stream to represent HttpRequest as input stream
		HttpRequest http("www.ultimatepp.org");
		InFilterStream in;
		http.WhenContent = callback(&in, &InFilterStream::Out);
		in.More = callback(&http, &HttpRequest::Do);
		http.Blocking();
		LOG("==================================");
		while(!in.IsEof())
			LOG(in.GetLine());
	}

	{ // In this case we are using OutFilterStream without out stream to represent anything with Put as stream
		OutFilterStream out;
		out.Filter = callback(AnythingPut);
		out.End = callback(AnythingEnd);
		out << "Hello world!";
	}
	LOG(data);
}
