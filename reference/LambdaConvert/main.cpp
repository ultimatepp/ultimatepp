#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.AddColumn().SetConvert(LambdaConvert([](const Value& v) { return FormatIntRoman(v); }));
	for(int i = 0; i < 100; i++)
		list.Add(i);

	TopWindow win;
	win << list.SizePos();
	win.Run();
}
