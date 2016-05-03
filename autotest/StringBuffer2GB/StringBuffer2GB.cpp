#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

#ifdef PLATFORM_WIN32 // Not enough memory in linux machine to run this test (TODO)
	{
		StringBuffer sb;
		
		for(int i = 0; i < INT_MAX; i++) {
			sb.Cat(i % 37);
		}
		
		ASSERT(sb.GetCount() == INT_MAX);
	
		for(int i = 0; i < INT_MAX; i++) {
			ASSERT((byte)sb[i] == i % 37);
		}
	}
	DUMP(MemoryUsedKb());

	{
		StringBuffer sb2;
		
		for(int i = 0; i < INT_MAX; i++) {
			char h[2] = { char(i % 43), 0 };
			sb2.Cat(h, 1);
		}
		
		ASSERT(sb2.GetCount() == INT_MAX);
	
		for(int i = 0; i < INT_MAX; i++) {
			ASSERT((byte)sb2[i] == i % 43);
		}
	}
	DUMP(MemoryUsedKb());

	{
		WStringBuffer wb;
		
		for(int i = 0; i < INT_MAX; i++) {
			wb.Cat(i % 37777);
		}
		
		ASSERT(wb.GetCount() == INT_MAX);
	
		for(int i = 0; i < INT_MAX; i++) {
			ASSERT(wb[i] == i % 37777);
		}
	}
	DUMP(MemoryUsedKb());

	{
		WStringBuffer wb2;
		
		for(int i = 0; i < INT_MAX; i++) {
			wchar wh[2] = { wchar(i % 37771), 0 };
			wb2.Cat(wh, 1);
		}

		ASSERT(wb2.GetCount() == INT_MAX);
	
		for(int i = 0; i < INT_MAX; i++) {
			ASSERT(wb2[i] == i % 37771);
		}
	}
	DUMP(MemoryUsedKb());
#endif

	LOG("=========== Everything OK!");
}
