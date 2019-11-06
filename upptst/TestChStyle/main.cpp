#include "TestChStyle.h"

void Dl(DropList& dl)
{
	dl.Add("Case1");
	dl.Add("Case2");
	dl <<= "Case1";
}

void Dc(WithDropChoice<EditString>& dc)
{
	dc.AddList("Case1");
	dc.AddList("Case2");
	dc <<= "Case1";
}


TestChStyle::TestChStyle()
{
	CtrlLayout(*this, "Window title");
	
	normal.NullText("Normal");
	readonly <<= "Read only";
	disabled <<= "Disabled";
	disabled.Disable();
	
	Dl(dl_normal);
	Dl(dl_readonly);
	Dl(dl_disabled);
	dl_disabled.Disable();

	Dc(dc_normal);
	Dc(dc_readonly);
	Dc(dc_disabled);
	dc_disabled.Disable();

#ifdef CPP_11
	standard << [] { Ctrl::SetSkin(ChStdSkin); };
	classic << [] { Ctrl::SetSkin(ChClassicSkin); };
	host << [] { Ctrl::SetSkin(ChHostSkin); };
#endif

	for(int i = 0; i < 100; i++)
		tab.Add("Tab " + AsString(i));
}

GUI_APP_MAIN
{
	Ctrl::SetDarkThemeEnabled();
	
	TestChStyle().Run();
}
