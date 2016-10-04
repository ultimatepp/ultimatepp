#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(PatternMatch("*.tpp", "test.tppi"));
	
	SaveFile("u:/test.tpp", "");
	SaveFile("u:/test.tppi", "");
	for(FindFile ff("u:/*.tpp"); ff; ff.Next())
		DUMP(ff.GetName());
}

