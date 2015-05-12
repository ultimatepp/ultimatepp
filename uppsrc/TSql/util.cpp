//////////////////////////////////////////////////////////////////////
// sql/util: utilities.

#include "TSql.h"
#pragma hdrstop

#undef GENID
#define GENID CPPID
#include <TSql/id.h>

NAMESPACE_UPP

extern const char *txtFnTSqlTemplateFetchSeqKeyNull()  { return t_("Table '%s', column '%s': NULL value requested."); }
extern const char *txtFnTSqlTemplateFetchSeqNotFound() { return t_("Row '%s' not found (table '%s', column '%s')."); }

GLOBAL_VARP(const SqlId, ALL, ("*"))
GLOBAL_VARP(const SqlId, QUE, ("?"))

namespace SqlUtil
{ // internal objects

class InsertScript : public FieldOperator
{
public:
	virtual void Field(const char *name, Ref f)
	{
		Value v(f);
		if(!optimize || !v.IsNull())
		{
			if(!IsNull(names))
			{
				names.Cat(", ");
				values.Cat(", ");
			}
			names.Cat(name);
			values.Cat(SqlFormat(v));
		}
	}

public:
	bool optimize;
	String names;
	String values;
};

class SelectColumns : public FieldOperator
{
public:
	virtual void Field(const char *name, Ref f)
	{
		columns.Cat(SqlId(name));
	}

public:
	SqlSet columns;
};

class InsertColumns : public FieldOperator
{
public:
	InsertColumns() {}

	virtual void Field(const char *name, Ref f)
	{
		columns.Add(SqlId(name));
		values.Add(Value(f));
	}

	SqlInsert    Get(const SqlVal& from_table) const       { return GetRaw(Nvl(~from_table, table)); }
	SqlInsert    GetSchema(const SqlId& from_table) const { return GetRaw(SchemaTableName(Nvl(~from_table, table))); }

private:
	SqlInsert    GetRaw(const String& dest) const
	{
		SqlId d = SqlId(dest);
		SqlInsert ins(d);
		for(int i = 0; i < columns.GetCount(); i++)
			ins(columns[i], values[i]);
		return ins;
	}

private:
	Vector<SqlId>  columns;
	Vector<SqlVal>  values;
};

class UpdateColumns : public FieldOperator
{
public:
	UpdateColumns() {}

	virtual void Field(const char *name, Ref f)
	{
		columns.Add(SqlId(name));
		values.Add(Value(f));
	}

	operator     bool () const                             { return columns.GetCount(); }
	SqlUpdate    Get(const SqlVal& from_table) const       { return GetRaw(Nvl(~from_table, table)); }
	SqlUpdate    GetSchema(const SqlId& from_table) const { return GetRaw(SchemaTableName(Nvl(~from_table, table))); }

private:
	SqlUpdate    GetRaw(const String& dest) const
	{
		SqlId d = SqlId(dest);
		SqlUpdate u(d);
		for(int i = 0; i < columns.GetCount(); i++)
			u(columns[i], values[i]);
		return u;
	}

private:
	Vector<SqlId>   columns;
	Vector<SqlVal>  values;
};

class GetValue : public FieldOperator
{
public:
	GetValue(SqlId column) : column(column) {}

	virtual void Field(const char *name, Ref f)
	{
		if(name == ~column)
			value = f;
	}

	Id column;
	Value value;
};

class GetRef : public FieldOperator
{
public:
	GetRef(SqlId column): column(column) {}

	virtual void Field(const char *name, Ref f)
	{
		if(name == ~column)
			ref = f;
	}

	Id column;
	Ref ref;
};

struct GetRefMap : FieldOperator {
	virtual void Field(const char *name, Ref f) {
		map.Add(Id(name), f);
	}
	VectorMap<Id, Ref> map;
};

struct GetValueMap : FieldOperator {
	virtual void Field(const char *name, Ref f) {
		map.Add(Id(name), f);
	}
	VectorMap<Id, Value> map;
};

class GetTableName : public FieldOperator {
	virtual void Field(const char *name, Ref f) {}
};

/*
class NthValue : public FieldOperator
{
public:
	NthValue(int i = 0) : n(n) {}
	virtual void Field(const char *_name, Ref f)
	{
		if(n-- == 0)
		{
			column = _name;
			value = f.GetValue();
		}
	}

	SqlSet set;
	Value  value;
	String column;
	int    n;
};
*/

} // namespace SqlUtil

