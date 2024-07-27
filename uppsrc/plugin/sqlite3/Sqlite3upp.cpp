#include <Core/Core.h>
#include <Sql/Sql.h>
#include "lib/sqlite3mc_amalgamation.h"
#include "Sqlite3.h"

namespace Upp {

#define LLOG(x) // LOG(x)

class Sqlite3Connection : public SqlConnection, public Link<> {
protected:
	virtual void        SetParam(int i, const Value& r);
	virtual bool        Execute();
	virtual int         GetRowsProcessed() const;
	virtual Value       GetInsertedId() const;
	virtual bool        Fetch();
	virtual void        GetColumn(int i, Ref f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual String      ToString() const;

	typedef Sqlite3Session::sqlite3 sqlite3;
	typedef Sqlite3Session::sqlite3_stmt sqlite3_stmt;

private:
	Sqlite3Session&  session;
	sqlite3*         db;
	Vector<Value>    param;

	sqlite3_stmt*    current_stmt;
	String           current_stmt_string;
	bool             got_first_row;
	bool             got_row_data;

	friend class Sqlite3Session;
	void             BindParam(int i, const Value& r);
	void             Reset();

public:
	Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db);
	virtual ~Sqlite3Connection();
};

Sqlite3Connection::Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db)
:	session(the_session), db(the_db), current_stmt(NULL), got_first_row(false), got_row_data(false)
{
	LinkBefore(&session.clink);
}

Sqlite3Connection::~Sqlite3Connection()
{
	Cancel();
	Unlink();
}

void Sqlite3Connection::Cancel()
{
	if (current_stmt) {
//		if (sqlite3_reset(current_stmt) != SQLITE_OK)
//			session.SetError(sqlite3_errmsg(db), "Resetting statement: " + current_stmt_string, sqlite3_errcode(db));
//		if (sqlite3_finalize(current_stmt) != SQLITE_OK)
//			session.SetError(sqlite3_errmsg(db), "Finalizing statement: "+ current_stmt_string, sqlite3_errcode(db));
		//this seems to be the correct way how to do error recovery...
		sqlite3_finalize(current_stmt);
		current_stmt = NULL;
		current_stmt_string.Clear();
		parse = true;
	}
}

void Sqlite3Connection::Reset()
{
	if(current_stmt && sqlite3_reset(current_stmt) != SQLITE_OK)
		session.SetError(sqlite3_errmsg(db), "Resetting statement: " + current_stmt_string, sqlite3_errcode(db));
}

void Sqlite3Connection::SetParam(int i, const Value& r)
{
	LLOG(Format("SetParam(%d,%s)",i,r.ToString()));
	param.At(i) = r;
}

void Sqlite3Connection::BindParam(int i, const Value& r) {
	if (IsNull(r))
		sqlite3_bind_null(current_stmt,i);
	else switch (r.GetType()) {
		case SQLRAW_V: {
			SqlRaw p = r;
			sqlite3_bind_blob(current_stmt, i, p, p.GetLength(), SQLITE_TRANSIENT);
			break;
		}
		case STRING_V:
		case WSTRING_V: {
			String p = r;
			sqlite3_bind_text(current_stmt, i, p, p.GetLength(), SQLITE_TRANSIENT);
			break;
		}
		case BOOL_V:
		case INT_V:
			sqlite3_bind_int(current_stmt, i, int(r));
			break;
		case INT64_V:
			sqlite3_bind_int64(current_stmt, i, int64(r));
			break;
		case DOUBLE_V:
			sqlite3_bind_double(current_stmt, i, double(r));
			break;
		case DATE_V: {
				Date d = r;
				String p = Format("%04d-%02d-%02d", d.year, d.month, d.day);
				sqlite3_bind_text(current_stmt,i,p,p.GetLength(),SQLITE_TRANSIENT);
			}
			break;
		case TIME_V: {
				Time t = r;
				String p = Format("%04d-%02d-%02d %02d:%02d:%02d",
						          t.year, t.month, t.day, t.hour, t.minute, t.second);
				sqlite3_bind_text(current_stmt,i,p,p.GetLength(),SQLITE_TRANSIENT);
			}
			break;
		default:
			NEVER();
	}
}

