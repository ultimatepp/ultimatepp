#include <plugin/sqlite3/Sqlite3.h>

SqlId all("*");
SqlId name("name");
SqlId date("date");
SqlId list("list");

CONSOLE_APP_MAIN
{
	Sqlite3Session session;
	if(!session.Open("C:\\Research\\Sqlite3\\test.db")) {
		printf("Can't connect to database file\n");
		return;
	}

	// Get the list of tables	
	Vector<String> table_list = session.EnumTables("");
	LOG(Format("Tables: (%d)",table_list.GetCount()));
	for (int i = 0; i < table_list.GetCount(); ++i)
		LOG(Format("  #%d: %s",i+1,table_list[i]));
	
	Sql sql(session);

	// Test to make sure it caches 
	sql.Execute("select * from list");
	sql*Select(all).From(list);
	sql*Select(name).From(list);

	// Test retrieving info from db
	sql*Select(all).From(list);
	LOG(sql.ToString());
	while(sql.Fetch())
		LOG(Format("%d: %s %d",sql[0],sql[1],sql[2]));
	
	// Test conditional retrieving info from db
	sql*Select(name,date).From(list).Where(date<20040101);
	LOG(sql.ToString());
	while(sql.Fetch())
		LOG(Format("?: %s %d",sql[0],sql[1]));
}
