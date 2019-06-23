#include "Sql.h"

namespace Upp {

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
	q << SqlCode(SQLITE3, "")("((")
	  << text
	  << SqlCode(SQLITE3, "")(")")
	  << op
	  << SqlCode(SQLITE3, "")("(")
	  << s2.text
	  << SqlCode(SQLITE3, "")("))")
	;
	text = q;
	return *this;
}

SqlSelect& SqlSelect::operator|=(const SqlSelect& s2) {
	return SetOp(s2, " union ");
}

SqlSelect& SqlSelect::operator+=(const SqlSelect& s2)
{
	return SetOp(s2, " union all ");
}

SqlSelect& SqlSelect::operator&=(const SqlSelect& s2) {
	return SetOp(s2, " intersect ");
}

SqlSelect& SqlSelect::operator-=(const SqlSelect& s2) {
	return SetOp(s2, SqlCode(MSSQL|PGSQL|SQLITE3," except ")(" minus "));
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

SqlSelect operator+(const SqlSelect& s1, const SqlSelect& s2)
{
	SqlSelect s = s1;
	s += s2;
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
	text << SqlCode(SQLITE3, "")(" for update");
 	return *this;
}
 
SqlSelect& SqlSelect::NoWait() {
	text << " nowait";
	return *this;
}

SqlSelect& SqlSelect::Limit(int limit) {
	ASSERT(text.StartsWith("select "));
	String s = AsString(limit);
	text.Insert(6, SqlCode(MSSQL, " top " + s)());
	text << SqlCode(MSSQL, "")(" limit " + s);
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
	text = "select " + text + SqlCode(ORACLE, " from DUAL")("");
	valid = true;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlSet& table) {
	if(table.IsEmpty())
		return Get();
	String ts = table(SqlSet::SETOP + 1);
	text = "select " + text + " from " + ts;
	valid = true;
	tables << ',' << Filter(ts, CharFilterNotWhitespace);
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table) {
	const String& t1 = table.ToString();
	text = String().Cat() << "select " << text << " from \t" << t1 << '\t';
	valid = true;
	tables << ',' << t1;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table1, const SqlId& table2) {
	const String& t1 = table1.ToString();
	const String& t2 = table2.ToString();
	text = String().Cat() << "select " << text << " from \t" << t1 << "\t, \t" << t2 << '\t';
	valid = true;
	tables << ',' << t1 << ',' << t2;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table1, const SqlId& table2, const SqlId& table3) {
	const String& t1 = table1.ToString();
	const String& t2 = table2.ToString();
	const String& t3 = table3.ToString();
	text = String().Cat() << "select " << text << " from \t" << t1 << "\t, \t" << t2 << "\t, \t" << t3 << '\t';
	valid = true;
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
	t << SqlCode(MSSQL|PGSQL|SQLITE3, "")("(")
	  << "(" << text << ")" << SqlCode(ORACLE,"")(" as") << " \t" << tab.ToString() << '\t'
	  << SqlCode(MSSQL|PGSQL|SQLITE3, "")(")");
	return SqlSet(String(t), SqlSet::HIGH);
}

SqlSelect::SqlSelect(Fields f)
{
	valid = on = false;
	SqlSet set(f);
	text = ~set;
}

//$-
#define E__SCat(I)       set.Cat(p##I)

#define E__QSqlSelectF(I) \
SqlSelect::SqlSelect(__List##I(E__SqlVal)) { \
	valid = on = false; \
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
//$+

// -------------------------------

SqlWith& SqlWith::With(SqlId table)
{
	text << (text.GetCount() ? ", " : "with ") << table.Quoted();
	args = false;
	return *this;
}

SqlWith& SqlWith::WithRecursive(SqlId table)
{
	text << (text.GetCount() ? ", " : "with ")
	     << SqlCode(MSSQL, "")("recursive ")
	     << table.Quoted();
	args = false;
	return *this;
}

SqlWith& SqlWith::Arg(SqlId arg)
{
	text << (args ? ", " : "(") << arg.Quoted();
	args = true;
	return *this;
}

SqlWith& SqlWith::As(const SqlSelect& select)
{
	if(args)
		text << ')';
	text << " as (" + SqlStatement(select).GetText() + ")";
	args = false;
	return *this;
}

SqlSelect SqlWith::operator()(const SqlSelect& select)
{
	SqlSet set;
	set.SetRaw(text + " " + SqlStatement(select).GetText());
	return set;
}

SqlStatement SqlWith::operator()(const SqlInsert& insert)
{
	return SqlStatement(text + " " + SqlStatement(insert).GetText());
}

SqlStatement SqlWith::operator()(const SqlUpdate& update)
{
	return SqlStatement(text + " " + SqlStatement(update).GetText());
}

SqlStatement SqlWith::operator()(const SqlDelete& deletes)
{
	return SqlStatement(text + " " + SqlStatement(deletes).GetText());
}

// -------------------------------

SqlStatement SqlCreateTable::As(const SqlSelect& select)
{
	String text = "create ";
	if(!permanent)
		text << SqlCode(ORACLE, "global temporary ")("temporary ");
	text << "table " << table.Quoted();
	if(!permanent){
		if(transaction)
			text << SqlCode(ORACLE, " on commit delete rows")("");
		else
			text << SqlCode(ORACLE, " on commit preserve rows")("");
	}
	text << " as (" + SqlStatement(select).GetText() + ")";
	return SqlStatement(text);
}

// -------------------------------

SqlDelete::SqlDelete(SqlVal table) {
	text = "delete from " + ~table;
}

SqlDelete& SqlDelete::Where(const SqlBool& b) {
	text << " where " << ~b;
	return *this;
}

SqlDelete& SqlDelete::Returning(const SqlSet& set) {
	text << " returning " + ~set;
	return *this;
}

// -------------------------------
#ifdef NEWINSERTUPDATE

void SqlInsert::Column(const SqlId& column, SqlVal val) {
	set1.Cat(column);
	set2.Cat(val);
	if(keycolumn.IsNull()) keycolumn = column;
	if(keyvalue.IsEmpty()) keyvalue =val;
	sel.Set(set2);
}

SqlInsert::operator SqlStatement() const {
	String s = "insert into " + table.Quoted();
	if(!set1.IsEmpty()) {
		s << set1();
		if(sel.IsValid())
			s << ' ' << SqlStatement(sel).GetText();
		else
		if(!set2.IsEmpty())
			s << " values " << set2();
	}
	if(!ret.IsEmpty())
		s << " returning " << ~ret;
	return SqlStatement(s);
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

SqlInsert& SqlInsert::Where(const SqlBool& w)
{
	if(!sel.IsValid())
		From();
	sel.Where(w);
	return *this;
}

SqlInsert& SqlInsert::Returning(const SqlSet& set)
{
	ret = set;
	return *this;
}

SqlInsert& SqlInsert::operator()(const ValueMap& data)
{
	for(int i = 0; i < data.GetCount(); i++)
		operator()((String)data.GetKey(i), data.GetValue(i));
	return *this;
}

#else

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
		if(from.IsEmpty() && where.IsEmpty() && groupby.IsEmpty()) {
			if(!set2.IsEmpty())
				s << " values " << set2();
		}
		else {
			SqlSelect sel;
			sel = Select(set2).From(from).Where(where);
			if(!groupby.IsEmpty())
				sel.GroupBy(groupby);
			if(!having.IsEmpty())
				sel.Having(having);
			s << ' ' + SqlStatement(sel).GetText();
		}
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

SqlInsert& SqlInsert::operator()(const ValueMap& data)
{
	for(int i = 0; i < data.GetCount(); i++)
		operator()((String)data.GetKey(i), data.GetValue(i));
	return *this;
}

#endif

/////////////////////////////////

#ifdef NEWINSERTUPDATE

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
	sel.Set(SqlSet(SqlId("X")));
}

SqlUpdate::SqlUpdate(const SqlId& table)
:	table(table)
{
	sel.Set(SqlSet(SqlId("X")));
}

SqlUpdate& SqlUpdate::operator()(Fields f) {
	UpdateFieldOperator ufo;
	ufo.update = this;
	f(ufo);
	return *this;
}

SqlUpdate& SqlUpdate::operator()(const ValueMap& data)
{
	for(int i = 0; i < data.GetCount(); i++)
		operator()((String)data.GetKey(i), data.GetValue(i));
	return *this;
}

SqlUpdate& SqlUpdate::Where(SqlBool w)
{
	if(sel.IsValid())
		sel.Where(w);
	else
		where = w;
	return *this;
}

SqlUpdate& SqlUpdate::Returning(const SqlSet& set)
{
	ret = set;
	return *this;
}

SqlUpdate::operator SqlStatement() const {
	StringBuffer stmt;
	stmt << "update " << table.Quoted() << " set " << ~set;
	if(sel.IsValid())
		stmt << SqlStatement(sel).GetText().Mid(9 + 2 * SqlId::IsUseQuotes());
	else
	if(!where.IsEmpty())
		stmt << " where " << ~where;
	if(!ret.IsEmpty())
		stmt << " returning " << ~ret;
	return SqlStatement(stmt);
}

void SqlUpdate::Column(const SqlId& column, SqlVal val) {
	set.Cat(SqlVal(SqlVal(column), " = ", val, SqlS::COMP));
}

void SqlUpdate::Column(const SqlSet& cols, const SqlSet& val)
{
	set.Cat(SqlVal(SqlS(cols(), SqlS::HIGH), " = ", SqlS(val(), SqlS::HIGH), SqlS::COMP));
}

#else
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

SqlUpdate& SqlUpdate::operator()(const ValueMap& data)
{
	for(int i = 0; i < data.GetCount(); i++)
		operator()((String)data.GetKey(i), data.GetValue(i));
	return *this;
}

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
#endif
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

}
