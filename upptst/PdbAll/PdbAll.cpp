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

void UppTypes()
{
	Date date = GetSysDate();
	Time time = GetSysTime();
	Color color = LtRed();
	RGBA rgba = LtRed();
	Font fnt = Arial();
	Image img = CtrlImg::exclamation();
	ImageBuffer ib(img);
	img = CtrlImg::exclamation();
	String string = "Hello world!";
	WString wstring = "Hello world!";

	DLOG("BREAKPOINT HERE");
}

void UppValue()
{
	ValueArray valuearray { "I", "II", "III", "IV", "V" };
	ValueMap valuemap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };

	Value img_value = CtrlImg::exclamation();
	Value value = valuemap;
	Value text_value = "Test";
	Value error_value = ErrorValue("Test");
	Value void_value;

	DLOG("BREAKPOINT HERE");
}

void UppContainers()
{
	Vector<String> vector { "I", "II", "III", "IV", "V" };
	BiVector<String> bivector { "I", "II", "III", "IV", "V" };
	Array<String> array { "I", "II", "III", "IV", "V" };
	BiArray<String> biarray { "I", "II", "III", "IV", "V" };
	Index<String> index { "I", "II", "III", "IV", "V" };
	VectorMap<int, String> vectormap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	ArrayMap<int, String> arraymap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };

	DLOG("BREAKPOINT HERE");
}

void StdTypes()
{
	WString wstring = "Hello world!";
	std::string std_string = "Hello world!";
	std::wstring std_wstring = wstring.ToStd();
	
	std::atomic<int> atomic_int;
	atomic_int.store(12345);
	std::atomic<int *> atomic_ptr;
	int x = 54321;
	atomic_ptr.store(&x);

	DLOG("BREAKPOINT HERE");
}

void StdContainers()
{
	std::vector<std::string> std_vector { "I", "II", "III", "IV", "V" };
	std::deque<std::string> std_deqeue { "I", "II", "III", "IV", "V" };
	std::list<std::string> std_list { "I", "II", "III", "IV", "V" };
	std::forward_list<std::string> std_forward_list { "I", "II", "III", "IV", "V" };
	std::set<std::string> std_set { "I", "II", "III", "IV", "V" };
	std::multiset<std::string> std_multiset { "I", "II", "III", "IV", "V" };
	std::map<int, std::string> std_map { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::multimap<int, std::string> std_multimap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::map<std::string, int> std_map2 { { "I", 1 }, { "II", 2 }, { "III", 3 }};
	std::multimap<std::string, int> std_multimap2 { { "I", 1 }, { "II", 2 }, { "III", 3 }};
	std::unordered_set<std::string> std_unordered_set { "I", "II", "III", "IV", "V" };
	std::unordered_multiset<std::string> std_unordered_multiset { "I", "II", "III", "IV", "V" };
	std::unordered_map<int, std::string> std_unordered_map { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };
	std::unordered_multimap<int, std::string> std_unordered_multimap { { 1 , "I"}, { 2, "II" }, { 3, "III"}, { 4, "IV"}, { 5, "V" } };

	DLOG("BREAKPOINT HERE");
}

GUI_APP_MAIN
{
	UppTypes();
	UppValue();
	UppContainers();
	
	StdTypes();
	StdContainers();

	for(String h : { "Hello world!", "Very very very very very very very long hello world!" }) {
		std::string sh = h.ToStd();
		WString wh = h.ToWString();
		std::wstring swh = wh.ToStd();
		
		DDUMP(h);
		DDUMP(sh);
		DDUMP(wh);
		DDUMP(swh);
	}
}
