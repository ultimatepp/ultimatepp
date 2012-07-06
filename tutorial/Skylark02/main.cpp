#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	ValueArray va;
	va.Add(1);
	va.Add("Hello");
	ValueMap m;
	m.Add("key1", "first value");
	m.Add("key2", "second value");

	http("MyValue", "some value")
	    ("MyRawValue", Raw("<b>raw <u>html</u></b>"))
	    ("MyRawValue2", "<b>another raw <u>html</u></b>")
	    ("MyArray", va)
	    ("MyMap", m)
	    .RenderResult("Skylark02/index");
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
