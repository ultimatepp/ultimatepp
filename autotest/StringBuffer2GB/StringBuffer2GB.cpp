#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	StringBuffer sb, sb2;
	WStringBuffer wb, wb2;
	
	for(int i = 0; i < INT_MAX; i++) {
		sb.Cat(i % 37);
		char h[2] = { i % 43, 0 };
		sb2.Cat(h, 1);
		wb.Cat(i % 37777);
		wchar wh[2] = { wchar(i % 37771), 0 };
		wb2.Cat(wh, 1);
	}
	
	ASSERT(sb.GetCount() == INT_MAX);
	ASSERT(sb2.GetCount() == INT_MAX);
	ASSERT(wb.GetCount() == INT_MAX);
	ASSERT(wb2.GetCount() == INT_MAX);

	for(int i = 0; i < INT_MAX; i++) {
		ASSERT((byte)sb[i] == i % 37);
		ASSERT((byte)sb2[i] == i % 43);
		ASSERT(wb[i] == i % 37777);
		ASSERT(wb2[i] == i % 37771);
	}

	LOG("=========== Everything OK!");
}
