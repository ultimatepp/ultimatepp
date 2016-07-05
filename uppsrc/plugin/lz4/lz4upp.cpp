#include "lz4.h"

#define LLOG(x) // LOG(x)

namespace Upp {

enum {
	LZ4F_MAGIC       = 0x184D2204,

	LZ4F_VERSIONMASK = 0b11000000,
	LZ4F_VERSION     = 0b01000000,
	LZ4F_BLOCKINDEPENDENCE = (1 << 5),
	LZ4F_BLOCKCHECKSUM     = (1 << 4),
	LZ4F_CONTENTSIZE       = (1 << 3),
	LZ4F_CONTENTCHECKSUM   = (1 << 2),
	
	LZ4F_MAXSIZEMASK      = 0x70,
    LZ4F_MAXSIZE_64KB     = 0x40,
    LZ4F_MAXSIZE_256KB    = 0x50,
    LZ4F_MAXSIZE_1024KB   = 0x60,
    LZ4F_MAXSIZE_4096KB   = 0x70,
};

void Lz4::Init()
{
	error = false;
	pos = 0;
	header = false;
	xxh.Reset();
}

void Lz4::Compress()
{
	compress = 1;
	buffer.Alloc(BLOCK_BYTES);
	Init();
}

void Lz4::Decompress()
{
	compress = 0;
	header_data.Clear();
	Init();
}

void Lz4::PutOut(const void *ptr, int size)
{
	LLOG("LZ4 PutOut " << out.GetCount());
	out.Cat((const char *)ptr, (int)size);
}

void Lz4::FlushOut()
{
	if(!header) {
		byte h[7];
		Poke32le(h, LZ4F_MAGIC);
		h[4] = LZ4F_VERSION | LZ4F_BLOCKINDEPENDENCE | LZ4F_CONTENTCHECKSUM;
		h[5] = LZ4F_MAXSIZE_1024KB;
		h[6] = xxHash(h + 4, 2) >> 8;
		WhenOut(h, 7);
		header = true;
		maxblock = BLOCK_BYTES;
		outbuf.Alloc(4 + LZ4_compressBound(maxblock));
	}
	
	if(!pos)
		return;

	int clen;
	{
		RTIMING("Compress");
		clen = LZ4_compress(buffer, ~outbuf + 4, pos);
	}
	if(clen < 0) {
		error = true;
		return;
	}
	xxh.Put(buffer, pos);
	if(clen >= pos) {
		Poke32le(~outbuf, 0x80000000 | pos);
		memcpy(~outbuf + 4, buffer, pos);
		WhenOut(~outbuf, pos + 4);
	}
	else {
		Poke32le(~outbuf, clen);
		WhenOut(~outbuf, clen + 4);
	}
	pos = 0;
}

void Lz4::End()
{
	ASSERT(compress >= 0);
	if(compress) {
		FlushOut();
		byte h[8];
		Poke32le(h, 0);
		Poke32le(h + 4, xxh.Finish());
		WhenOut(h, 8);
	}
	else
		if(pos)
			error = true;
}

void Lz4::TryHeader()
{
	if(header_data.GetCount() < 7)
		return;

	if(Peek32le(~header_data) != LZ4F_MAGIC) {
		error = true;
		return;
	}
	lz4hdr = header_data[4];
	if((lz4hdr & LZ4F_VERSIONMASK) != LZ4F_VERSION) {
		error = true;
		return;
	}
	if(!(lz4hdr & LZ4F_BLOCKINDEPENDENCE)) {
		error = true; // dependent blocks not supported
		return;
	}
	maxblock = header_data[5];
	maxblock = decode(maxblock & LZ4F_MAXSIZEMASK,
	                  LZ4F_MAXSIZE_64KB, 1024 * 64,
	                  LZ4F_MAXSIZE_256KB, 1024 * 256,
	                  LZ4F_MAXSIZE_1024KB, 1024 * 1024,
	                  LZ4F_MAXSIZE_4096KB, 1024 * 4096,
	                  -1);
	if(maxblock < 0) {
		error = true;
		return;
	}
	
	if((lz4hdr & LZ4F_CONTENTSIZE) && header_data.GetCount() < 15)
		return; // need to skip content length

	header = true;
	blockchksumsz = 4 * !!(lz4hdr & LZ4F_BLOCKCHECKSUM);
	buffer.Alloc(maxblock + 4 + blockchksumsz);
	outbuf.Alloc(maxblock);
}

void Lz4::Put(const void *ptr_, int size)
{
	LLOG("Put " << size);

	ASSERT(compress >= 0);
	
	if(error)
		return;
	
	const char *ptr = reinterpret_cast<const char *>(ptr_);

	if(compress) {
		while(size > 0) {
			if(error)
				return;
			int n = BLOCK_BYTES - pos;
			if(size >= n) {
				memcpy(~buffer + pos, ptr, n);
				pos = BLOCK_BYTES;
				FlushOut();
				size -= n;
				ptr += n;
			}
			else {
				memcpy(buffer + pos, ptr, size);
				pos += size;
				break;
			}
		}
	}
	else {
		while(!header && size > 0 && !error) {
			header_data.Cat(*ptr++);
			size--;
			TryHeader();
		}
		while(size > 0 && !error) { // TODO might try to avoid memcpy
			if(pos < 4) { // Get length first
				int n = min(size, 4 - pos);
				memcpy(~buffer + pos, ptr, n);
				pos += n;
				ptr += n;
				size -= n;
			}
			else {
				int count = Peek32le(~buffer);
				int len = count & 0x7fffffff;
				if(len > maxblock)
					error = true;
				else
				if(count == 0) {
					int need = 4 - pos + 4;
					if(size >= need) { // we have enough data for final checksum
						memcpy(~buffer + pos, ptr, need);
						if(Peek32le(~buffer + 4) != xxh.Finish()) {
							error = true;
							return;
						}
						size -= need;
						ptr += need;
						pos = 0;
					}
				}
				else {
					int need = len - (pos - 4 - blockchksumsz);
					if(size >= need) { // we have enough data to finish the block
						memcpy(~buffer + pos, ptr, need);
						const char *src = ~buffer + 4;
						if(count & 0x80000000) {
							WhenOut(src, len);
							xxh.Put(src, len);
						}
						else {
							int sz;
							{
								RTIMING("Decompress");
								sz = LZ4_decompress_safe(src, outbuf, len, maxblock);
							}
							if(sz < 0) {
								error = true;
								return;
							}
							WhenOut(outbuf, sz);
							xxh.Put(outbuf, sz);
						}
						ptr += need;
						size -= need;
						pos = 0;
					}
					else {
						memcpy(~buffer + pos, ptr, size);
						pos += size;
						break;
					}
				}
			}
		}
	}
}

Lz4::Lz4()
{
	compress = -1;
	
	WhenOut = callback(this, &Lz4::PutOut);
}

Lz4::~Lz4()
{
}

bool IsLZ4(Stream& s)
{
	int64 pos = s.GetPos();
	bool b = s.Get32le() == LZ4F_MAGIC;
	s.Seek(pos);
	return b;
}

};
