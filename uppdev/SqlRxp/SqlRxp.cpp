#include "SqlExp.h"

SqlId A("A");
SqlId TABLE("TABLE");
SqlId B("B");

CONSOLE_APP_MAIN
{
	SqlSet s = Select(Nvl(A, B)).From(TABLE).Where(A >= GetSysDate());
	DUMP(SqlCompile(MYSQL, ~s));
	DUMP(SqlCompile(MSSQL, ~s));
	DUMP(SqlCompile(SQLITE3, ~s));
	DUMP(SqlCompile(ORACLE, ~s));
}
