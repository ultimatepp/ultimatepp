#include "Sql.h"

NAMESPACE_UPP

String SqlStatement::Get(int dialect) const {
	ASSERT(dialect == ORACLE || dialect == SQLITE3 || dialect == MY_SQL || dialect == MSSQL ||
	       dialect == PGSQL || dialect == FIREBIRD || dialect == DB2);
	return SqlCompile(dialect, text);
}

#ifndef flagNOAPPSQL
String SqlStatement::Get() const
{
	return Get(SQL.GetDialect());
}
#endif

SqlSelect& SqlSelect::SetOp(const SqlSelect& s2, const char *op)
{
	String q;
	q << SqlCase(SQLITE3, "")("((")
	  << text
	  << SqlCase(SQLITE3, "")(")")
	  << op
	  << SqlCase(SQLITE3, "")("(")
	  << s2.text
	  << SqlCase(SQLITE3, "")("))")
	;
	text = q;
	return *this;
}

SqlSelect& SqlSelect::operator|=(const SqlSelect& s2) {
	return SetOp(s2, " union ");
}

SqlSelect& SqlSelect::operator&=(const SqlSelect& s2) {
	return SetOp(s2, " intersect ");
}

SqlSelect& SqlSelect::operator-=(const SqlSelect& s2) {
	return SetOp(s2, SqlCase(MSSQL|PGSQL|SQLITE3," except ")(" minus "));
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
	text = "select " + text + SqlCase(ORACLE, " from DUAL")("");
	return *this;
}

SqlSelect& SqlSelect::From(const SqlSet& table) {
	String ts = table(SqlSet::SETOP + 1);
	text = "select " + text + " from " + ts;
	tables << ',' << Filter(ts, CharFilterNotWhitespace);
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table) {
	const String& t1 = table.ToString();
	text = String().Cat() << "select " << text << " from \t" << t1 << '\t';
	tables << ',' << t1;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table1, const SqlId& table2) {
	const String& t1 = table1.ToString();
	const String& t2 = table2.ToString();
	text = String().Cat() << "select " << text << " from \t" << t1 << "\t, \t" << t2 << '\t';
	tables << ',' << t1 << ',' << t2;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table1, const SqlId& table2, const SqlId& table3) {
	const String& t1 = table1.ToString();
	const String& t2 = table2.ToString();
	const String& t3 = table3.ToString();
	text = String().Cat() << "select " << text << " from \t" << t1 << "\t, \t" << t2 << "\t, \t" << t3 << '\t';
	tables << ',' << t1 << ',' << t2 << ',' << t3;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::InnerJoin0(const String& table) {
	text << " inner join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::LeftJoin0(const String& table) {
	text << " left outer join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::RightJoin0(const String& table) {
	text << " right outer join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::FullJoin0(const String& table) {
	text << " full outer join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::InnerJoinRef(const SqlId& table)
{
	InnerJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::LeftJoinRef(const SqlId& table)
{
	LeftJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::RightJoinRef(const SqlId& table)
{
	RightJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::FullJoinRef(const SqlId& table)
{
	FullJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::On(const SqlBool& exp) {
	if(!exp.IsTrue() && !exp.IsEmpty())
		text << (on ? " and " : " on ") << ~exp;
	return *this;
}

SqlVal SqlSelect::AsValue() const
{
	return SqlVal(String("(").Cat() << text << ")", SqlVal::LOW);
}

SqlSet SqlSelect::AsTable(const SqlId& tab) const
{
	StringBuffer t;
	t << SqlCase(MSSQL, "")("(")
	<< "(" << text << ")" << (char)SQLC_AS << "\t" << tab.ToString() << '\t'
	<< SqlCase(MSSQL, "")(")");
	return SqlSet(String(t), SqlSet::HIGH);
}

SqlSelect::SqlSelect(Fields f)
{
	on = false;
	SqlSet set(f);
	text = ~set;
}

#define E__SCat(I)       set.Cat(p##I)

#define E__QSqlSelectF(I) \
SqlSelect::SqlSelect(__List##I(E__SqlVal)) { \
	on = false; \
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

void SqlInsert::Column(const SqlId& column, SqlVal val) {
	set1.Cat(column);
	set2.Cat(val);
	if(keycolumn.IsNull()) keycolumn = column;
	if(keyvalue.IsEmpty()) keyvalue =val;
}

SqlInsert::operator SqlStatement() const {
	String s = "insert into " + table.Quoted();
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

SqlInsert& SqlInsert::From(const SqlId& from) {
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
	stmt << "update " << table.Quoted() << " set " << ~set;
	if(!where.IsEmpty())
		stmt << " where " << ~where;
	return SqlStatement(stmt);
}

void SqlUpdate::Column(const SqlId& column, SqlVal val) {
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

#endif

END_UPP_NAMESPACE
