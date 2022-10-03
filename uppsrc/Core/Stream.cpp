#include "Core.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#include <termios.h>
#endif

namespace Upp {

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

int Stream::Skip(int size)
{
	int r = 0;
	while(size) {
		int n = min(int(rdlim - ptr), size);
		if(n == 0) {
			if(Get() < 0)
				break;
			r++;
			size--;
		}
		else {
			size -= n;
			r += n;
			ptr += n;
		}
	}
	return r;
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

void Stream::LoadError() {
	SetError(ERROR_LOADING_FAILED);
	if(style & STRM_THROW)
		throw LoadingError();
}

String Stream::GetErrorText() const
{
   return IsError() ? Upp::GetErrorMessage(errorcode) : String();
}

bool Stream::GetAll(void *data, int size) {
	if(Get(data, size) != size) {
		LoadError();
		return false;
	}
	return true;
}

void Stream::Put64(const void *data, int64 size)
{
#ifdef CPU_64
	byte *ptr = (byte *)data;
	while(size > INT_MAX) {
		Put(ptr, INT_MAX);
		ptr += INT_MAX;
		size -= INT_MAX;
	}
	Put(ptr, (int)size);
#else
	ASSERT(size <= INT_MAX);
	Put(data, (int)size);
#endif
}

int64 Stream::Get64(void *data, int64 size)
{
#ifdef CPU_64
	byte *ptr = (byte *)data;
	int64 n = 0;
	while(size > INT_MAX) {
		int q = Get(ptr, INT_MAX);
		n += q;
		if(q != INT_MAX)
			return n;
		ptr += INT_MAX;
		size -= INT_MAX;
	}
	int q = Get(ptr, (int)size);
	return n + q;
#else
	ASSERT(size <= INT_MAX);
	return Get(data, (int)size);
#endif
}

bool Stream::GetAll64(void *data, int64 size)
{
	if(Get64(data, size) != size) {
		LoadError();
		return false;
	}
	return true;
}

size_t Stream::Get(Huge& h, size_t size)
{
	while(h.GetSize() < size) {
		int sz = (int)min((size_t)h.CHUNK, size - h.GetSize());
		int len = Get(h.AddChunk(), sz);
		if(len < h.CHUNK) {
			h.Finish(len);
			break;
		}
	}
	return h.GetSize();
}

bool Stream::GetAll(Huge& h, size_t size)
{
	if(Get(h, size) != size) {
		LoadError();
		return false;
	}
	return true;
}

String Stream::Get(int size)
{
	StringBuffer b(size);
	int n = Get(~b, size);
	b.SetCount(n);
	return String(b);
}

String Stream::GetAll(int size)
{
	String result;
	if(size < 4 * 1024*1024)
		result = Get(size);
	else {
		Huge h;
		Get(h, size);
		result = h.Get();
	}
	if(result.GetCount() != size) {
		LoadError();
		result = String::GetVoid();
	}
	return result;
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

int Stream::GetUtf8()
{
	int code = Get();
	
	if(code < 0) {
		LoadError();
		return -1;
	}

	if(code < 0x80)
		return code;
	
	if(code >= 0xC2) {
		int c = 0, pos = GetPos();
		if(code < 0xE0) {
			int c0 = Get();
			if(c0 >= 0x80 && c0 < 0xC0 &&
			   (c = ((code - 0xC0) << 6) + c0 - 0x80) >= 0x80 && c < 0x800)
				return c;
		}
		else
		if(code < 0xF0) {
			int c0 = Get();
			int c1 = Get();
			if(c1 >= 0x80 && c1 < 0xC0 &&
			   c0 >= 0x80 && c0 < 0xC0 &&
			   (c = ((code - 0xE0) << 12) + ((c0 - 0x80) << 6) + c1 - 0x80) >= 0x800 && c < 0x10000)
				return c;
		}
		else
		if(code < 0xF8) {
			int c0 = Get();
			int c1 = Get();
			int c2 = Get();
			if(c2 >= 0x80 && c2 < 0xC0 &&
			   c1 >= 0x80 && c1 < 0xC0 &&
			   c0 >= 0x80 && c0 < 0xC0 &&
			   (c = ((code - 0xF0) << 18) + ((c0 - 0x80) << 12) + ((c1 - 0x80) << 6) + c2 - 0x80) >= 0x10000 && c < 0x110000)
				return c;
		}
	}

	LoadError();
	return -1;
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
		if(n == 0)
			break;
		Put(~buffer, n);
		size -= n;
	}
}

String Stream::GetAllRLE(int size)
{
	String result;
	while(result.GetCount() < size) {
		int c = Get();
		if(c < 0)
			break;
		if(c == 0xcb) {
			c = Get();
			result.Cat(c, Get());
		}
		else
			result.Cat(c);
	}
	return result.GetCount() == size ? result : String::GetVoid();
}

void Stream::SerializeRLE(byte *data, int size)
{
	ASSERT(size >= 0);
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

void Stream::SerializeRaw(byte *data, int64 size)
{
	ASSERT(size >= 0);
	if(IsError()) return;
	if(IsLoading())
		GetAll64(data, size);
	else
		Put64(data, size);
}

void Stream::SerializeRaw(word *data, int64 count)
{
	ASSERT(count >= 0);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
	SerializeRaw((byte *)data, 2 * count);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
}

void Stream::SerializeRaw(int16 *data, int64 count)
{
	SerializeRaw((word *)data, count);
}

void Stream::SerializeRaw(dword *data, int64 count)
{
	ASSERT(count >= 0);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
	SerializeRaw((byte *)data, 4 * count);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
}

void Stream::SerializeRaw(int *data, int64 count)
{
	SerializeRaw((dword *)data, count);
}

void Stream::SerializeRaw(uint64 *data, int64 count)
{
	ASSERT(count >= 0);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
	SerializeRaw((byte *)data, 8 * count);
#ifdef CPU_BE
	EndianSwap(data, count);
#endif
}

void Stream::SerializeRaw(float *data, int64 count)
{
	SerializeRaw((dword *)data, count);
}

void Stream::SerializeRaw(double *data, int64 count)
{
	SerializeRaw((uint64 *)data, count);
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
		int ff = Get();
		if(ff < 0) LoadError();
		else {
			a = !!(ff & 0x80);
			b = !!(ff & 0x40);
			c = !!(ff & 0x20);
			d = !!(ff & 0x10);
			e = !!(ff & 0x08);
			f = !!(ff & 0x04);
			g = !!(ff & 0x02);
			h = !!(ff & 0x01);
		}
	}
	else {
		int ff = 0;
		if(a) ff |= 0x80;
		if(b) ff |= 0x40;
		if(c) ff |= 0x20;
		if(d) ff |= 0x10;
		if(e) ff |= 0x08;
		if(f) ff |= 0x04;
		if(g) ff |= 0x02;
		if(h) ff |= 0x01;
		Put(ff);
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
		s = GetAll(len);
		if(s.IsVoid())
			LoadError();
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
		s = GetAllRLE(len);
		if(s.IsVoid())
			LoadError();
	}
	else
		SerializeRLE((byte *)~s, len);
	return *this;
}

Stream& Stream::operator%(WString& s)
{ // we do not support BE here anymore
	if(IsError()) return *this;
	if(IsLoading()) {
		dword len = Get();
		if(len == 0xff)
			len = Get32le();
		String h = GetAll(len * sizeof(char16));
		if(h.IsVoid())
			LoadError();
		else
			s = ToUtf32((const char16 *)~h, len);
	}
	else {
		Vector<char16> x = ToUtf16(s);
		dword len = x.GetCount();
		if(len < 0xff)
			Put(len);
		else {
			Put(0xff);
			Put32le(len);
		}
		SerializeRaw((byte*)x.begin(), len * sizeof(char16));
	}
	return *this;
}

Stream& Stream::operator/(WString& s) {
	if(IsError()) return *this;
	String h = ToUtf8(s);
	*this / h;
	s = ToUtf32(h);
	return *this;
}

Stream& Stream::operator/(int& i)            { dword w = 0; if(IsStoring()) w = i + 1; Pack(w); i = w - 1; return *this; }
Stream& Stream::operator/(unsigned int& i)   { dword w = 0; if(IsStoring()) w = i + 1; Pack(w); i = w - 1; return *this; }
Stream& Stream::operator/(long& i)           { dword w = 0; if(IsStoring()) w = i + 1; Pack(w); i = w - 1; return *this; }
Stream& Stream::operator/(unsigned long& i)  { dword w = 0; if(IsStoring()) w = i + 1; Pack(w); i = w - 1; return *this; }

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
		size_t p = ptr - buffer;
		if(limit != INT_MAX && p + sz > (size_t)limit)
			throw LimitExc();
		if(p + sz >= INT_MAX)
			Panic("2GB StringStream limit exceeded");
		int len = (int32)max((int64)128, min((int64)limit, max(2 * GetSize(), GetSize() + sz)));
		wdata.SetLength(len);
		SetWriteBuffer();
		ptr = buffer + p;
	}
	memcpy8(ptr, d, sz);
	ptr += sz;
}

