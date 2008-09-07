#include "simple.h"

#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

#include <Sql/sch_source.h>

CONSOLE_APP_MAIN
{
	LOG("-- Starting new run of Sqlite3 simple test");
#ifdef _DEBUG
	LOG("Debug mode!");
#endif

	Sqlite3Session sqlite3;
	sqlite3.LogErrors(true);
	if(!sqlite3.Open(ConfigFile("simple.db"))) {
		LOG("Can't create or open database file\n");
		return;
	}

	SQL = sqlite3;

	// Update the schema to match the schema described in "simple.sch"
	#ifdef _DEBUG
	SqlSchema sch(SQLITE3);
	StdStatementExecutor se(sqlite3);
	All_Tables(sch);
	if(sch.ScriptChanged(SqlSchema::UPGRADE))
		Sqlite3PerformScript(sch.Upgrade(),se);
	if(sch.ScriptChanged(SqlSchema::ATTRIBUTES)) {
		Sqlite3PerformScript(sch.Attributes(),se);
	}
	if(sch.ScriptChanged(SqlSchema::CONFIG)) {
		Sqlite3PerformScript(sch.ConfigDrop(),se);
		Sqlite3PerformScript(sch.Config(),se);
	}
	sch.SaveNormal();
	#endif
	
	SQL * Insert(TEST)
			(ID, 1)
			(DATE_TEST, Date(1234, 12, 31))
			(TIME_TEST, GetSysTime())
	;
	SQL * Select(DATE_TEST, TIME_TEST).From(TEST);
	while(SQL.Fetch()) {
		Date d = SQL[0];
		Time tm = SQL[1];
		DUMP(d);
		DUMP(tm);
	}
}
