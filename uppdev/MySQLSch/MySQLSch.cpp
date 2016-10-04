#include <MySql/MySql.h>

using namespace Upp;

SQLID(ADDRESSBOOK);
SQLID(FIRSTNAME);
SQLID(ID);

CONSOLE_APP_MAIN
{
	MySqlSession session;
	// substitute your 'username' and 'password' here:
	if(!session.Connect("root", "Passw0rd", "adrbook", "localhost")) {
		DDUMP(session.GetErrorCodeString());
		printf("Can't connect with MySql\n");
		return;
	}
	SQL = session;
	
	session.SetTrace();
	
	Sql sql;
	sql.Execute("use adrbook");
	sql.Execute("show tables");
	while(sql.Fetch())
		DDUMP((String)sql[0]);
	
	SQL * Insert(ADDRESSBOOK)(FIRSTNAME, "Pepíček");
	
	DDUMP(SQL.GetInsertedId());
	
	sql * Select(SqlAll().Of(ADDRESSBOOK)).From(ADDRESSBOOK);
	while(sql.Fetch())
		DDUMP(sql.GetRowMap());
}
