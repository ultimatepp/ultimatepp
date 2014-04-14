#include <Sql/Sql.h>

using namespace Upp;

SQLID(COLUMN)
SQLID(TABLE)

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	SqlStatement s = Select(COLUMN).From(TABLE);
	LOG(s.Get(PGSQL));
	SqlId::UseQuotes();
	LOG(s.Get(PGSQL));
	SqlId::ToLowerCase();
	LOG(s.Get(PGSQL));
}
