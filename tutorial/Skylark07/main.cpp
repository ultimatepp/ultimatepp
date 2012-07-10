#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	http.RenderResult("Skylark07/index");
}

SKYLARK(Submit, "submit:POST")
{
	Value h = http[".LIST"];
	ValueArray va;
	if(IsValueArray(h))
		va = h;
	va.Add(http["id"]);
	http.SessionSet(".LIST", va);
	http.Redirect(HomePage);
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
