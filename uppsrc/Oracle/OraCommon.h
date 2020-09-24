#ifndef ORACOMMON_H
#define ORACOMMON_H

class OciSqlConnection;


namespace Upp {

Date        OciDecodeDate(const byte data[7]);
bool        OciEncodeDate(byte data[7], Date d);
Time        OciDecodeTime(const byte data[7]);
bool        OciEncodeTime(byte data[7], Time t);

const char *OciParseString(const char *s);
String      OciParseStringError(const char *s);
String      OciParseRefError(const char *s);

int         OciParse(const char *statement, String& out, OciSqlConnection *conn, SqlSession *session);
Sql::ERRORCLASS OciErrorClass(int errcode);

class SqlSequence : public ValueGen {
	SqlId       ssq;
	SqlId&      seq;
	SqlSession *session;

public:
	virtual Value  Get();

	Value operator++()                                                  { return Get(); }

	void Set(SqlId id, SqlSession& s)                                   { ssq = id; session = &s; }

#ifndef NOAPPSQL
	void Set(SqlId id)                                                  { ssq = id; session = NULL; }
	SqlSequence(const char *name) : ssq(name), seq(ssq)                 { session = NULL; }
	SqlSequence(SqlId& seq) : seq(seq)                                  { session = NULL; }
#endif
	SqlSequence(const char *name, SqlSession& s) : ssq(name), seq(ssq)  { session = &s; }
	SqlSequence(SqlId& seq, SqlSession& s) : seq(seq)                   { session = &s; }
	SqlSequence() : seq(ssq)                                            { session = NULL; }
};

#ifndef NOAPPSQL
bool OraclePerformScript(const String& text, StatementExecutor& se = SQLStatementExecutor(), Gate<int, int> progress_canceled = false);
#else
bool OraclePerformScript(const String& text, StatementExecutor& se, Gate<int, int> progress_canceled = false);
#endif

Vector<String> OracleSchemaUsers(Sql& cursor);
Vector<String> OracleSchemaTables(Sql& cursor, String database);
Vector<String> OracleSchemaViews(Sql& cursor, String database);
Vector<String> OracleSchemaSequences(Sql& cursor, String database);
Vector<String> OracleSchemaPrimaryKey(Sql& cursor, String database, String table);
String         OracleSchemaRowID(Sql& cursor, String database, String table);
Vector<String> OracleSchemaReservedWords();

const int ORA_BLOB_V = -1;
const int ORA_CLOB_V = -2;

class OracleRef {
public:
	OracleRef(int ora_vtype = VOID_V) : ora_vtype(ora_vtype) {}
	OracleRef(const Value& v) : ora_vtype(IsValue(v) ? ValueTo<OracleRef>(v).ora_vtype : VOID_V) {}

	operator    Value () const                { return RawToValue(*this); }
	static bool IsValue(const Value& v)       { return IsTypeRaw<OracleRef>(v); }

	const char *Parse(const char *stmt);
	int         GetType() const               { return ora_vtype; }

	int         GetOraType() const;
	int         GetMaxLen() const;

private:
	int         ora_vtype;
};

class OciSqlConnection : public SqlConnection {
	friend int OciParse(const char *statement, String& out, OciSqlConnection *conn, SqlSession *session);

protected:
	virtual void SetParam(int i, OracleRef ref) = 0;
};

}

#endif
