#include "AddressBook.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

#define IMAGECLASS AdrBookImg
#define IMAGEFILE <AddressBookWeb/AdrBook.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS AdrBookImg
#define IMAGEFILE <AddressBookWeb/AdrBook.iml>
#include <Draw/iml_source.h>

struct AdrBook : SkylarkApp {
	void WorkThread();

	AdrBook();
};

void OpenSQL(Sqlite3Session& sqlite3)
{
	if(!sqlite3.Open(ConfigFile("db.sqlite3"))) {
		LOG("Can't create or open database file\n");
		Exit(1);
	}
#ifdef _DEBUG
	sqlite3.LogErrors();
	sqlite3.SetTrace();
#endif
	SQL = sqlite3;
}

void InitModel()
{
#ifdef _DEBUG
	Sqlite3Session sqlsession;
	OpenSQL(sqlsession);
	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	sch.SaveNormal();
#endif
}

void AdrBook::WorkThread()
{
	Sqlite3Session sqlite3;
	OpenSQL(sqlite3);
	RunThread();
}

AdrBook::AdrBook()
{
	prefork = 0;
	threads = 1; // Sqlite3 is not good with threads or concurrency...
#ifdef _DEBUG
	use_caching = false;
#endif
}

// After starting the server, enter "127.0.0.1:8001" in your browser

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	StdLogSetup(LOG_FILE|LOG_COUT);
	Ini::skylark_log = true;
	SKYLARKLOG("Active");
#endif
	InitModel();
	AdrBook().Run();	
}
