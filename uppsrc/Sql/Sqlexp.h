enum {
	ORACLE    = 1,
	SQLITE3   = 2,
	MY_SQL    = 4,
	MSSQL     = 8,
	PGSQL     = 16,
	FIREBIRD  = 32, // not implemented yet
	DB2       = 64, // not implemented yet
};

class SqlBool;
class SqlVal;
class SqlSet;
class SqlSelect;
class Case;

// ----------

struct FieldOperator {
	String table;

	void Table(const char *name)                           { table = name; }

	virtual void Field(Ref f);
	virtual void Field(const char *name, Ref f);
	virtual void Field(const char *name, Ref f, bool *b); // Ugly fix to resolve bool issue with S_info creation

	FieldOperator& operator()(const char *name, Ref f)     { Field(name, f, NULL); return *this; }
	FieldOperator& operator()(const String& name, Ref f)   { Field(name, f, NULL); return *this; }
	FieldOperator& operator()(Id id, Ref f)                { Field(~id, f); return *this; }
	FieldOperator& operator()(const char *name, bool& b);
	FieldOperator& operator()(const String& name, bool& b) { return (*this)(~name, b); }
	FieldOperator& operator()(Id id, bool& b)              { return (*this)(~id, b); }

	FieldOperator();
	virtual ~FieldOperator();
};

typedef Callback1<FieldOperator&> Fields;

struct FieldDumper : public FieldOperator {
	String s;
	virtual void Field(const char *name, Ref f);
};

template <class T>
String DumpFields(T& s) {
	FieldDumper dump;
	s.FieldLayout(dump);
	return dump.s;
}

void td_scalar(SqlSet& set, const String& prefix, const char *x);
void td_array(SqlSet& set, const String& prefix, const char *x, int cnt);
void td_var(SqlSet& set, const String& prefix, const char *x, SqlSet (*f)(const String&));
void td_shrink(String *array, int cnt);

bool   EqualFields(Fields a, Fields b);
String AsString(Fields a);

// ----------

enum {
	SQLC_IF = 1,
	SQLC_ELSEIF = 2,
	SQLC_ELSE = 3,
	SQLC_ENDIF = 4,
	SQLC_DATE = 5,
	SQLC_TIME = 6,
	SQLC_STRING = 7,
	SQLC_BINARY = 8,
	SQLC_ID = 9, // '\t'
	SQLC_OF = 10,
	SQLC_AS = 11,
	SQLC_COMMA = 12,
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
String SqlFormatBinary(const char *s, int l);
String SqlFormatBinary(const String& x);

class SqlId : Moveable<SqlId> {
protected:
	Id id;

private:
	void PutOf0(String& s, const SqlId& b) const;
	void PutOf(String& s, const SqlId& b) const;

public:
	static void   UseQuotes(bool b = true);

	bool          IsEqual(const SqlId& b) const  { return id == b.id; }
	bool          IsEqual(const Id& b) const     { return id == b; }
	bool          IsNull() const                 { return id.IsNull(); }

	operator      const Id&() const              { return id; }
	const String& ToString() const               { return id.ToString(); }
	const String& operator~() const              { return ToString(); }
	String        Quoted() const;

	SqlId         Of(SqlId id) const;
	SqlId         Of(const char *of) const;
	SqlId         As(const char *as) const;
	SqlId         As(SqlId id) const             { return As(~id.ToString()); }
	SqlId         operator[](int i) const; // deprecated
	SqlId         operator&(const SqlId& s) const; // depreceted
	SqlId         operator[](const SqlId& id) const; // deprecated

	SqlId         operator()(SqlId p) const;

//$	SqlId     operator()(SqlId p, SqlId p1, ...);
#define E__PutSqlId(I)      PutOf(x, p##I)
#define E__SqlId(I)         const SqlId& p##I
#define E__Of(I) \
	SqlId operator()(const SqlId& p, __List##I(E__SqlId)) const { \
	String x; \
	PutOf0(x, p); \
	__List##I(E__PutSqlId); \
	return x; \
}
__Expand(E__Of)
#undef  E__Of
#undef E__PutSqlId
#undef E__SqlId

	SqlId()                                      {}
	SqlId(const char *s) : id(s)                 {}
	SqlId(const String& s) : id(s)               {}
	SqlId(const Id& id) : id(id)                 {}
};

typedef SqlId SqlCol; // Deprecated

#define SQLID(x)     const UPP::SqlId x(#x);
#define SQL_ID(n, x) const UPP::SqlId n(#x);

class SqlS : Moveable<SqlS> {
protected:
	String text;
	byte   priority;

