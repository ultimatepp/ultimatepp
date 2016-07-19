#include "lz4.h"

#define LLOG(x) // LOG(x)

namespace Upp {

void LZ4CompressStream::Init()
{
	ClearError();
	pos = 0;
	header = false;
	xxh.Reset();
	outblock = inblock = 0;
	SetupBuffer();
}

void LZ4CompressStream::FinishBlock(char *outbuf, int clen, const char *origdata, int origsize)
{
	RTIMING("FinishBlock");
	if(IsError() || out && out->IsError())
		return;
	if(clen < 0) {
		SetError();
		return;
	}
	if(clen >= origsize) {
		Poke32le(outbuf, 0x80000000 | origsize);
		memcpy(outbuf + 4, origdata, origsize);
		out->Put(outbuf, origsize + 4);
	}
	else {
		Poke32le(outbuf, clen);
		out->Put(outbuf, clen + 4);
	}
}

void LZ4CompressStream::SetupBuffer()
{
	buffer.SetCount(BLOCK_BYTES);
	wrlim = (byte *)~buffer + BLOCK_BYTES;
	ptr =  (byte *)~buffer;
}

void LZ4CompressStream::FlushOut()
{
	if(!header) {
		byte h[7];
		Poke32le(h, LZ4F_MAGIC);
		h[4] = LZ4F_VERSION | LZ4F_BLOCKINDEPENDENCE | LZ4F_CONTENTCHECKSUM;
		h[5] = LZ4F_MAXSIZE_1024KB;
		h[6] = xxHash(h + 4, 2) >> 8;
		out->Put(h, 7);
		header = true;
	}
	
	if(ptr == (byte *)~buffer)
		return;

	int origsize = int((char *)ptr - ~buffer);
	
	pos += origsize;
	
	WhenPos(pos);
	
	if(co) {
		String bs = buffer;
		int    inblk = inblock++;
		CoWork::Start([=] {
			Buffer<char> outbuf(4 + LZ4_compressBound(BLOCK_BYTES));
			int clen = LZ4_compress(~bs, ~outbuf + 4, origsize);
			Mutex::Lock __(lock);
			while(outblock != inblk)
				cond.Wait(lock);
			FinishBlock(outbuf, clen, ~bs, origsize);
			outblock++;
			cond.Broadcast();
		});
		xxh.Put(~bs, origsize);
		SetupBuffer();
	}
	else {
		Buffer<char> outbuf(4 + LZ4_compressBound(BLOCK_BYTES));
		xxh.Put(~buffer, origsize);
		int clen = LZ4_compress(~buffer, ~outbuf + 4, origsize);
		FinishBlock(outbuf, clen, buffer, origsize);
		SetupBuffer();
	}
}

void LZ4CompressStream::Close()
{
	ASSERT(compress >= 0);
	FlushOut();
	if(co) {
		Mutex::Lock __(lock);
		while(outblock != inblock)
			cond.Wait(lock);
	}
	byte h[8];
	Poke32le(h, 0);
	Poke32le(h + 4, xxh.Finish());
	out->Put(h, 8);
	buffer.Clear();
}

bool LZ4CompressStream::IsOpen() const
{
	return (!out || out->IsOpen()) && buffer.GetCount();
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
		dword n = BLOCK_BYTES - int((char *)ptr - ~buffer);
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
	co = false;
	out = NULL;
	Init();
}

LZ4CompressStream::~LZ4CompressStream()
{
}

};