static String& AccessSchema()                  { static String s; return s; }
void           SetSchema(const String& schema) { AccessSchema() = schema; }
String         GetSchema()                     { return AccessSchema(); }

String SchemaTableName(const String& table)
{
	String s = GetSchema();
	if(s.IsEmpty())
		return table;
	return s + '.' + table;
}

SqlId SchemaTable(const SqlId& table)
{
	return SqlId(SchemaTableName(~table));
}

String GetInsertString(Fields nf, bool optimize)
{
	SqlUtil::InsertScript isfo;
	isfo.optimize = optimize;
	nf(isfo);
	String result;
	if(!IsNull(isfo.names))
		result << "insert into " << isfo.table
			<< " (" << isfo.names << ") values (" << isfo.values << ")";
	return result;
}

String GetSchemaInsertString(Fields nf, bool optimize)
{
	SqlUtil::InsertScript isfo;
	isfo.optimize = optimize;
	nf(isfo);
	String result;
	if(!IsNull(isfo.names))
		result << "insert into " << SchemaTableName(isfo.table)
			<< " (" << isfo.names << ") values (" << isfo.values << ")";
	return result;
}

#ifdef PLATFORM_WIN32
void InsertScript(SqlSchema& schema, Fields nf, bool optimize)
{
	schema.Config(GetInsertString(nf, optimize) + ";\n", NULL);
}

void InsertSchemaScript(SqlSchema& schema, Fields nf, bool optimize)
{
	schema.Config(GetSchemaInsertString(nf, optimize) + ";\n", NULL);
}
#endif

SqlSelect SelectColumns(Fields nf)
{
	SqlUtil::SelectColumns helper;
	nf(helper);
	return Select(helper.columns);
}

SqlSelect SelectTable(Fields nf)
{
	SqlUtil::SelectColumns helper;
	nf(helper);
	ASSERT(*helper.table != '$'); // cannot select from TYPE tables
	return Select(helper.columns).From(SqlId(helper.table));
}

SqlSelect SelectSchemaTable(Fields nf)
{
	SqlUtil::SelectColumns helper;
	nf(helper);
	ASSERT(*helper.table != '$'); // cannot select from TYPE tables
	return Select(helper.columns).FromSchema(helper.table);
}

Value GetValue(Fields nf, SqlId field)
{
	SqlUtil::GetValue helper(field);
	nf(helper);
	return helper.value;
}

Ref GetRef(Fields nf, SqlId field)
{
	SqlUtil::GetRef helper(field);
	nf(helper);
	return helper.ref;
}

String GetTableName(Fields nf)
{
	SqlUtil::GetTableName helper;
	nf(helper);
	return helper.table;
}

VectorMap<Id, Ref> GetRefMap(Fields nf, String *tablename)
{
	SqlUtil::GetRefMap helper;
	nf(helper);
	if(tablename)
		*tablename = helper.table;
	return helper.map;
}

VectorMap<Id, Value> GetValueMap(Fields nf, String *tablename)
{
	SqlUtil::GetValueMap helper;
	nf(helper);
	if(tablename)
		*tablename = helper.table;
	return helper.map;
}

/*
void FetchSeq(Fields fields, Sql& cursor)
{
	SqlUtil::NthValue key;
	fields(key);
	ASSERT(!IsNull(key.column));
	if(!cursor.Execute(Select(fields).From(fields.table).Where(Col(fields.column) == fields.value)))
		throw SqlExc(cursor);
	if(!cursor.Fetch(fields))
		throw SqlExc("øádek nebyl nalezen (tabulka %s, sloupec %s, hodnota %s)",
			key.table, key.column, SqlFormat(key.value));
}
*/

