#include "Core.h"

namespace Upp {

StringsStreamOut::StringsStreamOut(int part_size_)
{
	part_size = part_size_;
	ResetBuffer();
	style = STRM_WRITE;
}

void StringsStreamOut::ResetBuffer()
{
	wdata.SetCount(part_size);
	buffer = (byte *)wdata.begin();
	wrlim = (byte *)wdata.end();
	ptr = buffer;
}

int64 StringsStreamOut::GetSize() const
{
	return ptr - buffer + (int64)part.GetCount() * part_size;
}

bool StringsStreamOut::IsOpen() const
{
	return true;
}

void StringsStreamOut::_Put(int w)
{
	byte h = w;
	_Put(&h, 1);
}

void StringsStreamOut::_Put(const void *data, dword sz)
{
	if(!sz)
		return;
	const char *p = (const char *)data;
	for(;;) {
		dword psz = min(dword(wrlim - ptr), sz);
		if(psz) {
			memcpy8(ptr, p, psz);
			ptr += psz;
			sz -= psz;
		}
		if(sz == 0)
			break;
		p += psz;
		part.Add(wdata);
		ResetBuffer();
	}
}

Vector<String> StringsStreamOut::PickResult()
{
	if(ptr != buffer) {
		wdata.SetLength(int(ptr - buffer));
		part.Add(wdata);
	}
	return pick(part);
}

StringsStreamIn::StringsStreamIn(const Vector<String>& part)
:	part(part)
{
	i = 0;
	ResetBuffer();
	size = 0;
	for(const String& s : part)
		size += s.GetCount();
	style = STRM_READ;
}

void StringsStreamIn::ResetBuffer()
{
	if(i < part.GetCount()) {
		ptr = buffer = (byte *)part[i].begin();
		rdlim = (byte *)part[i].end();
	}
	else {
		static byte nothing;
		ptr = buffer = rdlim = &nothing;
	}
}

int StringsStreamIn::_Term()
{
	while(i < part.GetCount()) {
		if(ptr < rdlim)
			return *ptr;
		i++;
		ResetBuffer();
	}
	return -1;
}

int StringsStreamIn::_Get()
{
	int c = _Term();
	if(c >= 0)
		ptr++;
	return c;
}

dword StringsStreamIn::_Get(void *data, dword size)
{
	dword sz = size;
	char *p = (char *)data;
	for(;;) {
		dword psz = min(dword(rdlim - ptr), sz);
		if(psz) {
			memcpy8(p, ptr, psz);
			ptr += psz;
			sz -= psz;
		}
		if(sz == 0)
			return size;
		p += psz;
		if(i < part.GetCount()) {
			i++;
			ResetBuffer();
		}
		else
			return size - sz;
	}
}

int64 StringsStreamIn::GetSize() const
{
	return size;
}

bool StringsStreamIn::IsOpen() const
{
	return true;
}

}
