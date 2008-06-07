#include <CtrlLib/CtrlLib.h>

using namespace Upp;

static String sNumberAsText(int number)
{
	static const char * const digits[20] = {
		"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
		"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
		"seventeen", "eighteen", "nineteen"
	};
	static const char * const tens[10] = {
		"", "ten", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety"
	};

	if(number < 20)
		return digits[number];
	if(number < 100)
		return tens[number / 10] + String(" ") + sNumberAsText(number % 10);
	if(number < 1000)
		return digits[number / 100] + String(" hundred ") + sNumberAsText(number % 100);
	if(number < 1000000)
		return sNumberAsText(number / 1000) + " thousand, " + sNumberAsText(number % 1000);
	return "";
}

struct NumberToText : public Convert {
	virtual Value  Format(const Value& q) const {
		int n = q;
		return n == 0 ? String("zero") : sNumberAsText(n);
	}
};

GUI_APP_MAIN
{
	ArrayCtrl array;
	array.AddRowNumColumn("number", 20);
	array.AddRowNumColumn("text", 80).SetConvert(Single<NumberToText>());
	array.SetVirtualCount(900000);
	TopWindow win;
	win.Zoomable().Sizeable();
	win.Add(array.SizePos());
	win.Run();
}