	void   Init(const SqlS& a, const char *o, int olen, const SqlS& b, int pr, int prb);

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
	String        operator()(int at, byte cond) const;
	const String& operator~() const         { return text; }
	bool          IsEmpty() const           { return priority == EMPTY; }

	SqlS()                                  : priority(EMPTY) {}
	SqlS(const char *s, int pr)             : text(s), priority(pr) {}
	SqlS(const String& s, int pr)           : text(s), priority(pr) {}
	
	force_inline SqlS(const SqlS& a, const char *op, const SqlS& b, int pr, int prb) { Init(a, op, (int)strlen(op), b, pr, prb); }
	force_inline SqlS(const SqlS& a, const char *op, const SqlS& b, int pr)          { Init(a, op, (int)strlen(op), b, pr, pr); }
};

class SqlVal : public SqlS, Moveable<SqlVal> {
public:
	void SetNull()                          { text = "NULL"; priority = NULLVAL; }
	void SetHigh(const String& s)           { text = s; priority = HIGH; }
	bool IsNull() const                     { return priority == NULLVAL; }

	SqlVal As(const char *as) const;
	SqlVal As(const SqlId& id) const;

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
	SqlVal(const SqlId& id);
	SqlVal(const SqlId& (*id)());
	SqlVal(const Case& x);
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
SqlId  SqlAll();
SqlVal Count(const SqlVal& exp);
SqlVal Count(const SqlSet& exp);
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
SqlVal Length(const SqlVal& exp);
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

SqlVal SqlCurrentDate();
SqlVal SqlCurrentTime();

SqlVal Cast(const char* type, const SqlId& a);

SqlVal SqlNvl(const SqlVal& a);
inline SqlVal Nvl(const SqlVal& a)     { return SqlNvl(SqlVal(a)); }
inline SqlVal Nvl(const SqlId& a)      { return SqlNvl(SqlVal(a)); }

SqlVal SqlNvl(const SqlVal& a, const SqlVal& b);
inline SqlVal Nvl(const SqlVal& a, const SqlVal& b) { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlVal& a, const SqlId& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlId& a, const SqlVal& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlId& a, const SqlId& b)   { return SqlNvl(SqlVal(a), SqlVal(b)); }

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2);
SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3);
SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3, const SqlVal& exp4);

SqlVal Prior(const SqlId& a);

SqlVal NextVal(const SqlId& a);
SqlVal CurrVal(const SqlId& a);
SqlVal OuterJoin(const SqlId& col); //Oracle only, deprecated

inline SqlVal operator++(const SqlId& a)           { return NextVal(a); }

SqlVal SqlTxt(const char *s);

SqlVal SqlRowNum();

SqlVal SqlArg();

SqlVal SqlBinary(const String& data);

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

SqlBool        IsSame(const SqlVal& a, const SqlVal& b);

SqlBool        operator||(const SqlBool& a, const SqlBool& b);
SqlBool        operator&&(const SqlBool& a, const SqlBool& b);
SqlBool        operator- (const SqlBool& a, const SqlBool& b);
SqlBool        operator|=(SqlBool& a, const SqlBool& b);
SqlBool        operator&=(SqlBool& a, const SqlBool& b);

SqlBool        SqlIsNull(const SqlVal& a);
SqlBool        NotNull(const SqlVal& a);

inline SqlBool IsNull(const SqlId& a)  { return SqlIsNull(a); }
inline SqlBool IsNull(const SqlVal& a) { return SqlIsNull(a); }

SqlBool        Like(const SqlVal& a, const SqlVal& b, bool casesensitive = true);
SqlBool        NotLike(const SqlVal& a, const SqlVal& b, bool casesensitive = true);

SqlBool        Between(const SqlVal& a, const SqlVal& low, const SqlVal& high);
SqlBool        NotBetween(const SqlVal&a, const SqlVal& low, const SqlVal& high);

SqlBool        In(const SqlVal& a, const SqlSet& b);
SqlBool        NotIn(const SqlVal& a, const SqlSet& b);
SqlBool        Exists(const SqlSet& set);
SqlBool        NotExists(const SqlSet& set);

SqlBool        LikeUpperAscii(const SqlVal& a, const SqlVal& b);

