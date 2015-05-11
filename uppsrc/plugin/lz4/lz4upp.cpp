#include "lz4.h"

#define LLOG(x) // LOG(x)

namespace Upp {

void Lz4::Init()
{
	error = false;
	cbi = pos = 0;
	Zero(lz4Stream);
	Zero(lz4StreamDecode);
}

void Lz4::Compress()
{
	compress = 1;
	Init();
}

void Lz4::Decompress()
{
	compress = 0;
	Init();
}

void Lz4::PutOut(const void *ptr, int size)
{
	LLOG("LZ4 PutOut " << out.GetCount());
	out.Cat((const char *)ptr, (int)size);
}

void Lz4::FlushOut()
{
	if(!pos)
		return;
	LLOG("To compress " << pos << ' ' << MD5String(block[cbi], pos) << ' ' << cbi);
	int count = LZ4_compress_continue(lz4Stream, block[cbi], ~buffer + 4, pos);
	if(count < 0) {
		error = true;
		return;
	}
	Poke32le(~buffer, count);
	LLOG("WhenOut " << pos << ": " << count);
	WhenOut(~buffer, count + 4);
	cbi = !cbi;
	pos = 0;
}

void Lz4::End()
{
	ASSERT(compress >= 0);
	if(compress)
		FlushOut();
	else
		if(pos)
			error = true;
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
				memcpy(block[cbi] + pos, ptr, n); // TODO: Avoid unnecessary memcpy
				pos = BLOCK_BYTES;
				FlushOut();
				size -= n;
				ptr += n;
			}
			else {
				memcpy(block[cbi] + pos, ptr, size);
				pos += size;
				break;
			}
		}
	}
	else {
		while(size > 0) { // TODO might try to avoid memcpy
			if(pos < 4) { // Get length first
				int n = min(size, 4 - pos);
				memcpy(~buffer + pos, ptr, n);
				pos += n;
				ptr += n;
				size -= n;
			}
			if(pos < 4)
				return;
			int count = Peek32le(~buffer);
			int need = count - (pos - 4);
			if(size >= need) { // we have enough data to finish the block
				memcpy(~buffer + pos, ptr, need);
				int out_count = LZ4_decompress_safe_continue(lz4StreamDecode, ~buffer + 4, block[cbi], count, BLOCK_BYTES);
				if(out_count <= 0) {
					error = true;
					return;
				}
				WhenOut(block[cbi], out_count);
				cbi = !cbi;
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

Lz4::Lz4()
{
	block[0] = new char[2 * BLOCK_BYTES];
	block[1] = block[0] + BLOCK_BYTES;
	buffer.Alloc(LZ4_COMPRESSBOUND(BLOCK_BYTES) + 4);
	
	compress = -1;
	
	WhenOut = callback(this, &Lz4::PutOut);
}

Lz4::~Lz4()
{
	delete[] block[0];
}

};
