#include <Sql/Sql.h>

using namespace Upp;
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <S_type/S_type.sch>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>
#include <Sql/sch_schema.h>

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	S_TEST test;
	
	test.GetRef(TEXT) = "Hello!";
	test.GetRef(TEXT1) = "text1!";
	DUMP(test.Get(TEXT));
	DUMP(test.Get());
	DUMP(test);
	
	S_TEST test2 = test;
	ASSERT(test2 == test);
	ASSERT(test2.Get() == test.Get());
	test2.NUMBER = 1;
	ASSERT(test2 != test);
	ASSERT(test2.Get() != test.Get());
	
	LOG("------------------------");
	{
		ValueMap m;
		test.Clear();
		DUMP(test);
		for(int i = 0; i < test.GetCount(); i++) {
			ASSERT(IsNull(test.Get(i)));
			Ref f = test.GetRef(i);
			Value v;
			if(f.Is<int>())
				v = i + 1000;
			else
				v = "text " + AsString(i);
			m.Add(test.GetId(i), v);
			test.Set(i, v);
		}
		DUMP(test);
		DUMP(test.Get());
		ASSERT(test.Get() == m);
	}
	
	LOG("------------------------");
	ASSERT(test.Get(TEST).IsVoid());
	
	ValueMap m;
	m(TEXT, "texttest")(NUMBER1, 123456)(A[2], 10);
	test.Set(m);
	DUMP(test);
	test = m;
	DUMP(test);
	
	DDUMPC(S_TEST::GetColumnIds());
	DDUMP(SqlCompile(SQLITE3, ~S_TEST::ColumnSet()));
	DDUMP(SqlCompile(SQLITE3, ~S_TEST::ColumnSet("PREFIX_")));
	DDUMP(SqlCompile(SQLITE3, ~S_TEST::Of("TABLE")));
}
