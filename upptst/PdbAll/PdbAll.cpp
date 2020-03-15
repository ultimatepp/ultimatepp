#include <CtrlLib/CtrlLib.h>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace Upp;

GUI_APP_MAIN
{
	Date date = GetSysDate();
	Time time = GetSysTime();
	Color color = LtRed();
	RGBA rgba = LtRed();
	Font fnt = Arial();
	String string = "Hello world!";
	WString wstring = "Hello world!";
	Vector<String> vector { "I", "II", "III", "IV", "V" };
	BiVector<String> bivector { "I", "II", "III", "IV", "V" };
	Array<String> array { "I", "II", "III", "IV", "V" };
	BiArray<String> biarray { "I", "II", "III", "IV", "V" };
	Index<String> index { "I", "II", "III", "IV", "V" };
	VectorMap<int, String> vectormap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	ArrayMap<int, String> arraymap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	ValueArray valuearray { "I", "II", "III", "IV", "V" };
	ValueMap valuemap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	Value value = valuemap;

	std::string std_string = "Hello world!";
	std::wstring std_wstring = wstring.ToStd();
	std::vector<std::string> std_vector { "I", "II", "III", "IV", "V" };
	std::set<std::string> std_set { "I", "II", "III", "IV", "V" };
	std::multiset<std::string> std_multiset { "I", "II", "III", "IV", "V" };
	std::map<int, std::string> std_map { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::multimap<int, std::string> std_multimap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
}
