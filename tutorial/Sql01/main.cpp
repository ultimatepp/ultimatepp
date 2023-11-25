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

	Sql sql(sqlite3);
	sql.Execute("select date('now')");
	while(sql.Fetch())
		Cout() << sql[0] << '\n';
}
