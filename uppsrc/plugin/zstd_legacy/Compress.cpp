#include "zstd.h"

namespace Upp {
	
namespace Legacy {
	
// we simply store data as series of complete Zstd frames, as library gives us no easy way
// to do it in MT

void ZstdCompressStream::Open(Stream& out_, int level_)
{
	out = &out_;
	level = level_;
	ClearError();
	pos = 0;
	Alloc();
}

void ZstdCompressStream::Alloc()
{
	int N = 16;
	int sz = concurrent ? N * BLOCK_BYTES : BLOCK_BYTES;
	buffer.Alloc(sz);
	outbuf.Alloc(N * ZSTD_compressBound(BLOCK_BYTES));
	outsz.Alloc(N);
	Stream::buffer = ~buffer;
	wrlim = ~buffer + sz;
	ptr = ~buffer;
}

#ifdef _MULTITHREADED
void ZstdCompressStream::Co(bool b)
{
	FlushOut();
	concurrent = b;
	Alloc();
}
#endif

void ZstdCompressStream::FlushOut()
{
	if(ptr == (byte *)~buffer)
		return;
	
	CoWork co;
	
	int osz = (int)ZSTD_compressBound(BLOCK_BYTES);
	byte *t = ~outbuf;
	int   ii = 0;
	for(byte *s = ~buffer; s < ptr; s += BLOCK_BYTES) {
		int origsize = min((int)BLOCK_BYTES, int(ptr - s));
#ifdef _MULTITHREADED
		if(concurrent)
			co & [=] {
				outsz[ii] = (int)ZSTD_compress(t, osz, s, origsize, level);
			};
		else
#endif
			outsz[ii] = (int)ZSTD_compress(t, osz, s, origsize, level);
		ii++;
		t += osz;
	}
	
	if(concurrent)
		co.Finish();
	
	t = ~outbuf;
	for(int i = 0; i < ii; i++) {
		int clen = outsz[i];
		if(clen < 0) {
			SetError();
			return;
		}
		out->Put(t, clen);
		t += osz;
	}
	
	int origsize = int(ptr - ~buffer);
	pos += origsize;
	ptr = ~buffer;
}

void ZstdCompressStream::Close()
{
	if(out) {
		FlushOut();
		out = NULL;
	}
}

bool ZstdCompressStream::IsOpen() const
{
	return out && out->IsOpen();
}

void ZstdCompressStream::_Put(int w)
{
	FlushOut();
	*ptr++ = w;
}

void ZstdCompressStream::_Put(const void *data, dword size)
{
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

ZstdCompressStream::ZstdCompressStream()
{
	style = STRM_WRITE;
	concurrent = false;
	out = NULL;
}

ZstdCompressStream::~ZstdCompressStream()
{
	Close();
}

};

};
