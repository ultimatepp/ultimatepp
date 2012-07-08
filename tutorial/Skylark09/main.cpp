#include <Skylark/Skylark.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define  MODEL <Skylark09/myapp.sch>
#define  SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

SKYLARK(HomePage, "")
{
	Sql sql;
	sql * Select(ID, NAME, LASTNAME)
	      .From(PERSON)
	      .OrderBy(LASTNAME, NAME);
	ValueArray person;
	ValueMap vm;
	while(sql.Fetch(vm))
		person.Add(vm);
	http("PERSON", person)
	    .RenderResult("Skylark09/index");
}

struct MyApp : SkylarkApp {
	virtual void WorkThread();

	MyApp() {
		root = "myapp";
		threads = 1; // Sqlite3 does not like threads...
	#ifdef _DEBUG
		prefork = 0;
		use_caching = false;
	#endif
	}
};

void InitModel()
{
#ifdef _DEBUG
	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	sch.SaveNormal();
#endif
}

void OpenSQL(Sqlite3Session& sqlite3)
{
	if(!sqlite3.Open(ConfigFile("db"))) {
		LOG("Can't create or open database file\n");
		Exit(1);
	}
#ifdef _DEBUG
	sqlite3.LogErrors();
	sqlite3.SetTrace();
#endif
	SQL = sqlite3;
}

void MyApp::WorkThread()
{
	Sqlite3Session sqlite3;
	OpenSQL(sqlite3);
	RunThread();
}

void InitDB()
{
	Sqlite3Session sqlsession;
	OpenSQL(sqlsession);
	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());

	SQL * Insert(PERSON)(NAME,"Joe")(LASTNAME,"Smith");
	SQL * Insert(PERSON)(NAME,"Mike")(LASTNAME,"Carpenter");
	SQL * Insert(PERSON)(NAME,"Jon")(LASTNAME,"Goober");
}

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	StdLogSetup(LOG_FILE|LOG_COUT);
	Ini::skylark_log = true;
#endif

	DeleteFile(ConfigFile("db")); // for this example, always create a new DB
	InitDB();

	MyApp().Run();	
}
