#ifndef _SqlRxp_SqlExp_h_
#define _SqlRxp_SqlExp_h_

#include <Core/Core.h>

enum {
	ORACLE    = 1,
	SQLITE3   = 2,
	MYSQL     = 4,
	MSSQL     = 8,
	POSTGRESS = 16, // not implemented yet
	FIREBIRD  = 32, // not implemented yet
	DB2       = 64, // not implemented yet
};

class SqlCase {
	String s;

public:
	SqlCase(byte cond, const String& text);
	SqlCase operator()(byte cond, const String& text);

	String operator()(const String& text);
	String operator()();
};

String SqlCompile(byte dialect, const String& s);

String SqlFormat(int x);
String SqlFormat(double x);
String SqlFormat(int64 x);
String SqlFormat(const char *s, int len);
String SqlFormat(const char *s);
String SqlFormat(const String& x);
String SqlFormat(Date x);
String SqlFormat(Time x);
String SqlFormat(const Value& x);

class SqlCol : Moveable<SqlCol> {
	String name;
public:
	String        ToString() const          { return name; }
	const String& operator~() const         { return name; }

	SqlCol        As(const char *as) const;

	SqlCol(const String& name) : name(name) {}
};

class SqlId : Moveable<SqlId> {
protected:
	Id id;

public:
	bool          IsEqual(SqlId b) const     { return id == b.id; }
	bool          IsEqual(Id b) const        { return id == b; }
	bool          IsNull() const             { return id.IsNull(); }

	operator      Id() const                 { return id; }
	String        ToString() const           { return id.ToString(); }
	String        operator~() const          { return ToString(); }

	SqlCol        Of(SqlId id) const;
	SqlCol        As(const char *as) const;
	SqlCol        As(SqlId id) const         { return As(~~id); }
	SqlId         operator [] (int i) const;
	SqlId         operator&(const SqlId& s) const;

	SqlId()                                  {}
	SqlId(const char *s) : id(s)             {}
	SqlId(const String& s) : id(s)           {}
	SqlId(Id id) : id(id)                    {}
};

class SqlS : Moveable<SqlS> {
protected:
	String text;
	byte   priority;

public:
	enum PRIORITY {
		EMPTY    =   0,
		LOW      =  20,
		SETOP    =  50,
		SET      = 100,
		LOR      = 120,
		LAND     = 130,
		TRUEVAL  = 135,
		FALSEVAL = 136,
		COMP     = 140,
		ADD      = 150,
		MUL      = 160,
		UNARY    = 170,
		FN       = 180,
		HIGH     = 200,
		NULLVAL  = 250,
	};

	String        operator()() const;
	String        operator()(int at) const;
	const String& operator~() const         { return text; }
	bool          IsEmpty() const           { return priority == EMPTY; }

	SqlS()                                  : priority(EMPTY) {}
	SqlS(const char *s, int pr)             : text(s), priority(pr) {}
	SqlS(const String& s, int pr)           : text(s), priority(pr) {}
	SqlS(const SqlS& a, const char *op, const SqlS& b, int pr, int prb);
	SqlS(const SqlS& a, const char *op, const SqlS& b, int pr);
};

class SqlBool;
class SqlVal;
class SqlSet;

class SqlVal : public SqlS, Moveable<SqlVal> {
public:
	void SetNull()                          { text = "NULL"; priority = NULLVAL; }
	void SetHigh(const String& s)           { text = s; priority = HIGH; }
	bool IsNull() const                     { return priority == NULLVAL; }

	SqlVal()                                {}
	SqlVal(const String& s, int pr)         : SqlS(s, pr) {}
	SqlVal(const SqlS& a, const char *o, const SqlS& b, int pa, int pb)
		                                    : SqlS(a, o, b, pa, pb) {}
	SqlVal(const SqlS& a, const char *o, const SqlS& b, int p)
		                                    : SqlS(a, o, b, p) {}