void StringStream::Reserve(int n)
{
	SetWriteMode();
	intptr_t p = ptr - buffer;
	wdata.SetLength((int)GetSize() + n);
	SetWriteBuffer();
	ptr = buffer + p;
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
	memcpy8(data, ptr, read);
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
	memcpy8(data, ptr, size);
	ptr += size;
	return size;
}

void  MemStream::_Put(const void *data, dword size) {
	if(size > (dword)(uintptr_t)(wrlim - ptr)) {
		SetError(ERROR_NOT_ENOUGH_SPACE);
		return;
	}
	memcpy8(ptr, data, size);
	ptr += size;
}

bool  MemStream::IsOpen() const {
	return true;
}

void MemStream::Create(void *data, int64 size)
{
	style = STRM_WRITE|STRM_READ|STRM_SEEK|STRM_LOADING;
	ptr = buffer = (byte *) data;
	wrlim = rdlim = buffer + (size_t)size;
	pos = 0;
}

MemStream::MemStream(void *data, int64 size) {
	Create(data, size);
}

MemStream::MemStream() {}

// ----------------------- Memory read streamer -------------------------

void MemReadStream::Create(const void *data, int64 size)
{
	MemStream::Create((void *)data, size);
	style = STRM_READ|STRM_SEEK|STRM_LOADING;
	wrlim = buffer;
}

