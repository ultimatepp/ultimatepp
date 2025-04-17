#include "Core.h"

namespace Upp {

PackedData::~PackedData()
{
	if(ptr) MemoryFree(ptr);
}

void PackedData::Clear()
{
	if(ptr) MemoryFree(ptr);
	ptr = nullptr;
}

Vector<String> PackedData::Unpack() const
{
	Vector<String> r;
	const byte *s = (const byte *)ptr;
	for(;;) {
		int len = *s++;
		if(len == 255)
			break;
		if(len == 254) {
			memcpy(&len, s, 4);
			s += 4;
		}
		r << String(s, len);
		s += len;
	}
	return r;
}

void PackedData::SetData(int ii, const void *data, int datalen)
{
	size_t alloc = 32;
	char *result = (char *)MemoryAllocSz(alloc);
	char *t = result;
	auto Reserve = [&](int n) {
		size_t needs = t + n - result;
		if(needs > alloc) {
			alloc = 3 * needs / 2;
			char *r2 = (char *)MemoryAllocSz(alloc);
			memcpy(r2, result, t - result);
			MemoryFree(result);
			t = t - result + r2;
			result = r2;
		}
	};
	auto Out1 = [&](int c) {
		Reserve(1);
		*t++ = c;
	};
	auto Out = [&](const void *s, int len) {
		Reserve(len);
		if(len) {
			memcpy(t, s, len);
			t += len;
		}
	};
	int i = 0;
	const byte *p = (const byte *)ptr;
	const byte *s = p;
	const byte *b = s; // before last control code
	const byte *rb = NULL; // start of replaced area
	const byte *re = NULL; // end of replaced area
	if(s)
		for(;;) {
			b = s;
			int len = *s++;
			if(len == 255)
				break;
			if(len == 254) {
				memcpy(&len, s, sizeof(int));
				s += sizeof(int);
			}
			if(i == ii) {
				rb = b;
				s += len;
				re = s;
			}
			else
				s += len;
			i++;
		}
	
	auto Put = [&]() {
		if(datalen < 254)
			Out1(datalen);
		else {
			Out1(254);
			byte h[sizeof(int)];
			memcpy(h, &datalen, sizeof(int));
			Out(h, sizeof(int));
		}
		Out(data, datalen);
	};
	if(rb) { // we have found an area to replace
		Out(p, int(rb - p));
		Put();
		Out(re, int((s - re)));
	}
	else { // we need to add new entries
		if(p) // copy existing entries
			Out(p, int(b - p));
		while(i < ii) {
			Out1(0);
			i++;
		}
		Put();
		Out1(255);
	}
	if(ptr)
		MemoryFree(ptr);
	ptr = result;
}

size_t PackedData::GetPackedSize() const
{
	if(!ptr)
		return 0;
	const byte *s = (const byte *)ptr;
	for(;;) {
		int len = *s++;
		if(len == 255)
			break;
		if(len == 254) {
			memcpy(&len, s, 4);
			s += 4;
		}
		s += len;
	}
	return s - (const byte *)ptr;
}

};