#include "SqlExp.h"

SqlCol SqlCol::As(const char *as) const
{
	return name + " " + as;
}

SqlCol SqlId::Of(SqlId id) const
{
	return id.IsNull() ? ToString() : id.ToString() + '.' + ToString();
}

SqlCol SqlId::As(const char *as) const
{
	return id.IsNull() ? ToString() : ToString() + " " + as;
}

SqlId SqlId::operator [] (int i) const
{
	return SqlId(ToString() + FormatInt(i));
}

SqlId SqlId::operator&(const SqlId& s) const
{
	return SqlId(ToString() + "$" + s.ToString());
}

String SqlS::operator()() const
{
	return '(' + text + ')';
}

String SqlS::operator()(int at) const
{
	return at > priority ? operator()() : text;
}

SqlS::SqlS(const SqlS& a, const char *o, const SqlS& b, int pr, int prb) {
	text = a(pr) + o + b(prb);
	priority = pr;
}

SqlS::SqlS(const SqlS& a, const char *o, const SqlS& b, int pr) {
	text = a(pr) + o + b(pr);
	priority = pr;
}

SqlVal::SqlVal(const String& x) {
	if(::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const char *s) {
	if(s && *s)
		SetHigh(SqlFormat(s));
	else
		SetNull();
}

SqlVal::SqlVal(int x) {
	if(::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(int64 x) {
	if(::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(double x) {
	if(::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Date x) {
	if(::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Time x) {
	if(::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Value& x) {
	if(::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Nuller&) {
	SetNull();
}

SqlVal::SqlVal(SqlId id) {
	SetHigh(id.ToString());
}

SqlVal::SqlVal(const SqlId& (*id)())
{
	SetHigh((*id)().ToString());
}

SqlVal::SqlVal(SqlCol id) {
	SetHigh(id.ToString());
}

SqlVal operator-(const SqlVal& a) {
	return SqlVal('-' + a(SqlS::UNARY), SqlS::UNARY);
}

SqlVal operator+(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " + ", b, SqlS::ADD);
}

SqlVal operator-(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a," - ", b, SqlS::ADD, SqlS::ADD + 1);
}

SqlVal operator*(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " * ", b, SqlS::MUL);
}

SqlVal operator/(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " / ", b, SqlS::MUL, SqlS::MUL + 1);
}

SqlVal operator%(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("mod", a, b);
}

SqlVal operator|(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, SqlCase(ORACLE, " || ")(" + "), b, SqlS::MUL);
}

SqlVal& operator+=(SqlVal& a, const SqlVal& b)     { return a = a + b; }
SqlVal& operator-=(SqlVal& a, const SqlVal& b)     { return a = a - b; }
SqlVal& operator*=(SqlVal& a, const SqlVal& b)     { return a = a * b; }
SqlVal& operator/=(SqlVal& a, const SqlVal& b)     { return a = a / b; }
SqlVal& operator%=(SqlVal& a, const SqlVal& b)     { return a = a % b; }
SqlVal& operator|=(SqlVal& a, const SqlVal& b)     { return a = a | b; }

SqlVal SqlFunc(const char *name, const SqlVal& a) {
	return SqlVal(name + a(), SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b) {
	return SqlVal(String(n) + '(' + ~a + ", " + ~b + ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c) {
	return SqlVal(String(n) + '(' + ~a + ", " + ~b + ", " + ~c + ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d) {
	return SqlVal(String(n) + '(' + ~a + ", " + ~b + ", " + ~c + ", " + ~d + ')', SqlS::FN);
}

SqlVal SqlFunc(const char *name, const SqlSet& set) {
	return SqlVal(name + set(), SqlS::FN);
}

SqlVal Decode(const SqlVal& exp, const SqlSet& variants) {
	ASSERT(!variants.IsEmpty());
	return SqlVal("decode("  + ~exp  + ", " + ~variants + ')', SqlS::FN);
}

SqlVal Distinct(const SqlVal& exp) {
	return SqlVal("distinct " + exp(SqlS::ADD), SqlS::UNARY);
}

SqlSet Distinct(const SqlSet& columns) {
	return SqlSet("distinct " + ~columns, SqlSet::SET);
}

SqlVal All(const SqlVal& exp) {
	return SqlVal("all " + exp(SqlS::ADD), SqlS::UNARY);
}

SqlSet All(const SqlSet& columns) {
	return SqlSet("all " + ~columns, SqlSet::SET);
}

SqlVal Count(const SqlVal& exp) {
	return SqlFunc("count", exp);
}

SqlVal SqlAll()
{
	return SqlCol("*");
}

SqlVal SqlCountRows()
{
	return Count(SqlAll());
}

SqlVal Descending(const SqlVal& exp) {
	return SqlVal(exp(SqlS::ADD) + " desc", SqlS::UNARY);
}

SqlVal SqlMax(const SqlVal& exp) {
	return SqlFunc("max", exp);
}

SqlVal SqlMin(const SqlVal& exp) {
	return SqlFunc("min", exp);
}

SqlVal SqlSum(const SqlVal& exp) {
	return SqlFunc("sum", exp);
}

SqlVal Avg(const SqlVal& a) {
	return SqlFunc("avg", a);
}

SqlVal Stddev(const SqlVal& a) {
	return SqlFunc("stddev", a);
}

SqlVal Variance(const SqlVal& a) {
	return SqlFunc("variance", a);
}

SqlVal Greatest(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("greatest", a, b);
}

SqlVal Least(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("least", a, b);
}

SqlVal ConvertCharset(const SqlVal& exp, const SqlVal& charset) { //TODO Dialect!
	if(exp.IsEmpty()) return exp;
	return exp.IsEmpty() ? exp : SqlFunc("convert", exp, charset);
}

SqlVal ConvertAscii(const SqlVal& exp) { //TODO Dialect!
	return ConvertCharset(exp, "US7ASCII");
}

SqlVal Upper(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : SqlFunc("upper", exp);
}

SqlVal Lower(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : SqlFunc("lower", exp);
}

SqlVal UpperAscii(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : Upper(ConvertAscii(exp));
}

SqlVal Substr(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("SUBSTR", a, b);
}

SqlVal Substr(const SqlVal& a, const SqlVal& b, const SqlVal& c)
{
	return SqlFunc("SUBSTR", a, b, c);
}

SqlVal Instr(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("INSTR", a, b);
}

SqlVal Wild(const char* s) {
	String result;
	for(char c; (c = *s++) != 0;)
		if(c == '*')
			result << '%';
		else if(c == '?')
			result << '_';
		else if(c == '.' && *s == 0)
			return result;
		else
			result << c;
	result << '%';
	return result;
}

SqlVal SqlDate(const SqlVal& year, const SqlVal& month, const SqlVal& day) {//TODO Dialect!
	return SqlFunc("to_date", year|"."|month|"."|day, "SYYYY.MM.DD");
}

SqlVal AddMonths(const SqlVal& date, const SqlVal& months) {//TODO Dialect!
	return SqlFunc("add_months", date, months);
}

SqlVal LastDay(const SqlVal& date) {//TODO Dialect!
	return SqlFunc("last_day", date);
}

SqlVal MonthsBetween(const SqlVal& date1, const SqlVal& date2) {//TODO Dialect!
	return SqlFunc("months_between", date1, date2);
}

SqlVal NextDay(const SqlVal& date) {//TODO Dialect!
	return SqlFunc("next_day", date);
}

SqlVal SqlNvl(const SqlVal& a, const SqlVal& b) {
	return SqlFunc(SqlCase
						(MYSQL|SQLITE3, "ifnull")
						(MSSQL, "isnull")
						("nvl"),
					a, b);
}

SqlVal SqlNvl(const SqlVal& a) {
	return Nvl(a, SqlVal(0));
}

SqlVal Prior(SqlId a) {
	return SqlVal("priority " + ~a, SqlS::UNARY);
}

SqlVal NextVal(SqlId a) {
	return SqlVal(~a + ".NEXTVAL", SqlS::HIGH);
}

SqlVal CurrVal(SqlId a) {
	return SqlVal(~a + ".CURRVAL", SqlS::HIGH);
}

SqlVal SqlRowNum()
{
	return SqlCol("ROWNUM");
}

SqlVal SqlArg() {
	return SqlCol("?");
}

SqlVal OuterJoin(SqlCol col)
{
	return SqlCol(~col + "(+)");
}

void SqlBool::SetTrue() {
	priority = TRUEVAL;
	text = "0=0";
}

void SqlBool::SetFalse() {
	priority = FALSEVAL;
	text = "0!=0";
}

void SqlBool::SetBool(bool b) {
	if(b) SetTrue();
	else  SetFalse();
}

SqlBool operator!(const SqlBool& a) {
	if(a.IsBool())  return !a.AsBool();
	return SqlBool(" not " + a(SqlS::UNARY), SqlS::UNARY);
}

SqlBool operator<(const SqlVal& a, const SqlVal& b) {
	return SqlBool(a, " < ", b, SqlS::COMP);
}

SqlBool operator>(const SqlVal& a, const SqlVal& b) {
	return SqlBool(a, " > ", b, SqlS::COMP);
}

SqlBool operator<=(const SqlVal& a, const SqlVal& b) {
	return SqlBool(a, " <= ", b, SqlS::COMP);
}

SqlBool operator>=(const SqlVal& a, const SqlVal& b) {
	return SqlBool(a, " >= ", b, SqlS::COMP);
}

SqlBool operator==(const SqlVal& a, const SqlVal& b) {
	return a.IsNull() ? b.IsNull() ? true : SqlIsNull(b)
		              : b.IsNull() ? SqlIsNull(a) : SqlBool(a, " = ", b, SqlS::COMP);
}

SqlBool operator!=(const SqlVal& a, const SqlVal& b) {
	return a.IsNull() ? b.IsNull() ? false : NotNull(b)
		              : b.IsNull() ? NotNull(a) : SqlBool(a, " != ", b, SqlS::COMP);

}

SqlBool operator&&(const SqlBool& a, const SqlBool& b) {
	if(a.IsEmpty() || a.IsTrue())  return b;
	if(b.IsEmpty() || b.IsTrue())  return a;
	if(a.IsFalse() || b.IsFalse()) return false;
	return SqlBool(a, " and ", b, SqlS::LAND);
}

SqlBool operator- (const SqlBool& a, const SqlBool& b) {
	if(a.IsFalse() || b.IsTrue() || b.IsEmpty()) return SqlBool(false);
	if(b.IsFalse()) return a;
	return SqlBool(a, " and not ", SqlS(b(SqlS::COMP), SqlS::HIGH), SqlS::LAND);
}

SqlBool operator||(const SqlBool& a, const SqlBool& b) {
	if(a.IsEmpty() || a.IsFalse()) return b;
	if(b.IsEmpty() || b.IsFalse()) return a;
	if(a.IsTrue() || b.IsTrue())   return true;
	return SqlBool(a, " or ", b, SqlS::LOR);
}

SqlBool operator|=(SqlBool& a, const SqlBool& b) { return a = a || b; }

SqlBool operator&=(SqlBool& a, const SqlBool& b) { return a = a && b; }

SqlBool SqlIsNull(const SqlVal& a) {
	return a.IsNull() ? true : SqlBool(a(SqlS::COMP) + " is NULL", SqlS::COMP);
}

SqlBool NotNull(const SqlVal& a) {
	return a.IsNull() ? false : SqlBool(a(SqlS::COMP) + " is not NULL", SqlS::COMP);
}

SqlBool SqlFirstRow() {
	return SqlCol("ROWNUM") == 1;
}

SqlBool Like(const SqlVal& a, const SqlVal& b) {
	return SqlBool(a, SqlCase(MYSQL, " like binary ")(" like "), b, SqlS::COMP);
}

SqlBool LikeUpperAscii(const SqlVal& a, const SqlVal& b) {
	return Like(UpperAscii(a), UpperAscii(b));
}

SqlBool NotLike(const SqlVal& a, const SqlVal& b) {
	return SqlBool(a, " not like ", b, SqlS::COMP);
}

SqlBool Between(const SqlVal& a, const SqlVal& l, const SqlVal& h) {
	return SqlBool(a(SqlS::COMP) + " between " + l(SqlS::COMP) + " and " + h(SqlS::COMP),
		           SqlS::COMP);
}

SqlBool NotBetween(const SqlVal& a, const SqlVal& l, const SqlVal& h) {
	return SqlBool(a(SqlS::COMP) + " not between " + l(SqlS::COMP) + " and " + h(SqlS::COMP),
		           SqlS::COMP);
}

SqlBool In(const SqlVal& a, const SqlSet& b) {
	if(a.IsNull() || b.IsEmpty()) return false;
	return SqlBool(a(SqlS::COMP) + " in " + b(), SqlS::COMP);
}

SqlBool NotIn(const SqlVal& a, const SqlSet& b) {
	if(a.IsNull() || b.IsEmpty()) return true;
	return SqlBool(a(SqlS::COMP) + " not in " + b(), SqlS::COMP);
}

SqlBool Exists(const SqlSet& set) {
	return SqlBool("exists " + set(), SqlS::COMP);
}

SqlBool NotExists(const SqlSet& set) {
	return SqlBool("not exists " + set(), SqlS::COMP);
}

SqlBool LeftJoin(SqlVal v1, SqlVal v2) {
	return SqlCol(~v1 + "(+)") == v2;
}

SqlBool RightJoin(SqlVal v1, SqlVal v2) {
	return v1 == SqlCol(~v2 + "(+)");
}

SqlBool Join(SqlId tab1, SqlId tab2, SqlId key) {
	return key.Of(tab1) == key.Of(tab2);
}

SqlBool LeftJoin(SqlId tab1, SqlId tab2, SqlId key) {
	return SqlCol(~key.Of(tab1) + "(+)") == key.Of(tab2);
}

SqlBool RightJoin(SqlId tab1, SqlId tab2, SqlId key) {
	return key.Of(tab1) == SqlCol(~key.Of(tab2) + "(+)");
}

SqlSet operator|(const SqlSet& set1, const SqlSet& set2) {
	if(set1.IsEmpty()) return set2;
	if(set2.IsEmpty()) return set1;
	return SqlSet(set1() + " union " + set2(), SqlSet::SETOP);
}

SqlSet operator&(const SqlSet& set1, const SqlSet& set2) {
	if(set1.IsEmpty()) return set2;
	if(set2.IsEmpty()) return set1;
	return SqlSet(set1() + " intersect " + set2(), SqlSet::SETOP);
}

SqlSet operator-(const SqlSet& set1, const SqlSet& set2) {
	if(set1.IsEmpty() || set2.IsEmpty())
		return set1;
	return SqlSet(set1() + " minus " + set2(), SqlSet::SETOP);
}

String SqlSet::operator~() const {
	return IsEmpty() ? "null" : text;
}

String SqlSet::operator()() const {
	return IsEmpty() ? "null" : '(' + text + ')';
}

String SqlSet::operator()(int at) const {
	if(IsEmpty()) return "null";
	return at > priority ? '(' + text + ')' : text;
}

SqlSet& SqlSet::Cat(const SqlVal& val) {
	if(!IsEmpty()) text.Cat(", ");
	text.Cat(~val);
	priority = SET;
	return *this;
}

SqlSet& SqlSet::Cat(const SqlSet& set) {
	if(set.IsEmpty()) return *this;
	if(!IsEmpty()) text.Cat(", ");
	text.Cat(set.text);
	priority = SET;
	return *this;
}

SqlSet& SqlSet::Where(const SqlBool& exp) {
	if(!exp.IsTrue() && !exp.IsEmpty())
		text << " where " << ~exp;
	return *this;
}

SqlSet& SqlSet::StartWith(const SqlBool& exp) {
	text << " start with " << ~exp;
	return *this;
}

SqlSet& SqlSet::ConnectBy(const SqlBool& exp) {
	text << " connect by " << ~exp;
	return *this;
}

SqlSet& SqlSet::GroupBy(const SqlSet& set) {
	text << " group by " << ~set;
	return *this;
}

SqlSet& SqlSet::Having(const SqlBool& exp) {
	text << " having " << ~exp;
	return *this;
}

SqlSet& SqlSet::OrderBy(const SqlSet& set) {
	text << " order by " << ~set;
	return *this;
}

SqlSet& SqlSet::ForUpdate() {
	text << " for update";
	return *this;
}

SqlSet& SqlSet::NoWait() {
	text << " nowait";
	return *this;
}

SqlSet& SqlSet::Limit(int limit) {
	text << " limit " << limit;
	return *this;
}

SqlSet& SqlSet::Limit(int64 offset, int limit) {
	text << " limit " << offset << ", " << limit;
	return *this;
}

SqlSet& SqlSet::Offset(int64 offset) {
	text << " offset " << offset;
	return *this;
}

SqlSet::SqlSet(const SqlVal& p0) {
	Cat(p0);
	priority = SET;
}

static inline void sCat(SqlSet& s, SqlVal v) { s.Cat(v); }

#define E__Cat(I)       sCat(*this, p##I)

#define E__SqlSetF(I) \
SqlSet::SqlSet(const SqlVal& p0, __List##I(E__SqlVal)) { \
	Cat(p0); \
	__List##I(E__Cat); \
	priority = SET; \
}
__Expand(E__SqlSetF);

FieldOperator::FieldOperator() {}
FieldOperator::~FieldOperator() {}

struct FieldSqlSet : FieldOperator {
	SqlSet *set;

	virtual void Field(const char *name, Ref) {
		set->Cat(SqlCol(name));
	}
};

SqlSet::SqlSet(Fields nfields) {
	FieldSqlSet fo;
	fo.set = this;
	nfields(fo);
	priority = SET;
}

SqlSelect& SqlSelect::Hint(const char *hint)
{
	set = SqlSet("/*+ " + String(hint) + " */ "+ ~set, SqlSet::SETOP);
	return *this;
}

SqlSet SqlSelect::From(const SqlSet& table) {
	return SqlSet("select " + ~set + " from " + table(SqlSet::SETOP + 1), SqlSet::SETOP);
}

SqlSet SqlSelect::From(SqlId table) {
	return SqlSet("select " + ~set + " from " + ~table, SqlSet::SETOP);
}

SqlSet SqlSelect::From(SqlId table1, SqlId table2) {
	return SqlSet("select " + ~set + " from " + ~table1 + ", " + ~table2, SqlSet::SETOP);
}

SqlSet SqlSelect::From(SqlId table1, SqlId table2, SqlId table3) {
	return SqlSet("select " + ~set + " from " + ~table1 + ", " + ~table2 + ", " + ~table3,
		          SqlSet::SETOP);
}

SqlSelect Select(const SqlSet& set) {
	return SqlSelect(set);
}

#define E__SCat(I)       sCat(set, p##I)

#define E__QSelectF(I) \
SqlSelect Select(__List##I(E__SqlVal)) { \
	SqlSet set; \
	__List##I(E__SCat); \
	return Select(set); \
}
__Expand(E__QSelectF);

SqlSet Delete(SqlVal table) {
	return SqlSet("delete from " + ~table, SqlSet::SETOP);
}

void SqlInsert::Column(SqlId column, SqlVal val) {
	set1.Cat(column);
	set2.Cat(val);
	if(keycolumn.IsNull()) keycolumn = column;
	if(keyvalue.IsEmpty()) keyvalue =val;
}

SqlSet SqlInsert::Get() const {
	return SqlSet("insert into " + ~table + set1() + " values "  + set2(), SqlSet::SETOP);
}

SqlSet SqlInsert::From(SqlSet from) const {
	return SqlSet("insert into " + ~table + set1() + ' ' + ~Select(set2).From(from),
		          SqlSet::SETOP);
}

SqlSet SqlInsert::From(SqlId from) const {
	return From(SqlSet(from));
}

struct InsertFieldOperator : public FieldOperator {
	SqlInsert *insert;

	virtual void Field(const char *name, Ref f)
	{
		insert->Column(name, (Value)f);
	}
};

SqlInsert::SqlInsert(Fields f)
{
	InsertFieldOperator ifo;
	ifo.insert = this;
	f(ifo);
	table = ifo.table;
}

void SqlUpdate::Column(SqlId column, SqlVal val) {
	set.Cat(SqlVal(SqlVal(column), " = ", val, SqlS::COMP));
}

SqlSet SqlUpdate::Where(SqlBool where) {
	return SqlSet("update " + ~table + " set " + ~set, SqlSet::SETOP)
	       .Where(where);
}
