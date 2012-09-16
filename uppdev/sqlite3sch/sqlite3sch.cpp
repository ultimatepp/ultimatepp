#include <Core/Core.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <sqlite3sch/schema.sch>
#include "Sql/sch_header.h"

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
	
	SQL.Execute("drop table PERSON");
	
	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	SQL.ClearError();
	
	{
		SqlMassInsert ins(PERSON);
		ins(NAME, "a name")(ADDRESS, "an address").EndRow();
	}
	
	DDUMP(SQL ^ Select(ADDRESS, NAME).From(PERSON));
	Value v = SQL ^ Select(ADDRESS, NAME).From(PERSON);
	DDUMP(v[ADDRESS]);
	DDUMP(v[NAME]);
	
	{
		SqlMassInsert ins(PERSON);
		for(int i = 0; i < 10000; i++)
			ins(ADDRESS, "adr " + AsString(i))(NAME, "name " + AsString(i)).EndRow();
	}
	DDUMP(SQL ^ Select(ADDRESS, NAME).From(PERSON));
}