int ParseForArgs(const char* sqlcmd)
{
   int numargs = 0;
   const char* ptr = sqlcmd;
   while (*ptr)
      if(*ptr == '\'')
         while(*++ptr && (*ptr != '\'' || *++ptr && *ptr == '\''))
            ;
      else if(*ptr == '-' && *(ptr+1) == '-')
         while(*++ptr && *ptr != '\n' && *ptr != '\r')
            ;
      else if(*ptr == '/' && *(ptr+1) == '*')
      {
         ptr++;
         while(*++ptr && (*ptr != '*' || (*(ptr+1) && *(ptr+1) != '/')))
            ;
      }
      else if(*ptr++ == '?')
         ++numargs;
   return numargs;
}

bool Sqlite3Connection::Execute() {
	Cancel();
	if(statement.GetLength() == 0) {
		session.SetError("Empty statement", String("Preparing: ") + statement);
		return false;
	}
	String utf8_stmt = ToCharset(CHARSET_UTF8, statement, CHARSET_DEFAULT);
	if (SQLITE_OK != sqlite3_prepare(db,utf8_stmt,utf8_stmt.GetLength(),&current_stmt,NULL)) {
		LLOG("Sqlite3Connection::Compile(" << statement << ") -> error");
		session.SetError(sqlite3_errmsg(db), String("Preparing: ") + statement, sqlite3_errcode(db));
		return false;
	}
	current_stmt_string = statement;
	int nparams = ParseForArgs(current_stmt_string);
	ASSERT(nparams == param.GetCount());
	for (int i = 0; i < nparams; ++i)
		BindParam(i+1,param[i]);
	param.Clear();
	// Make sure that compiling the statement never fails.
	ASSERT(NULL != current_stmt);
	int retcode;
	dword ticks_start = msecs();
	int sleep_ms = 1;
	do{
		retcode = sqlite3_step(current_stmt);
		if(retcode!=SQLITE_BUSY && retcode!=SQLITE_LOCKED) break;
		if(session.busy_timeout == 0) break;
		if(session.busy_timeout > 0 && (int)(msecs() - ticks_start) > session.busy_timeout)
			break;
		if(retcode==SQLITE_LOCKED) sqlite3_reset(current_stmt);
		Sleep(sleep_ms);
		if(sleep_ms<128) sleep_ms += sleep_ms;
	}while(1);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW)) {
		session.SetError(sqlite3_errmsg(db), current_stmt_string, sqlite3_errcode(db));
		return false;
	}
	got_first_row = got_row_data = (retcode==SQLITE_ROW);
//	if (got_row_data) { // By WebChaot, 2009-01-15
		int numfields = sqlite3_column_count(current_stmt);
		info.SetCount(numfields);
		for (int i = 0; i < numfields; ++i) {
			SqlColumnInfo& field = info[i];
			field.name = sqlite3_column_name(current_stmt,i);
			field.binary = false;
			String coltype = sqlite3_column_decltype(current_stmt,i);
			switch (sqlite3_column_type(current_stmt,i)) {
				case SQLITE_INTEGER:
					field.type = INT_V;
					break;
				case SQLITE_FLOAT:
					field.type = DOUBLE_V;
					break;
				case SQLITE_TEXT:
					if(coltype == "date")
						field.type = DATE_V;
					else
					if(coltype == "datetime")
						field.type = TIME_V;
					else
						field.type = STRING_V;
					break;
				case SQLITE_NULL:
					if(coltype == "date")
						field.type = DATE_V;
					else
					if(coltype == "datetime")
						field.type = TIME_V;
					else
					if(coltype.Find("char") >= 0 || coltype.Find("text") >= 0 )
						field.type = STRING_V;
					else
					if(coltype.Find("integer") >= 0)
						field.type = INT_V;
					else
					if(coltype.Find("real") >= 0)
						field.type = DOUBLE_V;
					else
						field.type = VOID_V;
					break;
				case SQLITE_BLOB:
					field.type = STRING_V;
					field.binary = true;
					break;
				default:
					NEVER();
					break;
			}
		}
//	}
	return true;
}

int Sqlite3Connection::GetRowsProcessed() const
{
	LLOG("GetRowsProcessed()");
	return sqlite3_changes(db);
}

Value Sqlite3Connection::GetInsertedId() const
{
	LLOG("GetInsertedId()");
	return sqlite3_last_insert_rowid(db);
}

bool Sqlite3Connection::Fetch() {
	ASSERT(NULL != current_stmt);
	if (!got_row_data)
		return false;
	if (got_first_row) {
		got_first_row = false;
		return true;
	}
	ASSERT(got_row_data);
	int retcode = sqlite3_step(current_stmt);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW))
		session.SetError(sqlite3_errmsg(db), String("Fetching prepared statement: ")+current_stmt_string, sqlite3_errcode(db));
	got_row_data = (retcode==SQLITE_ROW);
	return got_row_data;
}

