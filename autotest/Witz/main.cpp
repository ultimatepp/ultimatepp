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
		path = GetFileFolder(GetFileFolder(GetDataFile("x")));
	}
};

Value WitzUrlEncode(const Vector<Value>& arg, const Renderer *) {
	return arg.GetCount() == 1 && IsString(arg[0]) ? UrlEncode(arg[0]) : String();
}

INITBLOCK {
	Compiler::Register("url_encode", WitzUrlEncode);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	Ini::skylark_log = true;
	                                  
	MyApp app;
	
	LOG("PATH: " << app.Config().path);
	LOG("--------------------------------------------");

	Renderer http;
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
	    ("MyMap", m);
	
	String result = http.RenderString("Witz/index");

	LOG("------------");
	LOG(result);
	LOG("------------");
//	SaveFile(GetDataFile("etalon"), result);
	ASSERT(result == LoadFile(GetDataFile("etalon")));

	result = http.RenderString("Witz/index2");

	LOG("------------");
	LOG(result);
	LOG("------------");
//	SaveFile(GetDataFile("etalon"), result);
	result.Replace("/home/upp", "/home/cxl");
	ASSERT(result == LoadFile(GetDataFile("etalon2")));
	
	m.Add("name", "What is that?");
	http("res", m);
	
	result = http.RenderString("Witz/test2.witz");
	ASSERT(result == "What+is+that%3F\r\n");
	LOG(result);
	
	LOG("========== OK");
}
