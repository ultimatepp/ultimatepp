#include "zstd.h"

#define LLOG(x) // LOG(x)

namespace Upp {

namespace Legacy {

void ZstdDecompressStream::Init()
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
	ClearError();
}

bool ZstdDecompressStream::Open(Stream& in_)
{
	Init();
	in = &in_;
	return true;
}

bool ZstdDecompressStream::Next()
{
	pos += ptr - buffer;
	ptr = rdlim = buffer;
	if(ii < count) {
		const Workblock& w = wb[ii++];
		ptr = (byte *)~w.d;
		rdlim = ptr + w.dlen;
		Stream::buffer = ptr;
		return true;
	}
	return false;
}

void ZstdDecompressStream::Fetch()
{
	if(Next())
		return;
	if(eof)
		return;
#ifdef _MULTITHREADED
	CoWork co;
#endif
	bool   error = false;
	ii = 0;
	count = concurrent ? 16 : 1;
	int osz = (int)ZSTD_compressBound(BLOCK_BYTES);
	for(int i = 0; i < count; i++) {
		Workblock& w = wb[i];
		if(in->IsEof()) { // This is EOF
			eof = true;
			count = i;
			break;
		}
		
		if(!w.c)
			w.c.Alloc(osz);
		if(!w.d || w.irregular_d) {
			w.d.Alloc(BLOCK_BYTES);
			w.irregular_d = false;
		}
		
		w.lc.Clear();
		
		char *t = w.c;
		char *te = ~w.c + osz;
	
		if(in->Get32le() != (int)ZSTD_MAGICNUMBER) {
			SetError();
			return;
		}
		int fhd = in->Get();
		if(fhd < 0) {
			SetError();
			return;
		}
		Poke32le(t, ZSTD_MAGICNUMBER);
		t += 4;
		*t++ = fhd;
	    dword dictID= fhd & 3;
	    dword directMode = (fhd >> 5) & 1;
	    dword const fcsId = fhd >> 6;
		static byte ZSTD_fcs_fieldSize[4] = { 0, 2, 4, 8 };
		static byte ZSTD_did_fieldSize[4] = { 0, 1, 2, 4 };
		int l = !directMode + ZSTD_did_fieldSize[dictID] + ZSTD_fcs_fieldSize[fcsId] + (directMode && !ZSTD_fcs_fieldSize[fcsId]);
		if(in->Get(t, l) != l) {
			SetError();
			return;
		}
		
		t += l;
		
		int64 h = ZSTD_getDecompressedSize(~w.c, t - ~w.c);
		if(h > 1024*1024*1024) {
			SetError();
			return;
		}
		
		w.dlen = (int)h;
		
		for(;;) {
			int blkhdr = 0;
			for(int i = 0; i < 3; i++) {
				int b = in->Get();
				if(b < 0) {
					SetError();
					return;
				}
				if(t && t == te) {
					w.lc.Cat(~w.c, t);
					t = NULL;
				}
				if(t)
					*t++ = b;
				else
					w.lc.Cat(b);
				blkhdr = (blkhdr << 8) | b;
			}
			
			int len = blkhdr & ((1 << 22) - 1);
			
			blkhdr >>= 22;
			
			if(blkhdr == 3) {
				w.clen = t ? int(t - ~w.c) : w.lc.GetCount();
				break;
			}
			if(blkhdr == 2)
				len = 1; // RLE, just single byte to repeat len times
			
			if(t && len > te - t) {
				w.lc.Cat(~w.c, t);
				t = NULL;
			}
			if(t) {
				if(in->Get(t, len) != len) {
					SetError();
					return;
				}
				t += len;
			}
			else {
				String h = in->Get(len);
				if(h.GetCount() != len) {
					SetError();
					return;
				}
				w.lc.Cat(h);
			}
		}

		auto decompress = [=] {
			Workblock& w = wb[i];
			if(w.dlen == 0) { // decompressed size is not known
				int n = 2*1024*1024;
				w.irregular_d = true;
				for(;;) {
					if(n >= 1024*1024*1024) {
						SetError();
						return;
					}
					w.d.Alloc(n);
					size_t len = ZSTD_decompress(~w.d, n, w.lc.GetCount() ? ~w.lc : ~w.c, w.clen);
					if(!ZSTD_isError(len)) {
						w.dlen = (int)len;
						break;
					}
					n += n;
				}
			}
			else {
				if(w.dlen > BLOCK_BYTES) {
					w.irregular_d = true;
					w.d.Alloc(w.dlen);
				}
				if(ZSTD_isError(ZSTD_decompress(~w.d, w.dlen, w.lc.GetCount() ? ~w.lc : ~w.c, w.clen))) {
					SetError();
					return;
				}
			}
		};
		
#ifdef _MULTITHREADED
		if(concurrent)
			co & decompress;
		else
#endif
			decompress();
	}
#ifdef _MULTITHREADED
	if(concurrent)
		co.Finish();
#endif
	if(error)
		SetError();
	else
		Next();
}

bool ZstdDecompressStream::IsOpen() const
{
	return in->IsOpen() && !IsError();
}

int ZstdDecompressStream::_Term()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr;
}

int ZstdDecompressStream::_Get()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr++;
}

dword ZstdDecompressStream::_Get(void *data, dword size)
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

ZstdDecompressStream::ZstdDecompressStream()
{
	style = STRM_READ|STRM_LOADING;
	in = NULL;
	concurrent = false;
}

ZstdDecompressStream::~ZstdDecompressStream()
{
}

bool IsZstd(Stream& s)
{
	int64 pos = s.GetPos();
	bool b = (dword)s.Get32le() == 0xFD2FB527;
	s.Seek(pos);
	return b;
}

};

};