void Sqlite3Connection::GetColumn(int i, Ref f) const {
	ASSERT(NULL != current_stmt);
	if(i == -1) {
		f = Value(sqlite3_last_insert_rowid(db));
		return;
	}

	ASSERT(got_row_data);
	String coltype;
	const char *s = sqlite3_column_decltype(current_stmt,i);
	if(s) coltype = ToLower(s);
	switch (sqlite3_column_type(current_stmt,i)) {
		case SQLITE_INTEGER:
			f = sqlite3_column_int64(current_stmt,i);
			break;
		case SQLITE_FLOAT:
			f = sqlite3_column_double(current_stmt,i);
			break;
		case SQLITE_TEXT:
			if(coltype == "date" || f.GetType() == DATE_V){
				const char *s = (const char *)sqlite3_column_text(current_stmt, i);
				if(strlen(s) >= 10)
					f = Value(Date(atoi(s), atoi(s + 5), atoi(s + 8)));
				else
					f = Null;
			}
			else
			if(coltype == "datetime" || f.GetType() == TIME_V) {
				const char *s = (const char *)sqlite3_column_text(current_stmt, i);
				if(strlen(s) >= 19)
					f = Value(Time(atoi(s), atoi(s + 5), atoi(s + 8), atoi(s + 11), atoi(s + 14), atoi(s + 17)));
				else
				if(strlen(s) >= 10)
					f = Value(ToTime(Date(atoi(s), atoi(s + 5), atoi(s + 8))));
				else
					f = Null;
			}
			else
				f = Value((const char *)sqlite3_column_text(current_stmt, i));
			break;
		case SQLITE_NULL:
			f = Null;
			break;
		case SQLITE_BLOB:
			f = Value(String( (const byte*)sqlite3_column_blob(current_stmt,i),
			                  sqlite3_column_bytes(current_stmt,i)                ));
			break;
		default:
			NEVER();
			break;
	}
	return;
}

SqlSession& Sqlite3Connection::GetSession() const { return session; }
String Sqlite3Connection::ToString() const {
	return statement;
}

//////////////////////////////////////////////////////////////////////
int Sqlite3Session::SetDBEncryption(int cipher) {
	// "default:cipher" => use SQLCipher during the entire lifetime of database instance
	// CIPHER_CHAHA2020_SQLEET settings are backward compatible with the previous sqleet implementation in the U++
	// Note: It is not recommended to use legacy mode for encrypting new databases. It is supported for compatibility
	// reasons only, so that databases that were encrypted in legacy mode can be accessed.

	int retcode = SQLITE_ERROR;
	switch (cipher) {
		case CIPHER_CHAHA2020_DEFAULT: {
			int value = sqlite3mc_config(db, "default:cipher", CODEC_TYPE_CHACHA20);
			if (value != -1)
				value = sqlite3mc_config_cipher(db, "chacha20", "kdf_iter", 64007);
			if (value != -1)
				value = sqlite3mc_config_cipher(db, "chacha20", "legacy", 0);
			if (value != -1)
				value = sqlite3mc_config_cipher(db, "chacha20", "legacy_page_size", 4096);
			if (value != -1)
				retcode = SQLITE_OK;
			} break;
		case CIPHER_CHAHA2020_SQLEET:
		default: {
			int value = sqlite3mc_config(db, "default:cipher", CODEC_TYPE_CHACHA20);
			if (value != -1)
				value = sqlite3mc_config_cipher(db, "chacha20", "kdf_iter", 12345);
			if (value != -1)
				value = sqlite3mc_config_cipher(db, "chacha20", "legacy", 1);
			if (value != -1)
				value = sqlite3mc_config_cipher(db, "chacha20", "legacy_page_size", 4096);
			if (value != -1)
				retcode = SQLITE_OK;
		} break;
	}
	return retcode;
}

int Sqlite3Session::ChangePassword(const String& password, int cipher) {
	int retcode = SQLITE_OK;

	if (!IsEncrypted()) {
		retcode = SetDBEncryption(cipher);
	}

	if(SQLITE_OK == retcode) {
		retcode = sqlite3_rekey(db, password, password.GetCount());
	}

	if(retcode) {
		SetError(sqlite3_errstr(retcode), "", retcode, sqlite3_errstr(retcode));
	} else {
		encrypted = password.GetCount() > 0;
	}
	return retcode;
}

