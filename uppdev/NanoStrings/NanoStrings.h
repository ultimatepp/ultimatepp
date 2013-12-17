#ifndef _NanoStrings_NanoStrings_h_
#define _NanoStrings_NanoStrings_h_

#include <Core/Core.h>

using namespace Upp;

class NanoStrings {
	struct Data {
		int                    count;
		Vector< Buffer<char> > data;
	};
	Data           data[48];
	Vector<String> over;
	bool           zs;

	enum {
		page_shift = 7,
		page_mask = (1 << page_shift) - 1,
	};

	Tuple2<const char *, int> Get2s(dword ws);

public:
	dword                     Add(const String& s);
	Tuple2<const char *, int> Get2(dword ws);
	String                    Get(dword ws);
	const char *              GetPtr(dword ws)           { return Get2(ws).a; }
	
	void Clear();
	void Shrink();

	void ZeroTerminated(bool b = true)                   { zs = b; }
	
	NanoStrings();
	
	void DumpProfile();
};

#endif
