#include "Sql.h"

namespace Upp {

#define LLOG(x) // DLOG(x)

bool SqlToBool(const String& s) {
	return !(IsNull(s) || *s == '0' && s[1] == '\0');
}

bool SqlToBool(const Value& v) {
	if(IsNull(v)) return false;
	if(IsString(v)) return SqlToBool(String(v));
	if(IsNumber(v)) return (int) v;
	return true;
}

const String& BoolToSql(bool b) {
	static String T("1"), F("0");
	return b ? T : F;
}

void FieldOperator::Field(Ref f) {}

void FieldOperator::Field(const char *name, Ref f) { Field(f); }

void FieldOperator::Field(const char *name, Ref f, bool *b) { Field(name, f); }

void FieldOperator::Width(int width) {}


FieldOperator& FieldOperator::operator()(const char *name, bool& b) {
	String x = BoolToSql(b);
	Field(name, x, &b);
	b = SqlToBool(x);
	return *this;
}

static char sql_error[] = "Database error";

#ifndef NOAPPSQL
SqlExc::SqlExc() : Exc(sql_error) {
	SetSessionError(SQL.GetSession());
}
#endif

SqlExc::SqlExc(const SqlSession& session) : Exc(sql_error) {
	SetSessionError(session);
}

SqlExc::SqlExc(const Sql& sql) : Exc(sql_error) {
	SetSessionError(sql.GetSession());
}

void SqlExc::SetSessionError(const SqlSession& session) {
	if(session.WasError())
		*this = session.GetLastError();
	else
		*this = String(sql_error);
	*this << "\nSQL error: " << session.GetErrorStatement();
}

SqlConnection::SqlConnection()  { parse = true; fetchrows = 32; longsize = 16384; }
SqlConnection::~SqlConnection() {}

void SqlConnection::Cancel() {}

int  SqlConnection::GetRowsProcessed() const {
	NEVER();
	return 0;
}

String SqlConnection::GetUser() const {
	NEVER();
	return Null;
}

Value SqlConnection::GetInsertedId() const
{
	NEVER();
	return Null;
}

String Sql::Compile(const SqlStatement& s)
{
	byte dialect = GetDialect();
	ASSERT(dialect);
	return s.Get(dialect);
}

void Sql::Clear() {
	if(cn) {
		cn->Cancel();
		cn->parse = true;
	}
}

void Sql::SetParam(int i, const Value& val) {
	cn->SetParam(i, val);
	if(GetSession().GetTrace())
		param.Set(i, val);
}

void Sql::SetStatement(const String& s) {
	cn->statement = s;
	cn->parse = true;
}

bool Sql::Execute() {
	SqlSession &session = GetSession();

	session.SetStatement(cn->statement);
	session.SetStatus(SqlSession::BEFORE_EXECUTING);
	cn->starttime = msecs();
	Stream *s = session.GetTrace();
	if(s) {
#ifndef NOAPPSQL
		if(this == &AppCursor())
			*s << "SQL* ";
		else
		if(this == &AppCursorR())
			*s << "SQLR* ";
#endif
		String st = cn->statement;
		if(session.IsTraceCompression())
			st = CompressLog(st);
		int i = 0;
		for(const char *q = st; *q; q++)
			if(*q == '?' && i < param.GetCount()) {
				Value v = param[i++];
				if(IsString(v))
					*s << '\'' << v << '\'';
				else
					*s << v;
			}
			else
				s->Put(*q);
		*s << '\n';
	}
	if(!session.IsOpen())
	{
		session.SetStatus(SqlSession::CONNECTION_ERROR);
		return false;
	}
	session.SetStatus(SqlSession::START_EXECUTING);
	bool b = cn->Execute();
	session.SetTime(msecs() - cn->starttime);
	session.SetStatus(SqlSession::END_EXECUTING);
	if(!b)
		session.SetStatus(SqlSession::EXECUTING_ERROR);
	if(!session.IsUseRealcase())
		for(int i = 0; i < cn->info.GetCount(); i++)
			cn->info[i].name = ToUpper(cn->info[i].name);

	session.SetStatus(SqlSession::AFTER_EXECUTING);
	if(!b && session.throwonerror)
		throw SqlExc(GetSession());
	return b;
}

void Sql::ExecuteX() {
	if(!Execute())
		throw SqlExc(GetSession());
}

bool Sql::Execute(const String& s) {
	SetStatement(s);
	return Execute();
}

void Sql::ExecuteX(const String& s) {
	SetStatement(s);
	ExecuteX();
}

//$-

#define E__SetParam(I)    SetParam(I - 1, p##I)

#define E__RunF(I) \
bool Sql::Run(__List##I(E__Value)) { \
	__List##I(E__SetParam); \
	return Run(); \
}
__Expand(E__RunF)

#define E__RunFX(I) \
void Sql::RunX(__List##I(E__Value)) { \
	__List##I(E__SetParam); \
	RunX(); \
}
__Expand(E__RunFX)

#define E__ExecuteF(I) \
bool Sql::Execute(const String& s, __List##I(E__Value)) { \
	SetStatement(s); \
	__List##I(E__SetParam); \
	return Execute(); \
}
__Expand(E__ExecuteF)

#define E__ExecuteFX(I) \
void Sql::ExecuteX(const String& s, __List##I(E__Value)) { \
	SetStatement(s); \
	__List##I(E__SetParam); \
	ExecuteX(); \
}
__Expand(E__ExecuteFX)

//$+

bool Sql::Fetch() {
	SqlSession& session = GetSession();
	session.SetStatus(SqlSession::START_FETCHING);

	dword t0 = msecs();
	bool b = cn->Fetch();
	dword t = msecs();

	dword total = cn->starttime == INT_MAX ? 0 : t - cn->starttime;
	dword fetch = t - t0;

	session.SetStatus(SqlSession::END_FETCHING);
	if(!b) {
		session.SetTime(total);
		session.SetStatus(SqlSession::END_FETCHING_MANY);
	}
	Stream *s = session.GetTrace();
	if(s) {
		if((int)total > session.traceslow)
			*s << "SLOW SQL: " << total << " ms: " << cn->statement << UPP::EOL;
		else
		if((int)fetch > session.traceslow)
			*s << "SLOW SQL: " << fetch << " ms further fetch: " << cn->statement << UPP::EOL;
	}
	cn->starttime = INT_MAX;
	return b;
}

//$-
#define E__GetColumn(I) cn->GetColumn(I - 1, p##I)

#define E__FetchF(I) \
bool Sql::Fetch(__List##I(E__Ref)) { \
	if(!Fetch()) return false; \
	__List##I(E__GetColumn); \
	return true; \
}
__Expand(E__FetchF)
//$+

Vector<Value> Sql::GetRow() const {
	Vector<Value> row;
	int n = GetColumns();
	row.SetCount(n);
	for(int i = 0; i < n; i++)
		row[i] = (*this)[i];
	return row;
}

bool Sql::Fetch(Vector<Value>& row) {
	if(!Fetch()) return false;
	row = GetRow();
	return true;
}

ValueMap Sql::GetRowMap() const
{
	ValueMap m;
	int n = GetColumns();
	for(int i = 0; i < n; i++)
		m.Add(GetColumnInfo(i).name, (*this)[i]);
	return m;
}

Value Sql::operator%(const SqlStatement& q)
{
	return Select0(Compile(q));
}

ValueMap Sql::operator^(const SqlStatement& q)
{
	Execute(q);
	ValueMap m;
	Fetch(m);
	return m;
}

ValueArray Sql::operator/(const SqlStatement& q)
{
	ValueArray va;
	Execute(q);
	ValueMap m;
	while(Fetch(m))
		va.Add(m);
	return va;
}

bool Sql::Fetch(ValueMap& row) {
	if(!Fetch()) return false;
	row = GetRowMap();
	return true;
}

struct sReadFields : public FieldOperator {
	Sql *sql;