int Sqlite3Session::CheckDBAccess() {
	// if the db is encrypted and password is wrong or database file is corupted
	// sqlite3_prepare_v2() function return error code SQLITE_NOTADB
	// (File opened that is not a database file)
	sqlite3_stmt *stmt;
	int retcode = sqlite3_prepare_v2(db, "SELECT name FROM sqlite_master", -1, &stmt, 0);
	if (SQLITE_OK == retcode)
		sqlite3_finalize(stmt);
	return retcode;
}

bool Sqlite3Session::Open(const char* filename, const String& password, int cipher) {
	// Only open db once.
	ASSERT(NULL == db);
	current_filename = filename;
	// By default, sqlite3 associates the opened db with "main.*"
	// However, using the ATTACH sql command, it can connect to more databases.
	// I don't know how to get the list of attached databases from the API
	current_dbname = "main";
	int retcode = sqlite3_open(filename, &db);
	if(SQLITE_OK == retcode && password.GetCount() > 0) {
		if(SQLITE_OK == SetDBEncryption(cipher)) {
			retcode = sqlite3_key(db, password, password.GetCount());
		// When setting a new key on an empty database (that is, a database with zero bytes length),
		// you have to make a subsequent write access so that the database will actually be encrypted.
		// You’d usually want to write to a new database anyway, but if not, you can execute the VACUUM
		// statement instead to force SQLite to write to the empty database.
			if (GetFileLength(filename) <= 0) {
				sqlite3_stmt *stmt;
				retcode = sqlite3_prepare_v2(db, "VACUUM;", -1, &stmt, 0);
				if (SQLITE_OK == retcode)
					sqlite3_finalize(stmt);
			}
			if (SQLITE_OK == retcode)
				encrypted = true;
		}
	}
	if(SQLITE_OK == retcode)
		retcode = CheckDBAccess();
	if(SQLITE_OK == retcode)
		return true;
	if(db) {
		SetError(sqlite3_errstr(retcode), "", retcode, sqlite3_errstr(retcode));
		sqlite3_close(db);
		db = NULL;
	}
	return false;
}

void Sqlite3Session::Close() {
	sql.Clear();
	if (NULL != db) {
		SessionClose();
	#ifdef _DEBUG
		int retval = sqlite3_close(db);
		// If this function fails, that means that some of the
		// prepared statements have not been finalized.
		// See lib/sqlite3.h:91
		ASSERT(SQLITE_OK == retval);
	#else
		sqlite3_close(db);
	#endif
		db = NULL;
	}
}

SqlConnection *Sqlite3Session::CreateConnection() {
	return new Sqlite3Connection(*this, db);
}

int Sqlite3Session::SqlExecRetry(const char *sql)
{
	ASSERT(NULL != sql);
	ASSERT(0 != *sql);
	int retcode;
	dword ticks_start = msecs();
	int sleep_ms = 1;
	do{
		retcode = sqlite3_exec(db,sql,NULL,NULL,NULL);
		if(retcode!=SQLITE_BUSY && retcode!=SQLITE_LOCKED) break;
		if(busy_timeout == 0) break;
		if(busy_timeout>0){
			if((int)(msecs()-ticks_start)>busy_timeout) break;
		}//else infinite retry
		Sleep(sleep_ms);
		if(sleep_ms<128) sleep_ms += sleep_ms;
	}while(1);
	return retcode;
}

void Sqlite3Session::Reset()
{
	for(Link<> *s = clink.GetNext(); s != &clink; s = s->GetNext())
		((Sqlite3Connection *)s)->Reset();
}

void Sqlite3Session::Cancel()
{
	for(Link<> *s = clink.GetNext(); s != &clink; s = s->GetNext())
		((Sqlite3Connection *)s)->Cancel();
}

Sqlite3Session::Sqlite3Session()
{
	db = NULL;
	Dialect(SQLITE3);
	busy_timeout = 0;
	see = true;
	encrypted = false;
}

Sqlite3Session::~Sqlite3Session()
{
	Close();
}

void Sqlite3Session::Begin() {
	static const char begin[] = "BEGIN;";
	if(trace)
		*trace << begin << "\n";
	Reset();
	if(SQLITE_OK != SqlExecRetry(begin))
		SetError(sqlite3_errmsg(db), begin, sqlite3_errcode(db), sqlite3_errstr(sqlite3_errcode(db)));
}

