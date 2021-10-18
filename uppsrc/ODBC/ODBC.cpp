#include "ODBC.h"

namespace Upp {

#define LLOG(x)  // DLOG(x)

class ODBCConnection : public SqlConnection
{
public:
	ODBCConnection(ODBCSession *session);
	virtual ~ODBCConnection();

	void                    Clear();

	virtual void            SetParam(int i, const Value& r);
	virtual bool            Execute();
	virtual int             GetRowsProcessed() const;
	virtual bool            Fetch();
	virtual void            GetColumn(int i, Ref r) const;
	virtual void            Cancel();
	virtual SqlSession&     GetSession() const { ASSERT(session); return *session; }
	virtual String          GetUser() const { ASSERT(session); return session->user; }
	virtual String          ToString() const;
	virtual Value           GetInsertedId() const;

private:
	friend class ODBCSession;

	ODBCSession           *session;
/*
	struct Param {
		Value  orig;
		int    ctype;
		int    sqltype;
		int    width;
		String data;
		SQLLEN li;
	};
*/
	Vector<Value>            param;
	String                   last_insert_table;

	int                      rowsprocessed;
	Vector<double>           number;
	Vector<int64>            num64;
	Vector<String>           text;
	Vector<Time>             time;
	Vector<Date>             date;
	int                      rowcount;
	int                      rowi;
	int                      number_i;
	int                      num64_i;
	int                      text_i;
	int                      time_i;
	int                      date_i;
	Vector<Value>            fetchrow;
	Vector<int>              string_type;
	
	bool                   IsOk(SQLRETURN ret) const;
	void                   FetchAll();
	bool                   Fetch0();
};

Array< Tuple2<String, String> > ODBCSession::EnumDSN()
{
	Array< Tuple2<String, String> > out;
	try {
		SQLHENV MIenv;
		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &MIenv);
		SQLSetEnvAttr(MIenv, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
		SQLRETURN ret;
		char l_dsn[256];
		char l_desc[256];
		short int l_len1,l_len2,l_next;
		l_next=SQL_FETCH_FIRST;
		while(SQL_SUCCEEDED(ret = SQLDataSources(MIenv, l_next, (SQLCHAR *)l_dsn, sizeof(l_dsn),
		                                         &l_len1, (SQLCHAR *)l_desc, sizeof(l_desc), &l_len2))) {
			Tuple2<String, String>& listdsn = out.Add();
			listdsn.a = l_dsn;
			listdsn.b = l_desc;
			l_next = SQL_FETCH_NEXT;
		}
	}
	catch(Exc e) {
		LLOG("ODBC::GetDSN->" << e);
	}
	return out;
}

bool ODBCSession::Connect(const char *cs)
{
	if(henv && IsOk(SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc))) {
		if(IsOk(SQLDriverConnect(hdbc, NULL, (SQLCHAR *)cs, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT))) {
			SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			if(tmode == NORMAL)
				SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
			SQLSetConnectAttr(hdbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TRANSACTION_SERIALIZABLE, SQL_NTS);

			Sql sql("select cast(current_user as text)", *this);
			if(sql.Execute() && sql.Fetch())
				user = sql[0];
			return true;
		}
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = SQL_NULL_HANDLE;
	}
	return false;
}

bool ODBCSession::IsOpen() const
{
	return hdbc != SQL_NULL_HANDLE;
}

void ODBCSession::Close()
{
	SessionClose();
	if(hdbc != SQL_NULL_HANDLE) {
		FlushConnections();
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = SQL_NULL_HANDLE;
		hstmt = SQL_NULL_HANDLE;
	}
}

void ODBCSession::FlushConnections()
{
	LLOG("FlushConnections");
	SQLFreeStmt(hstmt, SQL_CLOSE);
	LLOG("-FlushConnections");
}

