#include "Core.h"

#define LDUMP(x)  // DDUMP(x)
#define LLOG(x)   // DLOG(x)

namespace Upp {

#ifndef DEF_MEM_LEVEL
#define DEF_MEM_LEVEL 8
#endif

#ifndef OS_CODE
#define OS_CODE  0x03
#endif

//////////////////////////////////////////////////////////////////////

static voidpf zalloc_new(voidpf opaque, uInt items, uInt size)
{
	return MemoryAlloc(items * size);
}

static void zfree_new(voidpf opaque, voidpf address)
{
	MemoryFree(address);
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

static byte sGZip_header[10] = { GZ_MAGIC1, GZ_MAGIC2, Z_DEFLATED, 0, 0, 0, 0, 0, 0, OS_CODE };

void Crc32Stream::Out(const void *ptr, dword count)
{
	crc = crc32(crc, (byte *)ptr, count);
}

void Crc32Stream::Clear()
{
	crc = crc32(0, NULL, 0);
}

dword CRC32(const void *ptr, dword count)
{
	Crc32Stream c;
	c.Put(ptr, count);
	return c;
}

dword CRC32(const String& s)
{
	return CRC32(~s, s.GetLength());
}

void Zlib::Begin()
{
	Free();
	error = false;
	if(docrc || gzip)
		crc = crc32(0, NULL, 0);
	out.Clear();
	gzip_name.Clear();
	gzip_comment.Clear();
	footer.Clear();
	gzip_footer = false;
	gzip_header_done = false;
}

void Zlib::Compress()
{
	Begin();
	if(deflateInit2(&z, compression_level, Z_DEFLATED,
	                hdr && !gzip ? MAX_WBITS : -MAX_WBITS, DEF_MEM_LEVEL,
	                Z_DEFAULT_STRATEGY) != Z_OK)
		Panic("deflateInit2 failed");
	mode = DEFLATE;
	if(gzip)
		WhenOut(sGZip_header, 10);
}

void Zlib::Decompress()
{
	Begin();
	if(inflateInit2(&z, hdr && !gzip ? +MAX_WBITS : -MAX_WBITS) != Z_OK)
		Panic("inflateInit2 failed");
	mode = INFLATE;
}

void Zlib::Pump(bool finish)
{
	if(error)
		return;
	ASSERT(mode);
	if(!output)
		output.Alloc(chunk);
	for(;;) {
		if(gzip_footer) {
			footer.Cat(z.next_in, z.avail_in);
			z.avail_in = 0;
			return;
		}
		int code;
		z.avail_out = chunk;
		z.next_out = output;
		code = (mode == DEFLATE ? deflate : inflate)(&z, finish ? Z_FINISH : Z_NO_FLUSH);
		int count = chunk - z.avail_out;
		if(count) {
			if((docrc || gzip) && mode == INFLATE)
				crc = crc32(crc, output, count);
			WhenOut((const char *)~output, count);
			if(mode == INFLATE)
				total += count;
		}
		if(mode == INFLATE && code == Z_STREAM_END)
			gzip_footer = true;
		else {
			if(mode == INFLATE ? code == Z_BUF_ERROR : count == 0)
				break;
			if(code != Z_OK && code != Z_STREAM_END) {
				LLOG("ZLIB ERROR " << code);
				Free();
				error = true;
				break;
			}
		}
    }
}

int Zlib::GzipHeader(const char *ptr, int size)
{
	int pos = 10;
	if(pos > size)
		return 0;
	int flags = ptr[3];
	if(ptr[2] != Z_DEFLATED || (flags & RESERVED) != 0) {
		error = true;
		return 0;
	}
	if(flags & EXTRA_FIELD) {
		if(pos + 2 > size)
			return 0;
		int len = MAKEWORD(ptr[pos], ptr[pos + 1]);
		if(len < 0) {
			error = true;
			return 0;
		}
		if((pos += len + 2) > size)
			return 0;
	}
	gzip_name.Clear();
	gzip_comment.Clear();
	for(int i = !!(flags & ORIG_NAME) + !!(flags & COMMENT); i > 0; i--) {
		while(ptr[pos]) {
			(i ? gzip_name : gzip_comment).Cat(ptr[pos]);
			if(++pos > size)
				return 0;
		}
		if(++pos > size)
			return 0;
	}
	if(flags & HEAD_CRC)
		if((pos += 2) > size)
			return 0;
	return pos;
}

void Zlib::Put0(const char *ptr, int size)
{
	if(error)
		return;
	ASSERT(mode);
	if(size <= 0)
		return;
	if(gzip && !gzip_header_done && mode == INFLATE) {
		if(gzip_hs.GetCount()) {
			gzip_hs.Cat(ptr, size);
			ptr = ~gzip_hs;
			size = gzip_hs.GetCount();
		}
		int pos = GzipHeader(ptr, size);
		if(!pos) {
			if(gzip_hs.GetCount() == 0)
				gzip_hs.Cat(ptr, size);
			return;
		}
		
		gzip_header_done = true;
		size -= pos;
		ptr += pos;
	}

	if(size <= 0)
		return;

	if(mode == DEFLATE) {
		total += size;
		if(docrc || gzip)
			crc = crc32(crc, (const Bytef *)ptr, size);
	}

	z.next_in = (Bytef *)ptr;
	z.avail_in = size;
	Pump(false);
}
	
void Zlib::Put(const void *ptr, int size)
{
	if(error)
		return;
	LLOG("ZLIB Put " << size);
	const char *p = reinterpret_cast<const char *>(ptr);
	while(size) {
		int psz = (int) min(size, INT_MAX / 4);
		Put0(p, size);
		size -= psz;
		p += psz;
	}
}

void Zlib::PutOut(const void *ptr, int size)
{
	LLOG("ZLIB PutOut " << out.GetCount());
	out.Cat((const char *)ptr, (int)size);
}

void Zlib::End()
{
	LLOG("ZLIB End");
	if(mode != INFLATE || !gzip || gzip_header_done)
		Pump(true);
	if(gzip && mode == DEFLATE) {
		char h[8];
		Poke32le(h, crc);
		Poke32le(h + 4, total);
		WhenOut(h, 8);
	}
	if(gzip && mode == INFLATE &&
	   (footer.GetCount() != 8 || Peek32le(~footer) != (int)crc || Peek32le(~footer + 4) != total)) {
		LLOG("ZLIB GZIP FOOTER ERROR");
		error = true;
	}
	Free();
}

void Zlib::Free()
{
	if(mode == INFLATE)
		inflateEnd(&z);
	if(mode == DEFLATE)
		deflateEnd(&z);
	mode = NONE;
	gzip_hs.Clear();
	gzip_header_done = false;
	gzip_footer = false;
	total = 0;
}

void Zlib::Init()
{
	mode = NONE;
	gzip = false;
	error = false;
	z.next_in = (byte*)"";
	z.avail_in = 0;
}

void Zlib::Clear()
{
	Free();
	Init();
}

Zlib& Zlib::ChunkSize(int n)
{
	ASSERT(n < INT_MAX / 4);
	output.Clear();
	chunk = n;
	return *this;
}

Zlib::Zlib()
{
	Init();
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	docrc = false;
	crc = 0;
	hdr = true;
	chunk = 4096;
	WhenOut = callback(this, &Zlib::PutOut);
	compression_level = Z_DEFAULT_COMPRESSION;
}

Zlib::~Zlib()
{
	Free();
}

int64 zPress(Stream& out, Stream& in, int64 size, Gate<int64, int64> progress, bool gzip, bool compress,
             dword *crc = NULL, bool hdr = true)
{
	Zlib zlib;
	zlib.GZip(gzip).CRC(crc).Header(hdr);
	
	int64 r = -1;
	{
		OutFilterStream outs(out, zlib);
		if(compress)
			zlib.Compress();
		else
			zlib.Decompress();
		if(CopyStream(outs, in, size, progress) >= 0) {
			outs.Close();
			if(!out.IsError() && !outs.IsError())
				r = outs.GetCount();
		}
	}
	if(r >= 0 && crc)
		*crc = zlib.GetCRC();
	return r;
}

int64 ZCompress(Stream& out, Stream& in, int64 size, Gate<int64, int64>progress, bool hdr)
{
	return zPress(out, in, size, progress, false, true, NULL, hdr);
}

int64 ZDecompress(Stream& out, Stream& in, int64 size, Gate<int64, int64>progress, bool hdr)
{
	return zPress(out, in, size, progress, false, false, NULL, hdr);
}

int64 ZCompress(Stream& out, Stream& in, Gate<int64, int64>progress)
{
	return ZCompress(out, in, in.GetLeft(), progress);
}

int64 ZDecompress(Stream& out, Stream& in, Gate<int64, int64>progress)
{
	return zPress(out, in, in.GetLeft(), progress, false, false);
}

String ZCompress(const void *data, int64 len, Gate<int64, int64>progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return ZCompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String ZCompress(const String& s, Gate<int64, int64>progress)
{
	return ZCompress(~s, s.GetLength(), progress);
}

String ZDecompress(const void *data, int64 len, Gate<int64, int64>progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return ZDecompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String ZDecompress(const String& s, Gate<int64, int64>progress)
{
	return ZDecompress(~s, s.GetLength(), progress);
}

int64  GZCompress(Stream& out, Stream& in, int64 size, Gate<int64, int64>progress)
{
	return zPress(out, in, size, progress, true, true);
}

int64  GZDecompress(Stream& out, Stream& in, int64 size, Gate<int64, int64>progress)
{
	return zPress(out, in, size, progress, true, false);
}

int64  GZCompress(Stream& out, Stream& in, Gate<int64, int64>progress)
{
	return GZCompress(out, in, in.GetLeft(), progress);
}

String GZCompress(const void *data, int len, Gate<int64, int64>progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return GZCompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String GZCompress(const String& s, Gate<int64, int64>progress)
{
	return GZCompress(~s, s.GetCount(), progress);
}

int64  GZDecompress(Stream& out, Stream& in, Gate<int64, int64>progress)
{
	return GZDecompress(out, in, in.GetLeft(), progress);
}

String GZDecompress(const void *data, int len, Gate<int64, int64>progress)
{
	StringStream out;
	MemReadStream in(data, len);
	return GZDecompress(out, in, progress) < 0 ? String::GetVoid() : out.GetResult();
}

String GZDecompress(const String& s, Gate<int64, int64>progress)
{
	return GZDecompress(~s, s.GetCount(), progress);
}

bool GZCompressFile(const char *dstfile, const char *srcfile, Gate<int64, int64>progress)
{
	FileIn in(srcfile);
	if(!in)
		return false;
	FileOut out(dstfile);
	if(!out)
		return false;
	if(GZCompress(out, in, in.GetLeft(), progress) < 0)
		return false;
	out.Close();
	return !out.IsError();
}

bool GZCompressFile(const char *srcfile, Gate<int64, int64>progress)
{
	return GZCompressFile(~(String(srcfile) + ".gz"), srcfile, progress);
}

bool GZDecompressFile(const char *dstfile, const char *srcfile, Gate<int64, int64>progress)
{
	FileIn in(srcfile);
	if(!in)
		return false;
	FileOut out(dstfile);
	if(!out)
		return false;
	if(GZDecompress(out, in, in.GetLeft(), progress) < 0)
		return false;
	out.Close();
	return !out.IsError();
}

bool GZDecompressFile(const char *srcfile, Gate<int64, int64>progress)
{
	String dstfile = srcfile;
	if(dstfile.EndsWith(".gz"))
		dstfile.Trim(dstfile.GetLength() - 3);
	else
	if(dstfile.EndsWith(".gzip"))
		dstfile.Trim(dstfile.GetLength() - 5);
	else
		return false;
	return GZDecompressFile(~dstfile, srcfile, progress);
}

Gate<int64, int64> AsGate64(Gate<int, int> gate)
{
	Gate<int64, int64> h;
	h << [=](int64 a, int64 b) {
		if(b > ((int64)INT_MAX << 10))
			return gate((int)(a>>32), (int)(b>>32));
		if(b > INT_MAX)
			return gate((int)(a>>22), (int)(b>>22));
		return gate((int)a, (int)b);
	};
	return h;
}

#include "lib/lz4.h"

String FastCompress(const void *s, int sz)
{
	size_t maxsize = LZ4_compressBound(sz);
	if(maxsize + sizeof(int) >= INT_MAX)
		Panic("Compress result is too big!");
	StringBuffer b((int)maxsize + sizeof(int));
	*(int *)~b = sz;
	int clen = LZ4_compress_default((const char *)s, ~b + sizeof(int), sz, (int)maxsize);
	b.SetCount(clen + sizeof(int));
	b.Shrink();
	return String(b);
}

String FastCompress(const String& s)
{
	return FastCompress(~s, s.GetCount());
}

String FastDecompress(const String& data)
{
	int sz = *(int *)~data;
	StringBuffer b(sz);
	LZ4_decompress_safe(~data + sizeof(int), b, data.GetCount() - sizeof(int), sz);
	return String(b);
}

// following function is used in both plugin/lz4 and plugin/zstd
void sCompressStreamCopy_(Stream& out, Stream& in, Gate<int64, int64> progress, Stream& orig_in, int64 insz)
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

}
