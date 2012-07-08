#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	http.RenderResult("Skylark08/index");
}

SKYLARK(Add, "add:POST")
{
	String r = AsString(Nvl(http.Int("number1")) + Nvl(http.Int("number2")));
	http.Ux("result", "The result is: " + r)
	    .UxSet("number1", r);
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
