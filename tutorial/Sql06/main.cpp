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

	S_TEST x;
	for(int i = 0; i < 10; i++) {
		x.A = i;
		x.B = AsString(3 * i);
		SQL * Insert(x);
	}

	Sql sql;
	sql * Select(x).From(TEST);
	while(sql.Fetch(x))
		Cout() << x.A << " \'" << x.B << "\'\n";
}
