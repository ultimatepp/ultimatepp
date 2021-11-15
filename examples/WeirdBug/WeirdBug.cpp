#include <Core/Core.h>

using namespace Upp;

Vector<ValueMap> g_turbine_data; // one for turbine*scenario

CONSOLE_APP_MAIN
{
	String data = LoadFile("D:/xxx/td.bin");
	RDUMP(data.GetCount());
	LoadFromString(g_turbine_data, data);
	RDUMP(g_turbine_data);
}
