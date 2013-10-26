#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define N 1000000

GUI_APP_MAIN
{
	for(int i = 0; i < N; i++) {
		RTIMING("ParseQTF");
		ParseQTF("[G1 simple text");
	}
}
