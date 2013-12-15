#ifndef _NanoStrings_NanoStrings_h_
#define _NanoStrings_NanoStrings_h_

#include <Core/Core.h>

using namespace Upp;

struct NanoStrings {
	struct Data {
		int                    count;
		Vector< Buffer<char> > data;
	};
	Data           data[48];
	Vector<String> over;
	
	dword  Add(const String& s);
	String Get(dword ws);
	
	void Profile();
};

#endif
