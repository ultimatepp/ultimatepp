#include <Core/Core.h>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace Upp;

CONSOLE_APP_MAIN
{
	VectorMap<int, String> vectormap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::map<int, std::string> std_map { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
}
