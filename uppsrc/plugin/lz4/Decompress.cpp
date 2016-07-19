#include "lz4.h"

#define LLOG(x) // LOG(x)

namespace Upp {

void LZ4DecompressStream::Init()
{
	pos = 0;
	eof = false;
	buffer.Clear();
	xxh.Reset();
	outblock = inblock = 0;
	ClearError();
}

bool LZ4DecompressStream::Open(Stream& in_)
{
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

int LZ4DecompressStream::Fetch(char *t, int size)
{
	if(IsError() || in->IsError())
		return 0;
	int blksz = in->Get32le();
	if(blksz == 0) // This is EOF
		return 0;
	int len = blksz & 0x7fffffff;
	if(len > maxblock || len > size) {
		SetError();
		return 0;
	}
	if(blksz & 0x80000000) {
		if(in->Get(t, len) != len) {
			SetError();
			return 0;
		}
		return len;
	}
	String data = in->Get(len);
	if(data.GetCount() != len) {
		SetError();
		return 0;
	}
	int sz = LZ4_decompress_safe(~data, t, len, maxblock);
	if(sz < 0) {
		SetError();
		return 0;
	}
	if(lz4hdr & LZ4F_BLOCKCHECKSUM)
		in->Get32le();
	return sz;
}

String LZ4DecompressStream::Fetch()
{
	StringBuffer b(maxblock);
	int sz = Fetch(~b, maxblock);
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
	pos += buffer.GetCount();
	buffer = s;
	ptr = (byte *)buffer.begin();
	rdlim = (byte *)buffer.end();
	xxh.Put(s, s.GetCount());
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

dword LZ4DecompressStream::_Get(void *data, dword size)
{
	byte *t = (byte *)data;
	while(size) {
		if(IsError() || in->IsError() || IsEof())
			break;
		dword n = dword(rdlim - ptr);
		if(size < n) {
			memcpy(t, ptr, size);
			t += size;
			ptr += size;
			break;
		}
		else {
			memcpy(t, ptr, n);
			t += n;
			size -= n;
			if(size > (dword)maxblock) {
				pos += buffer.GetCount();
				buffer.Clear();
				int sz = Fetch((char *)t, size);
				xxh.Put(t, sz);
				if(sz == 0)
					CheckEof();
				size -= sz;
				pos += sz;
				t += sz;
			}
			else
				NewBuffer(Fetch());
		}
	}
	
	return t - (byte *)data;
}

LZ4DecompressStream::LZ4DecompressStream()
{
	parallel = false;
	in = NULL;
}

LZ4DecompressStream::~LZ4DecompressStream()
{
}

};
