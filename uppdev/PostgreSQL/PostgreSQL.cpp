#include "PostgreSQL.h"

#ifndef flagNOPOSTGRESQL

NAMESPACE_UPP

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
	PostgreSQLSession&  session;
	PGconn         *conn;
	Vector<String> param;
	PGresult      *result;
	int            rows;
	int            fetched_row; //-1, if not fetched yet

	void           FreeResult();
	String         ErrorMessage();

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
			stmt.Cat('\'');
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

bool PostgreSQLPerformScript(const String& txt, StatementExecutor& se, Gate2<int, int> progress_canceled)
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
		if(progress_canceled(text - txt.Begin(), txt.GetLength()))
			return false;
		if(!se.Execute(stmt))
			return false;
		if(*text) text++;
	}
	return true;
}

//////////////////////////////////////////////

String PostgreSQLConnection::ErrorMessage()
{
	return FromSystemCharset(PQerrorMessage(conn));
}

//////////////////////////////////////////////

String PostgreSQLSession::ErrorMessage()
{
	return FromSystemCharset(PQerrorMessage(conn));
}

//////////////////////////////////////////////

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
{
	Vector<String> vec;
	Sql sql(*this);
	sql.Execute("select datname from pg_database");
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

	Vector<SqlColumnInfo> vec;

	Sql sql(Format("select a.attname, a.atttypid, a.attlen, a.atttypmod, a.attnotnull "
	                 "from pg_attribute a "
	                "inner join pg_class c "
	                   "on a.attrelid = c.oid "
	                "inner join pg_namespace n "
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
		ci.type = OidToType((int)sql[1]);
		ci.width = sql[2];
		if(ci.width < 0)
			ci.width = type_mod;
		ci.prec = (type_mod >> 16) & 0xffff;
		ci.scale = type_mod & 0xffff;
		ci.decimals = ci.prec; //what is this for?
		//ci.null = sql[4] == "0"; //not supported in column info structure
	}
	return vec;
}

Vector<String> PostgreSQLSession::EnumPrimaryKey(String database, String table)
{
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

void PostgreSQLSession::StoreInOidTypeMap(const char *typname, int type_id, const VectorMap<String, int64> &typname_oid_map)
{
	int idx = typname_oid_map.Find(typname);
	if(idx <= 0)
		return;
	int64 oid = typname_oid_map[idx];
	oid_type_map.FindAdd((Oid)oid, type_id);
}

bool PostgreSQLSession::InitOidTypeMap()
{
	oid_type_map.Clear();
	PGresult *res = PQexec(conn, "select oid, typname from pg_type");
	if(PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		PQclear(res);
		return false;
	}
	int row_count = PQntuples(res);
	VectorMap<String, int64> typname_oid_map;
	for(int i = 0; i < row_count; i++)
	{
		typname_oid_map.FindAdd(PQgetvalue(res, i, 1), atol(PQgetvalue(res, i, 0)));
	}
	StoreInOidTypeMap("bool", BOOL_V, typname_oid_map);
	StoreInOidTypeMap("abstime", TIME_V, typname_oid_map);
	StoreInOidTypeMap("cid", INT_V, typname_oid_map);
	StoreInOidTypeMap("date", DATE_V, typname_oid_map);
	StoreInOidTypeMap("float4", DOUBLE_V, typname_oid_map);
	StoreInOidTypeMap("float8", DOUBLE_V, typname_oid_map);
	StoreInOidTypeMap("int2", INT_V, typname_oid_map);
	StoreInOidTypeMap("int4", INT_V, typname_oid_map);
	StoreInOidTypeMap("int8", INT_V, typname_oid_map); //should be INT64_V
	//StoreInOidTypeMap("interval", , typname_oid_map);
	StoreInOidTypeMap("money", DOUBLE_V, typname_oid_map);
	StoreInOidTypeMap("numeric", DOUBLE_V, typname_oid_map);
	StoreInOidTypeMap("oid", INT64_V, typname_oid_map);
	StoreInOidTypeMap("time", TIME_V, typname_oid_map);
	StoreInOidTypeMap("time_stamp", TIME_V, typname_oid_map);
	StoreInOidTypeMap("timestamp", TIME_V, typname_oid_map);
	StoreInOidTypeMap("timestamptz", TIME_V, typname_oid_map);
	StoreInOidTypeMap("timetz", TIME_V, typname_oid_map);
	//StoreInOidTypeMap("tinterval", , typname_oid_map);
	StoreInOidTypeMap("xid", INT_V, typname_oid_map);

	return true;
}

void PostgreSQLSession::ExecTrans(const char * statement)
{
	if(trace)
		*trace << statement << "\n";
	PGresult *res = PQexec(conn, statement);
	if(PQresultStatus(res) == PGRES_COMMAND_OK)
	{
		PQclear(res);
		return;
	}
	if(trace)
		*trace << statement << " failed: " << ErrorMessage() << "\n";
	SetError(ErrorMessage(), statement);
	PQclear(res);
}

int PostgreSQLSession::OidToType(Oid oid)
{
	int idx = oid_type_map.Find(oid);
	if(idx <= 0)
		return STRING_V;
	return oid_type_map[idx];
}

bool PostgreSQLSession::Open(const char *connect)
{
	Close();
	conn = PQconnectdb(connect);
	if(PQstatus(conn) != CONNECTION_OK)
	{
		SetError(ErrorMessage(), "Opening database");
		Close();
		return false;
	}
	//read oids of different types to a map (oid =>value-type). Execute() sets up type based on this
	if(!InitOidTypeMap())
	{
		SetError(ErrorMessage(), "Initializing type map");
		Close();
		return false;
	}
	return true;
}

void PostgreSQLSession::Close()
{
	if(!conn)
		return;
#ifndef flagNOAPPSQL
	if(SQL.IsOpen() && &SQL.GetSession() == this)
		SQL.Cancel();
#endif

	PQfinish(conn);
	conn = NULL;
}

void PostgreSQLSession::Begin()
{
	ExecTrans("begin");
}

void PostgreSQLSession::Commit()
{
	ExecTrans("commit");
}

void PostgreSQLSession::Rollback()
{
	ExecTrans("rollback");
}

/////////////////////////////////////////////////

void PostgreSQLConnection::SetParam(int i, const Value& r)
{
	String p;
	if(IsNull(r))
		p = "NULL";
	else
		switch(r.GetType()) {
		case WSTRING_V:
		case STRING_V: {
				String v = r;
				StringBuffer b(v.GetLength() * 2 + 3);
				char *q = b;
				*q = '\'';
				int *err = NULL;
				int n = PQescapeStringConn(conn, q + 1, v, v.GetLength(), err);
				q[1 + n] = '\'';
				b.SetCount(2 + n); //TODO - check this fix
				p = b;
			}
			break;
		case BOOL_V:
		case INT_V:
			p = Format("%d", int(r));
			break;
		case DOUBLE_V:
			p = Format("%.10g", double(r));
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
	String query;
	int pi = 0;
	const char *s = statement;
	while(s < statement.End())
		if(*s == '\'' || *s == '\"')
			s = PostgreSQLReadString(s, query);
		else {
			if(*s == '?')
				query.Cat(param[pi++]);
			else
				query.Cat(*s);
			s++;
		}
	param.Clear();

	Stream *trace = session.GetTrace();
	dword time;
	if(session.IsTraceTime())
		time = GetTickCount();

	//result = PQexec(conn, query);
	result = PQexecParams(conn, query, 0, NULL, NULL, NULL, NULL, 0);
	// TODO we should recieve data in binary format to avoid all the ato* stuff

	if(trace) {
		if(session.IsTraceTime())
			*trace << Format("--------------\nexec %d ms:\n", msecs(time));
		*trace << ToString() << '\n';
	}
	if(PQresultStatus(result) == PGRES_TUPLES_OK) //result set
	{
		rows = PQntuples(result);
		int fields = PQnfields(result);
		info.SetCount(fields);
		for(int i = 0; i < fields; i++)
		{
			SqlColumnInfo& f = info[i];
			f.name = ToUpper(PQfname(result, i));
			f.width = PQfsize(result, i);
			f.decimals = f.scale = f.prec = 0; // TODO
			Oid type_oid = PQftype(result, i);
			f.type = session.OidToType(type_oid);
		}
		return true;
	}
	if(PQresultStatus(result) == PGRES_COMMAND_OK) //command executed OK
	{
		rows = atoi(PQcmdTuples(result));
		return true;
	}

	session.SetError( ErrorMessage(), query );
	FreeResult();

	return false;
}

int PostgreSQLConnection::GetRowsProcessed() const
{
	return rows;
}

Value PostgreSQLConnection::GetInsertedId() const
{
	//it needs GetInsertedId(const char * sequence)
	//there is no other/better way to retrieve last inserted id
	const char * sequence = NULL;

	Sql sql(Format("select currval('%s')", sequence), session);
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
	char * s = PQgetvalue(result, fetched_row, i);
	switch(info[i].type)
	{
		//ntohl(*(int *)PQgetvalue(res, 0, 0));
		case INT64_V:
			f = atoi(s);
			//f = Value(*((int64 *)s));
			break;
		case INT_V:
			f = atoi(s);
			//f = Value(*((int32 *)s));
			break;
		case DOUBLE_V:
			f = atof(s);
			//f = Value(*(double *)(s));
			break;
		case DATE_V:
			f = Value(sDate(s));
			break;
		case TIME_V: {
				Time t = ToTime(sDate(s));
				t.hour = atoi(s + 11);
				t.minute = atoi(s + 14);
				t.second = atoi(s + 17);
				f = Value(t);
			}
			break;
		default:
			f = Value(String(s));
			break;
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

END_UPP_NAMESPACE

#endif
