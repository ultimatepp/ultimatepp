#include "TestChStyle.h"

void Dl(DropList& dl)
{
	dl.Add("Case1");
	dl.Add("Case2");
	dl <<= "Case1";
}

TestChStyle::TestChStyle()
{
	CtrlLayout(*this, "Window title");
	
	readonly <<= "Read only";
	disabled <<= "Disabled";
	disabled.Disable();
	
	Dl(dl_normal);
	Dl(dl_readonly);
	Dl(dl_disabled);
	dl_disabled.Disable();
}

GUI_APP_MAIN
{
	Ctrl::SetSkin(ChStdSkin);
	TestChStyle().Run();
}
