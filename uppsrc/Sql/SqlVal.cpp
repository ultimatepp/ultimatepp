#include "Sql.h"

NAMESPACE_UPP

SqlId SqlId::Of(const char *of) const
{
	return String().Cat() << of << (char)SQLC_OF << ToString();
}

SqlId SqlId::Of(SqlId id) const
{
	return id.IsNull() ? ToString() : String().Cat() << id.ToString() << (char)SQLC_OF << ToString();
}

SqlId SqlId::operator[](const SqlId& id) const
{
	return Of(id);
}

SqlId SqlId::As(const char *as) const
{
	return id.IsNull() ? ToString() : String().Cat() << ToString() << (char)SQLC_AS << as;
}

SqlId SqlId::operator()(SqlId p) const
{
	String x;
	PutOf0(x, p);
	return x;
}

SqlId SqlId::operator [] (int i) const
{
	return ToString() + FormatInt(i);
}

SqlId SqlId::operator&(const SqlId& s) const
{
	return ToString() + "$" + s.ToString();
}

String SqlS::operator()() const
{
	return String().Cat() << '(' << text << ')';
}

String SqlS::operator()(int at) const
{
	return at > priority ? operator()() : text;
}

String SqlS::operator()(int at, byte cond) const
{
	if(at <= priority)
		return text;
	StringBuffer out;
	out << SqlCase(cond, "(")() << text << SqlCase(cond, ")")();
	return out;
}

void SqlS::Init(const SqlS& a, const char *o, int olen, const SqlS& b, int pr, int prb)
{
	StringBuffer s;
	if(a.priority < pr) {
		s.Cat('(');
		s.Cat(~a);
		s.Cat(')');
	}
	else
		s.Cat(~a);
	s.Cat(o, olen);
	if(b.priority < prb) {
		s.Cat('(');
		s.Cat(~b);
		s.Cat(')');
	}
	else
		s.Cat(~b);
	text = s;
	priority = pr;
}

SqlVal SqlVal::As(const char *as) const {
	SqlVal v;
	v.SetHigh(String().Cat() << text << (char)SQLC_AS << '\t' << as << '\t');
	return v;
}

SqlVal SqlVal::As(const SqlId& id) const {
	return As(~id.ToString());
}

SqlVal::SqlVal(const String& x) {
	if(UPP::IsNull(x))
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
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(int64 x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(double x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Date x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Time x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Value& x) {
	if(UPP::IsNull(x))
		SetNull();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Nuller&) {
	SetNull();
}

SqlVal::SqlVal(const SqlId& id) {
	SetHigh(id.Quoted());
}

SqlVal::SqlVal(const SqlId& (*id)())
{
	SetHigh((*id)().Quoted());
}

SqlVal::SqlVal(const Case& x) {
	SetHigh(~x);
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
	return SqlVal(a, SqlCase(ORACLE|PGSQL|SQLITE3, " || ")(" + "), b, SqlS::MUL);		// Added (SQLITE3, " || ")
}

SqlVal& operator+=(SqlVal& a, const SqlVal& b)     { return a = a + b; }
SqlVal& operator-=(SqlVal& a, const SqlVal& b)     { return a = a - b; }
SqlVal& operator*=(SqlVal& a, const SqlVal& b)     { return a = a * b; }
SqlVal& operator/=(SqlVal& a, const SqlVal& b)     { return a = a / b; }
SqlVal& operator%=(SqlVal& a, const SqlVal& b)     { return a = a % b; }
SqlVal& operator|=(SqlVal& a, const SqlVal& b)     { return a = a | b; }

SqlVal SqlFunc(const char *name, const SqlVal& a) {
	return SqlVal(String().Cat() << name << '(' << ~a << ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b) {
	return SqlVal(String(n).Cat() << '(' + ~a << ", " << ~b << ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c) {
	return SqlVal(String(n).Cat() << '(' << ~a << ", " << ~b << ", " << ~c << ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d) {
	return SqlVal(String(n).Cat() << '(' << ~a << ", " << ~b << ", " << ~c << ", " << ~d << ')', SqlS::FN);
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

SqlVal Count(const SqlVal& exp)
{
	return SqlFunc("count", exp);
}

SqlVal Count(const SqlSet& exp)
{
	return SqlFunc("count", exp);
}

SqlId SqlAll()
{
	return SqlId("*");
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
	return SqlFunc(SqlCase
						(SQLITE3, "max")
						("greatest"),
					a, b);
}

SqlVal Least(const SqlVal& a, const SqlVal& b) {
	return SqlFunc(SqlCase
						(SQLITE3, "min")
						("least"),
					a, b);
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

SqlVal Length(const SqlVal& exp) {
	return exp.IsEmpty() ? exp : SqlFunc("length", exp);
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

SqlVal Cast(const char* type, const SqlId& a) {
	return SqlFunc(type, a);
}

SqlVal SqlNvl(const SqlVal& a, const SqlVal& b) {
	return SqlFunc(SqlCase
						(PGSQL, "coalesce")
						(MY_SQL|SQLITE3, "ifnull")
						(MSSQL, "isnull")
						("nvl"),
					a, b);
}

SqlVal SqlNvl(const SqlVal& a) {
	return Nvl(a, SqlVal(0));
}

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2) {
	return SqlFunc("coalesce", exp1, exp2);
}

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3) {
	return SqlFunc("coalesce", exp1, exp2, exp3);
}

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3, const SqlVal& exp4) {
	return SqlFunc("coalesce", exp1, exp2, exp3, exp4);
}

SqlVal Prior(const SqlId& a) {
	return SqlVal("prior " + a.Quoted(), SqlS::UNARY);
}

SqlVal NextVal(const SqlId& a) {
	return SqlVal(SqlCase
	                 (PGSQL, "nextval('" + a.ToString() + "')")
	                 (a.Quoted() + ".NEXTVAL")
	              , SqlS::HIGH);
}

SqlVal CurrVal(const SqlId& a) {
	return SqlVal(SqlCase
				    (PGSQL, "currval('" + a.ToString() + "')")
				    (a.Quoted() + ".CURRVAL")
				  , SqlS::HIGH);
}

SqlVal SqlTxt(const char *s)
{
	SqlVal v;
	v.SetHigh(s);
	return v;
}

SqlVal SqlRowNum()
{
	return SqlTxt("ROWNUM");
}

SqlVal SqlArg() {
	return SqlTxt("?");
}

SqlVal OuterJoin(const SqlId& col)
{
	return SqlId(col.Quoted() + "(+)");
}

SqlVal SqlBinary(const char *s, int l)
{
	SqlVal x;
	x.SetHigh(SqlFormatBinary(s, l));
	return x;
}

SqlVal SqlBinary(const String& data)
{
	return SqlBinary(~data, data.GetCount());
}

END_UPP_NAMESPACE
