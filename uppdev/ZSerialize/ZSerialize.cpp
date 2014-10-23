#include <Core/Core.h>

using namespace Upp;

bool ZLoad(Callback1<Stream&> serialize, Stream& stream, int version = Null)
{
	String h = stream.GetLine();
	if(h != "COMPRESSED")
		return false;
	int64 sz = stream.Get64();
	Zlib zlib;
	InFilterStream in(stream, zlib);
	zlib.Decompress();
	in.SetSize(sz);
	return Load(serialize, in, version);
}

bool ZStore(Callback1<Stream&> serialize, Stream& stream, int version = Null)
{
	stream.Put("COMPRESSED\n");
	int sz_pos = stream.GetPos();
	stream.Put64(0);
	Zlib zlib;
	OutFilterStream out(stream, zlib);
	zlib.Compress();
	if(!Store(serialize, out, version))
		return false;
	out.Close();
	int64 sz = out.GetPos();
	stream.Seek(sz_pos);
	stream.Put64(sz);
	return !stream.IsError();
}

template <class T>
bool ZLoad(T& x, Stream& s, int version = Null) {
	return ZLoad(SerializeCb(x), s, version);
}

template <class T>
bool ZStore(T& x, Stream& s, int version = Null) {
	return ZStore(SerializeCb(x), s, version);
}

/*
template <class T>
bool ZLoadFromFile(T& x, const char *name = NULL, int version = Null) {
	return LoadFromFile(SerializeCb(x), name, version);
}

template <class T>
bool StoreToFile(T& x, const char *name = NULL, int version = Null) {
	return StoreToFile(SerializeCb(x), name, version);
}
*/
template <class T>
String ZStoreAsString(T& x) {
	StringStream ss;
	ZStore(x, ss);
	return ss;
}

template <class T>
bool ZLoadFromString(T& x, const String& s) {
	StringStream ss(s);
	return ZLoad(x, ss);
}

CONSOLE_APP_MAIN
{
	String data;
	for(int i = 0; i < 10000; i++)
		data << AsString(i) << ": " << AsString(Uuid::Create()) << '\n';
	
	
// ---------------------------------------------------------------
	String path = GetHomeDirFile("test.z");
	{
		FileOut fout(path);
		DDUMP(data.GetLength());
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
		int64 sz = fin.Get64();
		Zlib zlib;
		InFilterStream in(fin, zlib);
		zlib.Decompress();
		in.SetSize(sz);
		in % data2;
	}
	
	ASSERT(data == data2);

	DLOG("========================");
	data = LoadFile(GetDataFile("FilterStream.cpp"));	
	String h = ZStoreAsString(data);
	DDUMP(data.GetCount());
	DDUMP(ZCompress(data).GetCount());
	DDUMP(h.GetCount());
	ZLoadFromString(data2, h);
	
	
	
	ASSERT(data == data2);
}