String ForceInsertRowid(const String& insert, Sql& cursor)
{
	if(!cursor.Execute(insert + " returning ROWID into ?%s"))
		throw SqlExc(cursor.GetSession());
	if(!cursor.Fetch())
		throw Exc(t_("FETCH internal error (ForceInsertRowid)"));
	ASSERT(!IsNull(cursor[0]));
	return cursor[0];
}

String ForceInsertRowid(const SqlInsert& insert, Sql& cursor)
{
	return ForceInsertRowid(SqlStatement(insert).Get(cursor.GetDialect()), cursor);
}

String ForceInsertRowid(Fields nf, Sql& cursor)
{
	return ForceInsertRowid(SqlId(), nf, cursor);
}

String ForceInsertRowid(SqlId table, Fields nf, Sql& cursor)
{
	SqlUtil::InsertColumns helper;
	nf(helper);
	return ForceInsertRowid(helper.Get(table), cursor);
}

String ForceSchemaInsertRowid(Fields nf, Sql& cursor)
{
	return ForceSchemaInsertRowid(SqlId(), nf, cursor);
}

String ForceSchemaInsertRowid(SqlId table, Fields nf, Sql& cursor)
{
	SqlUtil::InsertColumns helper;
	nf(helper);
	return ForceInsertRowid(helper.GetSchema(table), cursor);
}

void ForceInsert(Fields nf, Sql& cursor)
{
	ForceInsert(SqlId(), nf, cursor);
}

void ForceInsert(SqlId table, Fields nf, Sql& cursor)
{
	SqlUtil::InsertColumns helper;
	nf(helper);
	cursor & SqlStatement(helper.Get(table));
}

void ForceUpdate(Fields nf, SqlId key, const Value& keyval, Sql& cursor)
{
	ForceUpdate(SqlId(), nf, key, keyval, cursor);
}

void ForceUpdate(SqlId table, Fields nf, SqlId key, const Value& keyval, Sql& cursor)
{
	SqlUtil::UpdateColumns helper;
	nf(helper);
	cursor & helper.Get(table).Where(key == keyval);
}

void ForceUpdate(Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor)
{
	ForceUpdate(SqlId(), nf, of, key, keyval, cursor);
}

void ForceUpdate(SqlId table, Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor)
{
	String tbl;
	VectorMap<Id, Value> nmap = GetValueMap(nf, &tbl);
	VectorMap<Id, Value> omap = GetValueMap(of);
	SqlUpdate update(SqlId(Nvl(~table, tbl)));
	for(int i = 0; i < nmap.GetCount(); i++)
		if(nmap[i] != omap.Get(nmap.GetKey(i), Value()))
			update(SqlId(nmap.GetKey(i)), nmap[i]);
	if(update)
		update.Where(key == keyval).Force(cursor);
}

void ForceDelete(SqlId table, SqlId key, const Value& keyval, Sql& cursor)
{
	if(!cursor.Delete(table, key, keyval))
		throw SqlExc(cursor);
}

void ForceExecute(const String& s, Sql& cursor)
{
	if(!cursor.Execute(s))
		throw SqlExc(cursor);
}

void ForceSchemaInsert(Fields nf, Sql& cursor)
{
	ForceSchemaInsert(SqlId(), nf, cursor);
}

void ForceSchemaInsert(SqlId table, Fields nf, Sql& cursor)
{
	SqlUtil::InsertColumns helper;
	nf(helper);
	helper.GetSchema(table).Force(cursor);
}

void ForceSchemaUpdate(Fields nf, SqlId key, const Value& keyval, Sql& cursor)
{
	ForceSchemaUpdate(SqlId(), nf, key, keyval, cursor);
}

void ForceSchemaUpdate(SqlId table, Fields nf, SqlId key, const Value& keyval, Sql& cursor)
{
	SqlUtil::UpdateColumns helper;
	nf(helper);
	helper.GetSchema(table).Where(key == keyval).Force(cursor);
}

void ForceSchemaUpdate(Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor)
{
	ForceSchemaUpdate(SqlId(), nf, of, key, keyval, cursor);
}

