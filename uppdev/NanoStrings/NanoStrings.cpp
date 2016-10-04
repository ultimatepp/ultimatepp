#include "NanoStrings.h"

NanoStrings::NanoStrings()
{
	zs = false;
	for(int i = 0; i < 48; i++)
		data[i].count = 0;
}

dword NanoStrings::Add(const String& s)
{
	int len = s.GetCount();
	if(len < 48) {
		int lenz = len + zs;
		Data& d = data[len];
		if(d.count < 0x4000000) {
			int page = d.count >> page_shift;
			if(page >= d.data.GetCount())
				d.data.Add().Alloc(lenz << page_shift);
			memcpy(d.data[page] + (d.count & page_mask) * lenz, s, lenz);
			dword ws = (len << 26) | d.count;
			d.count++;
			return ws;
		}
	}
	
	if(over.GetCount() >= 0xc0000000)
		Panic("NanoStrings: Out of addressing space");
	dword ws = 0xc0000000 | over.GetCount();
	over.Add(s).Shrink();
	return ws;
}

inline
Tuple2<const char *, int> NanoStrings::Get2s(dword ws)
{
	int len = ws >> 26;
	int lenz = len + zs;
	ws = ws & 0x3ffffff;
	Tuple2<const char *, int> r;
	r.a = data[len].data[ws >> page_shift] + (ws & page_mask) * lenz;
	r.b = len;
	return r;
}

Tuple2<const char *, int> NanoStrings::Get2(dword ws)
{
	Tuple2<const char *, int> r;
	if((ws & 0xc0000000) == 0xc0000000) {
		const String& h = over[ws & 0x3ffffff];
		r.a = ~h;
		r.b = h.GetLength();
	}
	else
		r = Get2s(ws);
	return r;
}

String NanoStrings::Get(dword ws)
{
	if((ws & 0xc0000000) == 0xc0000000)
		return over[ws & 0x3ffffff];
	else {
		Tuple2<const char *, int> r = Get2s(ws);
		return String(r.a, r.b);
	}
}

void NanoStrings::Clear()
{
	over.Clear();
	for(int i = 0; i < 48; i++)
		data[i].data.Clear();
}

void NanoStrings::Shrink()
{
	over.Shrink();
	for(int i = 0; i < 48; i++)
		data[i].data.Shrink();
}

void NanoStrings::DumpProfile()
{
	for(int i = 0; i < 48; i++) {
		Data& d = data[i];
		RLOG(i << ": "  << d.count << ", pages: " << d.data.GetCount());
	}
}