SqlBool        LeftJoin(SqlVal v1, SqlVal v2); // Deprecated
SqlBool        RightJoin(SqlVal v1, SqlVal v2); // Deprecated

SqlBool        Join(SqlId tab1, SqlId tab2, SqlId key); // Deprecated
SqlBool        LeftJoin(SqlId tab1, SqlId tab2, SqlId key); // Deprecated
SqlBool        RightJoin(SqlId tab1, SqlId tab2, SqlId key); // Deprecated

SqlBool        SqlFirstRow(); // Oracle specific

inline SqlBool operator==(const SqlVal& a, const SqlSet& b) { return In(a, b); }
inline SqlBool operator!=(const SqlVal& a, const SqlSet& b) { return NotIn(a, b); }

inline const SqlVal& operator+(const SqlVal& a) { return a; }

class Case : public SqlS, Moveable<Case> {
public:
	Case(const SqlBool& cond, const SqlVal& val)
	{
		text = "case when " + ~cond + " then " + ~val + " end";
	}
	Case& operator()(const SqlBool& cond, const SqlVal& val)
	{
		text.Insert(text.GetLength() - 4, " when " + ~cond + " then " + ~val);
		return *this;
	}
	Case& operator()(const SqlVal& val)
	{
		text.Insert(text.GetLength() - 4, " else " + ~val);
		return *this;
	}
};

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
		HIGH     = 200,
	};

	String           operator~() const;
	String           operator()() const;
	String           operator()(int at) const;
	String           operator()(int at, byte cond) const;
	bool             IsEmpty() const                    { return text.IsEmpty(); }
	bool             operator==(const SqlSet& b) const  { return text == b.text && priority == b.priority; }
	bool             operator!=(const SqlSet& b) const  { return !operator==(b); }

	SqlSet&          Cat(const SqlVal& val); // adding a member
	SqlSet&          Cat(const SqlSet& set);
	SqlSet&          operator|=(const SqlVal& val)  { return Cat(val); }
	SqlSet&          operator<<(const SqlVal& val)  { return Cat(val); }

	SqlSet&          operator|=(const SqlSet& set)  { return *this = *this | set; }
	SqlSet&          operator&=(const SqlSet& set)  { return *this = *this & set; }
	SqlSet&          operator-=(const SqlSet& set)  { return *this = *this - set; }

	void             Clear()                                { text.Clear(); }

	SqlSet() {}
	explicit SqlSet(const SqlVal& p0);

#define E__SqlSet(I)   SqlSet(const SqlVal& p0, __List##I(E__SqlVal));
	__Expand(E__SqlSet);

	explicit SqlSet(Fields nfields);

	SqlSet(const String& s, PRIORITY p)   { text = s; priority = p; }
};

template <typename T>
SqlSet SqlSetFrom(const T& cont)
{
	SqlSet set;
	typename T::ConstIterator e = cont.End();
	for(typename T::ConstIterator it = cont.Begin(); it != e; it++)
		set.Cat(*it);
	return set;
}

class SqlSetC : public SqlSet {
public:
	SqlSetC(const String& s)      { text = s; priority = SET; }
	SqlSetC(const char* s)        { text = s; priority = SET; }
};

class SqlStatement {
	String text;

public:
	SqlStatement() {}
	explicit SqlStatement(const String& s) : text(s) {}

	String Get(int dialect) const;
#ifndef flagNOAPPSQL
	String Get() const;
#endif
	String GetText() const                           { return text; }
	bool   IsEmpty() const                           { return text.IsEmpty(); }
	operator bool() const                            { return !IsEmpty(); }

//Deprecated!!!
	bool  Execute(Sql& cursor) const;
	void  Force(Sql& cursor) const;
	Value Fetch(Sql& cursor) const;
	bool  Execute() const;
	void  Force() const;
	Value Fetch() const;
};

class SqlSelect {
	String  text;
	String  tables;
	bool    on;

	SqlSelect& InnerJoin0(const String& table);
	SqlSelect& LeftJoin0(const String& table);
	SqlSelect& RightJoin0(const String& table);
	SqlSelect& FullJoin0(const String& table);
	SqlSelect& SetOp(const SqlSelect& s2, const char *op);

public:
	SqlSelect& operator()(const SqlVal& val);

	operator bool() const                             { return text.GetCount(); }

	SqlSelect& Hint(const char *hint);

