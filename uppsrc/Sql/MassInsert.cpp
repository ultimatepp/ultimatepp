#include "Sql.h"

namespace Upp {

SqlMassInsert::~SqlMassInsert()
{
	Flush();
}

SqlMassInsert& SqlMassInsert::operator()(SqlId col, const Value& val)
{
	if(pos == 0) {
		cache.Add().nulls = 0;
		cache.Top();
	}
	if(cache.GetCount() == 1)
		column.Add(~col);
	else
		ASSERT(column[pos] == col.Quoted() || column[pos] == ~col);
	Row& r = cache.Top();
	r.value.Add(val);
	if(IsNull(val))
		r.nulls |= ((uint64)1 << pos);
	pos++;
	ASSERT(pos < 62);
	return *this;
}

SqlMassInsert& SqlMassInsert::operator()(const ValueMap& data)
{
	for(int i = 0; i < data.GetCount(); i++)
		operator()((String)data.GetKey(i), data.GetValue(i));
	return *this;
}

SqlMassInsert& SqlMassInsert::EndRow(SqlBool remove)
{
	cache.Top().remove = remove;
	if(pos == 0)
		return *this;
	if(cache.GetCount() && cache[0].value.GetCount() * cache.GetCount() > 5000 || cache.GetCount() > 990) // MSSQL maximum is 1000
		Flush();
	ASSERT(column.GetCount() == pos);
	pos = 0;
	return *this;
}

void SqlMassInsert::Flush()
{
	const uint64 DONE = (uint64)-1;
	if(cache.GetCount() == 0)
		return;
	if(use_transaction)
		sql.GetSession().Begin();
	SqlBool remove;
	bool doremove = false;
	for(int ii = 0; ii < cache.GetCount(); ii++) {
		SqlBool rm = cache[ii].remove;
		if(!rm.IsEmpty()) {
			doremove = true;
			remove = remove || rm;
		}
	}
	if(doremove)
		sql * Delete(table).Where(remove);
	String insert;
	int dialect = sql.GetDialect();
	if(findarg(dialect, MY_SQL, PGSQL, MSSQL) >= 0) {
		insert << "insert into " + ~table + '(';
		for(int i = 0; i < column.GetCount(); i++) {
			if(i)
				insert << ", ";
			insert << column[i];
		}
		insert << ") values ";
		for(int i = 0; i < cache.GetCount(); i++) {
			Row& r = cache[i];
			if(r.value.GetCount()) {
				if(i)
					insert << ", ";
				insert << "(";
				for(int i = 0; i < r.value.GetCount(); i++) {
					if(i)
						insert << ", ";
					insert << SqlCompile(dialect, SqlFormat(r.value[i]));
				}
				insert << ")";
			}
		}
	}
	else
	for(int ii = 0; ii < cache.GetCount(); ii++) {
		uint64 nulls = cache[ii].nulls;
		if(nulls != DONE) {
			insert << "insert into " + ~table + '(';
			bool nextcol = false;
			for(int i = 0; i < column.GetCount(); i++) {
				if(!(nulls & ((uint64)1 << i))) {
					if(nextcol)
						insert << ", ";
					nextcol = true;
					insert << column[i];
				}
			}
			insert << ')';
			bool nextsel = false;
			for(int i = ii; i < cache.GetCount(); i++) {
				Row& r = cache[i];
				if(r.nulls == nulls && r.value.GetCount()) {
					r.nulls = DONE;
					if(nextsel)
						insert << " union all";
					nextsel = true;
					insert << " select ";
					bool nextval = false;
					for(int i = 0; i < r.value.GetCount(); i++)
						if(!(nulls & ((uint64)1 << i))) {
							if(nextval)
								insert << ", ";
							nextval = true;
							insert << SqlCompile(dialect, SqlFormat(r.value[i]));
						}
					if(dialect == ORACLE)
						insert << " from dual";
				}
			}
		}
	}
	sql.Execute(insert);
	if(sql.WasError()) {
		error = true;
		if(use_transaction)
			sql.GetSession().Rollback();
	}
	else
		if(use_transaction)
			sql.GetSession().Commit();
	cache.Clear();
	column.Clear();
	pos = 0;
}

}

