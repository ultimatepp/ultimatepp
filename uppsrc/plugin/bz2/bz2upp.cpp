#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

#ifdef flagWIN32
	#include "lib/bzlib.h"
#else
	#include <bzlib.h>
#endif

NAMESPACE_UPP

static void* bzalloc_new(void *opaque, int items, int size)
{
	return new byte[items * size];
}

static void bzfree_new(void *opaque, void *addr)
{
	delete[] (byte *)addr;
}

String BZ2Decompress(String s, Gate2<int, int> progress)
{
	if(s.IsEmpty())
		return s;
	bz_stream z;
	Zero(z);
	z.bzalloc = bzalloc_new;
	z.bzfree = bzfree_new;
	z.opaque = 0;
	if(BZ2_bzDecompressInit(&z, 0, 0) != BZ_OK)
		return String::GetVoid();
	int buf_size = minmax(s.GetLength() / 2, 1024, 65536);
	Buffer<char> output(buf_size);
	z.next_in = (char *)s.Begin();
	z.avail_in = s.GetLength();
	z.next_out = output;
	z.avail_out = buf_size;

	String out;
	while(BZ2_bzDecompress(&z) == BZ_OK)
	{
		if(z.avail_out == (dword)buf_size)
		{ // no output generated - assume error
			BZ2_bzDecompressEnd(&z);
			return String::GetVoid();
		}
		out.Cat(output, buf_size - z.avail_out);
		z.next_out = output;
		z.avail_out = buf_size;
		if(progress((int)(uintptr_t)((const char *)z.next_in - ~s), s.GetLength()))
		{
			BZ2_bzDecompressEnd(&z);
			return String::GetVoid();
		}
	}
	if(z.avail_out < (unsigned)buf_size)
		out.Cat(output, buf_size - z.avail_out);

	BZ2_bzDecompressEnd(&z);

	return out;
}

String BZ2Decompress(Stream& strm, Gate2<int, int> progress)
{
	StringStream out;
	BZ2Decompress(out, strm, progress);
	return out;
}

String BZ2Compress(String s, Gate2<int, int> progress)
{
	if(s.IsEmpty())
		return s;
	bz_stream z;
	Zero(z);
	z.bzalloc = bzalloc_new;
	z.bzfree = bzfree_new;
	z.opaque = 0;
	if(BZ2_bzCompressInit(&z, 9, 0, 30) != BZ_OK)
		return String::GetVoid();
	int buf_size = minmax(s.GetLength(), 1024, 65536);
	Buffer<char> output(buf_size);
	z.next_in = (char *)s.Begin();
	z.avail_in = s.GetLength();
	z.next_out = output;
	z.avail_out = buf_size;

	String out;
	int res;
	while((res = BZ2_bzCompress(&z, z.avail_in ? BZ_RUN : BZ_FINISH)) == BZ_RUN_OK || res == BZ_FINISH_OK)
	{
		out.Cat(output, buf_size - z.avail_out);
		z.next_out = output;
		z.avail_out = buf_size;
		if(progress((int)(uintptr_t)((const char *)z.next_in - ~s), s.GetLength()))
		{
			BZ2_bzCompressEnd(&z);
			return String::GetVoid();
		}
	}
	out.Cat(output, buf_size - z.avail_out);
	BZ2_bzCompressEnd(&z);
	if(res != BZ_STREAM_END)
		out = String::GetVoid();
	return out;
}

void BZ2Decompress(Stream& out, Stream& in, Gate2<int, int> progress)
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

void BZ2Compress(Stream& out, Stream& in, Gate2<int, int> progress)
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

END_UPP_NAMESPACE