	SqlSelect& Get();
	SqlSelect& From()                                 { return Get(); }
	SqlSelect& From(const SqlSet& set);
	SqlSelect& From(const SqlId& table);
	SqlSelect& From(const SqlId& table1, const SqlId& table2);
	SqlSelect& From(const SqlId& table1, const SqlId& table2, const SqlId& table3);
	SqlSelect& From(const SqlVal& a)                  { return From(SqlSet(a)); }

	SqlSelect& InnerJoin(const SqlId& table)          { return InnerJoin0(table.Quoted()); }
	SqlSelect& LeftJoin(const SqlId& table)           { return LeftJoin0(table.Quoted()); }
	SqlSelect& RightJoin(const SqlId& table)          { return RightJoin0(table.Quoted()); }
	SqlSelect& FullJoin(const SqlId& table)           { return FullJoin0(table.Quoted()); }

	SqlSelect& InnerJoin(const SqlSet& set)           { return InnerJoin0(~set(SqlSet::SET)); }
	SqlSelect& LeftJoin(const SqlSet& set)            { return LeftJoin0(~set(SqlSet::SET)); }
	SqlSelect& RightJoin(const SqlSet& set)           { return RightJoin0(~set(SqlSet::SET)); }
	SqlSelect& FullJoin(const SqlSet& set)            { return FullJoin0(~set(SqlSet::SET)); }

	SqlSelect& InnerJoinRef(const SqlId& table);
	SqlSelect& LeftJoinRef(const SqlId& table);
	SqlSelect& RightJoinRef(const SqlId& table);
	SqlSelect& FullJoinRef(const SqlId& table);

	SqlSelect& Where(const SqlBool& exp);
	SqlSelect& On(const SqlBool& exp);
	SqlSelect& StartWith(const SqlBool& exp);
	SqlSelect& ConnectBy(const SqlBool& exp);
	SqlSelect& GroupBy(const SqlSet& columnset);
	SqlSelect& Having(const SqlBool& exp);
	SqlSelect& OrderBy(const SqlSet& columnset);
	SqlSelect& ForUpdate();
	SqlSelect& NoWait();

	SqlSelect& GroupBy(SqlVal a)                      { return GroupBy(SqlSet(a)); }
	SqlSelect& GroupBy(SqlVal a, SqlVal b)            { return GroupBy(SqlSet(a, b)); }
	SqlSelect& GroupBy(SqlVal a, SqlVal b, SqlVal c)  { return GroupBy(SqlSet(a, b, c)); }
	SqlSelect& OrderBy(SqlVal a)                      { return OrderBy(SqlSet(a)); }
	SqlSelect& OrderBy(SqlVal a, SqlVal b)            { return OrderBy(SqlSet(a, b)); }
	SqlSelect& OrderBy(SqlVal a, SqlVal b, SqlVal c)  { return OrderBy(SqlSet(a, b, c)); }
	SqlSelect& Limit(int limit);
	SqlSelect& Limit(int64 offset, int limit);
	SqlSelect& Offset(int64 offset);

	operator  SqlSet() const                           { return SqlSet(text, SqlSet::SETOP); }
	operator  SqlStatement() const                     { return SqlStatement(text); }
	SqlVal    AsValue() const;
	SqlSet    AsTable(const SqlId& tab) const;

	SqlSelect(Fields f);
	SqlSelect(const SqlSet& s)                        { text = ~s; on = false; }
	SqlSelect()                                       { on = false; }
#define E__QSelect(I)   SqlSelect(__List##I(E__SqlVal));
__Expand(E__QSelect);
#undef  E__QSelect

	bool IsEmpty()                                    { return text.IsEmpty(); }

