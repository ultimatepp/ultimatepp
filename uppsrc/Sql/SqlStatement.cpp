#include "Sql.h"

NAMESPACE_UPP

String SqlStatement::Get(int dialect) const {
	ASSERT(dialect == ORACLE || dialect == SQLITE3 || dialect == MY_SQL || dialect == MSSQL ||
	       dialect == PGSQL || dialect == FIREBIRD || dialect == DB2);
	return SqlCompile(dialect, text);
}

SqlSelect& SqlSelect::operator|=(const SqlSelect& s2) {
	text << " union " << SqlCase(SQLITE3, "")("(") << s2.text << SqlCase(SQLITE3, "")(")");
	return *this;
}

SqlSelect& SqlSelect::operator&=(const SqlSelect& s2) {
	text << " intersect " << SqlCase(SQLITE3, "")("(") << s2.text << SqlCase(SQLITE3, "")(")");
	return *this;
}

SqlSelect& SqlSelect::operator-=(const SqlSelect& s2) {
	text << SqlCase(MSSQL|PGSQL|SQLITE3," except ")(" minus ") << SqlCase(SQLITE3, "")("(")
	<< s2.text << SqlCase(SQLITE3, "")(")");
	return *this;
}

SqlSelect operator|(const SqlSelect& s1, const SqlSelect& s2) {
	SqlSelect s = s1;
	s |= s2;
	return s;
}

SqlSelect operator&(const SqlSelect& s1, const SqlSelect& s2) {
	SqlSelect s = s1;
	s &= s2;
	return s;
}

SqlSelect operator-(const SqlSelect& s1, const SqlSelect& s2) {
	SqlSelect s = s1;
	s -= s2;
	return s;
}