	void Field(const char *name, Ref f) {
		sql->GetColumn(SqlId(name), f);
	}
};

void Sql::Get(Fields fo)
{
	sReadFields ff;
	ff.sql = this;
	fo(ff);
}

bool Sql::Fetch(Fields fo) {
	if(!Fetch()) return false;
	Get(fo);
	return true;
}

int Sql::GetColumnCount() const
{
	return cn->info.GetCount();
}

int  Sql::GetColumns() const {
	return GetColumnCount();
}

void Sql::GetColumn(int i, Ref r) const {
	cn->GetColumn(i, r);
}

void Sql::GetColumn(SqlId colid, Ref r) const
{
	String s = ~colid;
	for(int j = 0; j < 2; j++) {
		for(int i = 0; i < cn->info.GetCount(); i++)
			if(cn->info[i].name == s) {
				GetColumn(i, r);
				return;
			}
		if(!GetSession().IsUseRealcase())
			s = ToUpper(s);
	}
	r.SetNull();
}

Value Sql::operator[](int i) const {
	Value v;
	cn->GetColumn(i, v);
	return v;
}

Value Sql::operator[](SqlId id) const {
	String s = ~id;
	for(int j = 0; j < 2; j++) {
		for(int i = 0; i < cn->info.GetCount(); i++)
			if(cn->info[i].name == s)
				return operator[](i);
		if(!GetSession().IsUseRealcase())
			s = ToUpper(s);
	}
	NEVER_(String().Cat() << "SQL [" << ~id << "] not found");
	return Value();
}

Value Sql::Select0(const String& s) {
	SetStatement(s);
	if(!Run())
		return ErrorValue(GetLastError());
	if(!Fetch())
		return Null;
	Value v;
	cn->GetColumn(0, v);
	return v;
}

Value Sql::Select(const String& s) {
	return Select0("select " + s);
}

//$-
#define E__SelectF(I) \
Value Sql::Select(const String& s, __List##I(E__Value)) { \
	__List##I(E__SetParam); \
	return Select(s); \
}
__Expand(E__SelectF)

#define E__Inserter(I)  clist += ", ", clist += c##I, qlist += ", ?", SetParam(I, v##I)

#define E__InsertF(I) \
bool Sql::Insert(const char *table, const char *c0, const Value& v0, __List##I(E__ColVal)) { \
	String  clist = c0; \
	String  qlist = "?"; \
	SetParam(0, v0); \
	__List##I(E__Inserter); \
	return Execute(String("insert into ") + table + '(' + clist + ") values(" + qlist + ')'); \
}
__Expand(E__InsertF)

#define E__InserterId(I)  clist += ", ", clist += c##I.ToString(), qlist += ", ?", SetParam(I, v##I)

#define E__InsertIdF(I) \
bool Sql::Insert(SqlId table, SqlId c0, const Value& v0, __List##I(E__IdVal)) { \
	String  clist = c0.ToString(); \
	String  qlist = "?"; \
	SetParam(0, v0); \
	__List##I(E__InserterId); \
	return Execute( \
			String("insert into ") + table.ToString() + '(' + clist + ") values(" + qlist + ')'); \
}
__Expand(E__InsertIdF)

static inline void sComma(int I, String& s) {
	if(I > 1) s.Cat(", ");
}

#define E__Updater(I)  sComma(I, list), list.Cat(c##I), list.Cat(" = ?"), SetParam(I - 1, v##I)

#define  E__UpdateF(I) \
bool Sql::Update(const char *table, const char *key, const Value& keyval, __List##I(E__ColVal)) { \
	String list; \
	__List##I(E__Updater); \
	SetParam(I, keyval); \
	return Execute(String ("update ") + table + " set " + list + " where " + key + " = ?"); \
}
__Expand(E__UpdateF)

#define E__UpdaterId(I)  sComma(I, list), list.Cat(c##I.ToString()), list.Cat(" = ?"), SetParam(I - 1, v##I)

#define  E__UpdateIdF(I) \
bool Sql::Update(SqlId table, SqlId key, const Value& keyval, __List##I(E__IdVal)) { \
	String list; \
	__List##I(E__UpdaterId); \
	SetParam(I, keyval); \
	return Execute(String ("update ") + table.ToString() + \
	               " set " + list + " where " + key.ToString() + " = ?"); \
}
__Expand(E__UpdateIdF)
//$+

bool Sql::Delete(const char *table, const char *key, const Value& keyval) {
	return Execute("delete from " + String(table) + " where " + key + " = ?", keyval);
}

bool Sql::Delete(SqlId table, SqlId key, const Value& keyval) {
	return Delete(~table.ToString(), ~key.ToString(), keyval);
}

int Sql::GetDialect() const {
	return GetSession().GetDialect();
}

struct NfInsert : public FieldOperator {
	int    i;
	Sql   *sql;
	String clist;
	String qlist;