void ForceSchemaUpdate(SqlId table, Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor)
{
	String tbl;
	VectorMap<Id, Value> nmap = GetValueMap(nf, &tbl);
	VectorMap<Id, Value> omap = GetValueMap(of);
	SqlUpdate update(SqlId(SchemaTableName(Nvl(~table, tbl))));
	for(int i = 0; i < nmap.GetCount(); i++)
		if(nmap[i] != omap.Get(nmap.GetKey(i), Value()))
			update(SqlId(nmap.GetKey(i)), nmap[i]);
	if(update)
		update.Where(key == keyval).Force(cursor);
}

void ForceSchemaDelete(SqlId table, SqlId key, const Value& keyval, Sql& cursor)
{
	Delete(SchemaTable(table)).Where(key == keyval).Force(cursor);
}

Vector<String> LoadStrColumn(SqlId column, const SqlVal& table, SqlSession& session, bool order)
{
	Vector<String> result;
	FetchContainer(result, Select(column).From(table), session);
	if(order)
		Sort(result, GetLanguageInfo());
	return result;
}

Vector<String>& SyncStrColumn(Vector<String>& dest, SqlId col, const SqlVal& table, SqlSession& session, bool order)
{
	if(dest.IsEmpty())
		dest = LoadStrColumn(col, table, session, order);
	return dest;
}

Vector<String> LoadSchemaStrColumn(SqlId column, const SqlId& table, SqlSession& session, bool order)
{
	Vector<String> result;
	FetchContainer(result, Select(column).FromSchema(table), session);
	if(order)
		Sort(result, GetLanguageInfo());
	return result;
}

Vector<String>& SyncSchemaStrColumn(Vector<String>& dest, SqlId col, const SqlId& table, SqlSession& session)
{
	if(dest.IsEmpty())
		dest = LoadSchemaStrColumn(col, table, session);
	return dest;
}

bool IsNotEmpty(const SqlSelect& select, Sql& cursor)
{
	return cursor * select && cursor.Fetch();
}

bool IsNotEmpty(const SqlVal& table, const SqlBool& cond, Sql& cursor)
{
	return IsNotEmpty(Select(1).From(table).Where(cond), cursor);
}

bool IsNotEmpty(const SqlVal& table, SqlId column, const Value& value, Sql& cursor)
{
	return IsNotEmpty(table, column == value, cursor);
}

bool IsNotSchemaEmpty(const SqlId& table, const SqlBool& cond, Sql& cursor)
{
	return IsNotEmpty(Select(1).FromSchema(table).Where(cond), cursor);
}

bool IsNotSchemaEmpty(const SqlId& table, SqlId column, const Value& value, Sql& cursor)
{
	return IsNotSchemaEmpty(table, column == value, cursor);
}

/*
#ifdef PLATFORM_WIN32
bool LockSql(Sql& cursor)
{
	if(cursor.Run())
		return true;

	enum
	{
		TIMEOUT = 10000, // milliseconds
	};

	Progress progress("Zamykám tabulky", TIMEOUT);
	dword start = GetTickCount();
	while(!cursor.Run())
	{
		dword elapsed = GetTickCount() - start;
		if(elapsed % 100 == 0)
		{
			progress.SetPos(elapsed);
			if(progress.Canceled())
				return false;
		}
		if(elapsed >= TIMEOUT)
		{
			MsgOK("Pøed provedením úprav je nutno tabulky zamknout proti pøepsání "
				"ostatními uživateli. To se zatím nedaøí - nìkdo jiný zøejmì právì "
				"provádí opravu. Zkuste provést požadovanou operaci pozdìji.");
			return false;
		}
	}
	return true;
}

bool LockSql(const SqlVal& table, SqlBool where, Sql& cursor)
{
	cursor.SetStatement(Select(ALL).From(table).Where(where).ForUpdate().NoWait());
	return LockSql(cursor);
}

bool LockSql(const SqlVal& table, SqlId column, const Value& value, Sql& cursor)
{
	return LockSql(table, column == value, cursor);
}

bool LockSchemaSql(const SqlVal& table, SqlBool where, Sql& cursor)
{
	cursor.SetStatement(Select(ALL).FromSchema(table).Where(where).ForUpdate().NoWait());
	return LockSql(cursor);
}

bool LockSchemaSql(const SqlVal& table, SqlId column, const Value& value, Sql& cursor)
{
	return LockSchemaSql(table, column == value, cursor);
}
#endif
*/

