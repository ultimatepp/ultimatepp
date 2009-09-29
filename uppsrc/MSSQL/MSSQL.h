#ifndef _MSSQL_MSSQL_h
#define _MSSQL_MSSQL_h

#include "ODBC/ODBC.h"

namespace Upp {

String MsSqlTextType(int width);

typedef ODBCSession MSSQLSession;

inline
bool   MSSQLPerformScript(const String& text, StatementExecutor& executor,
                         Gate2<int, int> progress_canceled = false)
{
	return ODBCPerformScript(text, executor, progress_canceled);
}

void IdentityInsert(Sql& sql, const SqlInsert& ins);
void IdentityInsert(const SqlInsert& ins);

};

#endif
