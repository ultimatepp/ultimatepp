#include "Core.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

NAMESPACE_UPP

#define LLOG(x) // RLOG(x)
#define LDUMP(x) // RDUMP(x)
#define LLOGHEXDUMP(x, y) // RLOGHEXDUMP(x, y)

void Stream::_Put(const void *data, dword size) {
	const byte *s = (const byte *) data;
	while(size--)
		Put(*s++);
}

dword Stream::_Get(void *data, dword size) {
	int c;
	byte *s = (byte *) data;
	dword sz;
	for(sz = 0; sz < size && (c = Get()) >= 0; sz++)
		*s++ = c;
	return sz;
}

void Stream::_Put(int w) {
	SetError(ERROR_NOT_ENOUGH_SPACE);
}

int  Stream::_Get() {
	return -1;
}

int  Stream::_Term() {
	return -1;
}

void Stream::Seek(int64) {
	NEVER();
}

int64 Stream::GetSize() const {
	return 0;
}

void Stream::SetSize(int64) {
	NEVER();
}

bool Stream::IsOpen() const { return false; }

void Stream::Close() {}

void Stream::Flush() {}

Stream::Stream() {
	pos = style = 0;
	buffer = NULL;
	ptr = rdlim = wrlim = NULL;
}

Stream::~Stream() {}

bool Stream::_IsEof() const
{
	return GetPos() >= GetSize();
}

void Stream::LoadError() {
	SetError(ERROR_LOADING_FAILED);
	if(style & STRM_THROW)
		throw LoadingError();
}

bool Stream::GetAll(void *data, dword size) {
	if(Get(data, size) != size) {
		LoadError();
		return false;
	}
	return true;
}

String Stream::Get(dword size)
{
	StringBuffer b(size);
	int n = Get(~b, size);
	b.SetCount(n);
	String s = b;
	s.Trim(n);
	return s;
}

int  Stream::_Get8()
{
	int c = Get();
	if(c < 0) {
		LoadError();
		return -1;
	}
	return c;
}

int  Stream::_Get16() {
	word w;
	return GetAll(&w, 2) ? w : -1;
}

int  Stream::_Get32() {
	int l;
	return GetAll(&l, 4) ? l : -1;
}

int64  Stream::_Get64() {
	int64 l;
	return GetAll(&l, 8) ? l : -1;
}

#ifdef CPU_LE
int  Stream::Get16be() {
	byte h[2];
	int q;
	h[1] = Get();
	h[0] = q = Get();
	if(q < 0) {
		LoadError();
		return -1;
	}
	return *(word *)h;
}

int Stream::Get32be() {
	byte h[4];
	int q;
	h[3] = Get();
	h[2] = Get();
	h[1] = Get();
	h[0] = q = Get();
	if(q < 0) {
		LoadError();
		return -1;
	}
	return *(int32 *)h;
}

int64 Stream::Get64be() {
	byte h[8];
	int q;
	h[7] = Get();
	h[6] = Get();
	h[5] = Get();
	h[4] = Get();
	h[3] = Get();
	h[2] = Get();
	h[1] = Get();
	h[0] = q = Get();
	if(q < 0) {
		LoadError();
		return -1;
	}
	return *(int64 *)h;
}
#else
int  Stream::Get16le() {
	byte h[2];
	int q;
	h[0] = Get();
	h[1] = q = Get();
	return q < 0 ? -1 : *(word *)h;
}

int Stream::Get32le() {
	byte h[4];
	int q;
	h[0] = Get();
	h[1] = Get();
	h[2] = Get();
	h[3] = q = Get();
	return q < 0 ? -1 : *(int32 *)h;
}

int64 Stream::Get64le() {
	byte h[8];
	int q;
	h[0] = Get();
	h[1] = Get();
	h[2] = Get();
	h[3] = Get();
	h[4] = Get();
	h[5] = Get();
	h[6] = Get();
	h[7] = q = Get();
	return q < 0 ? -1 : *(int64 *)h;
}
#endif