SqlSelect& SqlSelect::Where(const SqlBool& exp) {
	if(!exp.IsTrue() && !exp.IsEmpty())
		text << " where " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::On(const SqlBool& exp) {
	if(!exp.IsTrue() && !exp.IsEmpty())
		text << " on " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::StartWith(const SqlBool& exp) {
	text << " start with " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::ConnectBy(const SqlBool& exp) {
	text << " connect by " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::GroupBy(const SqlSet& set) {
	text << " group by " << ~set;
	return *this;
}

SqlSelect& SqlSelect::Having(const SqlBool& exp) {
	text << " having " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::OrderBy(const SqlSet& set) {
	if(!set.IsEmpty())
		text << " order by " << ~set;
	return *this;
}

SqlSelect& SqlSelect::ForUpdate() {
	text << " for update";
	return *this;
}

SqlSelect& SqlSelect::NoWait() {
	text << " nowait";
	return *this;
}

SqlSelect& SqlSelect::Limit(int limit) {
	text << " limit " << limit;
	return *this;
}

SqlSelect& SqlSelect::Limit(int64 offset, int limit) {
	text << " limit " << offset << ", " << limit;
	return *this;
}

SqlSelect& SqlSelect::Offset(int64 offset) {
	text << " offset " << offset;
	return *this;
}

SqlSelect& SqlSelect::operator()(const SqlVal& val)
{
	if(text.GetCount())
		text << ", ";
	text << ~val;
	return *this;
}

SqlSelect& SqlSelect::Hint(const char *hint)
{
	text = "/*+ " + String(hint) + " */ " + text;
	return *this;
}

SqlSelect& SqlSelect::Get() {
	text = "select " + text + SqlCase(ORACLE, "from DUAL")("");
	return *this;
}

SqlSelect& SqlSelect::From(const SqlSet& table) {
	text = "select " + text + " from " + table(SqlSet::SETOP + 1);
	return *this;
}

SqlSelect& SqlSelect::From(SqlId table) {
	text = "select " + text + " from " + ~table;
	return *this;
}

SqlSelect& SqlSelect::From(SqlId table1, SqlId table2) {
	text = "select " + text + " from " + ~table1 + ", " + ~table2;
	return *this;
}

SqlSelect& SqlSelect::From(SqlId table1, SqlId table2, SqlId table3) {
	text = "select " + text + " from " + ~table1 + ", " + ~table2 + ", " + ~table3;
	return *this;
}

SqlSelect& SqlSelect::InnerJoin0(const String& table) {
	text << " inner join " << ~table;
	return *this;
}

SqlSelect& SqlSelect::LeftJoin0(const String& table) {
	text << " left outer join " << ~table;
	return *this;
}

SqlSelect& SqlSelect::RightJoin0(const String& table) {
	text << " right outer join " << ~table;
	return *this;
}

SqlSelect& SqlSelect::FullJoin0(const String& table) {
	text << " full outer join " << ~table;
	return *this;
}

SqlVal SqlSelect::AsValue() const
{
	return SqlVal(String("(").Cat() << text << ")", SqlVal::LOW);
}

SqlSelect SqlSelect::AsTable(SqlId tab) const
{
	SqlSelect h;
	h.text = String("(").Cat() << text << ") as " << ~tab;
	return h;
}

SqlSelect::SqlSelect(Fields f)
{
	SqlSet set(f);
	text = ~set;
}

#define E__SCat(I)       set.Cat(p##I)

#define E__QSqlSelectF(I) \
SqlSelect::SqlSelect(__List##I(E__SqlVal)) { \
	SqlSet set; \
	__List##I(E__SCat); \
	text = ~set; \
}
__Expand(E__QSqlSelectF);

#define E__QSelectF(I) \
SqlSelect Select(__List##I(E__SqlVal)) { \
	SqlSet set; \
	__List##I(E__SCat); \
	return Select(set); \
}
__Expand(E__QSelectF);


// -------------------------------

SqlDelete::SqlDelete(SqlVal table) {
	text = "delete from " + ~table;
}

SqlDelete& SqlDelete::Where(const SqlBool& b) {
	text << " where " << ~b;
	return *this;
}

void SqlInsert::Column(SqlId column, SqlVal val) {
	set1.Cat(column);
	set2.Cat(val);
	if(keycolumn.IsNull()) keycolumn = column;
	if(keyvalue.IsEmpty()) keyvalue =val;
}

SqlInsert::operator SqlStatement() const {
	String s = "insert into " + ~table;
	if(!set1.IsEmpty()) {
		s << set1();
		if(from.IsEmpty()) {
			if(!set2.IsEmpty())
				s << " values " << set2();
		}
		else
			s << ' ' + SqlStatement(Select(set2).From(from).Where(where)).GetText();
	}
	return SqlStatement(s);
}

SqlInsert& SqlInsert::From(SqlId from) {
	return From(SqlSet(from));
}

struct InsertFieldOperator : public FieldOperator {
	SqlInsert *insert;
	bool       nokey;

	virtual void Field(const char *name, Ref f)	{
		if(!nokey)
			insert->Column(name, (Value)f);
		nokey = false;
	}
	
	InsertFieldOperator() { nokey = false; }
};

SqlInsert::SqlInsert(Fields f, bool nokey) {
	InsertFieldOperator ifo;
	ifo.insert = this;
	ifo.nokey = nokey;
	f(ifo);
	table = ifo.table;
}

SqlInsert& SqlInsert::operator()(Fields f, bool nokey)
{
	InsertFieldOperator ifo;
	ifo.insert = this;
	ifo.nokey = nokey;
	f(ifo);
	return *this;
}

struct UpdateFieldOperator : public FieldOperator {
	SqlUpdate *update;

	virtual void Field(const char *name, Ref f)	{
		update->Column(name, (Value)f);
	}
};

SqlUpdate::SqlUpdate(Fields f) {
	UpdateFieldOperator ufo;
	ufo.update = this;
	f(ufo);
	table = ufo.table;
}

SqlUpdate& SqlUpdate::operator()(Fields f) {
	UpdateFieldOperator ufo;
	ufo.update = this;
	f(ufo);
	return *this;
}

// ------------------------------------

SqlUpdate::operator SqlStatement() const {
	StringBuffer stmt;
	stmt << "update " << ~table << " set " << ~set;
	if(!where.IsEmpty())
		stmt << " where " << ~where;
	return SqlStatement(stmt);
}

void SqlUpdate::Column(SqlId column, SqlVal val) {
	set.Cat(SqlVal(SqlVal(column), " = ", val, SqlS::COMP));
}

void SqlUpdate::Column(const SqlSet& cols, const SqlSet& val)
{
	set.Cat(SqlVal(SqlS(cols(), SqlS::HIGH), " = ", SqlS(val(), SqlS::HIGH), SqlS::COMP));
}

// ------------------------------------
// deprecated

bool SqlStatement::Execute(Sql& cursor) const {
	ASSERT(text.GetCount());
	return cursor.Execute(*this);
}

void SqlStatement::Force(Sql& cursor) const {
	ASSERT(text.GetCount());
	if(!cursor.Execute(*this))
		throw SqlExc(cursor.GetSession());
}

Value SqlStatement::Fetch(Sql& cursor) const {
	ASSERT(text.GetCount());
	if(!Execute(cursor))
		return ErrorValue(SqlExc(cursor.GetSession()));
	if(!cursor.Fetch())
		return Null;
	return cursor[0];
}

#ifndef NOAPPSQL
bool SqlStatement::Execute() const {
	return Execute(SQL);
}

void SqlStatement::Force() const {
	Force(SQL);
}

Value SqlStatement::Fetch() const {
	return Fetch(SQL);
}

bool SqlPerformScript(SqlSession& session, Stream& script,
                      Gate2<int, int> progress_canceled, bool stoponerror)
{
	String stmt;
	int level = 0;
	bool ok = true;
	while(!script.IsEof()) {
		int c = script.Term();
		if(IsAlpha(c)) {
			String id;
			while(IsAlpha(script.Term())) {
				c = script.Get();
				stmt.Cat(c);
				id.Cat(ToUpper(c));
			}
			if(id == "BEGIN")
				level++;
			if(id == "END")
				level--;
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
		if(c == ';' && level == 0) {
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
			if(c == '(')
				level++;
			if(c == ')')
				level--;
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

bool SqlPerformScript(Stream& script,
                      Gate2<int, int> progress_canceled, bool stoponerror)
{
	return SqlPerformScript(SQL.GetSession(), script, progress_canceled, stoponerror);
}

bool SqlPerformScript(SqlSession& session, const String& script,
                      Gate2<int, int> progress_canceled, bool stoponerror)
{
	StringStream ss(script);
	return SqlPerformScript(session, ss, progress_canceled, stoponerror);
}

bool SqlPerformScript(const String& script,
                      Gate2<int, int> progress_canceled, bool stoponerror)
{
	return SqlPerformScript(SQL.GetSession(), script, progress_canceled, stoponerror);
}

#endif

END_UPP_NAMESPACE
