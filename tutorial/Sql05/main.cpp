#include "MyApp.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

CONSOLE_APP_MAIN
{
	Sqlite3Session sqlite3;
	if(!sqlite3.Open(ConfigFile("simple.db"))) {
		Cout() << "Can't create or open database file\n";
		return;
	}
	
#ifdef _DEBUG
	sqlite3.SetTrace();
#endif

	SQL = sqlite3;

	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	SQL.ClearError();

	for(int i = 0; i < 10; i++)
		SQL * Insert(TEST)(A, i)(B, AsString(3 * i));

	Sql sql;
	sql * Select(A, B).From(TEST);
	while(sql.Fetch())
		Cout() << sql[A] << " \'" << sql[B] << "\'\n";
}
