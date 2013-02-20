#include <Core/Core.h>
#include <PipeStream/PipeStream.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	PipeStream m;
	m.Put(123);
	ASSERT(m.Get() == 123);
}
