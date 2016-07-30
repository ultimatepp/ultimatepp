#include "lz4.h"

namespace Upp {
	
void sCompressStreamCopy_(Stream& out, Stream& in, Function<bool(int64, int64)> progress, Stream& orig_in, int64 insz)
{
	const int CHUNK = 32678;
	Buffer<byte> b(CHUNK);
	while(!in.IsEof()) {
		if(progress(orig_in.GetPos(), insz))
			break;
		int n = in.Get(b, CHUNK);
		out.Put(b, n);
	}
	progress(orig_in.GetPos(), insz);
}

static int64 sLZ4Compress(Stream& out, Stream& in, int64 size, Function<bool(int64, int64)> progress, bool co)
{
	LZ4CompressStream outs(out);
#ifdef _MULTITHREADED
	if(co)
		outs.Concurrent();
#endif
	sCompressStreamCopy_(outs, in, progress, in, size);
	outs.Close();
	if(!out.IsError() && !outs.IsError())
		return out.GetSize();
	return -1;
}

static int64 sLZ4Decompress(Stream& out, Stream& in, int64 size, Function<bool(int64, int64)> progress, bool co)
{
	LZ4DecompressStream ins(in);
#ifdef _MULTITHREADED
	if(co)
		ins.Concurrent();
#endif
	sCompressStreamCopy_(out, ins, progress, in, size);
	ins.Close();
	if(!out.IsError() && !ins.IsError())
		return out.GetSize();
	return -1;
}

int64 LZ4Compress(Stream& out, Stream& in, Function<bool(int64, int64)> progress)
{
	return sLZ4Compress(out, in, in.GetLeft(), progress, false);
}

int64 LZ4Decompress(Stream& out, Stream& in, Function<bool(int64, int64)> progress)
{
	return sLZ4Decompress(out, in, in.GetLeft(), progress, false);
}

String LZ4Compress(const void *data, int64 len, Function<bool(int64, int64)> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return LZ4Compress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String LZ4Compress(const String& s, Function<bool(int64, int64)> progress)
{
	return LZ4Compress(~s, s.GetLength(), progress);
}

String LZ4Decompress(const void *data, int64 len, Function<bool(int64, int64)> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return LZ4Decompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String LZ4Decompress(const String& s, Function<bool(int64, int64)> progress)
{
	return LZ4Decompress(~s, s.GetLength(), progress);
}

#ifdef _MULTITHREADED

int64 CoLZ4Compress(Stream& out, Stream& in, Function<bool(int64, int64)> progress)
{
	return sLZ4Compress(out, in, in.GetLeft(), progress, true);
}

int64 CoLZ4Decompress(Stream& out, Stream& in, Function<bool(int64, int64)> progress)
{
	return sLZ4Decompress(out, in, in.GetLeft(), progress, true);
}

String CoLZ4Compress(const void *data, int64 len, Function<bool(int64, int64)> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return CoLZ4Compress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String CoLZ4Compress(const String& s, Function<bool(int64, int64)> progress)
{
	return CoLZ4Compress(~s, s.GetLength(), progress);
}

String CoLZ4Decompress(const void *data, int64 len, Function<bool(int64, int64)> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return CoLZ4Decompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String CoLZ4Decompress(const String& s, Function<bool(int64, int64)> progress)
{
	return CoLZ4Decompress(~s, s.GetLength(), progress);
}

#endif

};