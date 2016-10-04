#include "DropTest.h"



DropTest::DropTest()
{
	CtrlLayout(*this, "Window title");
	edit <<= "DropList as frame test package.";
	drop.Add(0, "Item 1").Add(1, "Item 2").Add(2, "Item 3").Add(3, "Item 4") <<= 2;
	edit.AddFrame(drop.Height(16));
//	drop.SetFrame(BlackFrame());
}

GUI_APP_MAIN
{
	DropTest().Run();
}
