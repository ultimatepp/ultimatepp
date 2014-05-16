#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	DUMP(Environment());

	VectorMap<String, String> map = LoadIniFile(GetDataFile("test.ini"));
	DDUMPM(map);

	const Tuple2<String, String> et[] = {
		{ "alfa", "alfa_value" },
		{ "_beta", "beta_value" },
		{ "gamma", "gamma_value" },
		{ "test", "//TEST" },
		{ "included", "included_value" },
		{ "delta", "delta_value" },
		{ "etest", GetEnv("UPP_MAIN__") },
		{ "braces", GetEnv("UPP_MAIN__")+"123" },
		{ "var", "file://" + GetEnv("UPP_MAIN__") },
	};
	ASSERT(map.GetCount() == __countof(et));
	for(int i = 0; i < map.GetCount(); i++) {
		LOG(map.GetKey(i) << " :: " << et[i].a);
		ASSERT(map.GetKey(i) == et[i].a);
		LOG(map[i] << " :: " << et[i].b);
		ASSERT(map[i] == et[i].b);
	}
	
	LOG("=========== OK");
}
