#include "Sql.h"

namespace Upp {

struct SchTableInfo {
	Vector<String> column;
	Vector<String> ref_table;
	Vector<String> ref_column;
	String         primary_key;
	String         prefix;
	bool           is_table; // It can also be TYPE, which is not in DB
	
	SchTableInfo() { is_table = false; }
};

ArrayMap<String, SchTableInfo>& sSchTableInfo()
{
	static ArrayMap<String, SchTableInfo> x;
	return x;
}

static bool sColumn;

void SchDbInfoTable(const char *table)
{
	sSchTableInfo().Add(table).is_table = true;
	sColumn = false;
}

void SchDbInfoType(const char *table)
{
	sSchTableInfo().Add(table).is_table = false;
	sColumn = false;
}

void SchDbInfoColumn(const char *name)
{
	SchTableInfo& f = sSchTableInfo().Top();
	f.column.Add(ToUpper(f.prefix + name));
	f.ref_table.Add();
	f.ref_column.Add();
	if(IsNull(f.primary_key))
		f.primary_key = name;
	sColumn = true;
}

void SchDbInfoVar(void (*fn)(), const char *name)
{
	SchTableInfo& f = sSchTableInfo().Top();
	String h = f.prefix;
	f.prefix << name << '$';
	(*fn)();
	f.prefix = h;
}

void SchDbInfoReferences(const char *table)
{
	sSchTableInfo().Top().ref_table.Top() = table;
}

void SchDbInfoReferences(const char *table, const char *column)
{
	SchDbInfoReferences(table);
	sSchTableInfo().Top().ref_column.Top() = column;
}

void SchDbInfoPrimaryKey()
{
	SchTableInfo& f = sSchTableInfo().Top();
	f.primary_key = f.column.Top();
}

void SchDbInfoColumnArray(const char *name, int items)
{
	for(int i = 0; i < items; i++)
		SchDbInfoColumn(String().Cat() << name << i);
}

const SchTableInfo& GetSchTableInfo(const String& table)
{
	static SchTableInfo sSchTableInfoZero;
	return sSchTableInfo().Get(~table, sSchTableInfoZero);
}

bool MatchRefName(const String& fk, const String& pk, const String& tab, int phase)
{
	return phase || fk == pk || tab + '_' + pk == fk;
}

SqlBool Join(const String& tab1, const String& as1, const String& tab2, const String& as2, int phase)
{
	const SchTableInfo& t1 = GetSchTableInfo(tab1);
	const SchTableInfo& t2 = GetSchTableInfo(tab2);
	for(int i = 0; i < t1.ref_table.GetCount(); i++)
		if(t1.ref_table[i] == tab2 && MatchRefName(t1.column[i], t2.primary_key, tab2, phase))
			return SqlId(t1.column[i]).Of(SqlId(as1)) == SqlId(t2.primary_key).Of(SqlId(as2));
	for(int i = 0; i < t2.ref_table.GetCount(); i++)
		if(t2.ref_table[i] == tab1 && MatchRefName(t2.column[i], t1.primary_key, tab1, phase))
			return SqlId(t2.column[i]).Of(SqlId(as2)) == SqlId(t1.primary_key).Of(SqlId(as1));
	return SqlBool::False();
}

int sChrf(int c)
{
	return c < 32 ? ' ' : c;
}

StaticMutex sM;

ArrayMap<String, Array<SchColumnInfo>> GetSchAll()
{
	ArrayMap<String, Array<SchColumnInfo>> r;
	Mutex::Lock __(sM);
	for(int i = 0; i < sSchTableInfo().GetCount(); i++)
		if(sSchTableInfo()[i].is_table) {
			Array<SchColumnInfo>& cf = r.Add(sSchTableInfo().GetKey(i));
			const SchTableInfo& h = sSchTableInfo()[i];
			for(int j = 0; j < h.column.GetCount(); j++) {
				SchColumnInfo& f = cf.Add();
				f.name = h.column[j];
				f.references = h.ref_table[j];
				f.foreign_key = Nvl(h.ref_column[j], f.name);
			}
		}
	return r;
}

Vector<String> GetSchTables()
{
	Mutex::Lock __(sM);
	Vector<String> r;
	for(int i = 0; i < sSchTableInfo().GetCount(); i++)
		if(sSchTableInfo()[i].is_table)
			r.Add(sSchTableInfo().GetKey(i));
	return r;
}

Vector<String> GetSchColumns(const String& table)
{
	Mutex::Lock __(sM);
	return clone(GetSchTableInfo(table).column);
}

SqlBool FindSchJoin(const String& tables)
{
	Mutex::Lock __(sM);
	static VectorMap<String, SqlBool> cache;
	if(cache.GetCount() > 20000) // Defend against unlikely dynamic Join permutations
		cache.Clear();
	int q = cache.Find(tables);
	if(q >= 0)
		return cache[q];
	Vector<String> s = Split(Filter(tables, sChrf), ',');
	Vector<String> as;
	Vector<String> table;
	for(int i = 0; i < s.GetCount(); i++) {
		Vector<String> ss = Split(s[i], ' ');
		if(ss.GetCount()) {
			table.Add(ss[0]);
			as.Add(ss.Top());
		}
	}
	if(table.GetCount() >= 2) {
		String tab1 = table.Top();
		String as1 = as.Top();
		for(int phase = 0; phase < 2; phase++)
			for(int i = 0; i < table.GetCount() - 1; i++) {
				SqlBool b = Join(tab1, as1, table[i], as[i], phase);
				if(!b.IsFalse()) {
					cache.Add(tables, b);
					return b;
				}
			}
	}
	NEVER_("Schema join not found " + tables);
	return SqlBool::False();
}

}
