#include "PostgreSQL.h"

#ifdef PLATFORM_WIN32
#include "Winsock2.h"
#endif

#ifdef PLATFORM_POSIX
#include <sys/socket.h>
#endif

#define LLOG(x) // DLOG(x)

#ifndef flagNOPOSTGRESQL

namespace Upp {

enum PGSQL_StandardOid {
	PGSQL_BOOLOID = 16,
	PGSQL_BYTEAOID = 17,
	PGSQL_CHAROID = 18,
	PGSQL_NAMEOID = 19,
	PGSQL_INT8OID = 20,
	PGSQL_INT2OID = 21,
	PGSQL_INT2VECTOROID = 22,
	PGSQL_INT4OID = 23,
	PGSQL_REGPROCOID = 24,
	PGSQL_TEXTOID = 25,
	PGSQL_OIDOID = 26,
	PGSQL_TIDOID = 27,
	PGSQL_XIDOID = 28,
	PGSQL_CIDOID = 29,
	PGSQL_OIDVECTOROID = 30,
	PGSQL_FLOAT4OID = 700,
	PGSQL_FLOAT8OID = 701,
	PGSQL_DATEOID = 1082,
	PGSQL_TIMEOID = 1083,
	PGSQL_TIMESTAMPOID = 1114,
	PGSQL_TIMESTAMPZOID = 1184,
	PGSQL_NUMERICOID = 1700
};

int OidToType(Oid oid)
{
	switch(oid) {
		case PGSQL_BOOLOID:
			return BOOL_V;
		case PGSQL_INT8OID:
			return INT64_V;
		case PGSQL_INT2OID:
		case PGSQL_INT2VECTOROID:
		case PGSQL_INT4OID:
			return INT_V;
		case PGSQL_FLOAT4OID:
		case PGSQL_FLOAT8OID:
		case PGSQL_NUMERICOID:
			return DOUBLE_V;
		case PGSQL_DATEOID:
			return DATE_V;
		case PGSQL_TIMEOID:
		case PGSQL_TIMESTAMPOID:
		case PGSQL_TIMESTAMPZOID:
			return TIME_V;
		case PGSQL_BYTEAOID:
			return SQLRAW_V;
	}
	return STRING_V;
}

class PostgreSQLConnection : public SqlConnection {
protected:
	virtual void        SetParam(int i, const Value& r);
	virtual bool        Execute();
	virtual int         GetRowsProcessed() const;
	virtual Value       GetInsertedId() const;
	virtual bool        Fetch();
	virtual void        GetColumn(int i, Ref f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual String      GetUser() const;
	virtual String      ToString() const;

private:
	PostgreSQLSession& session;

	PGconn         *conn;
	Vector<String>  param;
	PGresult       *result;
	Vector<Oid>     oid;
	int             rows;
	int             fetched_row; //-1, if not fetched yet
	String          last_insert_table;

	void            FreeResult();
	String          ErrorMessage();
	String          ErrorCode();

	String          FromCharset(const String& s) const { return session.FromCharset(s); }
	String          ToCharset(const String& s) const   { return session.ToCharset(s); }

public:
	PostgreSQLConnection(PostgreSQLSession& a_session, PGconn *a_conn);
	virtual ~PostgreSQLConnection() { Cancel(); }
};

const char *PostgreSQLReadString(const char *s, String& stmt)
{
	//TODO: to clear this, currently this is based on sqlite
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

bool PostgreSQLPerformScript(const String& txt, StatementExecutor& se, Gate<int, int> progress_canceled)
{
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
				text = PostgreSQLReadString(text, stmt);
			else
			if(*text == '\"')
				text = PostgreSQLReadString(text, stmt);
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

String PostgreSQLConnection::ErrorMessage()
{
	return FromCharset(PQerrorMessage(conn));
}

String PostgreSQLConnection::ErrorCode()
{
	return PQresultErrorField(result, PG_DIAG_SQLSTATE);
}

String PostgreSQLSession::ErrorMessage()
{
	return FromCharset(PQerrorMessage(conn));
}

String PostgreSQLSession::ErrorCode()
{
	return PQresultErrorField(result, PG_DIAG_SQLSTATE);
}

Vector<String> PostgreSQLSession::EnumUsers()
{
	Vector<String> vec;
	Sql sql(*this);
	sql.Execute("select rolname from pg_authid where rolcanlogin");
	while(sql.Fetch())
		vec.Add(sql[0]);
	return vec;
}

Vector<String> PostgreSQLSession::EnumDatabases()
{// For now, we really enumerate namespaces rather than databases here
	Vector<String> vec;
	Sql sql(*this);
	sql.Execute("select nspname from pg_namespace where nspacl is not null");
	while(sql.Fetch())
		vec.Add(sql[0]);
	return vec;
}

Vector<String> PostgreSQLSession::EnumData(char type, const char *schema)
{
	Vector<String> vec;
	Sql sql(Format("select n.nspname || '.' || c.relname from pg_catalog.pg_class c "
		             "left join pg_catalog.pg_namespace n "
		               "on n.oid = c.relnamespace "
		            "where c.relkind = '%c' "
		              "and n.nspname like '%s' "
		              "and pg_catalog.pg_table_is_visible(c.oid)",
		              type, schema ? schema : "%"), *this);
	sql.Execute();
	while(sql.Fetch())
		vec.Add(sql[0]);
	return vec;
}

Vector<String> PostgreSQLSession::EnumTables(String database)
{
	return EnumData('r', database);
}

Vector<String> PostgreSQLSession::EnumViews(String database)
{
	return EnumData('v', database);
}

Vector<String> PostgreSQLSession::EnumSequences(String database)
{
	return EnumData('S', database);
}

Vector<SqlColumnInfo> PostgreSQLSession::EnumColumns(String database, String table)
{
	/* database means schema here - support for schemas is a something to fix in sql interface */

	int q = table.Find('.');
	if(q) table = table.Mid(q + 1);
	Vector<SqlColumnInfo> vec;
	Sql sql(Format("select a.attname, a.atttypid, a.attlen, a.atttypmod, a.attnotnull "
	                 "from pg_catalog.pg_attribute a "
	                "inner join pg_catalog.pg_class c "
	                   "on a.attrelid = c.oid "
	                "inner join pg_catalog.pg_namespace n "
	                   "on c.relnamespace = n.oid "
	                "where c.relname = '%s' "
	                  "and n.nspname = '%s' "
	                  "and a.attnum > 0 "
	                  "and a.attisdropped = '0' "
	                "order by a.attnum", table, database), *this);
	sql.Execute();
	while(sql.Fetch())
	{
		SqlColumnInfo &ci = vec.Add();
		int type_mod = int(sql[3]) - sizeof(int32);
		ci.name = sql[0];
		ci.type = OidToType(IsString(sql[1]) ? atoi(String(sql[1])) : (int)sql[1]);
		ci.width = sql[2];
		if(ci.width < 0)
			ci.width = type_mod;
		ci.precision = (type_mod >> 16) & 0xffff;
		ci.scale = type_mod & 0xffff;
		ci.nullable = AsString(sql[4]) == "0";
		ci.binary = false;
	}
	return vec;
}

Vector<String> PostgreSQLSession::EnumPrimaryKey(String database, String table)
{
//	SELECT cc.conname, a.attname
//	 FROM pg_constraint cc
//	 INNER JOIN pg_class c
//	    ON c.oid=conrelid
//	 INNER JOIN pg_attribute a
//	    ON a.attnum = ANY(conkey)
//	   AND a.attrelid = c.oid
//	WHERE contype='p'
//	AND relname = '?'
	return Vector<String>(); //TODO
}

String PostgreSQLSession::EnumRowID(String database, String table)
{
	return ""; //TODO
}

Vector<String> PostgreSQLSession::EnumReservedWords()
{
	return Vector<String>(); //TODO
}

SqlConnection * PostgreSQLSession::CreateConnection()
{
	return new PostgreSQLConnection(*this, conn);
}

void PostgreSQLSession::ExecTrans(const char * statement)
{
	if(trace)
		*trace << statement << UPP::EOL;
	
	int itry = 0;

	do {
		result = PQexec(conn, statement);
		if(PQresultStatus(result) == PGRES_COMMAND_OK) {
			PQclear(result);
			return;
		}
	}
	while(level == 0 && (!ConnectionOK() || ErrorMessage().Find("connection") >= 0 && itry == 0)
	      && WhenReconnect(itry++));

	if(trace)
		*trace << statement << " failed: " << ErrorMessage() << " (level " << level << ")\n";
	SetError(ErrorMessage(), statement, 0, ErrorCode());
	PQclear(result);
}

String PostgreSQLSession::FromCharset(const String& s) const
{
	if(!charset)
		return s;
	String r = UPP::ToCharset(GetDefaultCharset(), s, charset);
	return r;
}

String PostgreSQLSession::ToCharset(const String& s) const
{
	if(!charset)
		return s;
	String r = UPP::ToCharset(charset, s);
	return r;
}

void PostgreSQLSession::DoKeepAlive()
{
	if(keepalive && conn) {
		int optval = 1;
		setsockopt(PQsocket(conn), SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval));
	}
}

bool PostgreSQLSession::Open(const char *connect)
{
	Close();
	conns = connect;

	{
		MemoryIgnoreLeaksBlock __;
		// PGSQL, when sharing .dll SSL, does not free SSL data
		conn = PQconnectdb(connect);
	}

	if(PQstatus(conn) != CONNECTION_OK)
	{	
		SetError(FromSystemCharset(PQerrorMessage(conn)), "Opening database");
		Close();
		return false;
	}
	level = 0;
	
	if(PQclientEncoding(conn)) {
		if(PQsetClientEncoding(conn, "UTF8")) {
			SetError("Cannot set UTF8 charset", "Opening database");
			return false;
		}
		charset = CHARSET_UTF8;
	}
	else
		charset = CHARSET_DEFAULT;
	
	DoKeepAlive();

	LLOG( String("Postgresql client encoding: ") + pg_encoding_to_char( PQclientEncoding(conn) ) );

	Sql sql(*this);
	if(sql.Execute("select setting from pg_settings where name = 'bytea_output'") && sql.Fetch() && sql[0] == "hex")
		hex_blobs = true;

	return true;
}

bool PostgreSQLSession::ConnectionOK()
{
	return conn && PQstatus(conn) == CONNECTION_OK;
}

bool PostgreSQLSession::ReOpen()
{
	PQreset(conn);
	if(PQstatus(conn) != CONNECTION_OK)
	{
		SetError(ErrorMessage(), "Opening database");
		return false;
	}
	DoKeepAlive();
	level = 0;
	return true;	
}

void PostgreSQLSession::Close()
{
	if(!conn)
		return;
	SessionClose();
	PQfinish(conn);
	conn = NULL;
	level = 0;
}

void PostgreSQLSession::Begin()
{
	ExecTrans("begin");
	level++;
}

void PostgreSQLSession::Commit()
{
	ExecTrans("commit");
	level--;
}

void PostgreSQLSession::Rollback()
{
	ExecTrans("rollback");
	if(level > 0) level--;
}

int PostgreSQLSession::GetTransactionLevel() const
{
	return level;
}

void PostgreSQLConnection::SetParam(int i, const Value& r)
{
	String p;
	if(IsNull(r))
		p = "NULL";
	else
		switch(r.GetType()) {
		case SQLRAW_V: {
			String raw = SqlRaw(r);
			size_t rl;
			unsigned char *s = PQescapeByteaConn(conn, (const byte *)~raw, raw.GetLength(), &rl);
			p.Reserve(int(rl + 16));
			p = "\'" + String(s, int(rl - 1)) + "\'::bytea";
			PQfreemem(s);
			break;
		}
		case WSTRING_V:
		case STRING_V: {
				String v = r;
				v = ToCharset(v);
				StringBuffer b(v.GetLength() * 2 + 3);
				char *q = b;
				*q = '\'';
				int *err = NULL;
				int n = (int)PQescapeStringConn(conn, q + 1, v, v.GetLength(), err);
				q[1 + n] = '\'';
				b.SetCount(2 + n);
				p = b;
			}
			break;
		case BOOL_V:
		case INT_V:
			p << int(r);
			break;
		case INT64_V:
			p << int64(r);
			break;
		case DOUBLE_V:
			p = FormatDouble(double(r), 20);
			break;
		case DATE_V: {
				Date d = r;
				p = Format("\'%04d-%02d-%02d\'", d.year, d.month, d.day);
			}
			break;
		case TIME_V: {
				Time t = r;
				p = Format("\'%04d-%02d-%02d %02d:%02d:%02d\'",
						   t.year, t.month, t.day, t.hour, t.minute, t.second);
			}
			break;
		default:
			NEVER();
		}
	param.At(i, p);
}

bool PostgreSQLConnection::Execute()
{
	Cancel();
	if(statement.GetLength() == 0) {
		session.SetError("Empty statement", statement);
		return false;
	}

	CParser p(statement);
	if((p.Id("insert") || p.Id("INSERT")) && (p.Id("into") || p.Id("INTO")) && p.IsId())
		last_insert_table = p.ReadId();

	String query;
	int pi = 0;
	const char *s = statement;
	while(s < statement.End())
		if(*s == '\'' || *s == '\"')
			s = PostgreSQLReadString(s, query);
		else {
			if(*s == '?' && !session.noquestionparams) {
				if(s[1] == '?') {
					query.Cat('?');
					s++;
				}
				else {
					if(pi >= param.GetCount()) {
						session.SetError("Invalid number of parameters", statement);
						return false;
					}
					query.Cat(param[pi++]);
				}
			}
			else
				query.Cat(*s);
			s++;
		}
	param.Clear();

	Stream *trace = session.GetTrace();
	dword time;
	if(session.IsTraceTime())
		time = msecs();

	int itry = 0;
	int stat;
	do {
		result = PQexecParams(conn, query, 0, NULL, NULL, NULL, NULL, 0);
		stat = PQresultStatus(result);
	}
	while(stat != PGRES_TUPLES_OK && stat != PGRES_COMMAND_OK && session.level == 0 &&
	      (!session.ConnectionOK() || ErrorMessage().Find("connection") >= 0 && itry == 0) && session.WhenReconnect(itry++));

	if(trace) {
		if(session.IsTraceTime())
			*trace << Format("--------------\nexec %d ms:\n", msecs(time));
	}
	if(stat == PGRES_TUPLES_OK) //result set
	{
		rows = PQntuples(result);
		int fields = PQnfields(result);
		info.SetCount(fields);
		oid.SetCount(fields);
		for(int i = 0; i < fields; i++)
		{
			SqlColumnInfo& f = info[i];
			f.name = ToUpper(PQfname(result, i));
			f.width = PQfsize(result, i);
			int type_mod = PQfmod(result, i) - sizeof(int32);
			if(f.width < 0)
				f.width = type_mod;
			f.precision = (type_mod >> 16) & 0xffff;
			f.scale = type_mod & 0xffff;
			f.nullable = true;
			Oid type_oid = PQftype(result, i);
			f.type = OidToType(type_oid);
			oid[i] = type_oid;
		}
		return true;
	}
	if(stat == PGRES_COMMAND_OK) //command executed OK
	{
		rows = atoi(PQcmdTuples(result));
		return true;
	}

	session.SetError(ErrorMessage(), query, 0, ErrorCode());
	FreeResult();
	return false;
}

int PostgreSQLConnection::GetRowsProcessed() const
{
	return rows;
}

Value PostgreSQLConnection::GetInsertedId() const
{
	String pk = session.pkache.Get(last_insert_table, Null);
	if(IsNull(pk)) {
		String sqlc_expr; 
		sqlc_expr <<
		"SELECT " <<
		  "pg_attribute.attname " <<
		"FROM pg_index, pg_class, pg_attribute " <<
		"WHERE " <<
		  "pg_class.oid = '" << last_insert_table << "'::regclass AND "
		  "indrelid = pg_class.oid AND "
		  "pg_attribute.attrelid = pg_class.oid AND "
		  "pg_attribute.attnum = any(pg_index.indkey) "
		  "AND indisprimary";
		Sql sqlc(sqlc_expr, session);
		pk = sqlc.Execute() && sqlc.Fetch() ? sqlc[0] : "ID";
		session.pkache.Add(last_insert_table, pk);
	}
	Sql sql("select currval('" + last_insert_table + "_" + pk +"_seq')", session);
	if(sql.Execute() && sql.Fetch())
		return sql[0];
	else
		return Null;
}

bool PostgreSQLConnection::Fetch()
{
	fetched_row++;
	if(result && rows > 0 && fetched_row < rows)
		return true;
	Cancel();
	return false;
}

static Date sDate(const char *s)
{
	// 0123456789012345678
	// YYYY-MM-DD HH-MM-SS
	return Date(atoi(s), atoi(s + 5), atoi(s + 8));
}

void PostgreSQLConnection::GetColumn(int i, Ref f) const
{
	if(PQgetisnull(result, fetched_row, i))
	{
		f = Null;
		return;
	}
	char *s = PQgetvalue(result, fetched_row, i);
	switch(info[i].type)
	{
		case INT64_V:
			f.SetValue(ScanInt64(s));
			break;
		case INT_V:
			f.SetValue(ScanInt(s));
			break;
		case DOUBLE_V: {
				double d = ScanDouble(s);
				f.SetValue(IsNull(d) ? NAN : d);
			}
			break;
		case BOOL_V:
			f.SetValue(*s == 't' ? "1" : "0");
			break;
		case DATE_V:
			f.SetValue(sDate(s));
			break;
		case TIME_V: {
				Time t = ToTime(sDate(s));
				t.hour = atoi(s + 11);
				t.minute = atoi(s + 14);
				t.second = atoi(s + 17);
				f.SetValue(t);
			}
			break;
		default: {
			if(oid[i] == PGSQL_BYTEAOID) {
				if(session.hex_blobs)
					f.SetValue(ScanHexString(s, (int)strlen(s)));
				else {
					size_t len;
					unsigned char *q = PQunescapeBytea((const unsigned char *)s, &len);
					f.SetValue(String(q, (int)len));
					PQfreemem(q);
				}
			}
			else
				f.SetValue(FromCharset(String(s)));
		}
	}
}

void PostgreSQLConnection::Cancel()
{
	info.Clear();
	rows = 0;
	fetched_row = -1;
	FreeResult();
}

SqlSession& PostgreSQLConnection::GetSession() const
{
	return session;
}

String PostgreSQLConnection::GetUser() const
{
	return PQuser(conn);
}

String PostgreSQLConnection::ToString() const
{
	return statement;
}

void PostgreSQLConnection::FreeResult()
{
	if(result)
	{
		PQclear(result);
		result = NULL;
	}
}

PostgreSQLConnection::PostgreSQLConnection(PostgreSQLSession& a_session, PGconn *a_conn)
  : session(a_session), conn(a_conn)
{
	result = NULL;
}

Value PgSequence::Get()
{
#ifndef NOAPPSQL
	Sql sql(session ? *session : SQL.GetSession());
#else
	ASSERT(session);
	Sql sql(*session);
#endif
	if(!sql.Execute(Select(NextVal(seq)).Get()) || !sql.Fetch())
		return ErrorValue();
	return sql[0];
}

}

#endif
