#include <Skylark/Skylark.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	http << "<!DOCTYPE html><html><body>Hello world!</body></html>";
}

SKYLARK(Param, "*/param")
{
	http << "<!DOCTYPE html><html><body>Parameter: " << http[0] << "</html></body>";
}

SKYLARK(Params, "params/**")
{
	http << "<!DOCTYPE html><html><body>Parameters: ";
	for(int i = 0; i < http.GetParamCount(); i++)
		http << http[i] << " ";
	http << "</html></body>";
}

SKYLARK(CatchAll, "**")
{
	http.Redirect(HomePage);
}

SKYLARK(Favicon, "/favicon.ico")
{
	http.ContentType("image/png") << LoadFile(GetDataFile("favicon.png"));
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
