#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "**")
{
	http << "<html><body>Hello world!</body></html>";
}

SKYLARK(Param, "*/param")
{
	http << "<html><body>Param entered: " << http[0] << "</html></body>";
}

struct MyApp : SkylarkApp {
	MyApp() {
		root = "myapp";
		prefork = 0;
		use_caching = false;
	}
};

CONSOLE_APP_MAIN
{
#ifdef DEBUG
	StdLogSetup(LOG_FILE|LOG_COUT);
	skylark_log = true;
#endif

	MyApp().Run();	
}
