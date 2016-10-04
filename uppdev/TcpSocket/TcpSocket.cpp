#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	TcpSocket s;
	if(!s.Connect("www.idnessdfg.cz", 12314)) {
	    DDUMP(s.GetErrorDesc());
	}
	DLOG("OK");
}
