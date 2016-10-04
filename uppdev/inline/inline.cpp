#include <Core/Core.h>

#define INTERLOCKED static StaticCriticalSection MK__s; CriticalSection::Lock COMBINE(MK__s, _lock)(MK__s)

#define INITLOCK \
static volatile bool init; \
if(!init) \
	for(CriticalSection::LockMain __; !init; init = true) \

CONSOLE_APP_MAIN
{
for(static volatile bool b; !b;) \
	for(static StaticCriticalSection section; !b;) \
		for(CriticalSection::Lock lock(section); !b; b = true)
			LOG("INIT!");
		}
}
