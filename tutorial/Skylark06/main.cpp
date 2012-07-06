#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	http.RenderResult("Skylark06/index");
}

SKYLARK(Submit, "submit:POST")
{
	http("RESULT", ToUpper((String)http["id"]))
		.RenderResult("Skylark06/submit");
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
