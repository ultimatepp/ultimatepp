#include "lz4.h"

#define LLOG(x) // LOG(x)

namespace Upp {

void LZ4DecompressStream::Init()
{
	pos = 0;
	eof = false;
	buffer.Clear();
	ptr = rdlim = (byte *)~buffer;
	xxh.Reset();
	ClearError();
}

bool LZ4DecompressStream::Open(Stream& in_)
{
	Init();

	in = &in_;
	String header_data = in->Get(7);
	if(header_data.GetCount() < 7) {
		SetError();
		return false;
	}

	if(Peek32le(~header_data) != LZ4F_MAGIC) {
		SetError();
		return false;
	}
	lz4hdr = header_data[4];
	if((lz4hdr & LZ4F_VERSIONMASK) != LZ4F_VERSION) {
		SetError();
		return false;
	}
	if(!(lz4hdr & LZ4F_BLOCKINDEPENDENCE)) { // dependent blocks not supported
		SetError();
		return false;
	}
	maxblock = header_data[5];
	maxblock = decode(maxblock & LZ4F_MAXSIZEMASK,
	                  LZ4F_MAXSIZE_64KB, 1024 * 64,
	                  LZ4F_MAXSIZE_256KB, 1024 * 256,
	                  LZ4F_MAXSIZE_1024KB, 1024 * 1024,
	                  LZ4F_MAXSIZE_4096KB, 1024 * 4096,
	                  -1);
	if(maxblock < 0) {
		SetError();
		return false;
	}
	
	if((lz4hdr & LZ4F_CONTENTSIZE) && in->Get(8).GetCount() != 8) {
		SetError();
		return false;
	}

	return true;
}

String LZ4DecompressStream::Read(int& blksz)
{
	if(IsError() || in->IsError())
		return Null;
	blksz = in->Get32le();
	if(blksz == 0) // This is EOF
		return Null;
	int len = blksz & 0x7fffffff;
	if(len > maxblock) {
		SetError();
		return Null;
	}
	String data = in->GetAll(len);
	if(IsNull(data)) {
		SetError();
		return Null;
	}
	if(lz4hdr & LZ4F_BLOCKCHECKSUM)
		in->Get32le();
	return data;
}

String LZ4DecompressStream::Fetch()
{
	RTIMING("Fetch");
	int blksz;
	String data = Read(blksz);
	if((blksz & 0x80000000) || IsNull(data))
		return data;
	StringBuffer b(maxblock);
	int sz = LZ4_decompress_safe(~data, ~b, data.GetCount(), maxblock);
	if(sz < 0) {
		SetError();
		return 0;
	}
	if(sz <= 0)
		return Null;
	b.SetLength(sz);
	return b;
}

bool LZ4DecompressStream::IsOpen() const
{
	return in->IsOpen() && !IsError();
}

void LZ4DecompressStream::CheckEof()
{
	if(!eof) {
		if(in->Get32le() != xxh.Finish())
			SetError();
		eof = true;
	}
}

void LZ4DecompressStream::NewBuffer(const String& s)
{
	RTIMING("NewBuffer");
	pos += buffer.GetCount();
	buffer = s;
	ptr = (byte *)buffer.begin();
	rdlim = (byte *)buffer.end();
	{ RTIMING("XXH");
	xxh.Put(s, s.GetCount()); }
	if(ptr == rdlim)
		CheckEof();
}

int LZ4DecompressStream::_Term()
{
	if(eof)
		return -1;
	NewBuffer(Fetch());
	return ptr == rdlim ? -1 : *ptr;
}

int LZ4DecompressStream::_Get()
{
	if(eof)
		return -1;
	NewBuffer(Fetch());
	return ptr == rdlim ? -1 : *ptr++;
}

int64 copied;
EXITBLOCK { RDUMP(copied); }

dword LZ4DecompressStream::_Get(void *data, dword size)
{
	RTIMING("_Get");
	byte *t = (byte *)data;
	while(size) {
		if(IsError() || in->IsError() || IsEof())
			break;
		dword n = dword(rdlim - ptr);
		if(size < n) {
			{ RTIMING("memcpy1");
			memcpy(t, ptr, size); }
			t += size;
			ptr += size;
			break;
		}
		else {
			{ RTIMING("memcpy2"); copied += n;
			memcpy(t, ptr, n); }
			t += n;
			size -= n;
			NewBuffer(Fetch());
		}
	}
	
	return dword(t - (byte *)data);
}

LZ4DecompressStream::LZ4DecompressStream()
{
	in = NULL;
}

LZ4DecompressStream::~LZ4DecompressStream()
{
}

};
