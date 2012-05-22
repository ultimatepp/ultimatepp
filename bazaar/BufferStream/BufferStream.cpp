#include "BufferStream.h"

void BufferStream::_Put(const void *data, dword size)
{
	if(size > (dword)(uintptr_t)(wrlim - ptr)) {
		Reserve(size + 256);
	}
	memcpy(ptr, data, size);
	ptr += size;
}

void BufferStream::SetSize(int64 asize)
{
	ASSERT(asize < INT_MAX);
	int64 p = GetPos();
	data.SetCount((int)asize);
	Open(data);
	SetStoring();
	Seek(min(p, asize));
}

void BufferStream::Seek(int64 pos)
{
	if(pos > GetSize())
		SetSize(pos + 256);
	MemStream::Seek(pos);
}

void BufferStream::Open(Vector<byte> & d)
{
	if(&data != &d)
		data = d; //pick
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
	data.SetCount((int)GetPos());
	Vector<byte> d = data; //pick
	Create();
	return d;
}