MemReadStream::MemReadStream(const void *data, int64 size)
{
	Create(data, size);
}

MemReadStream::MemReadStream() {}

// --------------------------- Size stream -----------------------

int64 SizeStream::GetSize() const
{
	return int64(ptr - buffer + pos);
}

void SizeStream::_Put(const void *, dword sz)
{
	wrlim = buffer + sizeof(h);
	pos += ptr - buffer + sz;
	ptr = buffer;
}

void SizeStream::_Put(int w)
{
	_Put(NULL, 1);
}

bool SizeStream::IsOpen() const
{
	return true;
}

SizeStream::SizeStream()
{
	style = STRM_WRITE;
	buffer = ptr = h;
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
	wrlim = buffer + 1024;
	ptr = buffer;
	equal = true;
	ClearError();
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

void CompareStream::Compare(int64 pos, const void *data, int size) {
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
	wrlim = buffer + sizeof(h);
	ASSERT(ptr <= wrlim);
	Flush();
	pos += ptr - buffer;
	ptr = buffer;
	byte *b = (byte *) data;
	while(size && equal) {
		int sz = min<int>(size, sizeof(h));
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

OutStream::OutStream()
{
	const int bsz = 64 * 1024;
	h = (byte *)MemoryAlloc(bsz);
	buffer = ptr = h;
	wrlim = h + bsz;
}

OutStream::~OutStream()
{	// Note: cannot call Close here !
	MemoryFree(h);
}

void OutStream::_Put(int w)
{
	Flush();
	*ptr++ = w;
}

void OutStream::_Put(const void *data, dword size)
{
	if(ptr == buffer)
		Out(data, size);
	else
	if(ptr + size < wrlim) {
		memcpy8(ptr, data, size);
		ptr += size;
	}
	else {
		Flush();
		Out(data, size);
	}
}

void OutStream::Flush()
{
	if(ptr != buffer) {
		Out(buffer, int(ptr - buffer));
		ptr = h;
	}
}

void OutStream::Close()
{
	Flush();
}

bool OutStream::IsOpen() const
{
	return true;
}

void TeeStream::Out(const void *data, dword size)
{
	a.Put(data, size);
	b.Put(data, size);
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
#ifdef PLATFORM_WIN32
	String buffer;

	void Flush() {
		ONCELOCK {
			SetConsoleOutputCP(65001); // set console to UTF8 mode
		}
		static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		dword dummy;
		WriteFile(h, ~buffer, buffer.GetCount(), &dummy, NULL);
		buffer.Clear();
	}
#endif


	void Put0(int w) {
#ifdef PLATFORM_WIN32
		buffer.Cat(w);
		if(CheckUtf8(buffer) || buffer.GetCount() > 8)
			Flush();
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
#ifdef PLATFORM_POSIX
	virtual   void   Flush()       { fflush(stdout); }
#endif
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
#ifdef PLATFORM_POSIX
	virtual   void  _Put(const void *data, dword size) {
		fwrite(data, 1, size, stderr);
	}
#endif
	virtual   bool  IsOpen() const { return true; }
};

Stream& Cerr()
{
	return Single<CerrStream>();
}
#endif


String ReadStdIn()
{
	String r;
	for(;;) {
		int c = getchar();
		if(c < 0)
			return r.GetCount() ? r : String::GetVoid();
		if(c == '\n')
			return r;
		r.Cat(c);
	}
}


String ReadSecret()
{
	DisableEcho();
	String s = ReadStdIn();
	EnableEcho();
	Cout().PutEol();
	return s;
}

void EnableEcho(bool b)
{
#ifdef PLATFORM_POSIX
	termios t;
	tcgetattr(STDIN_FILENO, &t);
	if(b) t.c_lflag |=  ECHO;
	else  t.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSADRAIN, &t);
#elif PLATFORM_WIN32
	HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(h, &mode);
	if(b) mode |=  ENABLE_ECHO_INPUT;
	else  mode &= ~ENABLE_ECHO_INPUT;
	SetConsoleMode(h, mode);
#endif	
}

void DisableEcho()
{
	EnableEcho(false);
}


// ---------------------------------------------------------------------------

String LoadStream(Stream& in) {
	if(in.IsOpen()) {
		in.ClearError();
		int64 size = in.GetLeft();
		if(size >= 0 && size < INT_MAX) {
			StringBuffer s((int)size);
			in.Get(s, (int)size);
			if(!in.IsError())
				return String(s);
		}
	}
	return String::GetVoid();
}

String LoadFile(const char *filename) {
	FindFile ff(filename);
	if(ff && ff.IsFile()) {
	#ifdef PLATFORM_POSIX
		if(ff.GetLength() == 0) { // handle special cases like /proc/...
			int fd = open(filename,O_RDONLY);
			if(fd >= 0) {
				const int CHUNK = 32768;
				StringBuffer s;
				for(;;) {
					int n = s.GetCount();
					s.SetCount(n + CHUNK);
					int len = read(fd, ~s + n, CHUNK);
					if(len != CHUNK) {
						if(len >= 0)
							s.SetCount(n + len);
						close(fd);
						return s;
					}
				}
			}
			return String::GetVoid();
		}
	#endif
		FileIn in(filename);
		return LoadStream(in);
	}
	return String::GetVoid();
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

int64 CopyStream(Stream& dest, Stream& src, int64 count)
{
	return CopyStream(dest, src, count, Null);
}

int64 CopyStream(Stream& dest, Stream& src, int64 count, Gate<int64, int64> progress, int chunk_size)
{
	int block = (int)min<int64>(count, chunk_size);
	Buffer<byte> temp(block);
	int loaded;
	int64 done = 0;
	int64 total = count;
	while(count > 0 && (loaded = src.Get(~temp, (int)min<int64>(count, block))) > 0) {
		dest.Put(~temp, loaded);
		if(dest.IsError())
			return -1;
		count -= loaded;
		done += loaded;
		if(progress(done, total))
			return -1;
	}
	return done;
}

void CheckedSerialize(const Event<Stream&> serialize, Stream& stream, int version)
{
	int pos = (int)stream.GetPos();
	stream.Magic(0x61746164);
	if(!IsNull(version))
		stream.Magic(version);
	serialize(stream);
	stream.Magic(0x00646e65);
	pos = int(stream.GetPos() - pos);
	stream.Magic(pos);
}

bool Load(Event<Stream&> serialize, Stream& stream, int version) {
	StringStream backup;
	backup.SetStoring();
	serialize(backup);
	ASSERT(!backup.IsError());
	stream.SetLoading();
	stream.LoadThrowing();
	try {
		CheckedSerialize(serialize, stream, version);
	}
	catch(LoadingError) {
		backup.Seek(0);
		backup.SetLoading();
		serialize(backup);
		ASSERT(!backup.IsError());
		return false;
	}
	catch(ValueTypeError) {
		backup.Seek(0);
		backup.SetLoading();
		serialize(backup);
		ASSERT(!backup.IsError());
		return false;
	}
	return true;
}

bool Store(Event<Stream&> serialize, Stream& stream, int version) {
	stream.SetStoring();
	CheckedSerialize(serialize, stream, version);
	return !stream.IsError();
}

String Cfgname(const char *file) {
	return file ? String(file) : ConfigFile();
}

bool LoadFromFile(Event<Stream&> serialize, const char *file, int version) {
	FileIn f(Cfgname(file));
	return f ? Load(serialize, f, version) : false;
}

bool StoreToFile(Event<Stream&> serialize, const char *file, int version) {
	FileOut f(Cfgname(file));
	if(!f || !Store(serialize, f, version))
		return false;
	f.Close();
	return !f.IsError();
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

}
