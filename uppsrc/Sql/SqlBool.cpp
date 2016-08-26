#include "Sql.h"

namespace Upp {

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

SqlBool IsSame(const SqlVal& a, const SqlVal& b)
{
	return SqlBool(a, SqlCode(MY_SQL, "<=>")(" is not distinct from "), b, SqlS::COMP);
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
	return SqlRowNum() == 1;
}

SqlBool Like(const SqlVal& a, const SqlVal& b, bool cs) {
	return SqlBool(a, SqlCode
			(MY_SQL, cs ? " like binary " : " like ")
			(PGSQL, cs ? " like " : " ilike ")
			(" like "),	b, SqlS::COMP);
}

SqlBool LikeUpperAscii(const SqlVal& a, const SqlVal& b) {
	return Like(UpperAscii(a), UpperAscii(b));
}

SqlBool NotLike(const SqlVal& a, const SqlVal& b, bool cs) {
	return SqlBool(a, SqlCode
			(PGSQL, cs ? " not like " : " not ilike ")
			(" not like "), b, SqlS::COMP);
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
	return SqlId(~v1 + "(+)") == v2;
}

SqlBool RightJoin(SqlVal v1, SqlVal v2) {
	return v1 == SqlId(~v2 + "(+)");
}

SqlBool Join(SqlId tab1, SqlId tab2, SqlId key) {
	return key.Of(tab1) == key.Of(tab2);
}

SqlBool LeftJoin(SqlId tab1, SqlId tab2, SqlId key) {
	return SqlId(key.Of(tab1).Quoted() + "(+)") == key.Of(tab2).Quoted();
}

SqlBool RightJoin(SqlId tab1, SqlId tab2, SqlId key) {
	return key.Of(tab1).Quoted() == SqlId(key.Of(tab2).Quoted() + "(+)");
}

}
