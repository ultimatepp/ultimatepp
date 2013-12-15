#include "NanoStrings.h"

const int page_shift = 7;
const int page_mask = (1 << page_shift) - 1;

dword NanoStrings::Add(const String& s)
{
	int len = s.GetCount();
	if(len < 48) {
		Data& d = data[s.GetCount()];
		if(d.count < 0x4000000) {
			int page = d.count >> page_shift;
			if(page >= d.data.GetCount())
				d.data.Add().Alloc(len << page_shift);
			memcpy(d.data[page] + (d.count & page_mask) * len, s, len);
			dword ws = (len << 26) | d.count;
			d.count++;
			return ws;
		}
	}
	
	if(over.GetCount() >= 0xc0000000)
		Panic("NanoStrings: Out of addressing space");
	dword ws = 0xc0000000 | over.GetCount();
	over.Add(s);
	return ws;
}

String NanoStrings::Get(dword ws)
{
	if((ws & 0xc0000000) == 0xc0000000)
		return over[ws & 0x3ffffff];
	else {
		int len = ws >> 26;
		ws = ws & 0x3ffffff;
		return String(data[len].data[ws >> page_shift] + (ws & page_mask) * len, len);
	}
}

void NanoStrings::Profile()
{
	for(int i = 0; i < 48; i++) {
		Data& d = data[i];
		RLOG(i << ": "  << d.count << ", pages: " << d.data.GetCount());
	}
}
