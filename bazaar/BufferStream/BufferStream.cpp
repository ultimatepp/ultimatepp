#include "BufferStream.h"


void BufferStream::_Put(const void *data, dword size)
{
	if(size > (dword)(uintptr_t)(wrlim - ptr)) {
		Reserve(size + 128);
	}
	memcpy(ptr, data, size);
	ptr += size;
}

void BufferStream::SetSize(int64 asize)
{
	dword size = (dword)asize;
	dword p = (dword)(uintptr_t)(ptr - buffer);
	data.SetCount(size);
	Open(data);
	SetStoring();
	Seek(min(p, size));
}

void BufferStream::Seek(int64 pos)
{
	dword size = (dword)GetSize();
	if(pos > size) {
		size = (dword)pos;
		SetSize(size + 100);
	}
	ptr = buffer + min(pos, int64(rdlim - buffer));
}

void BufferStream::Open(Vector<byte> & d)
{
	if(&data != &d) data = d; //pick
	MemStream::Create((byte*)data, data.GetCount());
}

void BufferStream::Create()
{
	data.Clear();
	Open(data);
	SetStoring();	
}

Vector<byte> BufferStream::GetResult()
{
	data.SetCount((dword)(uintptr_t)(ptr - buffer));
	Vector<byte> d = data; //pick
	Create();
	return d;
}