int Stream::GetUtf8()
{
	int code = Get();
	if(code <= 0) {
		LoadError();
		return -1;
	}
	if(code < 0x80)
		return code;
	else
	if(code < 0xC2)
		return -1;
	else
	if(code < 0xE0) {
		if(IsEof()) {
			LoadError();
			return -1;
		}
		return ((code - 0xC0) << 6) + Get() - 0x80;
	}
	else
	if(code < 0xF0) {
		int c0 = Get();
		int c1 = Get();
		if(c1 < 0) {
			LoadError();
			return -1;
		}
		return ((code - 0xE0) << 12) + ((c0 - 0x80) << 6) + c1 - 0x80;
	}
	else
	if(code < 0xF8) {
		int c0 = Get();
		int c1 = Get();
		int c2 = Get();
		if(c2 < 0) {
			LoadError();
			return -1;
		}
		return ((code - 0xf0) << 18) + ((c0 - 0x80) << 12) + ((c1 - 0x80) << 6) + c2 - 0x80;
	}
	else
	if(code < 0xFC) {
		int c0 = Get();
		int c1 = Get();
		int c2 = Get();
		int c3 = Get();
		if(c3 < 0) {
			LoadError();
			return -1;
		}
		return ((code - 0xF8) << 24) + ((c0 - 0x80) << 18) + ((c1 - 0x80) << 12) +
		       ((c2 - 0x80) << 6) + c3 - 0x80;
	}
	else
	if(code < 0xFE) {
		int c0 = Get();
		int c1 = Get();
		int c2 = Get();
		int c3 = Get();
		int c4 = Get();
		if(c4 < 0) {
			LoadError();
			return -1;
		}
		return ((code - 0xFC) << 30) + ((c0 - 0x80) << 24) + ((c1 - 0x80) << 18) +
		       ((c2 - 0x80) << 12) + ((c3 - 0x80) << 6) + c4 - 0x80;

	}
	else {
		LoadError();
		return -1;
	}
}

String Stream::GetLine() {
	byte *q = ptr;
	while(q < rdlim)
		if(*q == '\n') {
			String result((const char *)ptr, (int)(uintptr_t)(q - ptr - (q > ptr && q[-1] == '\r')));
			ptr = q + 1;
			return result;
		}
		else
			q++;
	String result((const char *)ptr, (int)(uintptr_t)(q - ptr));
	ptr = q;
	for(;;) {
		byte *q = ptr;
		while(q < rdlim && *q != '\n')
			q++;
		result.Cat(ptr, (int)(uintptr_t)(q - ptr));
		ptr = q;
		int c = Get();
		if(c == '\n')
			break;
		if(c < 0) {
			if(result.GetCount() == 0)
				return String::GetVoid();
			break;
		}
		result.Cat(c);
	}
	if(*result.Last() == '\r')
		result.Trim(result.GetLength() - 1);
	return result;
}

#ifdef CPU_LE
void Stream::Put16be(word q) {
	byte *h = (byte *) &q;
	Put(h[1]);
	Put(h[0]);
}

void Stream::Put32be(dword q) {
	byte *h = (byte *) &q;
	Put(h[3]);
	Put(h[2]);
	Put(h[1]);
	Put(h[0]);
}

void Stream::Put64be(int64 q) {
	byte *h = (byte *) &q;
	Put(h[7]);
	Put(h[6]);
	Put(h[5]);
	Put(h[4]);
	Put(h[3]);
	Put(h[2]);
	Put(h[1]);
	Put(h[0]);
}
#else
void Stream::Put16le(word q) {
	byte *h = (byte *) &q;
	Put(h[0]);
	Put(h[1]);
}

void Stream::Put32le(dword q) {
	byte *h = (byte *) &q;
	Put(h[0]);
	Put(h[1]);
	Put(h[2]);
	Put(h[3]);
}

void Stream::Put64le(int64 q) {
	byte *h = (byte *) &q;
	Put(h[0]);
	Put(h[1]);
	Put(h[2]);
	Put(h[3]);
	Put(h[4]);
	Put(h[5]);
	Put(h[6]);
	Put(h[7]);
}
#endif

void Stream::PutUtf8(int c)
{
	word code = c;
	if(code < 0x80)
		Put(code);
	else
	if(code < 0x800) {
		Put(0xc0 | (code >> 6));
		Put(0x80 | (code & 0x3f));
	}
	else
	if((code & 0xFF00) == 0xEE00)
		Put(code);
	else {
		Put(0xe0 | (code >> 12));
		Put(0x80 | ((code >> 6) & 0x3f));
		Put(0x80 | (code & 0x3f));
	}
}

void Stream::Put(const char *s)
{
	while(*s) Put(*s++);
}

void Stream::Put(int c, int count) {

	while(count) {
		int n = min(count, (int)(intptr_t)(wrlim - ptr));
		if(n > 0) {
			memset(ptr, c, n);
			ptr += n;
			count -= n;
		}
		else {
			Put(c);
			count--;
		}
	}
}

void Stream::PutLine(const char *s) {
	Put(s);
	PutEol();
}

void Stream::PutLine(const String& s) {
	Put(s);
	PutEol();
}

void  Stream::Put(Stream& s, int64 size, dword click) {
	Buffer<byte> buffer(click);
	while(size) {
		dword n = s.Get(buffer, (int)min<int64>(click, size));
		Put(buffer.operator const byte *(), click);
		size -= n;
	}
}