	SqlVal(int x);
	SqlVal(int64 x);
	SqlVal(double x);
	SqlVal(const String& x);
	SqlVal(const char *x);
	SqlVal(Date x);
	SqlVal(Time x);
	SqlVal(const Value& x);
	SqlVal(const Nuller&);
	SqlVal(SqlId id);
	SqlVal(const SqlId& (*id)());
	SqlVal(SqlCol id);
};

SqlVal operator-(const SqlVal& a);
SqlVal operator+(const SqlVal& a, const SqlVal& b);
SqlVal operator-(const SqlVal& a, const SqlVal& b);
SqlVal operator*(const SqlVal& a, const SqlVal& b);
SqlVal operator/(const SqlVal& a, const SqlVal& b);
SqlVal operator%(const SqlVal& a, const SqlVal& b);
SqlVal operator|(const SqlVal& a, const SqlVal& b); // String concatenation

SqlVal& operator+=(SqlVal& a, const SqlVal& b);
SqlVal& operator-=(SqlVal& a, const SqlVal& b);
SqlVal& operator*=(SqlVal& a, const SqlVal& b);
SqlVal& operator/=(SqlVal& a, const SqlVal& b);
SqlVal& operator%=(SqlVal& a, const SqlVal& b);
SqlVal& operator|=(SqlVal& a, const SqlVal& b);

SqlVal SqlFunc(const char* name, const SqlVal& a);
SqlVal SqlFunc(const char* name, const SqlVal& a, const SqlVal& b);
SqlVal SqlFunc(const char* name, const SqlVal& a, const SqlVal& b, const SqlVal& c);
SqlVal SqlFunc(const char* nm, const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d);
SqlVal SqlFunc(const char* name, const SqlSet& set);

SqlVal Decode(const SqlVal& exp, const SqlSet& variants);
SqlVal Distinct(const SqlVal& exp);
SqlSet Distinct(const SqlSet& columns);
SqlVal All(const SqlVal& exp);
SqlSet All(const SqlSet& columns);
SqlVal SqlAll();
SqlVal Count(const SqlVal& exp);
SqlVal SqlCountRows();
SqlVal Descending(const SqlVal& exp);

SqlVal SqlMax(const SqlVal& a);
SqlVal SqlMin(const SqlVal& a);
SqlVal SqlSum(const SqlVal& a);

SqlVal Avg(const SqlVal& a);
SqlVal Stddev(const SqlVal& a);
SqlVal Variance(const SqlVal& a);

SqlVal Greatest(const SqlVal& a, const SqlVal& b);
SqlVal Least(const SqlVal& a, const SqlVal& b);

SqlVal ConvertCharset(const SqlVal& exp, const SqlVal& charset);
SqlVal ConvertAscii(const SqlVal& exp);
SqlVal Upper(const SqlVal& exp);
SqlVal Lower(const SqlVal& exp);
SqlVal UpperAscii(const SqlVal& exp);
SqlVal Substr(const SqlVal& a, const SqlVal& b);
SqlVal Substr(const SqlVal& a, const SqlVal& b, const SqlVal& c);
SqlVal Instr(const SqlVal& a, const SqlVal& b);
SqlVal Wild(const char* s);

SqlVal SqlDate(const SqlVal& year, const SqlVal& month, const SqlVal& day);
SqlVal AddMonths(const SqlVal& date, const SqlVal& months);
SqlVal LastDay(const SqlVal& date);
SqlVal MonthsBetween(const SqlVal& date1, const SqlVal& date2);
SqlVal NextDay(const SqlVal& date);

SqlVal SqlNvl(const SqlVal& a);
inline SqlVal Nvl(const SqlVal& a)     { return SqlNvl(SqlVal(a)); }
inline SqlVal Nvl(const SqlCol& a)     { return SqlNvl(SqlVal(a)); }
inline SqlVal Nvl(const SqlId& a)      { return SqlNvl(SqlVal(a)); }

