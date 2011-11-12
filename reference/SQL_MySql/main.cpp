#include <MySql/MySql.h>

using namespace Upp;

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <schema/schema.sch>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>
#include <Sql/sch_schema.h>

CONSOLE_APP_MAIN
{
	MySqlSession session;
	// edit the connection parameters if necessary
	if(session.Connect("root", NULL, "test")) {
		Cout() << "Connected\n";
		SQL = session;
		
		SqlSchema sch(MY_SQL);
		All_Tables(sch);
		// create the table if necessary
		SqlPerformScript(sch.Upgrade());
		SqlPerformScript(sch.Attributes());
		SQL.ClearError();
		
		try {
			// insert some random data
			SQL & Insert(TEST_TABLE)(value, Uuid::Create().ToString());
			// fetch some data
			Sql sql;
			SQL * Select(ID, VALUE).From(TEST_TABLE)
			                       .OrderBy(Descending(id))
			                       .Limit(5);
			while(SQL.Fetch())
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
	SetExitCode(0);
}
