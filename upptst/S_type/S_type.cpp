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
			Ref f = test.GetRef(i);
			if(!f.Is<bool>())
				ASSERT(IsNull(test.Get(i)));
			Value v;
			if(f.Is<bool>())
				v = (bool)(i % 2);
			else
			if(f.Is<int>())
				v = i + 1000;
			else
				v = "text " + AsString(i);
			m.Add(test.GetId(i), v);
			test.Set(i, v);
			ASSERT(test.Get(i) == v);
		}
		DUMP(test);
		DUMP(test.Get());
		ASSERT(test.Get() == m);
	}
	
	test.Set(FLAG, "1");
	ASSERT(test.FLAG);
	test.Set(FLAG, "0");
	ASSERT(!test.FLAG);
	
	DUMP(test.GetWidth(TEXT1));
	ASSERT(test.GetWidth(TEXT1) == 200);
	DUMP(test.GetWidth(FLAG));
	ASSERT(test.GetWidth(FLAG) == 0);
	
	LOG("------------------------");
	ASSERT(test.Get(TEST).IsVoid());
	
	ValueMap m;
	m(TEXT, "texttest")(NUMBER1, 123456)(A[2], 10)(FLAG, true)(FLAG2, "1");
	test.Set(m);
	DUMP(test);
	test = m;
	DUMP(test);
	
	DDUMPC(S_TEST::GetColumnIds());
	DDUMP(SqlCompile(SQLITE3, ~S_TEST::ColumnSet()));
	ASSERT(SqlCompile(SQLITE3, ~S_TEST::ColumnSet()) == "NUMBER1, TEXT1, A0, A1, A2, A3, A4, FLAG, FLAG2, NUMBER2, TEXT2, NUMBER, TEXT");
	DDUMP(SqlCompile(SQLITE3, ~S_TEST::ColumnSet("PREFIX_")));
	ASSERT(SqlCompile(SQLITE3, ~S_TEST::ColumnSet("PREFIX_")) == "PREFIX_NUMBER1, PREFIX_TEXT1, PREFIX_A0, PREFIX_A1, PREFIX_A2, PREFIX_A3, PREFIX_A4, PREFIX_FLAG, PREFIX_FLAG2, PREFIX_NUMBER2, PREFIX_TEXT2, PREFIX_NUMBER, PREFIX_TEXT");
	DDUMP(SqlCompile(SQLITE3, ~S_TEST::Of("TABLE")));
	ASSERT(SqlCompile(SQLITE3, ~S_TEST::Of("TABLE")) == "TABLE.NUMBER1, TABLE.TEXT1, TABLE.A0, TABLE.A1, TABLE.A2, TABLE.A3, TABLE.A4, TABLE.FLAG, TABLE.FLAG2, TABLE.NUMBER2, TABLE.TEXT2, TABLE.NUMBER, TABLE.TEXT");
	
	LOG("--------- FINISHED");
}
