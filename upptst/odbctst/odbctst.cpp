#include <ODBC/ODBC.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ODBCSession session;
	session.LogErrors();
	session.SetTrace();
	if(!session.Connect("DRIVER=SQL Server Native Client 10.0;SERVER=QUAD\\SQLEXPRESS;DATABASE=Holly;UID=test;PWD=koblih;"))
		return;
	LOG("Connected!");
	SQL = session;
	Sql sql;
	sql.Execute("select AID, TEXT, SINCE, TILL from SUBJECT_ATTR where SINCE > ?", Date(2009, 2, 20));
//	sql.Execute("select ID, TEXT, SINCE, TILL from SUBJECT_ATTR where ID > ?", 1700);
	while(sql.Fetch()) {
		for(int i = 0; i < sql.GetColumns(); i++) {
			const SqlColumnInfo& f = sql.GetColumnInfo(i);
			LOG(f.name << '=' << sql[i]);
		}
	}
}

