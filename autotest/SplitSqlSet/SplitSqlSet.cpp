#include <Sql/Sql.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	SqlSet set1;
	SQLID(TEST)
	set1 << TEST.As("TEST")
	    << GetSysTime()
	    << GetSysDate()
	    << Time(1970, 1, 1, 0, 0, 0)
	    << "'Hello', \"friend\"!"
	    << 123
	    << 123.23
	    << (TEST|TEST)
	    << NextVal(TEST)
	    << ConvertAscii(TEST|"test")
	    << TEST * 4
	    << Select(Greatest(TEST, TEST + 1)).From(TEST).AsValue()
	    << SqlBinary(String(0, 4))
	    << TEST * 4
	    << NextVal(TEST)
	;
//	DUMPHEX(~set);
	Vector<SqlVal> v = SplitSqlSet(set1);
	SqlSet set2;
	for(int i = 0; i < v.GetCount(); i++) {
		LOG(i << " oracle " << SqlCompile(ORACLE, ~v[i]));
		LOG(i << " MSSQL " << SqlCompile(MSSQL, ~v[i]));
		DUMPHEX(~v[i]);
		LOG("----------------------------");
		set2 << v[i];
	}
//	DUMPHEX(~set2);
	DDUMP(SqlCompile(ORACLE, ~set1));
	DDUMP(SqlCompile(ORACLE, ~set2));

	DDUMP(SqlCompile(MSSQL, ~set1));
	DDUMP(SqlCompile(MSSQL, ~set2));
	
	ASSERT(SqlCompile(ORACLE, ~set1) == SqlCompile(ORACLE, ~set2));
	ASSERT(SqlCompile(MSSQL, ~set1) == SqlCompile(MSSQL, ~set2));
	
	LOG("=================== OK");
}
