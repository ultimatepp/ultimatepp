#include <MySql/MySql.h>

using namespace Upp;

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <SQL_MultiDB/mysql_schema.sch>
#include <Sql/sch_source.h> // creates the code for mapping schema structures to database rows
#include <Sql/sch_schema.h> // creates the code to generate sql scripts that upload schema to DB
#undef SCHEMADIALECT

namespace DB2 {
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <SQL_MultiDB/sqlite3_schema.sch>
#include <Sql/sch_source.h>
#include <Sql/sch_schema.h>
#undef SCHEMADIALECT
}

CONSOLE_APP_MAIN
{
	Sqlite3Session sqlite3;
	MySqlSession mysql;
	// edit the connection parameters if necessary
	if(mysql.Connect("root", "Passw0rd", "test")) {
		Cout() << "Connected\n";
		SQL = mysql;
		
		SqlSchema sch(MY_SQL);
		All_Tables(sch);
		// create the table if necessary
		SqlPerformScript(sch.Upgrade());
		SqlPerformScript(sch.Attributes());
		SQL.ClearError();
		
		try {
			// insert some random data
			SQL & Insert(TEST_TABLE)(VALUE, Uuid::Create().ToString());
			// fetch some data
			Sql sql;
			sql * Select(ID, VALUE).From(TEST_TABLE)
			                       .OrderBy(Descending(ID))
			                       .Limit(5);
			while(sql.Fetch())
				Cout() << AsString(sql[0]) << ": " << AsString(sql[VALUE]) << "\n";
		}
		catch(SqlExc &ex) {
			Cerr() << "ERROR: " << ex << "\n";
			SetExitCode(1);
		}
	}
	else {
		Cerr() <<"ERROR: Unable to connect to database\n";
		SetExitCode(1);
	}
}
