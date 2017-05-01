#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

#ifdef flagWIN32
	#include "lib/bzlib.h"
#else
	#include <bzlib.h>
#endif

namespace Upp {

static void* bzalloc_new(void *opaque, int items, int size)
{
	return new byte[items * size];
}

static void bzfree_new(void *opaque, void *addr)
{
	delete[] (byte *)addr;
}

void BZ2Decompress(Stream& out, Stream& in, Gate<int, int> progress)
{
	enum { BUF_SIZE = 65536 };
	Buffer<char> input(BUF_SIZE), output(BUF_SIZE);
	int avail = in.Get(input, BUF_SIZE);
	if(avail == 0)
		return;
	bz_stream z;
	Zero(z);
	z.bzalloc = bzalloc_new;
	z.bzfree = bzfree_new;
	z.opaque = 0;
	if(BZ2_bzDecompressInit(&z, 0, 0) != BZ_OK)
	{
		out.SetError();
		return;
	}
	z.next_in = input;
	z.avail_in = avail;
	z.next_out = output;
	z.avail_out = BUF_SIZE;
	int code;
	bool running = true;
	int64 total = in.GetLeft();
	int done = 0;
	do
	{
		if(z.avail_in == 0 && running)
		{
			if((z.avail_in = in.Get(z.next_in = input, BUF_SIZE)) == 0)
				running = false;
			done += z.avail_in;
			if(progress(done, (int)total) || in.IsError())
			{
				BZ2_bzDecompressEnd(&z);
				out.SetError();
				return;
			}
		}
		code = BZ2_bzDecompress(&z);
		if(z.avail_out == 0)
		{
			out.Put(z.next_out = output, z.avail_out = BUF_SIZE);
			if(out.IsError())
			{
				BZ2_bzDecompressEnd(&z);
				return;
			}
		}
	}
	while(code == BZ_OK);
	if(z.avail_out < BUF_SIZE)
		out.Put(output, BUF_SIZE - z.avail_out);
	BZ2_bzDecompressEnd(&z);
}

void BZ2Compress(Stream& out, Stream& in, Gate<int, int> progress)
{
	enum { BUF_SIZE = 65536 };
	Buffer<char> input(BUF_SIZE), output(BUF_SIZE);
	bz_stream z;
	z.bzalloc = bzalloc_new;
	z.bzfree = bzfree_new;
	z.opaque = 0;
	if(BZ2_bzCompressInit(&z, 9, 0, 30) != BZ_OK)
	{
		out.SetError();
		return;
	}
	z.avail_in = 0;
	z.avail_out = BUF_SIZE;
	z.next_out = output;
	int code;
	int flush = BZ_RUN;
	int64 total = in.GetLeft();
	int done = 0;
	do
	{
		if(z.avail_in == 0 && flush == BZ_RUN)
		{
			z.next_in = input;
			if((z.avail_in = in.Get(z.next_in = input, BUF_SIZE)) == 0)
				flush = BZ_FINISH;
			done += z.avail_in;
			if(progress(done, (int)total) || in.IsError())
			{
				BZ2_bzCompressEnd(&z);
				out.SetError();
				return;
			}
		}
		code = BZ2_bzCompress(&z, flush);
		if(z.avail_out == 0)
		{
			out.Put(z.next_out = output, z.avail_out = BUF_SIZE);
			if(out.IsError())
			{
				BZ2_bzCompressEnd(&z);
				return;
			}
		}
	}
	while(code == BZ_RUN_OK || code == BZ_FINISH_OK);
	if(z.avail_out < BUF_SIZE)
		out.Put(output, BUF_SIZE - z.avail_out);
	BZ2_bzCompressEnd(&z);
	if(code != BZ_STREAM_END)
		out.SetError();
}

String BZ2Compress(Stream& in, Gate<int, int> progress)
{
	StringStream out;
	BZ2Compress(out, in, progress);
	return out;
}

String BZ2Decompress(Stream& in, Gate<int, int> progress)
{
	StringStream out;
	BZ2Decompress(out, in, progress);
	return out;
}

String BZ2Compress(const void *data, int64 len, Gate<int, int> progress)
{
	MemReadStream in(data, len);
	return BZ2Compress(in, progress);
}

String BZ2Decompress(const void *data, int64 len, Gate<int, int> progress)
{
	MemReadStream in(data, len);
	return BZ2Decompress(in, progress);
}

String BZ2Compress(const String& data, Gate<int, int> progress)
{
	return BZ2Compress(~data, data.GetLength(), progress);
}

String BZ2Decompress(const String& data, Gate<int, int> progress)
{
	return BZ2Decompress(~data, data.GetLength(), progress);
}

}