void Stream::SerializeRLE(byte *data, dword size)
{
	if(IsError()) return;
	byte *s =   (byte *)data;
	byte *lim = s + size;
	if(IsLoading())
		while(s != lim) {
			if(IsEof() || s > lim) {
				LoadError();
				return;
			}
			byte c = Get();
			if(c == 0xcb) {
				c = Get();
				int n = Get();
				if(s + n > lim) {
					LoadError();
					return;
				}
				memset(s, c, n);
				s += n;
			}
			else
				*s++ = c;
		}
	else
		while(s < lim) {
			byte  c = *s;
			byte *t = s + 1;
			byte *lm = min(s + 250, lim);
			while(*t == c && t < lm)
				t++;
			if(t >= s + 3 || c == 0xcb) {
				Put(0xcb);
				Put(c);
				Put(byte(t - s));
			}
			else {
				Put(*s);

				if(t == s + 2)
					Put(*s);
			}
			if(IsError()) break;
			s = t;
		}
}

void Stream::SerializeRaw(byte *data, dword size) {
	if(IsError()) return;
	if(IsLoading())
		GetAll(data, size);
	else
		Put(data, size);
}

void Stream::SerializeRaw(word *data, dword count) {
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
	SerializeRaw((byte *)data, 2 * count);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
}

void Stream::SerializeRaw(dword *data, dword count) {
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
	SerializeRaw((byte *)data, 4 * count);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
}

void Stream::SerializeRaw(uint64 *data, dword count) {
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
	SerializeRaw((byte *)data, 8 * count);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
}

void Stream::Pack(dword& w) {
	if(IsError()) return;
	if(IsLoading()) {
		int q = Get();
		if(q < 0)
			LoadError();
		else {
			if(q != 255)
				w = q;
			else
				SerializeRaw(&w, 1);
		}
	}
	else {
		if(w < 255)
			Put(w);
		else {
			Put(255);
			SerializeRaw(&w, 1);
		}
	}
}

void    Stream::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g, bool& h) {
	if(IsError()) return;
	if(IsLoading()) {
		int f = Get();
		if(f < 0) LoadError();
		else {
			a = !!(f & 0x80);
			b = !!(f & 0x40);
			c = !!(f & 0x20);
			d = !!(f & 0x10);
			e = !!(f & 0x08);
			f = !!(f & 0x04);
			g = !!(f & 0x02);
			h = !!(f & 0x01);
		}
	}
	else {
		int f = 0;
		if(a) f |= 0x80;
		if(b) f |= 0x40;
		if(c) f |= 0x20;
		if(d) f |= 0x10;
		if(e) f |= 0x08;
		if(f) f |= 0x04;
		if(g) f |= 0x02;
		if(h) f |= 0x01;
		Put(f);
	}
}

void  Stream::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g) {
	bool h = false; Pack(a, b, c, d, e, f, g, h);
}

void  Stream::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f) {
	bool h = false; Pack(a, b, c, d, e, f, h, h);
}

void  Stream::Pack(bool& a, bool& b, bool& c, bool& d, bool& e) {
	bool h = false; Pack(a, b, c, d, e, h, h, h);
}

void  Stream::Pack(bool& a, bool& b, bool& c, bool& d) {
	bool h = false; Pack(a, b, c, d, h, h, h, h);
}

void  Stream::Pack(bool& a, bool& b, bool& c) {
	bool h = false; Pack(a, b, c, h, h, h, h, h);
}

void  Stream::Pack(bool& a, bool& b) {
	bool h = false; Pack(a, b, h, h, h, h, h, h);
}

#if 1
Stream& Stream::operator%(bool& d)
{
	SerializeRaw((byte *)&d, 1);
	return *this;
}

Stream& Stream::operator%(char& d)
{
	SerializeRaw((byte *)&d, 1);
	return *this;
}

Stream& Stream::operator%(signed char& d)
{
	SerializeRaw((byte *)&d, 1);
	return *this;
}

Stream& Stream::operator%(unsigned char& d)
{
	SerializeRaw((byte *)&d, 1);
	return *this;
}

Stream& Stream::operator%(short& d)
{
	SerializeRaw((word *)&d, 1);
	return *this;
}

Stream& Stream::operator%(unsigned short& d)
{
	SerializeRaw((word *)&d, 1);
	return *this;
}

Stream& Stream::operator%(int& d)
{
	SerializeRaw((dword *)&d, 1);
	return *this;
}

Stream& Stream::operator%(unsigned int& d)
{
	SerializeRaw((dword *)&d, 1);
	return *this;
}

Stream& Stream::operator%(long& d)
{
	SerializeRaw((dword *)&d, 1);
	return *this;
}

