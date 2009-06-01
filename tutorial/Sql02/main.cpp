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

	for(int i = 0; i < 10; i++)
		SQL.Execute("insert into TEST(A, B) values (?, ?)", i, AsString(3 * i));

	Sql sql;
	sql.Execute("select * from TEST");
	for(int i = 0; i < sql.GetColumns(); i++)
		Cout() << sql.GetColumnInfo(i).name << '\n';
	while(sql.Fetch())
		Cout() << sql[0] << " \'" << sql[1] << "\'\n";
}