	virtual void Field(const char *name, Ref f) {
		if(i) {
			clist += ", ";
			qlist += ", ";
		}
		clist += name;
		qlist += "? ";
		sql->SetParam(i++, f);
	}
};

bool Sql::Insert(Fields nf, const char *table) {
	NfInsert w;
	w.i = 0;
	w.sql = this;
	nf(w);
	return Execute(String("insert into ") + (table ? String(table) : w.table) +
	               '(' + w.clist + ") values(" + w.qlist + ')');
}

bool Sql::Insert(Fields nf) {
	return Insert(nf, NULL);
}

bool Sql::Insert(Fields nf, SqlId table) {
	return Insert(nf, (const char *)~table);
}

struct NfInsertNoKey : public FieldOperator {
	int    i;
	Sql   *sql;
	String clist;
	String qlist;

	virtual void Field(const char *name, Ref f) {
		if(clist.GetCount()) {
			clist += ", ";
			qlist += ", ";
		}
		if(i) {
			clist += name;
			qlist += "? ";
			sql->SetParam(i - 1, f);
		}
		i++;
	}
};

bool Sql::InsertNoKey(Fields nf, const char *table) {
	NfInsertNoKey w;
	w.i = 0;
	w.sql = this;
	nf(w);
	return Execute(String("insert into ") + (table ? String(table) : w.table) +
	               '(' + w.clist + ") values(" + w.qlist + ')');
}

bool Sql::InsertNoKey(Fields nf) {
	return InsertNoKey(nf, NULL);
}

bool Sql::InsertNoKey(Fields nf, SqlId table) {
	return InsertNoKey(nf, (const char *)~table);
}

struct NfInsertNoNulls : public FieldOperator {
	int    i;
	Sql   *sql;
	String clist;
	String qlist;

