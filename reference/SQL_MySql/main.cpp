#include <MySql/MySql.h>

// debian: sudo apt-get install libmysqlclient-dev

using namespace Upp;

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <SQL_MySql/schema.sch>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>
#include <Sql/sch_schema.h>

CONSOLE_APP_MAIN
{
	MySqlSession session;
	// edit the connection parameters if necessary
	if(session.Connect("root", "Passw0rd", "test")) {
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
