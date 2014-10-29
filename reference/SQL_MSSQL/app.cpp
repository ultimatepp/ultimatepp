#include "app.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	MSSQLSession mssql;
	for(;;) {
		String cs = "Driver={SQL Server Native Client 11.0};";
		Cout() << "Server:";
		cs << "Server=" << ReadStdIn() << ';';
		Cout() << "User (empty for trusted connection):";
		String h = ReadStdIn();
		if(IsNull(h))
			cs << "Trusted_Connection=Yes;";
		else {
			cs << "UID=" << h << ';';
			Cout() << "Password:";
			cs << "PWD=" << ReadStdIn() << ';';
			Cout() << "Database (empty for default):";
			h = ReadStdIn();
			if(IsNull(h))
				cs << "Database=" << h << ';';
		}
		if(!mssql.Connect(cs))
			Cout() << "Connect failed: " << mssql.GetLastError() << '\n';
		else
			break;
	}
	SQL = mssql;

#ifdef _DEBUG
	mssql.SetTrace();
#endif

	SqlSchema sch(MSSQL);
	StdStatementExecutor se(SQL.GetSession());
	All_Tables(sch);
	ODBCPerformScript(sch.Upgrade(), se);
	ODBCPerformScript(sch.Attributes(), se);
	
	for(int i = 0; i < 10; i++)
		SQL * Insert(TEST)(TEXT, String('A' + i, 1));
	
	S_TEST tst;
	Sql sql;
	sql * Select(tst).From(TEST);
	while(sql.Fetch(tst))
		Cout() << tst.ID << ", " << tst.TEXT << '\n';
}
