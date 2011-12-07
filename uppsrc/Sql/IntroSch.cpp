#include "Sql.h"

NAMESPACE_UPP

ArrayMap<String, SchTableInfo>& sSchTableInfo()
{
	static ArrayMap<String, SchTableInfo> x;
	return x;
}

SchTableInfo& SchTableInfo::Column(const char *name)
{
	column.Add(name);
	ref_table.Add();
	ref_column.Add();
	return *this;
}

SchTableInfo& SchTableInfo::References(const char *table)
{
	ref_table.Top() = table;
	return *this;
}

SchTableInfo& SchTableInfo::References(const char *table, const char *column)
{
	References(table);
	ref_column.Top() = column;
	return *this;
}

SchTableInfo& SchDbInfo(const char *table)
{
	return sSchTableInfo().GetAdd(table);
}

const SchTableInfo& GetSchTableInfo(const String& table)
{
	static SchTableInfo sSchTableInfoZero;
	return sSchTableInfo().Get(~table, sSchTableInfoZero);
}

SqlBool Join(const String& tab1, const String& tab2)
{
	const SchTableInfo& t1 = GetSchTableInfo(tab1);
	const SchTableInfo& t2 = GetSchTableInfo(tab2);
	for(int i = 0; i < t1.ref_table.GetCount(); i++)
		if(t1.ref_table[i] == tab2)
			return SqlId(t1.column[i]).Of(SqlId(tab1)) == SqlId(t2.column[0]).Of(SqlId(tab2));
	for(int i = 0; i < t2.ref_table.GetCount(); i++)
		if(t2.ref_table[i] == tab1)
			return SqlId(t2.column[i]).Of(SqlId(tab2)) == SqlId(t1.column[0]).Of(SqlId(tab1));
	return SqlBool::False();
}

SqlBool FindSchJoin(const String& tables)
{
	INTERLOCKED {
		static VectorMap<String, SqlBool> cache;
		if(cache.GetCount() > 20000)
			cache.Clear();
		int q = cache.Find(tables);
		if(q >= 0)
			return cache[q];
		Vector<String> s = Split(tables, ',');
		if(s.GetCount() >= 2) {
			String tab1 = s.Top();
			for(int i = 0; i < s.GetCount() - 1; i++) {
				SqlBool b = Join(tab1, s[i]);
				if(!b.IsFalse()) {
					cache.Add(tables, b);
					return b;
				}
			}
		}
		NEVER();
		return SqlBool::False();
	}
}

END_UPP_NAMESPACE