	SqlSelect& operator|=(const SqlSelect& s2);
	SqlSelect& operator&=(const SqlSelect& s2);
	SqlSelect& operator-=(const SqlSelect& s2);

//Deprecated!!!
	bool  Execute(Sql& sql) const                     { return SqlStatement(*this).Execute(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Value Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  Execute() const                             { return SqlStatement(*this).Execute(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Value Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

SqlSelect operator|(const SqlSelect& s1, const SqlSelect& s2);
SqlSelect operator&(const SqlSelect& s1, const SqlSelect& s2);
SqlSelect operator-(const SqlSelect& s1, const SqlSelect& s2);

inline SqlSelect SelectAll()                { return SqlSelect(SqlAll()); }
inline SqlSelect Select(const SqlSet& set)  { return SqlSelect(set); }
inline SqlSelect Select(Fields f)           { return SqlSelect(f); }

#define E__QSelect(I)   SqlSelect Select(__List##I(E__SqlVal));
__Expand(E__QSelect);
#undef  E__QSelect

class SqlDelete {
	String text;

public:
	SqlDelete& Where(const SqlBool& b);

	operator SqlStatement() const                     { return SqlStatement(text); }

	SqlDelete(SqlVal table);

//Deprecated!!!
	bool  Execute(Sql& sql) const                     { return SqlStatement(*this).Execute(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Value Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  Execute() const                             { return SqlStatement(*this).Execute(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Value Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

inline SqlDelete Delete(SqlVal table) { return SqlDelete(table); }

class SqlInsert {
	SqlId   table;
	SqlId   keycolumn;
	SqlVal  keyvalue;
	SqlSet  set1;
	SqlSet  set2;
	SqlSet  from;
	SqlBool where;
	SqlSet  groupby;
	SqlBool having;

public:
	void Column(const SqlId& column, SqlVal val);
	void Column(const SqlId& column)                       { Column(column, column); }
	SqlInsert& operator()(const SqlId& column, SqlVal val) { Column(column, val); return *this; }
	SqlInsert& operator()(const SqlId& column)             { Column(column, column); return *this; }
	SqlInsert& operator()(Fields f, bool nokey = false);
	SqlInsert& operator()(const ValueMap& data);
	SqlInsert& From(const SqlId& from);
	SqlInsert& From(SqlSet _from)                    { from = _from; return *this; }
	SqlInsert& From(SqlVal from)                     { return From(SqlSet(from)); }
	SqlInsert& Where(SqlBool w)                      { where = w; return *this; }
	SqlInsert& GroupBy(const SqlSet& columnset)      { groupby = columnset; return *this; }
	SqlInsert& GroupBy(SqlVal a)                     { return GroupBy(SqlSet(a)); }
	SqlInsert& GroupBy(SqlVal a, SqlVal b)           { return GroupBy(SqlSet(a, b)); }
	SqlInsert& GroupBy(SqlVal a, SqlVal b, SqlVal c) { return GroupBy(SqlSet(a, b, c)); }
	SqlInsert& Having(const SqlBool& exp)            { having = exp; return *this; }

	SqlId    GetTable() const                        { return table; }
	SqlId    GetKeyColumn() const                    { return keycolumn; }
	SqlVal   GetKeyValue() const                     { return keyvalue; }

	operator SqlStatement() const;
	operator bool() const                            { return !set1.IsEmpty(); }

	SqlInsert(const SqlId& table) : table(table) {}
	SqlInsert(const SqlId& table, const SqlSet& set1, const SqlSet& set2)
		: table(table), set1(set1), set2(set2) {}
	SqlInsert(Fields f, bool nokey = false);

//Deprecated!!!
	bool  Execute(Sql& sql) const                     { return SqlStatement(*this).Execute(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Value Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  Execute() const                             { return SqlStatement(*this).Execute(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Value Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

inline SqlInsert Insert(const SqlId& table)           { return SqlInsert(table); }
inline SqlInsert Insert(Fields f)                     { return SqlInsert(f); }
inline SqlInsert InsertNoKey(Fields f)                { return SqlInsert(f, true); }

class SqlUpdate {
	SqlId   table;
	SqlSet  set;
	SqlBool where;

public:
	void Column(const SqlId& column, SqlVal val);
	void Column(const SqlSet& cols, const SqlSet& val);
	SqlUpdate& operator()(const SqlId& column, SqlVal val)       { Column(column, val); return *this; }
	SqlUpdate& operator()(const SqlSet& cols, const SqlSet& val) { Column(cols, val); return *this; }
	SqlUpdate& operator()(Fields f);
	SqlUpdate& operator()(const ValueMap& data);
	SqlUpdate& Where(SqlBool w)                      { where = w; return *this; }

	operator SqlStatement() const;

	operator bool() const                            { return !set.IsEmpty(); }

	SqlUpdate(const SqlId& table) : table(table) {}
	SqlUpdate(Fields f);

//Deprecated!!!
	bool  Execute(Sql& sql) const                     { return SqlStatement(*this).Execute(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Value Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  Execute() const                             { return SqlStatement(*this).Execute(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Value Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

inline SqlUpdate Update(const SqlId& table)           { return SqlUpdate(table); }
inline SqlUpdate Update(Fields f)                     { return SqlUpdate(f); }
