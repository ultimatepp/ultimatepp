#include <Oracle/Oracle7.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Oracle7 oracle;
	oracle.Open("scott/tiger@cxl"); // adjust for your Oracle server
	Sql sql(oracle);
	sql.Execute("select ENAME, JOB from EMP");
	while(sql.Fetch())
		Cout() << sql[0] << ": " << sql[1];
}
