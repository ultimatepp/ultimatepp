#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Mutex m;
	static StaticMutex n;
	for(int i = 0; i < 10000000; i++) {
		{
			RTIMING("Mutex::Enter");
			m.Enter();
			m.Leave();
		}
		{
			RTIMING("Mutex::TryEnter");
			if(m.TryEnter())
				m.Leave();
		}
		{
			RTIMING("StaticMutex::Enter");
			__BREAK__;
			n.Enter();
			n.Leave();
		}
		{
			RTIMING("StaticMutex::TryEnter");
			if(n.TryEnter())
				n.Leave();
		}
	}
}
