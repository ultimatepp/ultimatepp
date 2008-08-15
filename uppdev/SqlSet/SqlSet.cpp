#include <Core/Core.h>
#include <Sql/Sql.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Value v1 = 10;
	Value v2 = "Daniel";
	SqlInsert q = Insert("table")
		("col1", v1)
		("col2", v2);
	LOG(((SqlStatement) q).Get(ORACLE));
}
