#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void CoFill(RGBA *t, RGBA c, int len)
{
	const int CHUNK = 1024;
	std::atomic<int> ii(0);
	CoDo([&] {
		for(;;) {
			int pos = CHUNK * ii++;
			if(pos >= len)
				break;
			Fill(t + pos, c, min(CHUNK, len - pos));
		}
	});
}

GUI_APP_MAIN
{
	Color c = Red();
	
	int len = 4000 * 2000;
	
	Buffer<RGBA> b(len);

	for(int i = 0; i < 10; i++) {
		{
			RTIMING("memsetd");
			memsetd(b, *(dword*)&(c), len);
		}
		{
			RTIMING("Fill");
			Fill(b, c, len);
		}
		{
			RTIMING("CoFill");
			CoFill(b, c, len);
		}
	}
}
