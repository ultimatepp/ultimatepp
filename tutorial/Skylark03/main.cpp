#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	http.RenderResult("Skylark03/index");
}

SKYLARK(Page2, "page2")
{
	http.RenderResult("Skylark03/page2");
}

SKYLARK(Param, "paramtest/*")
{
	http("PARAM", http[0]).RenderResult("Skylark03/param");
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
