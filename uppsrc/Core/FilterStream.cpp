#include "Core.h"

namespace Upp {

InFilterStream::InFilterStream()
{
	Init();
}

void InFilterStream::Init()
{
	pos = 0;
	in = NULL;
	eof = false;
	style = STRM_READ|STRM_LOADING;
	SetLoading();
	buffer.Clear();
	ptr = rdlim = Stream::buffer = NULL;
	todo = 0;
	t = NULL;
}

bool InFilterStream::IsOpen() const
{
	return in->IsOpen();
}

int InFilterStream::_Term()
{
	while(ptr == rdlim && !eof)
		Fetch();
	return ptr == rdlim ? -1 : *ptr;
}

int InFilterStream::_Get()
{
	while(ptr == rdlim && !eof)
		Fetch();
	return ptr == rdlim ? -1 : *ptr++;
}

dword InFilterStream::_Get(void *data, dword size)
{
	t = (byte *)data;
	dword sz0 = min(dword(rdlim - ptr), size);
	memcpy8(t, ptr, sz0);
	t += sz0;
	ptr += sz0;
	todo = size - sz0;
	while(todo && !eof)
		Fetch();
	return size - todo;
}

void InFilterStream::SetRd()
{
	Stream::buffer = ptr = buffer.begin();
	rdlim = buffer.end();
}

void InFilterStream::Out(const void *p, int size)
{
	const byte *s = (byte *)p;
	if(todo) {
		dword sz = min(todo, (dword)size);
		memcpy8(t, s, sz);
		t += sz;
		s += sz;
		todo -= sz;
		size -= sz;
		pos += sz;
	}
	if(size) {
		int l = buffer.GetCount();
		buffer.SetCountR(l + size);
		memcpy8(buffer.begin() + l, s, size);
		SetRd();
	}
	WhenOut();
}

void InFilterStream::Fetch()
{
	ASSERT(ptr == rdlim);
	pos += buffer.GetCount();
	buffer.SetCount(0); // SetCount instead of Clear to maintain capacity
	if(!eof) {
		if(More)
			eof = !More();
		else {
			if(!inbuffer)
				inbuffer.Alloc(buffersize);
			int n = in->Get(~inbuffer, buffersize);
			if(n == 0) {
				End();
				eof = true;
			}
			else {
				Filter(~inbuffer, n);
				eof = FilterEof();
			}
		}
	}
	SetRd();
}

/////////////////////////////////////////////////////////////////////////////////////////////

OutFilterStream::OutFilterStream()
{
	Init();
}

void OutFilterStream::Init()
{
	buffer.Alloc(4096);
	wrlim = ~buffer + 4096;
	ptr = ~buffer;
	out = NULL;
	count = pos = 0;
	style = STRM_WRITE;
	pos = 0;
	Stream::buffer = ~buffer;
}

OutFilterStream::~OutFilterStream()
{
	Close();
}

void OutFilterStream::Close()
{
	if(buffer) {
		FlushOut();
		End();
		buffer.Clear();
	}
}

void OutFilterStream::FlushOut()
{
	if(ptr != ~buffer) {
		int sz = (int)(ptr - ~buffer);
		pos += sz;
		WhenPos(pos);
		Filter(~buffer, sz);
		ptr = ~buffer;
	}
}

void OutFilterStream::_Put(int w)
{
	FlushOut();
	*ptr++ = w;
}

void OutFilterStream::_Put(const void *data, dword size)
{
	const byte *p = (const byte *)data;
	for(;;) {
		int n = min(Avail(), size);
		memcpy8(ptr, p, n);
		size -= n;
		p += n;
		ptr += n;
		if(size == 0)
			return;
		FlushOut();
	}
}

bool OutFilterStream::IsOpen() const
{
	return buffer;
}

void OutFilterStream::Out(const void *ptr, int size)
{
	count += size;
	out->Put(ptr, size);
}

}