SqlVal SqlNvl(const SqlVal& a, const SqlVal& b);
inline SqlVal Nvl(const SqlVal& a, const SqlVal& b) { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlVal& a, const SqlCol& b) { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlVal& a, const SqlId& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlCol& a, const SqlVal& b) { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlCol& a, const SqlCol& b) { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlCol& a, const SqlId& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlId& a, const SqlVal& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlId& a, const SqlCol& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlId& a, const SqlId& b)   { return SqlNvl(SqlVal(a), SqlVal(b)); }

SqlVal Prior(SqlId a);

SqlVal NextVal(SqlId a);
SqlVal CurrVal(SqlId a);
SqlVal OuterJoin(SqlCol col);

inline SqlVal operator++(SqlId a)           { return NextVal(a); }

SqlVal SqlRowNum();

SqlVal SqlArg();

class SqlBool : public SqlS, Moveable<SqlBool> {
public:
	void     SetTrue();
	void     SetFalse();
	void     SetBool(bool b);

	bool     IsTrue()  const                { return priority == TRUEVAL; }
	bool     IsFalse() const                { return priority == FALSEVAL; }
	bool     IsBool()  const                { return IsTrue() || IsFalse(); }
	bool     AsBool()  const                { ASSERT(IsBool()); return IsTrue(); }

	SqlBool(const String& s, int pr)        : SqlS(s, pr) {}
	SqlBool(const SqlS& a, const char *o, const SqlS& b, int pa, int pb)
		                                    : SqlS(a, o, b, pa, pb) {}
	SqlBool(const SqlS& a, const char *o, const SqlS& b, int p)
		                                    : SqlS(a, o, b, p) {}

	static SqlBool True()                   { return SqlBool(true); }
	static SqlBool False()                  { return SqlBool(false); }

	SqlBool()                               {}
	SqlBool(bool b)                         { SetBool(b); }
};

SqlBool        operator!(const SqlBool& a);

SqlBool        operator<(const SqlVal& a, const SqlVal& b);
SqlBool        operator<=(const SqlVal& a, const SqlVal& b);
SqlBool        operator>=(const SqlVal& a, const SqlVal& b);
SqlBool        operator>(const SqlVal& a, const SqlVal& b);
SqlBool        operator==(const SqlVal& a, const SqlVal& b);
SqlBool        operator!=(const SqlVal& a, const SqlVal& b);

SqlBool        operator||(const SqlBool& a, const SqlBool& b);
SqlBool        operator&&(const SqlBool& a, const SqlBool& b);
SqlBool        operator- (const SqlBool& a, const SqlBool& b);
SqlBool        operator|=(SqlBool& a, const SqlBool& b);
SqlBool        operator&=(SqlBool& a, const SqlBool& b);

SqlBool        SqlIsNull(const SqlVal& a);
SqlBool        NotNull(const SqlVal& a);

inline SqlBool IsNull(const SqlId& a)  { return SqlIsNull(a); }
inline SqlBool IsNull(const SqlVal& a) { return SqlIsNull(a); }
inline SqlBool IsNull(const SqlCol& a) { return SqlIsNull(a); }

SqlBool        Like(const SqlVal& a, const SqlVal& b);
SqlBool        NotLike(const SqlVal& a, const SqlVal& b);

SqlBool        Between(const SqlVal& a, const SqlVal& low, const SqlVal& high);
SqlBool        NotBetween(const SqlVal&a, const SqlVal& low, const SqlVal& high);

SqlBool        In(const SqlVal& a, const SqlSet& b);
SqlBool        NotIn(const SqlVal& a, const SqlSet& b);
SqlBool        Exists(const SqlSet& set);
SqlBool        NotExists(const SqlSet& set);

SqlBool        LikeUpperAscii(const SqlVal& a, const SqlVal& b);

SqlBool        LeftJoin(SqlVal v1, SqlVal v2);
SqlBool        RightJoin(SqlVal v1, SqlVal v2);

