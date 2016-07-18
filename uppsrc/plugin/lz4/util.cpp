#include "lz4.h"

namespace Upp {

int64 lz4Press(Stream& out, Stream& in, int64 size, Gate2<int64, int64> progress, bool compress, bool co = false)
{
	Lz4 lz4;
	
	int64 r = -1;
	{
		OutFilterStream outs(out, lz4);
		if(co)
			lz4.Parallel();
		if(compress)
			lz4.Compress();
		else
			lz4.Decompress();
		if(CopyStream(outs, in, size, progress) >= 0) {
			outs.Close();
			if(!out.IsError() && !outs.IsError() && !lz4.IsError())
				r = outs.GetCount();
		}
	}
	return r;
}

int64 LZ4Compress(Stream& out, Stream& in, Gate2<int64, int64> progress)
{
	return lz4Press(out, in, in.GetLeft(), progress, true);
}

int64 LZ4Decompress(Stream& out, Stream& in, Gate2<int64, int64> progress)
{
	return lz4Press(out, in, in.GetLeft(), progress, false);
}

String LZ4Compress(const void *data, int64 len, Gate2<int64, int64> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return LZ4Compress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String LZ4Compress(const String& s, Gate2<int64, int64> progress)
{
	return LZ4Compress(~s, s.GetLength(), progress);
}

String LZ4Decompress(const void *data, int64 len, Gate2<int64, int64> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return LZ4Decompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String LZ4Decompress(const String& s, Gate2<int64, int64> progress)
{
	return LZ4Decompress(~s, s.GetLength(), progress);
}

int64 CoLZ4Compress(Stream& out, Stream& in, Gate2<int64, int64> progress)
{
	return lz4Press(out, in, in.GetLeft(), progress, true, true);
}

int64 CoLZ4Decompress(Stream& out, Stream& in, Gate2<int64, int64> progress)
{
	return lz4Press(out, in, in.GetLeft(), progress, false, true);
}

String CoLZ4Compress(const void *data, int64 len, Gate2<int64, int64> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return CoLZ4Compress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String CoLZ4Compress(const String& s, Gate2<int64, int64> progress)
{
	return CoLZ4Compress(~s, s.GetLength(), progress);
}

String CoLZ4Decompress(const void *data, int64 len, Gate2<int64, int64> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return CoLZ4Decompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String CoLZ4Decompress(const String& s, Gate2<int64, int64> progress)
{
	return CoLZ4Decompress(~s, s.GetLength(), progress);
}

};