bool ODBCSession::IsOk(SQLRETURN ret)
{
	if(SQL_SUCCEEDED(ret))
		return true;
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	String        error;
	int i = 1;
	while(SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, i++, SqlState, &NativeError,
	                 Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		if(error.GetCount())
			error << "\r\n";
		error << (char *)Msg;
	}
	SetError(error, statement, NativeError);
	return false;
}

ODBCSession::ODBCSession()
{
	hdbc = SQL_NULL_HANDLE;
	hstmt = SQL_NULL_HANDLE;
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if(henv)
		SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
	tlevel = 0;
	Dialect(MSSQL);
	tmode = NORMAL;
}

ODBCSession::~ODBCSession()
{
	if(henv)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void ODBCSession::SetTransactionMode(int mode)
{
	tmode = mode;
	SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT,
	                  mode == IMPLICIT ? (SQLPOINTER)SQL_AUTOCOMMIT_OFF
	                                   : (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
}

void ODBCSession::Begin()
{
	if(tmode == IMPLICIT) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
		tlevel = 0;
		return;
	}
	if(tlevel == 0)
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
	tlevel++;
}

void ODBCSession::Commit()
{
	if(tmode == IMPLICIT) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
		tlevel = 0;
		return;
	}
	tlevel--;
	ASSERT(tlevel >= 0);
	if(tlevel == 0) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
	}
}

void ODBCSession::Rollback()
{
	if(tmode == IMPLICIT) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
		tlevel = 0;
		return;
	}
	tlevel--;
	ASSERT(tlevel >= 0);
	if(tlevel == 0) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
	}
}

String ODBCSession::Savepoint()
{
	NEVER();
	return "";
}

void ODBCSession::RollbackTo(const String& savepoint)
{
	NEVER();
}

Vector<String> ODBCSession::EnumUsers()
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumDatabases()
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumTables(String database)
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumViews(String database)
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumSequences(String database)
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumPrimaryKeys(String database, String table)
{
	return Vector<String>();
}

String ODBCSession::EnumRowID(String database, String table)
{
	return "";
}

bool   ODBCPerformScript(const String& text, StatementExecutor& executor, Gate<int, int> progress_canceled)
{
	const char *p = text;
	while(*p) {
		String cmd;
		while(*p && *p != ';')
			if(*p == '\'') {
				const char *s = p;
				while(*++p && (*p != '\'' || *++p == '\''))
					;
				cmd.Cat(s, int(p - s));
			}
			else {
				if(*p > ' ')
					cmd.Cat(*p);
				else if(!cmd.IsEmpty() && *cmd.Last() != ' ')
					cmd.Cat(' ');
				p++;
			}
		if(progress_canceled(int(p - text.Begin()), text.GetLength()))
			return false;
		if(!IsNull(cmd) && !executor.Execute(cmd))
			return false;
		if(*p == ';')
			p++;
	}
	return true;
}

SqlConnection *ODBCSession::CreateConnection()
{
	return new ODBCConnection(this);
}

ODBCConnection::ODBCConnection(ODBCSession *session_)
:	session(session_)
{
	LLOG("ODBCConnection " << (void *)this << " " << (void *)session);
	rowcount = rowi = 0;
	number_i = 0;
	num64_i = 0;
	text_i = 0;
	time_i = 0;
	date_i = 0;
}

ODBCConnection::~ODBCConnection()
{
	LLOG("~ODBCConnection " << (void *)this << " " << (void *)session);
}

