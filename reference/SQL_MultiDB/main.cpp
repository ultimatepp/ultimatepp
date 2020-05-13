#include "multidb.h"

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <SQL_MultiDB/mysql_schema.sch>
#include <Sql/sch_source.h> // creates the code for mapping schema structures to database rows and SqlIds
#include <Sql/sch_schema.h> // creates the code to generate sql scripts that upload schema to DB
#undef MODEL
#undef SCHEMADIALECT

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Sqlite3Session sqlite3;
	MySqlSession mysql;
	// edit the connection parameters if necessary
	if(!mysql.Connect("root", "Passw0rd", "test")) {
		LOG("Failed to connect MySQL!");
		Exit(1);
	}
	mysql.SetTrace();

	if(!sqlite3.Open(ConfigFile("test.db"))) {
		LOG("Failed to connect Sqlite3!");
		Exit(1);
	}
	sqlite3.SetTrace();

	SQL = mysql; // MySql will be our default database
	
	{ // Create/Upgrade DB schema in MySql
		SqlSchema sch(MY_SQL);
		All_Tables(sch);
		SqlPerformScript(sch.Upgrade());
		SqlPerformScript(sch.Attributes());
		SQL.ClearError();
	}

	{ // Create/Upgrade DB schema in Sqlite3
		SqlSchema sch(SQLITE3);
		DB2::DB2_All_Tables(sch);
		SqlPerformScript(sqlite3, sch.Upgrade());
		SqlPerformScript(sqlite3, sch.Attributes());
		SQL.ClearError();
	}

	{ // Do something with MySql
		SQL * Insert(TEST_TABLE)(VALUE, FormatIntBase(Random(), 36));
		Sql sql;
		sql * Select(ID, VALUE).From(TEST_TABLE);
		while(sql.Fetch())
			LOG("ID: " << sql[ID] << ", VALUE: " << sql[VALUE]);
	}

	{ // Do something with Sqlite3; ID and VALUE SqlIds are from mysql schema, but these are just text, so no problem
		Sql sql(sqlite3); // need to specify non-default session in the constructor!
		sql * Insert(TEST_TABLE)(VALUE, FormatIntBase(Random(), 36))(DB2::NAME, AsString(Random()));
		sql * Select(ID, VALUE, DB2::NAME).From(DB2::TEST_TABLE); // the value of DB2::TEST_TABLE and TEST_TABLE is the same, using DB2:: here just for demonstration
		while(sql.Fetch())
			LOG("ID: " << sql[ID] << ", VALUE: " << sql[VALUE] << ", NAME: " << sql[DB2::NAME]);
	}
}
