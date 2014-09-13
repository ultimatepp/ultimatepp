#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	Value v;
	for(int i = 0; i < 3; i++) {
		Value& p = v.At(i)("person");
		p("name") = "Name" + AsString(i + 1);
		p("lastname") = "LastName" + AsString(i + 1);
	}
	LOG(AsJSON(v));
}
