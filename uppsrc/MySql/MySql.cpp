#include "MySql.h"

#ifndef flagNOMYSQL

#define LLOG(x) // DLOG(x)

namespace Upp {

class MySqlConnection : public SqlConnection {
protected:
	virtual void        SetParam(int i, const Value& r);
	virtual bool        Execute();
	virtual int         GetRowsProcessed() const;
	virtual bool        Fetch();
	virtual void        GetColumn(int i, Ref f) const;
	virtual void        Cancel();
	virtual Value       GetInsertedId() const;
	virtual SqlSession& GetSession() const;
	virtual String      GetUser() const;
	virtual String      ToString() const;

private:
	MySqlSession&  session;
	MYSQL         *mysql;
	Vector<String> param;
	MYSQL_RES     *result;
	MYSQL_ROW      row;
	unsigned long *len;
	int            rows;
	int            lastid;
	Buffer<bool>   convert;

	String         MakeQuery() const;
	void           FreeResult();
	String         EscapeString(const String& v);
	bool           MysqlQuery(const char *query);

public:
	MySqlConnection(MySqlSession& session, MYSQL *mysql);
	virtual ~MySqlConnection() { Cancel(); }
};

bool MySqlSession::IsOpen() const { return mysql; }

static const char *sEmpNull(const char *s) {
	return s && *s == '\0' ? NULL : s;
}

INITBLOCK {
	mysql_library_init(0, NULL, NULL);
}

EXITBLOCK {
	mysql_library_end();
}

bool MySqlSession::DoConnect()
{
	mysql = mysql_init((MYSQL*) 0);
	level = 0;
	if(mysql && mysql_real_connect(mysql, sEmpNull(connect_host), sEmpNull(connect_user),
	                               sEmpNull(connect_password), sEmpNull(connect_database),
	                               connect_port, sEmpNull(connect_socket), 0)) {
		Sql sql(*this);
		username = sql.Select("substring_index(USER(),'@',1)");
		mysql_set_character_set(mysql, "utf8");
		sql.Execute("SET NAMES 'utf8'");
		sql.Execute("SET CHARACTER SET utf8");
		return true;
	}
	SetError(mysql_error(mysql), "", mysql_errno(mysql));
	Close();
	return false;
}

bool MySqlSession::Reconnect()
{
	LLOG("Trying to reconnect");
	Close();
	return DoConnect();
}

bool MySqlSession::Connect(const char *user, const char *password, const char *database,
                           const char *host, int port, const char *socket) {
	connect_user = user;
	connect_password = password;
	connect_database = database;
	connect_host = host;
	connect_port = port;
	connect_socket = socket;
	return DoConnect();
}

inline static const char *EmpNull(const String& s)
{
	return *s ? (const char *)s : 0;
}

bool MySqlSession::Open(const char *connect) {
	String user, pwd, socket;
	String database = Null;
	String host = Null;
	int port = MYSQL_PORT;
	const char *p = connect, *b;
	for(b = p; *p && *p != '/' && *p != '@'; p++)
		;
	user = String(b, p);
	if(*p == '/')
	{
		for(b = ++p; *p && *p != '@'; p++)
			;
		pwd = String(b, p);
	}
	if(*p == '@')
	{
		for(b = ++p; *p && *p != '/' && *p != ':' && *p != ','; p++)
			;
		if(*p == '/' || *p == 0)
		{
			database = String(b, p);
			if(*p)
				p++;
			b = p;
		}
		while(*p && *p != ':' && *p != ',')
			p++;
		host = String(b, p);
		if(*p == ':')
		{ // port
			if(!IsDigit(*++p))
				throw Exc("Port number expected.");
			port = stou(p, &p);
		}
		if(*p == ',') // socket
			socket = p + 1;
	}
	return Connect(EmpNull(user), EmpNull(pwd),
		EmpNull(database), EmpNull(host), port, EmpNull(socket));
}

void MySqlSession::Close() {
	SessionClose();
	if(mysql) {
		mysql_close(mysql);
		mysql = NULL;
		level = 0;
	}
}

bool MySqlSession::MysqlQuery(const char *query)
{
	int itry = 0;
	for(;;) {
		if(!mysql_query(mysql, query))
			break;
		int code = mysql_errno(mysql);
		if(level == 0 && itry++ == 0 &&
		   (code == 2006 || code == 2013 || code == 2055) &&
		   WhenReconnect())
			continue;
		SetError(mysql_error(mysql), query, code);
		return false;
	}
	return true;
}

bool MySqlConnection::MysqlQuery(const char *query)
{
	return session.MysqlQuery(query);
}

void MySqlSession::Begin()
{
	static const char btrans[] = "start transaction";
	if(trace)
		*trace << btrans << ";\n";
	if(MysqlQuery(btrans))
		level++;
}

void MySqlSession::Commit()
{
	static const char ctrans[] = "commit";
	if(trace)
		*trace << ctrans << ";\n";
	if(MysqlQuery(ctrans))
		level--;
}

void MySqlSession::Rollback()
{
	static const char rtrans[] = "rollback";
	if(trace)
		*trace << rtrans << ";\n";
	if(MysqlQuery(rtrans) && level > 0)
		level--;
}

int MySqlSession::GetTransactionLevel() const
{
	return level;
}

static Vector<String> FetchList(Sql& cursor, bool upper = false)
{
	Vector<String> out;
	String s;
	while(cursor.Fetch(s))
		out.Add(upper ? ToUpper(s) : s);
	return out;
}

Vector<String> MySqlSession::EnumUsers()
{
	Vector<String> out;
	Sql cursor(*this);
	if(Select(SqlId("USER")).From(SqlId("MYSQL.USER")).Execute(cursor))
		out = FetchList(cursor);
	return out;
}

Vector<String> MySqlSession::EnumDatabases()
{
	Vector<String> out;
	Sql cursor(*this);
	if(cursor.Execute("show databases"))
		out = FetchList(cursor); // 06-09-12 cxl: was false; In Linux, names are case sensitive
	return out;
}

Vector<String> MySqlSession::EnumTables(String database)
{
	Vector<String> out;
	Sql cursor(*this);
	if(cursor.Execute("show tables from " + database))
		out = FetchList(cursor); // 06-09-12 cxl: was false; In Linux, names are case sensitive
	return out;
}

SqlConnection *MySqlSession::CreateConnection() {
	return new MySqlConnection(*this, mysql);
}

MySqlConnection::MySqlConnection(MySqlSession& session, MYSQL *mysql)
  : session(session), mysql(mysql) {
	result = NULL;
	lastid = 0;
}

String MySqlConnection::EscapeString(const String& v)
{
	StringBuffer b(v.GetLength() * 2 + 3);
	char *q = b;
	*q = '\"';
	int n = mysql_real_escape_string(mysql, q + 1, v, v.GetLength());
	q[1 + n] = '\"';
	b.SetCount(2 + n); //TODO - check this fix
	return String(b);
}

void MySqlConnection::SetParam(int i, const Value& r) {
	String p;
	if(IsNull(r))
		p = "NULL";
	else
		switch(r.GetType()) {
		case 34:
			p = EscapeString(SqlRaw(r));
			break;
		case WSTRING_V:
		case STRING_V:
			p = EscapeString(ToCharset(CHARSET_UTF8, r));
			break;
		case BOOL_V:
		case INT_V:
			p = Format("%d", int(r));
			break;
		case INT64_V:
			p = IntStr64(r);
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

bool MySqlConnection::Execute() {
	String query;
	int pi = 0;
	const char *s = statement;
	while(s < statement.End())
		if(*s == '\'' || *s == '\"')
			s = MySqlReadString(s, query);
		else {
			if(*s == '?')
				query.Cat(param[pi++]);
			else
				query.Cat(*s);
			s++;
		}
	Cancel();
	if(!MysqlQuery(query))
		return false;
	result = mysql_store_result(mysql);
	rows = (int)mysql_affected_rows(mysql);

	while(mysql_more_results (mysql)) { // Only first resultset is considered, rest is ignored
		mysql_next_result (mysql);      // This is required to avoid synchronization error on CALL
	}

	if(result) {
		int fields = mysql_num_fields(result);
		info.SetCount(fields);
		convert.Alloc(fields, false);
		for(int i = 0; i < fields; i++) {
			MYSQL_FIELD *field = mysql_fetch_field_direct(result, i);
			SqlColumnInfo& f = info[i];
			f.name = field->name;
			switch(field->type) {
			case FIELD_TYPE_TINY:
			case FIELD_TYPE_SHORT:
			case FIELD_TYPE_LONG:
			case FIELD_TYPE_INT24:
				f.type = INT_V;
				break;
			case FIELD_TYPE_LONGLONG:
				f.type = INT64_V;
				break;
			case FIELD_TYPE_DECIMAL:
			case FIELD_TYPE_FLOAT:
			case FIELD_TYPE_DOUBLE:
			case FIELD_TYPE_NEWDECIMAL:
				f.type = DOUBLE_V;
				break;
			case FIELD_TYPE_DATE:
				f.type = DATE_V;
				break;
			case FIELD_TYPE_DATETIME:
			case FIELD_TYPE_TIMESTAMP:
				f.type = TIME_V;
				break;
			case FIELD_TYPE_VAR_STRING:
			case FIELD_TYPE_STRING:
				convert[i] = true;
			default:
				f.type = STRING_V;
				break;
			}
			f.width = field->length;
			f.scale = f.precision = 0;
		}
	}
	else {
		lastid = (int)mysql_insert_id(mysql);
		if(lastid) {
			SqlColumnInfo& f = info.Add();
			f.width = f.scale = f.precision = 0;
			f.binary = false;
			f.type = DOUBLE_V;
			f.name = "LAST_INSERT_ID";
			rows = 1;
		}
	}
	return true;
}

int  MySqlConnection::GetRowsProcessed() const {
	return rows;
}

Value MySqlConnection::GetInsertedId() const
{
	return lastid;
}

bool MySqlConnection::Fetch() {
	if(result) {
		row = mysql_fetch_row(result);
		if(row) {
			len = mysql_fetch_lengths(result);
			return true;
		}
	}
	else
	if(lastid && rows > 0) {
		rows--;
		return true;
	}
	FreeResult();
	return false;
}

// 0123456789012345678
// YYYY-MM-DD HH-MM-SS

static Date sDate(const char *s) {
	return Date(atoi(s), atoi(s + 5), atoi(s + 8));
}

void MySqlConnection::GetColumn(int i, Ref f) const {
	if(lastid) {
		f = lastid;
		return;
	}
	const char *s = row[i];
	if(s == NULL)
		f = Null;
	else {
		switch(info[i].type) {
		case INT_V:
			f = atoi(s);
			break;
		case INT64_V:
			f = ScanInt64(s);
			break;
		case DOUBLE_V:
			f = ScanDouble(s, NULL, true);
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
			if(convert[i])
				f = Value(ToCharset(CHARSET_DEFAULT, String(s, len[i]), CHARSET_UTF8));
			else
				f = Value(String(s, len[i]));
			break;
		}
	}
}

void MySqlConnection::FreeResult() {
	lastid = 0;
	if(result) {
		mysql_free_result(result);
		result = NULL;
	}
}

void MySqlConnection::Cancel() {
	param.Clear();
	info.Clear();
	rows = 0;
	FreeResult();
}

SqlSession& MySqlConnection::GetSession() const { return session; }
String      MySqlConnection::GetUser() const    { return session.GetUser(); }
String      MySqlConnection::ToString() const   { return statement; }

String MySqlTextType(int n) {
	return n < 65536 ? Format("varchar(%d)", n) : String("text");
}

const char *MySqlReadString(const char *s, String& stmt) {
	stmt.Cat(*s);
	int c = *s++;
	for(;;) {
		if(*s == '\0') break;
		else
		if(*s == '\'' && s[1] == '\'') {
			stmt.Cat('\'');
			s += 2;
		}
//		else
//		if(*s == '\"' && s[1] == '\"') {
//			stmt.Cat('\"');
//			s += 2;
//		}
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

bool MySqlPerformScript(const String& txt, StatementExecutor& se, Gate<int, int> progress_canceled) {
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
				text = MySqlReadString(text, stmt);
			else
			if(*text == '\"')
				text = MySqlReadString(text, stmt);
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

bool MySqlUpdateSchema(const SqlSchema& sch, int i, StatementExecutor& se) {
	if(sch.UpdateNormalFile(i)) {
		MySqlPerformScript(LoadFile(sch.NormalFileName(i + 1)), se);
		sch.UpdateNormalFile(i + 1);
		return MySqlPerformScript(sch.Script(i), se);
	}
	return true;
}

}

#endif
