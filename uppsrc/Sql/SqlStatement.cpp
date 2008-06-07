#include "Sql.h"

NAMESPACE_UPP

String SqlStatement::Get(int dialect) const {
	ASSERT(dialect == ORACLE || dialect == SQLITE3 || dialect == MY_SQL || dialect == MSSQL ||
	       dialect == PGSQL || dialect == FIREBIRD || dialect == DB2);
	return SqlCompile(dialect, text);
}

SqlSelect& SqlSelect::operator|=(const SqlSelect& s2) {
	text << " union (" << s2.text << ')';
	return *this;
}

SqlSelect& SqlSelect::operator&=(const SqlSelect& s2) {
	text << " intersect (" << s2.text << ')';
	return *this;
}

SqlSelect& SqlSelect::operator-=(const SqlSelect& s2) {
	text << " minus (" << s2.text << ')';
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

SqlSelect& SqlSelect::InnerJoin(SqlId table) {
	text << " inner join " << ~table;
	return *this;
}

SqlSelect& SqlSelect::LeftJoin(SqlId table) {
	text << " left outer join " << ~table;
	return *this;
}

SqlSelect& SqlSelect::RightJoin(SqlId table) {
	text << " right outer join " << ~table;
	return *this;
}

SqlSelect& SqlSelect::FullJoin(SqlId table) {
	text << " full outer join " << ~table;
	return *this;
}

SqlSelect::SqlSelect(Fields f) {
	SqlSet set(f);
	text = ~set;
}

#define E__SCat(I)       set.Cat(p##I)

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
	String s = "insert into " + ~table + set1();
	if(from.IsEmpty())
		s << " values " << set2();
	else
		s << ' ' + SqlStatement(Select(set2).From(from).Where(where)).GetText();
	return SqlStatement(s);
}

SqlInsert& SqlInsert::From(SqlId from) {
	return From(SqlSet(from));
}

struct InsertFieldOperator : public FieldOperator {
	SqlInsert *insert;

	virtual void Field(const char *name, Ref f)	{
		insert->Column(name, (Value)f);
	}
};

SqlInsert::SqlInsert(Fields f) {
	InsertFieldOperator ifo;
	ifo.insert = this;
	f(ifo);
	table = ifo.table;
}

SqlInsert& SqlInsert::operator()(Fields f) {
	InsertFieldOperator ifo;
	ifo.insert = this;
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
	return SqlStatement("update " + ~table + " set " + ~set + " where " + ~where);
}

void SqlUpdate::Column(SqlId column, SqlVal val) {
	set.Cat(SqlVal(SqlVal(column), " = ", val, SqlS::COMP));
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
#endif

END_UPP_NAMESPACE
