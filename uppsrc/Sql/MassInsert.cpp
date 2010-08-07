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
		ASSERT(column[pos] == ~col);
	Row& r = cache.Top();
	r.value.Add(val);
	if(IsNull(val))
		r.nulls |= (1 << pos);
	pos++;
	ASSERT(pos < 30);
	return *this;
}

SqlMassInsert& SqlMassInsert::EndRow()
{
	if(cache.GetCount() && cache[0].value.GetCount() * cache.GetCount() > 5000)
		Flush();
	ASSERT(column.GetCount() == pos);
	pos = 0;
	return *this;
}

void SqlMassInsert::Flush()
{
	const dword DONE = 0xffffffff;
	if(cache.GetCount() == 0)
		return;
	for(int ii = 0; ii < cache.GetCount(); ii++) {
		dword nulls = cache[ii].nulls;
		if(nulls != DONE) {
			String insert;
			insert << "insert into " + table + '(';
			bool nextcol = false;
			for(int i = 0; i < column.GetCount(); i++) {
				if(!(nulls & (1 << i))) {
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
				if(r.nulls == nulls) {
					r.nulls = DONE;
					if(nextsel)
						insert << " union all";
					nextsel = true;
					insert << " select ";
					bool nextval = false;
					for(int i = 0; i < r.value.GetCount(); i++)
						if(!(nulls & (1 << i))) {
							if(nextval)
								insert << ", ";
							nextval = true;
							insert << SqlCompile(sql.GetDialect(), SqlFormat(r.value[i]));
						}
					if(sql.GetDialect() == ORACLE)
						insert << " from dual";
				}
			}
			sql.Execute(insert);
		}
	}
	cache.Clear();
	column.Clear();
	pos = 0;
}

};
