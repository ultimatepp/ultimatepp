#include "lz4.h"

#ifndef flagCOMPLEXLZ4

#define LLOG(x) // LOG(x)

namespace Upp {

void LZ4CompressStream::Open(Stream& out_)
{
	out = &out_;
	ClearError();
	pos = 0;
	xxh.Reset();
	Alloc();
	pos = 0;
	byte h[7];
	Poke32le(h, LZ4F_MAGIC);
	h[4] = LZ4F_VERSION | LZ4F_BLOCKINDEPENDENCE | LZ4F_CONTENTCHECKSUM;
	h[5] = LZ4F_MAXSIZE_1024KB;
	h[6] = xxHash(h + 4, 2) >> 8;
	out->Put(h, 7);
}

void LZ4CompressStream::Alloc()
{
	int N = 16;
	int sz = concurrent ? N * BLOCK_BYTES : BLOCK_BYTES;
	buffer.Alloc(sz);
	outbuf.Alloc(N * LZ4_compressBound(BLOCK_BYTES));
	outsz.Alloc(N);
	wrlim = ~buffer + sz;
	ptr = ~buffer;
}

#ifdef _MULTITHREADED
void LZ4CompressStream::Concurrent(bool b)
{
	FlushOut();
	concurrent = b;
	Alloc();
}
#endif

void LZ4CompressStream::FlushOut()
{
	if(ptr == (byte *)~buffer)
		return;
	
	CoWork co;
	
	int osz = LZ4_compressBound(BLOCK_BYTES);
	byte *t = ~outbuf;
	int   ii = 0;
	for(byte *s = ~buffer; s < ptr; s += BLOCK_BYTES) {
		int origsize = min((int)BLOCK_BYTES, int(ptr - s));
#ifdef _MULTITHREADED
		if(concurrent)
			co & [=] {
				outsz[ii] = LZ4_compress_default((char *)s, (char *)t, origsize, osz);
			};
		else
#endif
			outsz[ii] = LZ4_compress_default((char *)s, (char *)t, origsize, osz);
		ii++;
		t += osz;
	}
	
	if(concurrent)
		co.Finish();
	
	byte *s = ~buffer;
	t = ~outbuf;
	for(int i = 0; i < ii; i++) {
		int origsize = min((int)BLOCK_BYTES, int(ptr - s));
		int clen = outsz[i];
		if(clen < 0) {
			SetError();
			return;
		}
		if(clen >= origsize || clen == 0) {
			out->Put32le(0x80000000 | origsize);
			out->Put(s, origsize);
		}
		else {
			out->Put32le(clen);
			out->Put(t, clen);
		}
		s += BLOCK_BYTES;
		t += osz;
	}
	
	int origsize = int(ptr - ~buffer);
	xxh.Put(~buffer, origsize);
	pos += origsize;
	WhenPos(pos);
	ptr = ~buffer;
}

void LZ4CompressStream::Close()
{
	ASSERT(compress >= 0);
	if(out) {
		FlushOut();
		out->Put32le(0);
		out->Put32le(xxh.Finish());
		out = NULL;
	}
}

bool LZ4CompressStream::IsOpen() const
{
	return out && out->IsOpen();
}

void LZ4CompressStream::_Put(int w)
{
	FlushOut();
	*ptr++ = w;
}

void LZ4CompressStream::_Put(const void *data, dword size)
{
	LLOG("Put " << size);

	ASSERT(compress >= 0);
	
	const char *s = reinterpret_cast<const char *>(data);

	while(size > 0) {
		if(IsError() || out && out->IsError())
			return;
		dword n = dword(wrlim - ptr);
		if(size >= n) {
			memcpy(ptr, s, n);
			ptr = wrlim;
			FlushOut();
			size -= n;
			s += n;
		}
		else {
			memcpy(ptr, s, size);
			ptr += size;
			break;
		}
	}
}

LZ4CompressStream::LZ4CompressStream()
{
	concurrent = false;
	out = NULL;
}

LZ4CompressStream::~LZ4CompressStream()
{
	Close();
}

};

#endif