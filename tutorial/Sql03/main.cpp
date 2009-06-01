#include <Core/Core.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

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
	
	SQL.Execute("drop table TEST");
	SQL.ClearError();
	SQL.Execute("create table TEST (A INTEGER, B TEXT)");

	SqlId A("A"), B("B"), TEST("TEST");

	for(int i = 0; i < 10; i++)
		SQL * Insert(TEST)(A, i)(B, AsString(3 * i));

	Sql sql;
	sql * Select(A, B).From(TEST);
	while(sql.Fetch())
		Cout() << sql[A] << " \'" << sql[B] << "\'\n";
}
