#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void Do()
{
	PromptOK("OK!");
}

void MiniMenu(Bar& bar)
{
	bar.Add("Just a test", callback(Do));
}

GUI_APP_MAIN
{
	HeaderCtrl header;
	header.Add("1", 250).WhenAction = callback(Do);
	header.Add("2", 350).WhenBar = callback(MiniMenu);
	header.Add("3", 250);
	header.Add("4", 150);
	header.Add("5", 350);
	header.Moving();
//	header.Absolute();
	
	TopWindow win;
	win.AddFrame(header);
	win.Run();
};
