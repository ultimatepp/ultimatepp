#include "Sql.h"

namespace Upp {

void SqlSession::Attach(Sql& sql, SqlConnection *con)
{
	sql.Attach(con); // Duck tape to fix OleDB
}

SqlSession::SqlSession()
{
	trace = NULL;
	traceslow = INT_MAX / 4;
	tracetime = false;
	dialect = 255;
	errorcode_number = Null;
	errorclass = Sql::ERROR_UNSPECIFIED;
	error_handler = NULL;
	throwonerror = false;
	error_log = NULL;
	trace_compression = true;
}

SqlSession::~SqlSession()
{
}

void           SqlSession::Begin()                                       { NEVER(); }
void           SqlSession::Commit()                                      { NEVER(); }
void           SqlSession::Rollback()                                    { NEVER(); }
String         SqlSession::Savepoint()                                   { NEVER(); return Null; }
void           SqlSession::RollbackTo(const String&)                     { NEVER(); }
bool           SqlSession::IsOpen() const                                { return false; }
int            SqlSession::GetTransactionLevel() const                   { return 0; }
RunScript      SqlSession::GetRunScript() const                          { return NULL; }
SqlConnection *SqlSession::CreateConnection()                            { return NULL; }
Vector<String> SqlSession::EnumUsers()                                   { return Vector<String>(); }
Vector<String> SqlSession::EnumDatabases()                               { return Vector<String>(); }
Vector<String> SqlSession::EnumTables(String database)                   { return Vector<String>(); }
Vector<String> SqlSession::EnumViews(String database)                    { return Vector<String>(); }
Vector<String> SqlSession::EnumSequences(String database)                { return Vector<String>(); }
Vector<String> SqlSession::EnumPrimaryKey(String database, String table) { return Vector<String>(); }
Vector<String> SqlSession::EnumReservedWords()                           { return Vector<String>(); }
String         SqlSession::EnumRowID(String database, String table)      { return Null; }

Vector<SqlColumnInfo> SqlSession::EnumColumns(String database, String table)
{
	Sql cursor(*this);
	Vector<SqlColumnInfo> info;
	SqlBool none;
	none.SetFalse();
	String full_name = database;
	if(!IsNull(database))
		full_name << '.';
	full_name << table;
	if(cursor.Execute(Select(SqlAll()).From(SqlSet(SqlId(full_name))).Where(none))) {
		info.SetCount(cursor.GetColumns());
		for(int i = 0; i < info.GetCount(); i++)
			info[i] = cursor.GetColumnInfo(i);
	}
	return info;
}

void   SqlSession::SetError(String error, String stmt, int code, const char *scode, Sql::ERRORCLASS clss) {
	if(error_handler && (*error_handler)(error, stmt, code, scode, clss))
		return;
	if(GetTransactionLevel() && errorstatement.GetCount())
		return;
	lasterror = error;
	errorstatement = stmt;
	errorcode_number = code;
	errorcode_string = scode;
	errorclass = clss;
	String err;
	err << "ERROR " << error << "(" << code << "): " << stmt << '\n';
	if(error_log)
		*error_log << err;
	if(trace && trace != error_log)
		*GetTrace() << err;
}

void SqlSession::InstallErrorHandler(bool (*handler)(String error, String stmt, int code, const char *scode, Sql::ERRORCLASS clss))
{
	error_handler = handler;
}

void Detach_SQL();

StaticMutex          sDefs;
static SqlSession   *sGlobalSession;
static SqlSession   *sGlobalSessionR;


static bool sPerThread;

thread_local SqlSession *sThreadSession;
thread_local SqlSession *sThreadSessionR;

void SqlSession::SessionClose()
{
#ifndef flagNOAPPSQL
	if(SQL.IsOpen() && &SQL.GetSession() == this) {
		SQL.Cancel();
		SQL.Detach();
		if(sPerThread)
			sThreadSession = NULL;
		else
			sGlobalSession = NULL;
	}
	if(SQLR.IsOpen() && &SQLR.GetSession() == this) {
		SQLR.Cancel();
		SQLR.Detach();
		if(sPerThread)
			sThreadSessionR = NULL;
		else
			sGlobalSessionR = NULL;
	}
#endif
	if(sql) {
		sql->Cancel();
		sql.Clear();
	}
	if(sqlr) {
		sqlr->Cancel();
		sqlr.Clear();
	}
}

Sql& SqlSession::GetSessionSql()
{
	if(!sql)
		sql = new Sql(*this);
	return *sql;
}

Sql& SqlSession::GetSessionSqlR()
{
	if(!sqlr)
		sqlr = new Sql(*this);
	return *sqlr;
}

void   SqlSession::ClearError()
{
	lasterror.Clear();
	errorstatement.Clear();
	errorcode_number = Null;
	errorcode_string = Null;
	errorclass = Sql::ERROR_UNSPECIFIED;
}

void Sql::PerThread(bool b)
{
	sPerThread = b;
}

void SqlSession::PerThread(bool b)
{
	sPerThread = b;
}

#ifndef NOAPPSQL

void Detach_SQL()
{
	if(sPerThread)
		sThreadSessionR = sThreadSession = NULL;
	else
		sGlobalSessionR = sGlobalSession = NULL;
}

void Sql::operator=(SqlSession& s)
{
	Mutex::Lock __(sDefs);
	if(this == &AppCursor()) {
		if(sPerThread)
			sThreadSession = &s;
		else
			sGlobalSession = &s;
		return;
	}
	if(this == &AppCursorR()) {
		if(sPerThread)
			sThreadSessionR = &s;
		else
			sGlobalSessionR = &s;
		return;
	}
	NEVER();
}

Sql& AppCursor()
{
	if(sPerThread) {
		if(sThreadSession)
			return sThreadSession->GetSessionSql();
	}
	else
		if(sGlobalSession)
			return sGlobalSession->GetSessionSql();
	static Sql *empty;
	ONCELOCK {
		static Sql0 h;
		empty = &h;
	}
	return *empty;
}

Sql& AppCursorR()
{
	if(sPerThread) {
		if(sThreadSessionR)
			return sThreadSessionR->GetSessionSqlR();
	}
	else
	if(sGlobalSessionR)
		return sGlobalSessionR->GetSessionSqlR();
	if(sPerThread) {
		if(sThreadSession)
			return sThreadSession->GetSessionSqlR();
	}
	else
	if(sGlobalSession)
		return sGlobalSession->GetSessionSqlR();
	static Sql *empty;
	ONCELOCK {
		static Sql0 h;
		empty = &h;
	}
	return *empty;
}

#endif

}
