#include "Core.h"

#ifdef PLATFORM_WIN32
#include <plugin/z/lib/zlib.h>
#include <plugin/z/lib/zutil.h>
#else
#include <zlib.h>

#ifndef DEF_MEM_LEVEL
#define DEF_MEM_LEVEL 8
#endif

#ifndef OS_CODE
#define OS_CODE  0x03
#endif

#endif

NAMESPACE_UPP

//////////////////////////////////////////////////////////////////////

static voidpf zalloc_new(voidpf opaque, uInt items, uInt size)
{
	return new byte[items * size];
}

static void zfree_new(voidpf opaque, voidpf address)
{
	delete[] (byte *)address;
}

enum
{
	GZ_MAGIC1    = 0x1f,
	GZ_MAGIC2    = 0x8b,

	ASCII_FLAG   = 0x01, /* bit 0 set: file probably ascii text */
	HEAD_CRC     = 0x02, /* bit 1 set: header CRC present */
	EXTRA_FIELD  = 0x04, /* bit 2 set: extra field present */
	ORIG_NAME    = 0x08, /* bit 3 set: original file name present */
	COMMENT      = 0x10, /* bit 4 set: file comment present */
	RESERVED     = 0xE0, /* bits 5..7: reserved */
};

void Crc32::Put(const void *ptr, int count)
{
	crc = crc32(crc, (byte *)ptr, count);
}

void Crc32::Put(char c)
{
	crc = crc32(crc, (byte *)&c, 1);
}

void Crc32::Put(byte c)
{
	crc = crc32(crc, (byte *)&c, 1);
}

Crc32::Crc32()
{
	crc = crc32(0, NULL, 0);
}

static int sZpress(Stream& out, Stream& in, int size, Gate2<int, int> progress, bool nohdr, dword *crc,
                    bool compress)
{
	const int BUF_SIZE = 65536;
	Buffer<Bytef> input(BUF_SIZE), output(BUF_SIZE);
	z_stream z;
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	if(compress ? deflateInit2(&z, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
	                          nohdr ? -MAX_WBITS : MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY)
	            : inflateInit2(&z, nohdr ? -MAX_WBITS : +MAX_WBITS) != Z_OK) {
		out.SetError();
		return -1;
	}
	int code;
	int flush;
	int done = 0;
	int outsz = 0;
	if(crc)
		*crc = crc32(0, NULL, 0);
	do {
		z.avail_in = in.Get(z.next_in = input, min(size - done, BUF_SIZE));
		if(progress(done += z.avail_in, size) || in.IsError())
			goto error;
		if(!compress && z.avail_in == 0)
			break;
		flush = z.avail_in > 0 ? Z_NO_FLUSH : Z_FINISH;
		if(crc && compress)
			*crc = crc32(*crc, input, z.avail_in);
		do {
			z.avail_out = BUF_SIZE;
			z.next_out = output;
			code = (compress ? deflate : inflate)(&z, flush);
			if(code == Z_STREAM_ERROR)
				goto error;
			int count = BUF_SIZE - z.avail_out;
			if(crc && !compress)
			    *crc = crc32(*crc, output, count);
			out.Put(output, count);
			if(out.IsError())
				goto error;
			outsz += count;
	    }
		while(z.avail_out == 0);
    }
	while(compress ? flush != Z_FINISH : code != Z_STREAM_END);
	if(!compress || code == Z_STREAM_END) {
		(compress ? deflateEnd : inflateEnd)(&z);
		return outsz;
	}
error:
	(compress ? deflateEnd : inflateEnd)(&z);
	return -1;
}

int ZCompress(Stream& out, Stream& in, int size, Gate2<int, int> progress, bool nohdr, dword *crc)
{
	return sZpress(out, in, size, progress, nohdr, crc, true);
}

int ZDecompress(Stream& out, Stream& in, int size, Gate2<int, int> progress, bool nohdr, dword *crc)
{
	return sZpress(out, in, size, progress, nohdr, crc, false);
}

int    ZCompress(Stream& out, Stream& in, Gate2<int, int> progress)
{
	return ZCompress(out, in, (int)in.GetLeft(), progress);
}

int    ZDecompress(Stream& out, Stream& in, Gate2<int, int> progress)
{
	return ZDecompress(out, in, (int)in.GetLeft(), progress);
}

String ZCompress(const void *data, int len, Gate2<int, int> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	ZCompress(out, in, progress);
	return out;
}

String ZDecompress(const void *data, int len, Gate2<int, int> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	ZDecompress(out, in, progress);
	return out;
}

String ZCompress(const String& s, Gate2<int, int> progress)
{
	return ZCompress(~s, s.GetCount(), progress);
}

String ZDecompress(const String& s, Gate2<int, int> progress)
{
	return ZDecompress(~s, s.GetCount(), progress);
}

int GZCompress(Stream& out, Stream& in, int size, Gate2<int, int> progress)
{
	static byte gzip_header[10] = { GZ_MAGIC1, GZ_MAGIC2, Z_DEFLATED, 0, 0, 0, 0, 0, 0, OS_CODE };
	out.Put(gzip_header, 10);
	dword crc;
	int sz = ZCompress(out, in, size, progress, true, &crc);
	out.Put32le(crc);
	out.Put32le(size);
	return sz;
}

static int sSkipZ(Stream& stream)
{
	int q = 0;
	while(stream.Get() > 0)
		q++;
	return q;
}

int GZDecompress(Stream& out, Stream& in, int size, Gate2<int, int> progress)
{
	byte buffer[10];
	if(!in.GetAll(buffer, 10) || buffer[0] != GZ_MAGIC1 || buffer[1] != GZ_MAGIC2) {
		out.SetError();
		return -1;
	}
	int flags = buffer[3];
	if(buffer[2] != Z_DEFLATED || (flags & RESERVED) != 0)
		return false;
	size -= 10;
	if(flags & EXTRA_FIELD) {
		int len = in.Get16le();
		in.SeekCur(len);
		size -= len;
	}
	if(flags & ORIG_NAME)
		size -= sSkipZ(in);
	if(flags & COMMENT)
		size -= sSkipZ(in);
	if(flags & HEAD_CRC) {
		in.Get16le();
		size -= 2;
	}
	if(in.IsEof() || size < 0) {
		out.SetError();
		return -1;
	}
	dword crc;
	int sz = ZDecompress(out, in, size, progress, true, &crc);
	return sz < 0 || in.Get32le() != (int)crc || in.Get32le() != sz ? -1 : sz;
}

int GZCompress(Stream& out, Stream& in, Gate2<int, int> progress = false)
{
	return GZCompress(out, in, (int)in.GetLeft(), progress);
}

int GZDecompress(Stream& out, Stream& in, Gate2<int, int> progress = false)
{
	return GZDecompress(out, in, (int)in.GetLeft(), progress);
}

String GZCompress(const void *data, int len, Gate2<int, int> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	GZCompress(out, in, progress);
	return out;
}

String GZDecompress(const void *data, int len, Gate2<int, int> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	GZDecompress(out, in, progress);
	return out;
}

String GZCompress(const String& s, Gate2<int, int> progress)
{
	return GZCompress(~s, s.GetCount(), progress);
}

String GZDecompress(const String& s, Gate2<int, int> progress)
{
	return GZDecompress(~s, s.GetCount(), progress);
}


END_UPP_NAMESPACE
