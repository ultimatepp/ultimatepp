#include "Sql.h"

namespace Upp {

bool StdStatementExecutor::Execute(const String& stmt)
{
	cursor.Execute(stmt);
	return true;
}

#ifndef NOAPPSQL
struct SQLStatementExecutorClass : StatementExecutor {
	virtual bool Execute(const String& stmt) { SQL.Execute(stmt); return true; }
};

StatementExecutor& SQLStatementExecutor() {
	return Single<SQLStatementExecutorClass>();
}
#endif

bool SqlPerformScript(SqlSession& session, Stream& script,
                      Gate<int, int> progress_canceled, bool stoponerror)
{
	String stmt;
	int level = 0;
	bool ok = true;
	bool esc = false;
	while(!script.IsEof()) {
		int c = script.Term();
		if(IsAlpha(c) || c == '_') {
			String id;
			while(IsAlNum(script.Term()) || script.Term() == '_') {
				c = script.Get();
				stmt.Cat(c);
				id.Cat(ToUpper(c));
			}
			if(!esc) {
				if(id == "BEGIN")
					level++;
				if(id == "END") {
					level--;
					stmt.Cat(';');
				}
			}
		}
		else
		if(c == '\'') {
			stmt.Cat(c);
			script.Get();
			for(;;) {
				c = script.Get();
				if(c < 0) {
					ok = false;
					if(stoponerror)
						return false;
					break;
				}
				stmt.Cat(c);
				if(c == '\'')
					break;
			}
		}
		else
		if(session.GetDialect() == PGSQL && c == '$') {
			stmt.Cat('$');
			script.Get();
			if(script.Term() == '$') {
				script.Get();
				stmt.Cat('$');
				esc = !esc;
			}
		}
		else
		if(c == ';' && level == 0 && !esc) {
			Sql sql(session);
			session.ClearError();
			int q = 0;
			while(stmt[q] == '\r' || stmt[q] == '\n')
				q++;
			stmt = stmt.Mid(q);
			if(!sql.Execute(stmt)) {
				ok = false;
				if(stoponerror)
					break;
			}
			stmt.Clear();
			script.Get();
		}
		else {
			if(!esc) {
				if(c == '(')
					level++;
				if(c == ')')
					level--;
			}
			if(c != '\r') {
				if(session.GetDialect() == ORACLE && c == '\n')
					stmt.Cat('\r');
				stmt.Cat(c);
			}
			script.Get();
		}
	}
	return ok;
}

bool SqlPerformScript(SqlSession& session, const String& script,
                      Gate<int, int> progress_canceled, bool stoponerror)
{
	StringStream ss(script);
	return SqlPerformScript(session, ss, progress_canceled, stoponerror);
}

#ifndef NOAPPSQL

bool SqlPerformScript(Stream& script,
                      Gate<int, int> progress_canceled, bool stoponerror)
{
	return SqlPerformScript(SQL.GetSession(), script, progress_canceled, stoponerror);
}

bool SqlPerformScript(const String& script,
                      Gate<int, int> progress_canceled, bool stoponerror)
{
	return SqlPerformScript(SQL.GetSession(), script, progress_canceled, stoponerror);
}

#endif

}