//SqlSelect SelectHint(const char *hint, SqlSet set)
//{
//	return hint && *hint ? SqlSelect(SqlSet(String().Cat() << "/*+ " << hint << " */ " << ~set), SqlSet::SET)
//	                     : SqlSelect(set);
//}

static inline void sCat(SqlSet& s, SqlVal v) { s.Cat(v); }

/*
#define E__SCat(I)       sCat(set, p##I)

#define E__QSelectF(I) \
SqlSelect SelectHint(const char *hint, __List##I(E__SqlVal)) { \
	SqlSet set; \
	__List##I(E__SCat); \
	return SelectHint(hint, set); \
}
__Expand(E__QSelectF);
*/
SqlSet DeleteHint(const char *hint, const SqlVal& table)
{
	return SqlSet(String().Cat() << "delete /*+ " << hint << " */ from " << ~table, SqlSet::SETOP);
}

SqlSet DeleteSchemaHint(const char *hint, const SqlId& table)
{
	return DeleteHint(hint, SchemaTable(table));
}

SqlVal Alias(const SqlId& value, const SqlId& alias)
{
	if(~value == ~alias)
		return value;
	StringBuffer out;
	out << ~value << (char)SQLC_AS << ~alias;
	return SqlId((String)out);
}

SqlVal SchemaAlias(const SqlId& table, const SqlId& alias)
{
	return Alias(SchemaTable(table), alias);
}

SqlVal SchemaAlias(const SqlId& table)
{
	return SchemaAlias(table, table);
}

SqlId SchemaId(const SqlId& table_id, const SqlId& alias_id)
{
	return SqlId(SchemaTableName(~table_id) + SqlCode(MSSQL, " as ")(" ") + ~alias_id);
}

SqlId SchemaId(const SqlId& table_id)
{
	return SchemaId(table_id, table_id);
}

VectorMap<String, SqlColumnInfo> Describe(const SqlVal& table, Sql& cursor)
{
	VectorMap<String, SqlColumnInfo> map;
	if(cursor * SelectAll().From(table).Where(SqlBool::False()))
		for(int i = 0, n = cursor.GetColumns(); i < n; i++)
		{
			const SqlColumnInfo& sci = cursor.GetColumnInfo(i);
			map.Add(sci.name, sci);
		}
	return map;
}

VectorMap<String, SqlColumnInfo> DescribeSchema(const SqlId& table, Sql& cursor)
{
	return Describe(SchemaTable(table), cursor);
}

int64 SelectCount(const SqlVal& table, const SqlBool& cond, Sql& cursor)
{
	return Select(SqlCountRows()).From(table).Where(cond).Fetch(cursor);
}

int64 SelectSchemaCount(const SqlId& table, const SqlBool& cond, Sql& cursor)
{
	return Select(SqlCountRows()).FromSchema(table).Where(cond).Fetch(cursor);
}

SqlVal GetUpper(const SqlVal& val)
{
	return SqlFunc("UPPER", val);
}

SqlVal GetCsVal(const SqlVal& val)
{
	return SqlFunc("REPLACE", SqlFunc("UPPER", val), "CH", "HŽŽ");
}

SqlVal GetCs(const char* col)
{
	return GetCsVal(SqlId(col));
}

SqlVal GetCsAsciiVal(const SqlVal& val)
{
	return ConvertAscii(GetCsVal(val));
}

SqlVal GetCsAscii(const char* col)
{
	return GetCsAsciiVal(SqlId(col));
}

SqlBool LikeSmartWild(const SqlVal& exp, const String& text)
{
	const char *s = text;
	if(*s == 0)
		return SqlBool::True();
	if((*s == '.' && s[1] != 0 && *++s != '.') || HasNlsLetters(WString(s)))
	{
		SqlBool e = Like(Upper(exp), Wild(ToUpper(s)));
		if(ToUpper((byte)*s) == 'C' && s[1] == 0)
			e &= NotLike(Upper(exp), "CH%"); // CH patch
		return e;
	}
	else
		return LikeUpperAscii(exp, Wild(s));
}

