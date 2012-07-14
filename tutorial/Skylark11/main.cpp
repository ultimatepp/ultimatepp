#include <Skylark/Skylark.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

SKYLARK(HomePage, "")
{
	http("VAR", t_("Aborted by user."))
	    .RenderResult("Skylark11/index");
}

SKYLARK(SetLanguage, "setlanguage/*")
{
	int lang = LNGFromText(http[0]);
	if(lang)
		http.SetLanguage(lang);
	http.Redirect(HomePage);
}

struct MyApp : SkylarkApp {
	MyApp() {
		root = "myapp";
		threads = 1; // Sqlite3 does not like threads...
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