SqlBool        Join(SqlId tab1, SqlId tab2, SqlId key);
SqlBool        LeftJoin(SqlId tab1, SqlId tab2, SqlId key);
SqlBool        RightJoin(SqlId tab1, SqlId tab2, SqlId key);

SqlBool        SqlFirstRow();

inline SqlBool operator==(const SqlVal& a, const SqlSet& b) { return In(a, b); }
inline SqlBool operator!=(const SqlVal& a, const SqlSet& b) { return NotIn(a, b); }

inline const SqlVal& operator+(const SqlVal& a) { return a; }

//////////////////////////////////////////////////////////////////////

class Sql;

#define E__SqlVal(I)      const SqlVal& p##I

SqlSet operator|(const SqlSet& s1, const SqlSet& s2); // union
SqlSet operator&(const SqlSet& s1, const SqlSet& s2); // intersection
SqlSet operator-(const SqlSet& s1, const SqlSet& s2); // difference

class SqlSet : Moveable<SqlSet> {
protected:
	String text;
	byte   priority;

public:
	enum PRIORITY {
		SETOP    =  50,
		SET      = 100,
	};

	String           operator~() const;
	String           operator()() const;
	String           operator()(int at) const;
	bool             IsEmpty() const                { return text.IsEmpty(); }

	SqlSet&          Cat(const SqlVal& val); // adding a member
	SqlSet&          Cat(const SqlSet& set);
	SqlSet&          operator|=(const SqlVal& val)  { return Cat(val); }

	SqlSet&          operator|=(const SqlSet& set)  { return *this = *this | set; }
	SqlSet&          operator&=(const SqlSet& set)  { return *this = *this & set; }
	SqlSet&          operator-=(const SqlSet& set)  { return *this = *this - set; }

	SqlSet&          Where(const SqlBool& where);
	SqlSet&          StartWith(const SqlBool& exp);
	SqlSet&          ConnectBy(const SqlBool& exp);
	SqlSet&          GroupBy(const SqlSet& columnset);
	SqlSet&          Having(const SqlBool& exp);
	SqlSet&          OrderBy(const SqlSet& set);
	SqlSet&          ForUpdate();
	SqlSet&          NoWait();

	SqlSet&          GroupBy(SqlVal column)                 { return GroupBy(SqlSet(column)); }
	SqlSet&          OrderBy(SqlVal val)                    { return OrderBy(SqlSet(val)); }
	SqlSet&          OrderBy(SqlVal a, SqlVal b)            { return OrderBy(SqlSet(a, b)); }
	SqlSet&          OrderBy(SqlVal a, SqlVal b, SqlVal c)  { return OrderBy(SqlSet(a, b, c)); }
	SqlSet&          Limit(int limit);
	SqlSet&          Limit(int64 offset, int limit);
	SqlSet&          Offset(int64 offset);

	void             Clear()                                { text.Clear(); }

	SqlSet() {}
	explicit SqlSet(const SqlVal& p0);

#define E__SqlSet(I)   SqlSet(const SqlVal& p0, __List##I(E__SqlVal));
	__Expand(E__SqlSet);

//	explicit SqlSet(Fields nfields);

	SqlSet(const String& s, PRIORITY p)   { text = s; priority = p; }

private:
	SqlSet&          StartWith(const char *cond);
	SqlSet&          Where(const char *cond);
	SqlSet&          ConnectBy(const char *cond);
	SqlSet&          GroupBy(const char *column_list);
	SqlSet&          Having(const char *cond);
	SqlSet&          OrderBy(const char *column_list);
};

class SqlSetC : public SqlSet {
public:
	SqlSetC(const String& s)      { text = s; priority = SET; }
	SqlSetC(const char* s)        { text = s; priority = SET; }
};

class SqlSelect {
	SqlSet set;

public:
	operator bool() const                       { return !set.IsEmpty(); }

	SqlSelect& Hint(const char *hint);