void Sqlite3Session::Commit() {
	Cancel();
	static const char commit[] = "COMMIT;";
	if(trace)
		*trace << commit << "\n";
	Reset();
	if(SQLITE_OK != SqlExecRetry(commit))
		SetError(sqlite3_errmsg(db), commit, sqlite3_errcode(db), sqlite3_errstr(sqlite3_errcode(db)));
}

void Sqlite3Session::Rollback() {
	Cancel();
	static const char rollback[] = "ROLLBACK;";
	if(trace)
		*trace << rollback << "\n";
	if(SQLITE_OK != SqlExecRetry(rollback))
		SetError(sqlite3_errmsg(db), rollback, sqlite3_errcode(db), sqlite3_errstr(sqlite3_errcode(db)));
}

Vector<String> Sqlite3Session::EnumDatabases() {
	Vector<String> out;
	Sql sql(*this);
	sql.Execute("PRAGMA database_list;");
	while (sql.Fetch())
		out.Add(sql[1]);  // sql[1] is database name, sql[2] is filename
	return out;
}

Vector<String> Sqlite3Session::EnumTables(String database) {
	Vector<String> out;
	String dbn=database;
	if(dbn.IsEmpty()) dbn=current_dbname; // for backward compatibility
	Sql sql(*this);
	sql.Execute("SELECT name FROM "+dbn+".sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;");
	while (sql.Fetch())
		out.Add(sql[0]);
	return out;
}

Vector<String> Sqlite3Session::EnumViews(String database) {
	Vector<String> out;
	String dbn=database;
	if(dbn.IsEmpty()) dbn=current_dbname;
	Sql sql(*this);
	sql.Execute("SELECT name FROM "+dbn+".sqlite_master WHERE type='view' AND name NOT LIKE 'sqlite_%' ORDER BY 1;");
	while (sql.Fetch())
		out.Add(sql[0]);
	return out;
}

int Sqlite3Session::GetTransactionLevel() const
{
	int autocommit = sqlite3_get_autocommit(db);
	return (autocommit ? 0 : 1);
}

//////////////////////////////////////////////////////////////////////////

Vector<SqlColumnInfo> Sqlite3Session::EnumColumns(String database, String table) {
	Vector<SqlColumnInfo> out;
	SqlColumnInfo info;
	String ColType;
	Sql sql(*this);
	sql.Execute("PRAGMA table_info("+table+");");
	while (sql.Fetch()) {
		info.width = info.scale = info.precision = 0;
		info.name = sql[1];
		ColType   = sql[2];
		if(ColType =="integer")
			info.type = INT_V;
		else
		if(ColType =="real")
			info.type = DOUBLE_V;
		else
		if (ColType =="date")
			info.type = DATE_V;
		else
		if (ColType == "datetime")
			info.type = TIME_V;
		else
			info.type = STRING_V;
		out.Add(info);
	}
	return out;
}

//////////////////////////////////////////////////////////////////////

const char *Sqlite3ReadString(const char *s, String& stmt) {
	stmt.Cat(*s);
	int c = *s++;
	for(;;) {
		if(*s == '\0') break;
		else
		if(*s == '\'' && s[1] == '\'') {
			stmt.Cat("\'\'");
			s += 2;
		}
		else
		if(*s == c) {
			stmt.Cat(c);
			s++;
			break;
		}
		else
		if(*s == '\\') {
			stmt.Cat('\\');
			if(*++s)
				stmt.Cat(*s++);
		}
		else
			stmt.Cat(*s++);
	}
	return s;
}

bool Sqlite3PerformScript(const String& txt, StatementExecutor& se, Gate<int, int> progress_canceled) {
	const char *text = txt;
	for(;;) {
		String stmt;
		while(*text <= 32 && *text > 0) text++;
		if(*text == '\0') break;
		for(;;) {
			if(*text == '\0')
				break;
			if(*text == ';')
				break;
			else
			if(*text == '\'')
				text = Sqlite3ReadString(text, stmt);
			else
			if(*text == '\"')
				text = Sqlite3ReadString(text, stmt);
			else
				stmt.Cat(*text++);
		}
		if(progress_canceled(int(text - txt.Begin()), txt.GetLength()))
			return false;
		if(!se.Execute(stmt))
			return false;
		if(*text) text++;
	}
	return true;
}

}
