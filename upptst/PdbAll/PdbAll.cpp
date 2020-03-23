#include <CtrlLib/CtrlLib.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <unordered_set>
#include <unordered_map>

using namespace Upp;

GUI_APP_MAIN
{
	Date date = GetSysDate();
	Time time = GetSysTime();
	Color color = LtRed();
	RGBA rgba = LtRed();
	Font fnt = Arial();
	Image img = CtrlImg::exclamation();
	ImageBuffer ib(img);
	img = CtrlImg::exclamation();
	Value img_value = img;
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
	std::deque<std::string> std_deqeue { "I", "II", "III", "IV", "V" };
	std::list<std::string> std_list { "I", "II", "III", "IV", "V" };
	std::forward_list<std::string> std_forward_list { "I", "II", "III", "IV", "V" };
	std::set<std::string> std_set { "I", "II", "III", "IV", "V" };
	std::multiset<std::string> std_multiset { "I", "II", "III", "IV", "V" };
	std::map<int, std::string> std_map { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::multimap<int, std::string> std_multimap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::unordered_set<std::string> std_unordered_set { "I", "II", "III", "IV", "V" };
	std::unordered_multiset<std::string> std_unordered_multiset { "I", "II", "III", "IV", "V" };
	std::unordered_map<int, std::string> std_unordered_map { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::unordered_multimap<int, std::string> std_unordered_multimap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
}
