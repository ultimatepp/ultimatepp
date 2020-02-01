#include "simple.h"

#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

#include <Sql/sch_source.h>

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	LOG("-- Starting new run of Sqlite3 simple test");
#ifdef _DEBUG
	LOG("Debug mode!");
#endif

	DDUMP(~SIMPLE_TEST1);
	DDUMP(~NAME);

	Sqlite3Session sqlite3;
	sqlite3.LogErrors(true);
	sqlite3.SetTrace();
	if(!sqlite3.Open(ConfigFile("simple.db"))) {
		LOG("Can't create or open database file\n");
		return;
	}

	SQL = sqlite3;

	// Update the schema to match the schema described in "simple.sch"
#ifdef _DEBUG
	SqlSchema sch(SQLITE3);
	All_Tables(sch);

	SqlPerformScript(sch.SchemaDrop());
	SqlPerformScript(sch.Upgrade());

	sch.SaveNormal();
#endif

	// Now test out some functionality:
	//--------------------------------------------------------------------
	// Get the list of tables
	Vector<String> table_list = sqlite3.EnumTables("");
	LOG(Format("Tables: (%d)",table_list.GetCount()));
	for (int i = 0; i < table_list.GetCount(); ++i)
		LOG(Format("  #%d: %s",i+1,table_list[i]));
	
	DDUMP(~SIMPLE_TEST1);
	DDUMP(~NAME);


	S_SIMPLE_TEST1 row;
	Sql sql;

	// Test basic insertion:
	// (check buglog for errors about duplicate entries if you run this more
	//  than once on the same db!)
	sql*Insert(SIMPLE_TEST1)(ID,0)(NAME,"Joe")(LASTNAME,"Smith")(BDATE,20000101);
	LOG(sql.ToString());
	sql*Insert(SIMPLE_TEST1)(ID,1)(NAME,"Mike")(LASTNAME,"Smith")(BDATE,20000102);
	LOG(sql.ToString());
	sql*Insert(SIMPLE_TEST1)(ID,2)(NAME,"Jon")(LASTNAME,"Goober")(BDATE,20000103);
	LOG(sql.ToString());

	// Test deletion
	sql*Delete(SIMPLE_TEST1).Where(ID > 3);
	LOG(sql.ToString());

	// Test insertion
	sql*Insert(SIMPLE_TEST1)(ID,5)(NAME,"wrongname")(LASTNAME,"wronglastname")(BDATE,20010604);
	LOG(sql.ToString());

	// Test update
	sql*SqlUpdate(SIMPLE_TEST1)(NAME,"rightname")(LASTNAME,"rightlastname")(BDATE,20010604).Where(ID==5);
	LOG(sql.ToString());

	// Test insertion with args
	Sql insert("insert into SIMPLE_TEST1(name,bdate) values(?,?)",sqlite3);
	insert.Run("joseph",19990101);
	insert.Run(  "neel",19990102);
	insert.Run(   "bob",19990103);
	insert.Run(    "al",19990104);

	// Test selection:
	sql*Select(row).From(SIMPLE_TEST1);
	LOG(sql.ToString());
	while (sql.Fetch()) {
		LOG(Format("%d %s %s %d",sql[0],sql[1],sql[2],sql[3]));
		LOG(Format("%s %s %s %s",sql[ID],sql[NAME],sql[LASTNAME],sql[BDATE]));
	}

	// Test selection using *
	sql*Select(SqlAll()).From(SIMPLE_TEST1);
	LOG(sql.ToString());
	while (sql.Fetch()) {
		LOG(Format("%d %s %s %d",sql[0],sql[1],sql[2],sql[3]));
		LOG(Format("blank: %s %s %s %s",sql[ID],sql[NAME],sql[LASTNAME],sql[BDATE]));
	}
	
	DDUMP(GetSchAll());
	DDUMP(GetSchTables());
	DDUMP(GetSchColumns("TEST"));

	LOG("-- Normal exit of Sqlite3 simple test");
}