	SqlSet From(const SqlSet& set);
	SqlSet From(SqlId table);
	SqlSet From(SqlId table1, SqlId table2);
	SqlSet From(SqlId table1, SqlId table2, SqlId table3);
	SqlSet From(const SqlVal& a)                                   { return From(SqlSet(a)); }

	SqlSelect(const SqlSet& s)                  { set = s; }
};

inline SqlSelect Select(const SqlSet& set)  { return SqlSelect(set); }

#define E__QSelect(I)   SqlSelect Select(__List##I(E__SqlVal));
__Expand(E__QSelect);

SqlSet Delete(SqlVal table);

class SqlInsert {
	SqlId   table;
	SqlId   keycolumn;
	SqlVal  keyvalue;
	SqlSet  set1;
	SqlSet  set2;

public:
	void Column(SqlId column, SqlVal val);
	void Column(SqlId column)                        { Column(column, column); }
	SqlInsert& operator()(SqlId column, SqlVal val)  { Column(column, val); return *this; }
	SqlInsert& operator()(SqlId column)              { Column(column, column); return *this; }
	SqlSet   Get() const;
	SqlSet   From(SqlId from) const;
	SqlSet   From(SqlSet from) const;
	SqlSet   From(SqlVal from) const                 { return From(SqlSet(from)); }

	SqlId    GetTable() const                        { return table; }
	SqlId    GetKeyColumn() const                    { return keycolumn; }
	SqlVal   GetKeyValue() const                     { return keyvalue; }

	operator SqlSet() const                          { return Get(); }
	operator bool() const                            { return !set1.IsEmpty(); }

	SqlInsert(SqlId table) : table(table) {}
	SqlInsert(SqlId table, SqlSet set1, SqlSet set2) : table(table), set1(set1), set2(set2) {}
//	SqlInsert(Fields f);
};

inline SqlInsert Insert(SqlId table)                 { return SqlInsert(table); }
//inline SqlInsert Insert(Fields f)                    { return SqlInsert(f); }

class SqlUpdate {
	SqlId  table;
	SqlSet set;

public:
	void Column(SqlId column, SqlVal val);
	SqlUpdate& operator()(SqlId column, SqlVal val)  { Column(column, val); return *this; }
	SqlSet Where(SqlBool where);

	operator bool() const                            { return !set.IsEmpty(); }

	SqlUpdate(SqlId table) : table(table) {}
};

inline SqlUpdate Update(SqlId table) { return SqlUpdate(table); }

// --------------- Schema support

/*
struct FieldOperator {
	String table;

	void Table(const char *name)                           { table = name; }

	virtual void Field(Ref f);
	virtual void Field(const char *name, Ref f);

	FieldOperator& operator()(const char *name, Ref f)     { Field(name, f); return *this; }
	FieldOperator& operator()(const String& name, Ref f)   { Field(name, f); return *this; }
	FieldOperator& operator()(Id id, Ref f)                { Field(~id, f); return *this; }
	FieldOperator& operator()(const char *name, bool& b);
	FieldOperator& operator()(const String& name, bool& b) { return (*this)(~name, b); }
	FieldOperator& operator()(Id id, bool& b)              { return (*this)(~id, b); }

	FieldOperator();
	virtual ~FieldOperator();
};

typedef Callback1<FieldOperator&> Fields;

template <class T>
struct FieldDumper : public FieldOperator {
	int i;
	String s;
	virtual void Field(const char *name, Ref f) {
		if(!s.IsEmpty())
			s.Cat(", ");
		s << name << " = " << Value(f);
	}
};

template <class T>
String DumpFields(T& s) {
	FieldDumper<T> dump;
	dump.i = 0;
	s.FieldLayout(dump);
	return dump.s;
}

void td_scalar(SqlSet& set, const String& prefix, const char *x);
void td_array(SqlSet& set, const String& prefix, const char *x, int cnt);
void td_var(SqlSet& set, const String& prefix, const char *x, SqlSet (*f)(const String&));
void td_shrink(String *array, int cnt);
*/

#endif
