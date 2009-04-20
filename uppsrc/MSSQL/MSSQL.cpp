#include "MSSQL.h"

namespace Upp {

String MsSqlTextType(int width)
{
	if(width <= 4000)
		return NFormat("varchar(%d)", width);
	return "text";
}

};