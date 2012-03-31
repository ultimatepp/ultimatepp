#include "Core.h"

#define LDUMP(x)  // DDUMP(x)
#define LLOG(x)   // DLOG(x)

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

static byte sGZip_header[10] = { GZ_MAGIC1, GZ_MAGIC2, Z_DEFLATED, 0, 0, 0, 0, 0, 0, OS_CODE };

void Crc32Stream::Out(const void *ptr, dword count)
{
	crc = crc32(crc, (byte *)ptr, count);
}

Crc32Stream::Crc32Stream()
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
}

void Zlib::Compress()
{
	Begin();
	if(deflateInit2(&z, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
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
	if(gzip_footer)
		return;
	ASSERT(mode);
	LDUMP(mode);
	if(!output)
		output.Alloc(chunk);
	for(;;) {
		LLOG("---");
		LDUMP(z.avail_in);
		int code;
		z.avail_out = chunk;
		z.next_out = output;
		code = (mode == DEFLATE ? deflate : inflate)(&z, finish ? Z_FINISH : Z_NO_FLUSH);
		LDUMP(code);
		LDUMP(z.avail_in);
		LDUMP(z.avail_out);
		int count = chunk - z.avail_out;
		LDUMP(count);
		LDUMP(count);
		if(count) {
			if((docrc || gzip) && mode == INFLATE)
				crc = crc32(crc, output, count);
			WhenOut((const char *)~output, count);
			if(mode == INFLATE)
				total += count;
		}
		if(mode == INFLATE && code == Z_STREAM_END) {
			gzip_footer = true;
			break;
		}
		if(mode == INFLATE ? code == Z_BUF_ERROR : count == 0)
			break;
		if(code != Z_OK && code != Z_STREAM_END) {
			LLOG("ERROR " << code);
			Free();
			error = true;
			break;
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
	for(int i = !!(flags & ORIG_NAME) + !!(flags & COMMENT); i > 0; i--) {
		while(ptr[pos])
			if(++pos > size)
				return 0;
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
	if(gzip && !gzip_done && mode == INFLATE) {
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
		
		gzip_done = true;
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
	Pump(Z_NO_FLUSH);
}
	
void Zlib::Put(const void *ptr, dword size)
{
	if(error)
		return;
	const char *p = reinterpret_cast<const char *>(ptr);
	while(size) {
		LLOG("Put " << size);
		int psz = (int) min(size, dword(INT_MAX / 4));
		Put0(p, size);
		size -= psz;
		p += psz;
	}
}

void Zlib::PutOut(const void *ptr, dword size)
{
	out.Cat((const char *)ptr, (int)size);
	LDUMP(out.GetCount());
}

void Zlib::End()
{
	LLOG("End");
	if(mode != INFLATE || !gzip || gzip_done)
		Pump(Z_FINISH);
	if(gzip && mode == DEFLATE) {
		char h[8];
		Poke32le(h, crc);
		Poke32le(h + 4, total);
		WhenOut(h, 8);
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
	gzip_done = false;
	gzip_footer = false;
	total = 0;
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
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	docrc = false;
	crc = 0;
	hdr = true;
	WhenOut = callback(this, &Zlib::PutOut);
	mode = NONE;
	gzip = false;
}

Zlib::~Zlib()
{
	if(mode)
		End();
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
	out.Put(sGZip_header, 10);
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
