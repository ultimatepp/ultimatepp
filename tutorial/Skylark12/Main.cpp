#include "Skylark12.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

Skylark12::Skylark12()
{
#ifdef _DEBUG
	prefork = 0;
	use_caching = false;
#endif
	threads = 1; // Sqlite3 does not work well with multiple threads
	port = 8001;
}

void OpenSQL(Sqlite3Session& session)
{
	if(!session.Open(ConfigFile("db.sqlite3"))) {
		SKYLARKLOG("Can't create or open database file");
		Exit(1);
	}
#ifdef _DEBUG
	session.LogErrors();
	session.SetTrace();
#endif
	SQL = session;
}

void InitModel()
{
#ifdef _DEBUG
	Sqlite3Session session;
	OpenSQL(session);
	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	sch.SaveNormal();
#endif
}

void Skylark12::WorkThread()
{
	Sqlite3Session session;
	OpenSQL(session);
	RunThread();
}

// Local server URL: 127.0.0.1:8001
CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	StdLogSetup(LOG_FILE|LOG_COUT);
	Ini::skylark_log = true;
#endif
	InitModel();
	Skylark12().Run();
}

