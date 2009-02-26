#include "ODBC.h"

NAMESPACE_UPP

bool ODBCSession::Connect(const char *cs)
{
	if(henv && IsOk(SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc))) {
		if(IsOk(SQLDriverConnect(hdbc, NULL, (SQLCHAR *)cs, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT)))
		   return true;
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
	if(hdbc != SQL_NULL_HANDLE) {
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = SQL_NULL_HANDLE;
	}
}

ODBCSession::ODBCSession()
{
	hdbc = SQL_NULL_HANDLE;
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if(henv)
		SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
}

ODBCSession::~ODBCSession()
{
	if(henv)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void ODBCSession::Begin()
{
}

void ODBCSession::Commit()
{
}

void ODBCSession::Rollback()
{
}

String ODBCSession::Savepoint()
{
	return "";
}

void ODBCSession::RollbackTo(const String& savepoint)
{
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

bool   ODBCPerformScript(const String& text, StatementExecutor& executor, Gate2<int, int> progress_canceled)
{
	return false;
}

class ODBCConnection : public Link<ODBCConnection>, public SqlConnection
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
	virtual String          GetUser() const    { ASSERT(session); return session->user; }
	virtual String          ToString() const;
	virtual Value           GetInsertedId() const;

private:
	ODBCSession           *session;
	HSTMT                  hstmt;
	struct Param {
		int    ctype;
		int    sqltype;
		String data;
		SQLLEN li;
	};
	Array<Param>           param, bparam;
	
	bool                   IsOk(SQLRETURN ret);
};

SqlConnection *ODBCSession::CreateConnection()
{
	return new ODBCConnection(this);
}

ODBCConnection::ODBCConnection(ODBCSession *session_)
:	session(session_)
{
	if(session)
		LinkAfter(&session->clink);
	hstmt = SQL_NULL_HANDLE;
	SQLAllocHandle(SQL_HANDLE_STMT, session->hdbc, &hstmt);
}

ODBCConnection::~ODBCConnection()
{
	if(hstmt != SQL_NULL_HANDLE)
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	if(session)
		Unlink();
}

bool ODBCConnection::IsOk(SQLRETURN ret)
{
	if(session->IsOk(ret))
		return true;
	param.Clear();
	return false;
}

void ODBCConnection::SetParam(int i, const Value& r)
{
	Param& p = param.At(i);
	if(IsNumber(r)) {
		double x = r;
		p.ctype = SQL_C_DOUBLE;
		p.sqltype = SQL_DOUBLE;
		p.data = String((char *)&x, sizeof(x));
		p.li = sizeof(x);
	}
	if(IsString(r)) {
		p.ctype = SQL_C_CHAR;
		p.sqltype = SQL_LONGVARCHAR;
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
	if(IsNull(r))
		p.li = SQL_NULL_DATA;
}

bool ODBCConnection::Execute()
{
	if(hstmt == SQL_NULL_HANDLE)
		return false;
	if(parse) {
		if(!IsOk(SQLPrepare(hstmt, (SQLCHAR *)~statement, statement.GetCount())))
			return false;
		parse = false;
	}
	bparam = param;
	param.Clear();
	for(int i = 0; i < bparam.GetCount(); i++) {
		Param& p = bparam[i];
		if(!IsOk(SQLBindParameter(hstmt, i + 1, SQL_PARAM_INPUT, p.ctype, p.sqltype,
		                          p.data.GetCount(), 0, (SQLPOINTER)~p.data, p.data.GetLength(),
		                          &p.li)))
			return false;
	}
	SQLSMALLINT ncol;
	if(!IsOk(SQLExecute(hstmt) || !IsOk(SQLNumResultCols(hstmt, &ncol))))
		return false;
	for(int i = 1; i <= ncol; i++) {
		SQLCHAR      ColumnName[256];
		SQLSMALLINT  NameLength;
		SQLSMALLINT  DataType;
		SQLULEN      ColumnSize;
		SQLSMALLINT  DecimalDigits;
		SQLSMALLINT  Nullable;
		if(!IsOk(SQLDescribeCol(hstmt, i, ColumnName, 255, &NameLength, &DataType, &ColumnSize,
		                        &DecimalDigits, &Nullable)))
			return false;
		SqlColumnInfo& f = info.Add();
		f.nullable = Nullable != SQL_NO_NULLS;
		f.precision = DecimalDigits;
		f.scale = 0; // ?!
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
		case SQL_BIGINT:
			f.type = DOUBLE_V;
			break;
		case SQL_TYPE_DATE:
		case SQL_TYPE_TIMESTAMP:
			f.type = TIME_V;
			break;
		default:
			f.type = STRING_V;
			break;
		}
	}
	return true;
}

int ODBCConnection::GetRowsProcessed() const
{
	SQLLEN rc;
	SQLRowCount(hstmt, &rc);
	return (int)rc;
}

bool ODBCConnection::Fetch()
{
	if(!hstmt)
		return false;
	int ret = SQLFetch(hstmt);
	return ret != SQL_NO_DATA && IsOk(ret);
}

void ODBCConnection::GetColumn(int i, Ref r) const
{
	SQLLEN li;
	double dbl;
	SQL_TIMESTAMP_STRUCT tm;
	Value v = Null;
	switch(info[i].type) {
	case DOUBLE_V:
		SQLGetData(hstmt, i + 1, SQL_C_DOUBLE, &dbl, sizeof(dbl), &li);
		if(li != SQL_NULL_DATA)
			v = dbl;
		break;
	case TIME_V:
		SQLGetData(hstmt, i + 1, SQL_C_TYPE_TIMESTAMP, &tm, sizeof(tm), &li);
		if(li != SQL_NULL_DATA) {
			Time m;
			m.year = tm.year;
			m.month = (byte)tm.month;
			m.day = (byte)tm.day;
			m.hour = (byte)tm.hour;
			m.minute = (byte)tm.minute;
			m.second = (byte)tm.second;
			v = m;
		}
		break;
	default:
		StringBuffer sb;
		sb.SetLength(256);
		SQLGetData(hstmt, i + 1, SQL_C_CHAR, ~sb, 255, &li);
		if(li > 255) {
			sb.SetLength(li);
			SQLGetData(hstmt, i + 1, SQL_C_CHAR, ~sb, li, &li);
		}
		if(li != SQL_NULL_DATA) {
			sb.SetLength(li);
			v = String(sb);
		}
		break;
	}
	r.SetValue(v);
}

void ODBCConnection::Cancel()
{
	param.Clear();
	bparam.Clear();
}

String ODBCConnection::ToString() const
{
	return "";
}

Value ODBCConnection::GetInsertedId() const
{
	return Null;
}

END_UPP_NAMESPACE