bool ODBCConnection::IsOk(SQLRETURN ret) const
{
	if(SQL_SUCCEEDED(ret) || ret == SQL_NO_DATA)
		return true;
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	String        error;
	int i = 1;
	while(SQLGetDiagRec(SQL_HANDLE_STMT, session->hstmt, i++, SqlState, &NativeError,
	                    Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		if(error.GetCount())
			error << "\r\n";
		error << (char *)Msg;
	}
	session->SetError(error, statement);
	return false;
}

void ODBCConnection::SetParam(int i, const Value& r)
{
	param.At(i) = r;
/*
	Param& p = param.At(i);
	p.orig = r;
	p.width = 0;
	if(IsNull(r)) {
		p.li = SQL_NULL_DATA;
		p.ctype = SQL_C_CHAR;
		p.sqltype = SQL_VARCHAR;
		p.data = NULL;
		return;
	}
	if(IsNumber(r)) {
		if(r.Is<int64>()) {
			int64 x = r;
			p.ctype = SQL_C_SBIGINT;
			p.sqltype = SQL_BIGINT;
			p.data = String((char *)&x, sizeof(x));
			p.li = sizeof(x);
		}
		else {
			double x = r;
			if(x >= INT_MIN && x < INT_MAX && (int)x == x) {
				long int h = (int)x;
				p.ctype = SQL_C_SLONG;
				p.sqltype = SQL_INTEGER;
				p.data = String((char *)&h, sizeof(h));
				p.li = sizeof(h);
			}
			else {
				p.ctype = SQL_C_DOUBLE;
				p.sqltype = SQL_DOUBLE;
				p.data = String((char *)&x, sizeof(x));
				p.li = sizeof(x);
			}
		}
	}
	if(IsString(r)) {
		p.ctype = SQL_C_CHAR;
		p.sqltype = SQL_VARCHAR;
		p.data = r;
		p.li = p.data.GetLength();
	}
	if(IsDateTime(r)) {
		p.ctype = SQL_C_TYPE_TIMESTAMP;
		p.sqltype = SQL_TYPE_TIMESTAMP;
		Time t = r;
		SQL_TIMESTAMP_STRUCT tm;
		tm.year = t.year;
		tm.month = t.month;
		tm.day = t.day;
		tm.hour = t.hour;
		tm.minute = t.minute;
		tm.second = t.second;
		tm.fraction = 0;
		p.data = String((char *)&tm, sizeof(tm));
		p.li = sizeof(tm);
	}
	if(r.GetType() == 34) {
		p.data = SqlRaw(r);
		p.ctype = SQL_C_BINARY;
		p.sqltype = SQL_LONGVARBINARY;
		p.width = p.li = p.data.GetLength();
	}
*/
}

const char *ODBCReadString(const char *s, String& stmt)
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

bool ODBCConnection::Execute()
{
	LLOG("Execute " << (void *)this << " " << (void *)session << " " << statement);
	if(session->hstmt == SQL_NULL_HANDLE)
		return false;
	session->FlushConnections();
	last_insert_table.Clear();
	number.Clear();
	text.Clear();
	time.Clear();
	CParser p(statement);
	if((p.Id("insert") || p.Id("INSERT")) && (p.Id("into") || p.Id("INTO")) && p.IsId())
		last_insert_table = p.ReadId();

	String query;
	int pi = 0;
	const char *s = statement;
	while(s < statement.End())
		if(*s == '\'' || *s == '\"')
			s = ODBCReadString(s, query);
		else {
			if(*s == '?') {
				if(pi >= param.GetCount()) {
					session->SetError("Invalid number of parameters", statement);
					return false;
				}
				Value v = param[pi++];
				if(session->charset >= 0 && IsString(v))
					v = ToCharset(session->charset, (String)v, CHARSET_DEFAULT, '?');
				if(v.GetType() == 34)
					query.Cat(SqlCompile(MSSQL, ~SqlBinary(SqlRaw(v))));
				else
					query.Cat(SqlCompile(MSSQL, ~SqlVal(v)));
			}
			else
				query.Cat(*s);
			s++;
		}
	param.Clear();
	if(!IsOk(SQLPrepare(session->hstmt, (SQLCHAR *)~query, query.GetCount())))
		return false;

/*
	}
	else {
		if(!IsOk(SQLPrepare(session->hstmt, (SQLCHAR *)~statement, statement.GetCount())))
			return false;
		parse = false;
		bparam = pick(param);
		param.Clear();
		for(int i = 0; i < bparam.GetCount(); i++) {
			Param& p = bparam[i];
			if(!IsOk(SQLBindParameter(session->hstmt, i + 1, SQL_PARAM_INPUT, p.ctype, p.sqltype,
			                          p.width, 0, (SQLPOINTER)~p.data, p.data.GetLength(), &p.li)))
				return false;
		}
	}
*/
	SQLSMALLINT ncol;
	if(!IsOk(SQLExecute(session->hstmt)) || !IsOk(SQLNumResultCols(session->hstmt, &ncol))) {
		SQLFreeStmt(session->hstmt, SQL_CLOSE);
		return false;
	}
	info.Clear();
	string_type.Clear();
	for(int i = 1; i <= ncol; i++) {
		SQLCHAR      ColumnName[256];
		SQLSMALLINT  NameLength;
		SQLSMALLINT  DataType;
		SQLULEN      ColumnSize;
		SQLSMALLINT  DecimalDigits;
		SQLSMALLINT  Nullable;
		if(!IsOk(SQLDescribeCol(session->hstmt, i, ColumnName, 255, &NameLength, &DataType,
		                        &ColumnSize, &DecimalDigits, &Nullable)))
			return false;
		string_type.Add(SQL_C_CHAR);
		SqlColumnInfo& f = info.Add();
		f.nullable = Nullable != SQL_NO_NULLS;
		f.binary = false;
		f.precision = DecimalDigits;
		f.scale = 0;
		f.width = ColumnSize;
		f.name = (char *)ColumnName;
		switch(DataType) {
		case SQL_DECIMAL:
		case SQL_NUMERIC:
		case SQL_SMALLINT:
		case SQL_INTEGER:
		case SQL_REAL:
		case SQL_FLOAT:
		case SQL_DOUBLE:
		case SQL_BIT:
		case SQL_TINYINT:
			f.type = DOUBLE_V;
			break;
		case SQL_BIGINT:
			f.type = INT64_V;
			break;
		case SQL_TYPE_DATE:
			f.type = DATE_V;
			break;
		case SQL_TYPE_TIMESTAMP:
			f.type = TIME_V;
			break;
		case SQL_BINARY:
		case SQL_VARBINARY:
		case SQL_LONGVARBINARY:
			f.type = STRING_V;
			f.binary = true;
			string_type.Top() = SQL_C_BINARY;
			break;
		case SQL_WCHAR:
		case SQL_WVARCHAR:
		case SQL_WLONGVARCHAR:
			f.type = STRING_V;
			f.binary = true;
			string_type.Top() = SQL_C_WCHAR;
			break;
		default:
			f.type = STRING_V;
			break;
		}
	}
	SQLLEN rc;
	SQLRowCount(session->hstmt, &rc);
	rowsprocessed = rc;
	FetchAll();
	return true;
}

int ODBCConnection::GetRowsProcessed() const
{
	return rowsprocessed;
}

bool ODBCConnection::Fetch0()
{
	LLOG("Fetch0 " << (void *)this << " " << (void *)session);
	int ret = SQLFetch(session->hstmt);
	if(ret == SQL_NO_DATA || !IsOk(ret))
		return false;
	fetchrow.Clear();
	double dbl;
	int64 n64;
	SQL_TIMESTAMP_STRUCT tm;
	SQLLEN li;
	for(int i = 0; i < info.GetCount(); i++) {
		Value v = Null;
		int type = info[i].type;
		switch(type) {
		case DOUBLE_V:
			if(!IsOk(SQLGetData(session->hstmt, i + 1, SQL_C_DOUBLE, &dbl, sizeof(dbl), &li)))
			   break;
			if(li != SQL_NULL_DATA)
				v = dbl;
			break;
		case INT64_V:
			if(!IsOk(SQLGetData(session->hstmt, i + 1, SQL_C_SBIGINT, &n64, sizeof(n64), &li)))
			   break;
			if(li != SQL_NULL_DATA)
				v = n64;
			break;
		case TIME_V:
		case DATE_V:
			if(!IsOk(SQLGetData(session->hstmt, i + 1, SQL_C_TYPE_TIMESTAMP, &tm, sizeof(tm), &li)))
			   break;
			if(li != SQL_NULL_DATA) {
				Time m;
				m.year = tm.year;
				m.month = (byte)tm.month;
				m.day = (byte)tm.day;
				m.hour = (byte)tm.hour;
				m.minute = (byte)tm.minute;
				m.second = (byte)tm.second;
				if(type == DATE_V)
					v = (Date)m;
				else
					v = m;
			}
			break;
		default:
			int ct = string_type[i];
			if(!IsOk(SQLGetData(session->hstmt, i + 1, ct, &tm, 0, &li)))
			   break;
			if(li != SQL_NULL_DATA && li >= 0) {
				if(ct == SQL_C_WCHAR) {
					Buffer<WCHAR> sb(li / 2);
					if(!IsOk(SQLGetData(session->hstmt, i + 1, ct, ~sb, li + 2, &li)))
					   break;
					v = ToUtf32(sb, li / 2);
				}
				else {
					StringBuffer sb;
					sb.SetLength(li);
					if(!IsOk(SQLGetData(session->hstmt, i + 1, ct, ~sb, li + 1, &li)))
					   break;
					String s = sb;
					if(session->charset >= 0)
						s = ToCharset(CHARSET_DEFAULT, s, session->charset, '?');
					v = s;
				}
			}
			break;
		}
		fetchrow.Add(v);
	}
	return ret != SQL_NO_DATA && IsOk(ret);
}

bool ODBCConnection::Fetch()
{
	if(rowi >= rowcount)
		return false;
	fetchrow.Clear();
	for(int i = 0; i < info.GetCount(); i++) {
		Value v;
		switch(info[i].type) {
		case DOUBLE_V:
			v = number[number_i++];
			break;
		case INT64_V:
			v = num64[num64_i++];
			break;
		case TIME_V:
			v = time[time_i++];
			break;
		case DATE_V:
			v = date[date_i++];
			break;
		case STRING_V:
			v = text[text_i++];
			break;
		default:
			NEVER();
		}
		fetchrow.Add(v);
	}
	++rowi;
	return true;
}

void ODBCConnection::GetColumn(int i, Ref r) const
{
	LLOG("GetColumn " << (void *)this << " " << (void *)session);
	r.SetValue(fetchrow[i]);
}

void ODBCConnection::FetchAll()
{
	LLOG("FetchAll " << (void *)this);
	rowcount = 0;
	rowi = 0;
	number.Clear();
	num64.Clear();
	text.Clear();
	time.Clear();
	date.Clear();
	number_i = 0;
	num64_i = 0;
	text_i = 0;
	time_i = 0;
	date_i = 0;
	while(info.GetCount() && Fetch0()) {
		rowcount++;
		for(int i = 0; i < info.GetCount(); i++)
			switch(info[i].type) {
			case DOUBLE_V:
				number.Add(fetchrow[i]);
				break;
			case INT64_V:
				num64.Add(fetchrow[i]);
				break;
			case STRING_V:
				text.Add(fetchrow[i]);
				break;
			case TIME_V:
				time.Add(fetchrow[i]);
				break;
			case DATE_V:
				date.Add(fetchrow[i]);
				break;
			default:
				NEVER();
			}
	}
	LLOG("Flush fetched " << rowcount << " info count: " << info.GetCount());
}

void ODBCConnection::Cancel()
{
	param.Clear();

	number.Clear();
	num64.Clear();
	text.Clear();
	time.Clear();
	date.Clear();

	number_i = 0;
	num64_i = 0;
	text_i = 0;
	time_i = 0;
	date_i = 0;
}

String ODBCConnection::ToString() const
{
	return statement;
}

Value ODBCConnection::GetInsertedId() const
{
	Sql sql(GetSession());
	return last_insert_table.GetCount() ? sql.Select("IDENT_CURRENT('" + last_insert_table + "')")
	                                    : sql.Select("@@IDENTITY");
}

}
