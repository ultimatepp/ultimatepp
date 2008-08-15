#include "sstrm.h"

void StringStream::Reads()
{
	wdata.Clear();
	buffer = (byte *) ~data;
	pos = 0;
	ptr = wrlim = buffer;
	rdlim = buffer + data.GetCount();
	writes = false;
}

void StringStream::Bufr()
{
	buffer = (byte *)wdata.Begin();
	rdlim = wrlim = (byte *)wdata.End();
}

void StringStream::Writes()
{
	if(writes) return;
	dword p = ptr - buffer;
	wdata = data;
	Bufr();
	ptr = buffer + p;
	writes = true;
	size = rdlim - buffer;
}

void  StringStream::Open(const String& adata)
{
	data = adata;
	style = STRM_READ|STRM_WRITE|STRM_SEEK|STRM_LOADING;
	Reads();
	ClearError();
}

void  StringStream::Create()
{
	Open(String());
	SetStoring();
	Writes();
	ClearError();
}

dword StringStream::GetSize() const
{
	return writes ? max((dword)(ptr - buffer), size) : data.GetLength();
}

String StringStream::GetResult()
{
	if(writes) {
		wdata.SetLength(GetSize());
		dword p = ptr - buffer;
		data = wdata;
		Reads();
		ptr = buffer + p;
	}
	return data;
}

void  StringStream::_Put(const void *d, dword sz)
{
	Writes();
	if(ptr + sz >= wrlim) {
		dword p = ptr - buffer;
	#ifdef _DEBUG
		wdata.SetLength(max((dword)1, max(2 * GetSize(), GetSize() + sz)));
	#else
		wdata.SetLength(max((dword)128, max(2 * GetSize(), GetSize() + sz)));
	#endif
		Bufr();
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
	dword read = min(dword(rdlim - ptr), sz);
	memcpy(data, ptr, read);
	ptr += read;
	return read;
}

int  StringStream::_Get()
{
	return ptr < rdlim ? *ptr++ : -1;
}

int  StringStream::_Term() {
	return ptr < rdlim ? *ptr : -1;
}

void  StringStream::Seek(dword pos) {
	size = max(size, (dword)(ptr - buffer));
	if(pos + buffer > rdlim) {
		Writes();
		wdata.SetLength(pos + 100);
		Bufr();
	}
	ptr = buffer + min(GetSize(), pos);
}

void  StringStream::SetSize(dword asize) {
	Writes();
	dword p = ptr - buffer;
	Seek(asize);
	Bufr();
	size = asize;
	ptr = min(buffer + p, buffer + size);
}

bool  StringStream::IsOpen() const {
	return true;
}
