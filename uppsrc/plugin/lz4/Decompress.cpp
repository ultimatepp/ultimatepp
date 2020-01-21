#include "lz4.h"

#define LLOG(x) // LOG(x)

namespace Upp {

void LZ4DecompressStream::Init()
{
	for(int i = 0; i < 16; i++)
		wb[i].Clear();
	ii = 0;
	count = 0;
	dlen = 0;
	pos = 0;
	eof = false;
	static byte h;
	ptr = rdlim = buffer = &h;
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

bool LZ4DecompressStream::Next()
{
	pos += ptr - buffer;
	ptr = rdlim = buffer;
	if(ii < count) {
		ptr = (byte *)~wb[ii].d;
		Stream::buffer = ptr;
		rdlim = ptr + wb[ii].dlen;
		ii++;
		return true;
	}
	return false;
}


void LZ4DecompressStream::Fetch()
{
	if(Next())
		return;
	if(eof)
		return;
	CoWork co;
	bool   error = false;
	bool last = false;
	ii = 0;
	count = concurrent ? 16 : 1;
	for(int i = 0; i < count; i++) {
		Workblock& t = wb[i];
		int blksz = in->Get32le();
		if(blksz == 0) { // This is EOF
			last = true;
			count = i;
			break;
		}
		t.clen = blksz & 0x7fffffff;
		if(t.clen > maxblock) {
			SetError();
			return;
		}
		if(!t.c) {
			t.c.Alloc(maxblock);
			t.d.Alloc(maxblock);
		}
		if(blksz & 0x80000000) { // block is not compressed
			t.dlen = t.clen;
			if(!in->GetAll(~t.d, t.clen)) {
				SetError();
				return;
			}
		}
		else {
			if(!in->GetAll(~t.c, t.clen)) {
				SetError();
				return;
			}
			if(concurrent)
				co & [=, &error] {
					Workblock& t = wb[i];
					t.dlen = LZ4_decompress_safe(~t.c, ~t.d, t.clen, maxblock);
					CoWork::FinLock();
					if(t.dlen < 0)
						error = true;
				};
			else {
				t.dlen = LZ4_decompress_safe(~t.c, ~t.d, t.clen, maxblock);
				if(t.dlen < 0)
					error = true;
			}
		}
		if(lz4hdr & LZ4F_BLOCKCHECKSUM)
			in->Get32le(); // just skip it
	}
	if(concurrent)
		co.Finish();
	if(error)
		SetError();
	else {
		for(int i = 0; i < count; i++)
			xxh.Put(wb[i].d, wb[i].dlen);
		if(last) {
			if(in->Get32le() != xxh.Finish())
				SetError();
			eof = true;
		}
		Next();
	}
}

bool LZ4DecompressStream::IsOpen() const
{
	return in->IsOpen() && !IsError();
}

int LZ4DecompressStream::_Term()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr;
}

int LZ4DecompressStream::_Get()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr++;
}

dword LZ4DecompressStream::_Get(void *data, dword size)
{
	byte *t = (byte *)data;
	while(size) {
		if(Ended())
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
			ptr = rdlim;
			Fetch();
		}
	}
	
	return dword(t - (byte *)data);
}

LZ4DecompressStream::LZ4DecompressStream()
{
	style = STRM_READ|STRM_LOADING;
	in = NULL;
	concurrent = false;
}

LZ4DecompressStream::~LZ4DecompressStream()
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
