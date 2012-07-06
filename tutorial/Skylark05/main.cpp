#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	http.RenderResult("Skylark05/index");
}

SKYLARK(Submit, "submit")
{
	http("RESULT", ToUpper((String)http["id"]))
		.RenderResult("Skylark05/submit");
}

struct MyApp : SkylarkApp {
	MyApp() {
		root = "myapp";
	#ifdef _DEBUG
		prefork = 0;
		use_caching = false;
	#endif
	}
};

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	StdLogSetup(LOG_FILE|LOG_COUT);
	Ini::skylark_log = true;
#endif

	MyApp().Run();	
}
