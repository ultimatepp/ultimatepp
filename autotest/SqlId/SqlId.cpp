#include <Sql/Sql.h>

using namespace Upp;

SQLID(COLUMN)
SQLID(TABLE)

SQLID(column)
SQLID(table)

void Test(SqlStatement s, const char *e)
{
	String h = s.Get(PGSQL);
	
	LOG(h);
	LOG("   " << AsCString(h));
	
	ASSERT(h == e);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Test(Select(COLUMN).From(TABLE), "select COLUMN from TABLE");
	SqlId::UseQuotes();
	Test(Select(COLUMN).From(TABLE), "select \"COLUMN\" from \"TABLE\"");
	SqlId::ToUpperCase();
	Test(Select(column).From(table), "select \"COLUMN\" from \"TABLE\"");
	SqlId::ToLowerCase();
	Test(Select(COLUMN).From(TABLE), "select \"column\" from \"table\"");
	SqlId::UseQuotes(false);
	SqlId::ToUpperCase();
	Test(Select(column).From(table), "select COLUMN from TABLE");
	SqlId::ToLowerCase();
	Test(Select(COLUMN).From(TABLE), "select column from table");
	
	LOG("================ OK");
}