Stream& Stream::operator%(unsigned long& d)
{
	SerializeRaw((dword *)&d, 1);
	return *this;
}

Stream& Stream::operator%(float& d)
{
	SerializeRaw((dword *)&d, 1);
	return *this;
}

Stream& Stream::operator%(double& d)
{
	SerializeRaw((uint64 *)&d, 1);
	return *this;
}

Stream& Stream::operator%(int64& d)
{
	SerializeRaw((uint64 *)&d, 1);
	return *this;
}

Stream& Stream::operator%(uint64& d)
{
	SerializeRaw((uint64 *)&d, 1);
	return *this;
}

#else

Stream& Stream::operator%(bool& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(char& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(signed char& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(unsigned char& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(short& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(unsigned short& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(int& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(unsigned int& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(long& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(unsigned long& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(float& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(double& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(int64& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream& Stream::operator%(uint64& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}
#endif


Stream& Stream::operator%(String& s) {
	if(IsError()) return *this;
	if(IsLoading()) {
		dword len;
		len = Get();
		if(len != 0xff) {
			if(len & 0x80) {
				len &= 0x7f;
				Get(); // reserved for future use... or removal
			}
		}
		else {
			len = Get32le();
			if(len & 0x80000000) {
				len &= 0x7fffffff;
				Get(); // reserved for future use... or removal
			}
		}
		if(IsError() || len + GetPos() > GetSize())
			LoadError();
		else {
			StringBuffer sb(len);
			SerializeRaw((byte*)~sb, len);
			s = sb;
		}
	}
	else {
		dword len = s.GetLength();
		if(len < 127)
			Put(len);
		else {
			Put(0xff);
			Put32le(len);
		}
		SerializeRaw((byte *)~s, len);
	}
	return *this;
}

Stream& Stream::operator/(String& s) {
	if(IsError()) return *this;
	dword len = s.GetLength();
	Pack(len);
	if(IsLoading()) {
		StringBuffer b(len);
		SerializeRLE((byte *)~b, len);
		s = b;
	}
	else
		SerializeRLE((byte *)~s, len);
	return *this;
}

Stream& Stream::operator%(WString& s) {
	if(IsError()) return *this;
	if(IsLoading()) {
		dword len;
		Pack(len);
		if(IsError() || len + GetPos() > GetSize())
			LoadError();
		else {
			WStringBuffer sb(len);
			SerializeRaw((byte*)~sb, len * sizeof(wchar));
			s = sb;
		}
	}
	else {
		dword len = s.GetLength();
		if(len < 0xff)
			Put(len);
		else {
			Put(0xff);
			Put32le(len);
		}
		SerializeRaw((byte*)~s, len * sizeof(wchar));
	}
	return *this;
}

Stream& Stream::operator/(WString& s) {
	if(IsError()) return *this;
	String h = ToUtf8(s);
	*this / h;
	s = FromUtf8(h);
	return *this;
}

void Stream::Magic(dword magic) {
	dword a = magic;
	*this % a;
	if(magic != a) LoadError();
}

// -------------------------- String stream -----------------------------

void StringStream::SetWriteBuffer()
{
	buffer = (byte *)wdata.Begin();
	rdlim = buffer;
	wrlim = (byte *)wdata.End();
}

void StringStream::SetWriteMode()
{
	if(writemode) return;
	intptr_t p = ptr - buffer;
	size = data.GetLength();
	wdata = data;
	SetWriteBuffer();
	ptr = buffer + p;
	writemode = true;
}

void   StringStream::SetReadMode()
{
	if(!writemode) return;
	wdata.SetLength((dword)GetSize());
	dword p = (dword)(uintptr_t)(ptr - buffer);
	data = wdata;
	buffer = (byte *) ~data;
	ptr = buffer + p;
	wrlim = buffer;
	rdlim = buffer + data.GetCount();
	writemode = false;
}

void  StringStream::Open(const String& adata)
{
	pos = 0;
	data = adata;
	style = STRM_READ|STRM_WRITE|STRM_SEEK|STRM_LOADING;
	wdata.Clear();
	buffer = (byte *) ~data;
	ptr = wrlim = buffer;
	rdlim = buffer + data.GetCount();
	writemode = false;
	ClearError();
}

void  StringStream::Create()
{
	Open(String());
	SetStoring();
	SetWriteMode();
	ClearError();
}

int64 StringStream::GetSize() const
{
	return writemode ? max<int64>(GetPos(), size) : data.GetLength();
}

String StringStream::GetResult()
{
	SetReadMode();
	return data;
}

void  StringStream::_Put(const void *d, dword sz)
{
	SetWriteMode();
	if(ptr + sz >= wrlim) {
		intptr_t p = ptr - buffer;
	#ifdef _DEBUG
		wdata.SetLength(max(1, max(2 * (int)GetSize(), (int)GetSize() + (int)sz)));
	#else
		wdata.SetLength(max(128, max(2 * (int)GetSize(), (int)GetSize() + (int)sz)));
	#endif
		SetWriteBuffer();
		ptr = buffer + p;
	}
	memcpy(ptr, d, sz);
	ptr += sz;
}

void  StringStream::_Put(int w)
{
	byte h = w;
	_Put(&h, 1);
}

dword StringStream::_Get(void *data, dword sz)
{
	SetReadMode();
	dword read = min((dword)(uintptr_t)(rdlim - ptr), sz);
	memcpy(data, ptr, read);
	ptr += read;
	return read;
}

int  StringStream::_Get()
{
	SetReadMode();
	return ptr < rdlim ? *ptr++ : -1;
}

int  StringStream::_Term() {
	SetReadMode();
	return ptr < rdlim ? *ptr : -1;
}

void  StringStream::Seek(int64 pos) {
	size = (dword)GetSize();
	if(pos > size) {
		SetWriteMode();
		size = (dword)pos;
		wdata.SetLength((dword)pos + 100);
		SetWriteBuffer();
	}
	ptr = buffer + min(GetSize(), pos);
}

void  StringStream::SetSize(int64 asize) {
	SetWriteMode();
	dword p = (dword)(uintptr_t)GetPos();
	Seek(asize);
	size = (dword)asize;
	Seek(min(p, size));
}

bool  StringStream::IsOpen() const {
	return true;
}

// -------------------- Memory read-write stream ------------------------

void MemStream::Seek(int64 pos) {
	ptr = buffer + min(pos, int64(rdlim - buffer));
}

int64 MemStream::GetSize() const {
	return rdlim - buffer;
}

dword MemStream::_Get(void *data, dword size) {
	if(size > (dword)(intptr_t)(rdlim - ptr))
		size = (dword)(intptr_t)(rdlim - ptr);
	memcpy(data, ptr, size);
	ptr += size;
	return size;
}

void  MemStream::_Put(const void *data, dword size) {
	if(size > (dword)(uintptr_t)(wrlim - ptr)) {
		SetError(ERROR_NOT_ENOUGH_SPACE);
		return;
	}
	memcpy(ptr, data, size);
	ptr += size;
}

bool  MemStream::IsOpen() const {
	return true;
}

void MemStream::Create(void *data, int size)
{
	style = STRM_WRITE|STRM_READ|STRM_SEEK|STRM_LOADING;
	ptr = buffer = (byte *) data;
	wrlim = rdlim = buffer + size;
	pos = 0;
}

MemStream::MemStream(void *data, int size) {
	Create(data, size);
}

MemStream::MemStream() {}

#ifdef flagSO
MemStream::~MemStream() {}
#endif

// ----------------------- Memory read streamer -------------------------

void MemReadStream::Create(const void *data, int size)
{
	MemStream::Create((void *)data, size);
	style = STRM_READ|STRM_SEEK|STRM_LOADING;
	wrlim = buffer;
}

MemReadStream::MemReadStream(const void *data, int size)
{
	Create(data, size);
}

MemReadStream::MemReadStream() {}

// --------------------------- Size stream -----------------------

void SizeStream::Seek(int64 apos) {
	if(ptr - buffer + pos > size)
		size = ptr - buffer + pos;
	pos = apos;
	if(pos > GetSize())
		pos = GetSize();
	ptr = buffer;
}

int64 SizeStream::GetSize() const {
	return max(int64(ptr - buffer + pos), size);
}

void SizeStream::SetSize(int64 asize) {
	size = asize;
	if(ptr - buffer + pos > size) {
		ptr = buffer;
		pos = size;
	}
}

void SizeStream::_Put(const void *, dword sz) {
	wrlim = buffer + 128;
	pos += ptr - buffer + sz;
	ptr = buffer;
}

void SizeStream::_Put(int w) {
	_Put(NULL, 1);
}

bool SizeStream::IsOpen() const {
	return true;
}

SizeStream::SizeStream() {
	size = 0;
	style = STRM_WRITE|STRM_SEEK;
	buffer = h;
}

// ------------------------------ Compare stream ----------------------------

CompareStream::CompareStream() {
	stream = NULL;
	equal = false;
	size = 0;
	buffer = h;
}

CompareStream::CompareStream(Stream& astream) {
	stream = NULL;
	buffer = h;
	Open(astream);
}

void CompareStream::Open(Stream& astream) {
	ASSERT(astream.IsOpen());
	Close();
	style = STRM_WRITE|STRM_SEEK;
	stream = &astream;
	size = pos = 0;
	wrlim = buffer + 128;
	ptr = buffer;
	equal = true;
}

bool CompareStream::IsOpen() const {
	return !!stream;
}

int64 CompareStream::GetSize() const {
	return max(int64(ptr - buffer + pos), size);
}

void CompareStream::Close() {
	if(!stream) return;
	if(GetPos() > size)
		size = GetPos();
	Flush();
	if(stream->GetSize() != GetSize())
		equal = false;
	stream = NULL;
}

void CompareStream::SetSize(int64 asize) {
	Flush();
	pos += ptr - buffer;
	ptr = buffer;
	size = asize;
	if(pos > size)
		pos = size;
}

void CompareStream::Seek(int64 apos) {
	Flush();
	int64 sz = ptr - buffer + pos;
	if(sz > size)
		size = sz;
	pos = apos;
	ptr = buffer;
}

void CompareStream::Compare(int64 pos, const void *data, dword size) {
	ASSERT(stream);
	if(!size) return;
	Buffer<byte> b(size);
	if(stream->GetPos() != pos)
		stream->Seek(pos);
	if(stream->Get(b, size) != size || memcmp(b.operator const byte *(), data, size))
		equal = false;
}

void CompareStream::Flush() {
	Compare(pos, buffer, (int)(ptr - buffer));
}

void CompareStream::_Put(const void *data, dword size) {
	wrlim = buffer + 128;
	ASSERT(ptr <= wrlim);
	Flush();
	pos += ptr - buffer;
	ptr = buffer;
	byte *b = (byte *) data;
	while(size && equal) {
		int sz = min<int>(size, 128);
		Compare(pos, b, sz);
		pos += sz;
		b += sz;
		size -= sz;
	}
}

void CompareStream::_Put(int w) {
	byte b = w;
	_Put(&b, 1);
}

struct NilStreamClass : public Stream {
	virtual void    _Put(int w)    {}
	virtual bool    IsOpen() const { return true; }
	virtual   int   _Term()        { return -1; }
	virtual   int   _Get()         { return -1; }
};

Stream& NilStream()
{
	return Single<NilStreamClass>();
}

#ifndef PLATFORM_WINCE
class CoutStream : public Stream {
	void Put0(int w) {
#ifdef PLATFORM_WIN32
		static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		char s[1];
		s[0] = w;
		dword dummy;
		WriteFile(h, s, 1, &dummy, NULL);
#else
		putchar(w);
#endif
	}
	virtual void    _Put(int w) {
		if(w == '\n') {
#ifdef PLATFORM_WIN32
			Put0('\r');
#endif
			Put0('\n');
		}
		else
		if(w != '\r')
			Put0(w);
	}
	virtual   bool  IsOpen() const { return true; }
};

Stream& Cout()
{
	return Single<CoutStream>();
}

class CerrStream : public Stream {
	virtual void    _Put(int w) {
	#ifdef PLATFORM_WIN32
		static HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
		char s[1];
		s[0] = w;
		dword dummy;
		WriteFile(h, s, 1, &dummy, NULL);
	#else
		putc(w, stderr);
	#endif
	}
	virtual   bool  IsOpen() const { return true; }
};

Stream& Cerr()
{
	return Single<CerrStream>();
}
#endif

static int sMappingGranularity_()
{
#ifdef PLATFORM_WIN32
	static int mg = 0;
	if(!mg) {
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		mg = info.dwAllocationGranularity;
	}
#else
	static int mg = 4096;
#endif
	return mg;
}

FileMapping::FileMapping(const char *file_, bool delete_share_)
{
#ifdef PLATFORM_WIN32
	hfile = INVALID_HANDLE_VALUE;
	hmap = NULL;
#endif
#ifdef PLATFORM_POSIX
	hfile = -1;
	Zero(hfstat);
#endif
	base = rawbase = NULL;
	size = rawsize = 0;
	offset = rawoffset = 0;
	filesize = -1;
	write = false;
	if(file_)
		Open(file_, delete_share_);

}

bool FileMapping::Open(const char *file, bool delete_share)
{
	Close();
	write = false;
#ifdef PLATFORM_WIN32
	if(IsWinNT())
		hfile = UnicodeWin32().CreateFileW(ToSystemCharsetW(file), GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE | (delete_share && IsWinNT() ? FILE_SHARE_DELETE : 0),
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	else
		hfile = CreateFile(ToSystemCharset(file), GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE | (delete_share && IsWinNT() ? FILE_SHARE_DELETE : 0),
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hfile == INVALID_HANDLE_VALUE)
		return false;
	filesize = ::GetFileSize(hfile, NULL);
	hmap = CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(!hmap) {
		Close();
		return false;
	}
#endif
#ifdef PLATFORM_POSIX
	hfile = open(ToSystemCharset(file), O_RDONLY);
	if(hfile == -1)
		return false;
	if(fstat(hfile, &hfstat) == -1) {
		Close();
		return false;
	}
	filesize = hfstat.st_size;
#endif
	return true;
}

bool FileMapping::Create(const char *file, int64 filesize_, bool delete_share)
{
	Close();
	write = true;
#ifdef PLATFORM_WIN32
	if(IsWinNT())
		hfile = UnicodeWin32().CreateFileW(ToSystemCharsetW(file), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE | (delete_share ? FILE_SHARE_DELETE : 0),
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	else
		hfile = CreateFile(ToSystemCharset(file), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE | (delete_share ? FILE_SHARE_DELETE : 0),
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hfile == INVALID_HANDLE_VALUE)
		return false;
	hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if(!hmap) {
		Close();
		return false;
	}
#endif
#ifdef PLATFORM_POSIX
	hfile = open(ToSystemCharset(file), O_RDWR | O_CREAT);
	if(hfile == -1)
		return false;
#endif
	filesize = filesize_;
	return true;
}

bool FileMapping::Map(int64 mapoffset, dword maplen)
{
	ASSERT(IsOpen());
	if(maplen == 0)
		return Unmap();
	mapoffset = minmax<int64>(mapoffset, 0, filesize);
	int gran = sMappingGranularity_();
	int64 rawoff = mapoffset & -gran;
	maplen = (dword)min<int64>(maplen, filesize - mapoffset);
	dword rawsz = (dword)min<int64>((maplen + (dword)(mapoffset - rawoff) + gran - 1) & -gran, filesize - rawoff);
	if(rawbase && (mapoffset < rawoffset || mapoffset + maplen > rawoffset + rawsize))
		Unmap();
	if(!rawbase) {
		rawoffset = rawoff;
		rawsize = rawsz;
#ifdef PLATFORM_WIN32
		rawbase = (byte *)MapViewOfFile(hmap, /*write ? FILE_MAP_WRITE :*/ FILE_MAP_READ,
			(dword)(rawoffset >> 32), (dword)(rawoffset >> 0), rawsize);
#else
		rawbase = (byte *)mmap(0, rawsize, PROT_READ | PROT_WRITE,
#ifdef PLATFORM_FREEBSD
			MAP_NOSYNC,
#else
			MAP_SHARED,
#endif
			hfile, (dword)rawoffset);
#endif
		if(!rawbase)
			return false;
	}
	offset = mapoffset;
	size = maplen;
	base = rawbase + (int)(offset - rawoffset);
	return true;
}

bool FileMapping::Unmap()
{
	bool ok = true;
	if(rawbase) {
#ifdef PLATFORM_WIN32
		ok = !!UnmapViewOfFile(rawbase);
#endif
#ifdef PLATFORM_POSIX
		ok = (munmap((void *)rawbase, rawsize) == 0);
#endif
	}
	base = rawbase = NULL;
	size = 0;
	return ok;
}

bool FileMapping::Expand(int64 new_filesize)
{
	ASSERT(IsOpen());
	if(new_filesize > filesize) {
		if(!Unmap())
			return false;
#ifdef PLATFORM_WIN32
		if(!CloseHandle(hmap)) {
			hmap = NULL;
			return false;
		}
		hmap = NULL;
#endif
#ifdef PLATFORM_POSIX
		if(FTRUNCATE64_(hfile, (dword)(new_filesize - filesize)) != 0) {
			Close();
			return false;
		}
#endif
		filesize = new_filesize;
	}
	return true;
}

bool FileMapping::Close()
{
	bool ok = Unmap();
#ifdef PLATFORM_WIN32
	if(hmap) {
		if(!CloseHandle(hmap)) ok = false;
		hmap = NULL;
	}
	if(IsOpen()) {
		if(!CloseHandle(hfile)) ok = false;
		hfile = INVALID_HANDLE_VALUE;
	}
#endif
#ifdef PLATFORM_POSIX
	if(IsOpen()) {
		if(close(hfile) != 0) ok = false;
		Zero(hfstat);
		hfile = -1;
	}
#endif
	filesize = -1;
	offset = 0;
	size = 0;
	write = false;
	return ok;
}

Time FileMapping::GetTime() const
{
	ASSERT(IsOpen());
#ifdef PLATFORM_WIN32
	FileTime ft;
	GetFileTime(hfile, NULL, NULL, &ft);
	return ft;
#endif
#ifdef PLATFORM_POSIX
	return Time(hfstat.st_mtime);
#endif
}

String FileMapping::GetData(int64 offset, dword len)
{
	if(IsOpen() && Map(offset, len))
		return String(base, len);
	else {
		NEVER();
		return String::GetVoid();
	}
}

String ReadStdIn()
{
	String r;
	for(;;) {
		int c = getchar();
		if(c < 0 || c == '\n')
			return r;
		r.Cat(c);
	}
}

// ---------------------------------------------------------------------------

String LoadStream(Stream& in) {
	if(in.IsOpen()) {
		in.ClearError();
		int size = (int)in.GetLeft();
		StringBuffer s(size);
		if((dword)size != 0xffffffff)
			in.Get(s, size);
		if(!in.IsError())
			return s;
	}
	return String::GetVoid();
}

String LoadFile(const char *filename) {
	FileIn in(filename);
	return LoadStream(in);
}

bool SaveStream(Stream& out, const String& data) {
	if(!out.IsOpen() || out.IsError()) return false;
	out.Put((const char *)data, data.GetLength());
	out.Close();
	return out.IsOK();
}

bool SaveFile(const char *filename, const String& data) {
	FileOut out(filename);
	return SaveStream(out, data);
}

int64 CopyStream(Stream& dest, Stream& src, int64 count) {
	int block = (int)min<int64>(count, 65536);
	Buffer<byte> temp(block);
	int loaded;
	int64 done = 0;
	while(count > 0 && (loaded = src.Get(temp, (int)min<int64>(count, block))) > 0) {
		dest.Put(temp.operator const byte *(), loaded);
		count -= loaded;
		done += loaded;
	}
	return done;
}

void CheckedSerialize(const Callback1<Stream&> serialize, Stream& stream)
{
	int pos = (int)stream.GetPos();
	stream.Magic(0x61746164);
	serialize(stream);
	stream.Magic(0x00646e65);
	pos = int(stream.GetPos() - pos);
	stream.Magic(pos);
}

bool Load(Callback1<Stream&> serialize, Stream& stream) {
	StringStream backup;
	backup.SetStoring();
	serialize(backup);
	ASSERT(!backup.IsError());
	stream.SetLoading();
	stream.LoadThrowing();
	try {
		CheckedSerialize(serialize, stream);
	}
	catch(LoadingError) {
		backup.Seek(0);
		backup.SetLoading();
		serialize(backup);
		ASSERT(!backup.IsError());
		return false;
	}
	return true;
}

bool Store(Callback1<Stream&> serialize, Stream& stream) {
	stream.SetStoring();
	CheckedSerialize(serialize, stream);
	return !stream.IsError();
}

String Cfgname(const char *file) {
	return file ? String(file) : ConfigFile();
}

bool LoadFromFile(Callback1<Stream&> serialize, const char *file) {
	FileIn f(Cfgname(file));
	return f ? Load(serialize, f) : false;
}

bool StoreToFile(Callback1<Stream&> serialize, const char *file) {
	FileOut f(Cfgname(file));
	return f ? Store(serialize, f) : false;
}

Stream& Pack16(Stream& s, int& i) {
	if(s.IsLoading()) {
		i = (int16) s.Get16le();
		if(i == -32768)
			i = s.Get32le();
	}
	else
		if(i < -32767 || i > 32767) {
			s.Put16le((word)-32768);
			s.Put32le(i);
		}
		else
			s.Put16le((word)i);
	return s;
}

Stream& Pack16(Stream& s, int& i1, int& i2) {
	Pack16(s, i1);
	Pack16(s, i2);
	return s;
}

Stream& Pack16(Stream& s, int& i1, int& i2, int& i3) {
	Pack16(s, i1, i2);
	Pack16(s, i3);
	return s;
}

Stream& Pack16(Stream& s, int& i1, int& i2, int& i3, int& i4) {
	Pack16(s, i1, i2, i3);
	Pack16(s, i4);
	return s;
}

Stream& Pack16(Stream& s, int& i1, int& i2, int& i3, int& i4, int& i5) {
	Pack16(s, i1, i2, i3, i4);
	Pack16(s, i5);
	return s;
}

int StreamHeading(Stream& stream, int ver, int minver, int maxver, const char* tag)
{
	if(stream.IsLoading() && stream.IsEof() || stream.IsError())
		return Null;
	String text = tag;
	dword len = text.GetLength();
	stream.Pack(len);
	if(stream.IsLoading()) {
		if(stream.IsError() || (int)len != text.GetLength()) {
			stream.SetError();
			return Null;
		}
		StringBuffer b(len);
		stream.SerializeRaw((byte *)~b, len);
		String in = b;
		if(stream.IsError() || in != text) {
			stream.SetError();
			return Null;
		}
	}
	else
		stream.SerializeRaw((byte *)(const char*)text, len);
	stream / ver;
	if(ver < minver || ver > maxver) {
		stream.SetError();
		return Null;
	}
	return ver;
}

END_UPP_NAMESPACE
