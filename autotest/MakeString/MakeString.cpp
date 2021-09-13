#include <Core/Core.h>

using namespace Upp;

String Make(int alloc, int chars)
{
	String s = String::Make(alloc, [&](char *t) {
		for(int i = 0; i < chars; i++)
			*t++ = 'A' + i;
		return chars;
	});
	
	if(s.GetAlloc() > 32)
		ASSERT(s.GetAlloc() < 2 * s.GetCount() + 32);
	
	return s;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = 0; i < 50; i++) {
		DUMP(Make(max(i, 14), i));
		DUMP(Make(max(i, 31), i));
		DUMP(Make(max(i, 40), i));
		DUMP(Make(max(i, 200), i));
	}
	
	CheckLogEtalon();
}