	virtual void Field(const char *name, Ref f) {
		if(!f.IsNull()) {
			if(clist.GetCount()) {
				clist += ", ";
				qlist += ", ";
			}
			clist << name;
			qlist << "? ";
			sql->SetParam(i++, f);
		}
	}
};

bool Sql::InsertNoNulls(Fields nf, const char *table)
{
	NfInsertNoNulls w;
	w.i = 0;
	w.sql = this;
	nf(w);
	return Execute(String("insert into ") + (table ? String(table) : w.table) +
	               '(' + w.clist + ") values(" + w.qlist + ')');
}

bool Sql::InsertNoNulls(Fields nf)
{
	return InsertNoNulls(nf, NULL);
}

bool Sql::InsertNoNulls(Fields nf, SqlId table)
{
	return InsertNoNulls(nf, (const char *)~table);
}

#define E__Updater(I)  sComma(I, list), list.Cat(c##I), list.Cat(" = ?"), SetParam(I - 1, v##I)

struct NfUpdate : public FieldOperator {
	int    i;
	Sql   *sql;
	String list;
	String key;
	Value  keyval;

	virtual void Field(const char *name, Ref f) {
		if(i == 0) {
			key = name;
			keyval = f;
		}
		else {
			if(i > 1)
				list += ", ";
			list << name << " = ?";
			sql->SetParam(i - 1, f);
		}
		i++;
	}
};

bool Sql::Update(Fields nf, const char *table) {
	NfUpdate w;
	w.i = 0;
	w.sql = this;
	nf(w);
	SetParam(w.i - 1, w.keyval);
	return Execute(String ("update ") + (table ? String(table) : w.table) +
	               " set " + w.list + " where " + w.key + " = ?");
}

bool Sql::Update(Fields nf) {
	return Update(nf, NULL);
}

bool Sql::Update(Fields nf, SqlId table) {
	return Update(nf, (const char *)~table);
}

void Sql::SetSession(SqlSource& s) {
	Detach();
	cn = s.CreateConnection();
}

void Sql::SetError(String err, String stmt, int code, const char *scode, ERRORCLASS clss)
{
	GetSession().SetError(err, stmt, code, scode, clss);
}

void   Sql::ClearError()                          { GetSession().ClearError(); }

String Sql::GetLastError() const                  { return GetSession().GetLastError(); }
String Sql::GetErrorStatement() const             { return GetSession().GetErrorStatement(); }
int    Sql::GetErrorCode() const                  { return GetSession().GetErrorCode(); }
String Sql::GetErrorCodeString() const            { return GetSession().GetErrorCodeString(); }
Sql::ERRORCLASS Sql::GetErrorClass() const        { return GetSession().GetErrorClass(); }
bool   Sql::WasError() const                      { return GetSession().WasError(); }

void   Sql::Begin()                               { ClearError(); GetSession().Begin(); }
void   Sql::Commit()                              { GetSession().Commit(); }
void   Sql::Rollback()                            { GetSession().Rollback(); }
int    Sql::GetTransactionLevel()                 { return GetSession().GetTransactionLevel(); }

String Sql::Savepoint()                           { return GetSession().Savepoint(); }
void   Sql::RollbackTo(const String& savepoint)   { GetSession().RollbackTo(savepoint); }

bool   Sql::IsOpen()                              { return cn && GetSession().IsOpen(); }

void SqlConnection::Attach(Sql& sql, SqlConnection *con)
{
	sql.Attach(con); // Duck tape to fix Oci8
}

#ifndef NOAPPSQL
Sql::Sql() {
	cn = NULL;
	if(SQL.cn)
		cn = SQL.GetSession().CreateConnection();
}
#endif

Sql::Sql(SqlSource& s) {
	cn = s.CreateConnection();
}

#ifndef NOAPPSQL
Sql::Sql(const char *stmt) {
	cn = SQL.GetSession().CreateConnection();
	SetStatement(stmt);
}
#endif

Sql::Sql(const char *stmt, SqlSource& s) {
	cn = s.CreateConnection();
	SetStatement(stmt);
}

#ifndef NOAPPSQL
Sql::Sql(const SqlStatement& stmt) {
	cn = SQL.GetSession().CreateConnection();
	SetStatement(stmt);
}
#endif

Sql::Sql(const SqlStatement& stmt, SqlSource& s) {
	cn = s.CreateConnection();
	SetStatement(stmt);
}

Sql::Sql(SqlConnection *connection)
: cn(connection)
{}

void Sql::Detach()
{
	if(cn) delete cn;
	cn = NULL;
	param.Clear();
}

void Sql::Attach(SqlConnection *connection)
{
	Detach();
	cn = connection;
}

Sql::~Sql() {
	Detach();
}

#ifndef NOAPPSQL

SqlR::SqlR()
:	Sql(SQLR.GetSession()) {}

SqlR::SqlR(const char *stmt)
:	Sql(stmt, SQLR.GetSession()) {}

SqlR::SqlR(const SqlStatement& s)
:	Sql(s, SQLR.GetSession()) {}

#endif

#ifndef NOAPPSQL
void operator*=(ValueMap& map, SqlSelect select)
{
	map.Clear();
	Sql sql;
	sql * select;
	while(sql.Fetch())
		map.Add(sql[0], sql[1]);
}
#endif

}
