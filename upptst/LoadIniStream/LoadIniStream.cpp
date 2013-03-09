#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	VectorMap<String, String> map = LoadIniFile(GetDataFile("test.ini"));
	DDUMPM(map);

	const Tuple2<const char *, const char *> et[] = {
		{ "alfa", "alfa_value" },
		{ "_beta", "beta_value" },
		{ "gamma", " gamma_value" },
		{ "included", "included_value" },
		{ "delta", "delta_value" },
	};
	ASSERT(map.GetCount() == __countof(et));
	for(int i = 0; i < map.GetCount(); i++) {
		ASSERT(map.GetKey(i) == et[i].a);
		ASSERT(map[i] == et[i].b);
	}
}