SqlBool GetTextRange(const String& s1, const String& s2, const SqlVal& exp)
{
	if(IsNull(s1) && IsNull(s2))
		return SqlBool();
	if(IsNull(s1) || IsNull(s2)) // just one - use as template
		return LikeSmartWild(exp, s1 + s2);
//	return Between(GetCsAsciiVal(exp), GetCsAsciiVal(s1), GetCsAsciiVal(s2 + "ŽŽ"));
	return Between(UpperAscii(exp), Upper(s1), Upper(s2 + "ZZ"));
}

static const double above = 1 + 1e-9, below = 1 - 1e-9;

SqlBool GetNumRange(double min, double max, const SqlVal& exp)
{
	bool nl = IsNull(min), nh = IsNull(max);
	if(nl && nh)
		return SqlBool();
	if(nl || nh)
	{ // just 1 defined
		min = Nvl(min, max);
		if(min == floor(min + 0.5)) // integer - use exact match
			return exp == min;
		else if(min >= 0)
			return Between(exp, min * below, min * above);
		else
			return Between(exp, min * above, min * below);
	}

	// TODO (idea): here could be an ordering check, something like:
	// if(max < min)
	//     throw SqlExc("Maximální hodnota je menší než minimální hodnota.");
	// Is it a good idea?

	return Between(exp,
		min * (min <= 0 ? above : below),
		max * (max >= 0 ? above : below));
}

SqlBool GetDateRange(Date min, Date max, const SqlVal& exp, bool force_range)
{
	bool nl = IsNull(min), nh = IsNull(max);
	if(nl && nh)
		return SqlBool();
	if(nh) // 1st defined - equality
		return (force_range ? exp >= min : exp == min);
	if(nl)
		return exp <= max;

	// TODO (idea): here could be an ordering check, something like:
	// if(max < min)
	//     throw SqlExc("Koncové datum pøedchází poèáteèní datum.");
	// Is it a good idea?

	return Between(exp, min, max);
}

SqlBool GetTimeRange(Time min, Time max, const SqlVal& exp, bool force_range)
{
	bool nl = IsNull(min), nh = IsNull(max);
	if(nl && nh)
		return SqlBool();
	if(nh) // 1st defined - equality
		return (force_range ? exp >= min : exp == min);
	if(nl)
		return exp <= max;

	// TODO (idea): here could be an ordering check, something like:
	// if(max < min)
	//     throw SqlExc("Koncové datum pøedchází poèáteèní datum.");
	// Is it a good idea?

	return Between(exp, min, max);
}

SqlVal GetYearDayIndex(const SqlVal& date)
{
	SqlVal mssql("substring(convert(varchar(max), " + ~date + ", 1), 1, 5)", SqlS::FN);
	SqlVal oracle(SqlFunc("to_char", date, "MM/DD"));
	return SqlVal(SqlCode(MSSQL, ~mssql)(~oracle), SqlS::FN);
}

String GetYearDayIndex(Date date)
{
	return NFormat("%02d/%02d", date.month, date.day);
}

SqlBool GetYearDayRange(const SqlVal& date, Date min, Date max)
{
	SqlVal index = GetYearDayIndex(date);
	bool nl = IsNull(min), nh = IsNull(max);
	if(nl && nh)
		return SqlBool();
	if(nl || nh)
		return index == GetYearDayIndex(Nvl(min, max));
	if(min.month == 1 && min.day == 1)
		nl = true;
	if(max.month == 12 && max.day == 31)
		nh = true;

	if(nl && nh)
		return SqlBool();
	if(!nh && !nl && min > max)
	{
		min.day--;
		max.day++;
		return NotBetween(index, GetYearDayIndex(max), GetYearDayIndex(min));
	}

	SqlBool result;
	if(!nl)
		result &= index >= GetYearDayIndex(min);
	if(!nh)
	{
		max.day++;
		result &= index < GetYearDayIndex(max);
	}
	return result;
}

END_UPP_NAMESPACE
