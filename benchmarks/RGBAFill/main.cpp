#include <CtrlLib/CtrlLib.h>

using namespace Upp;

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
	}
}
