#include "testOnglet.h"



testOnglet::testOnglet()
{
	CtrlLayout(*this, "Window title");
	Sizeable();
	//Add(tabs.SizePos());
	CtrlLayout(tab1);
	CtrlLayout(tab2);

	tabs.Add(tab1, "first");
	tabs.Add(tab2, "second");
	
	WithTab1<ParentCtrl>* dynTab = new WithTab1<ParentCtrl>();
	CtrlLayout(*dynTab);
	dynTab->SizePos();
	tabs.Add(*dynTab, "third");

	// MENU BAR
	AddFrame(menuBar);
	menuBar.Set(THISBACK(mainBar));

	// BOTTOM STATUS BAR
//	statusBar.SetFrame(TopSeparatorFrame());
	AddFrame(statusBar);
	
	bClose << THISBACK(closeOnglet);
}

void testOnglet::closeOnglet()
{
	if (tabs.GetCount() > 0) tabs.Remove(0);
}

void testOnglet::mainBar(Bar& bar)
{
	bar.Add(t_("File"), THISBACK(Close));
	bar.Add(t_("Data"), THISBACK(Close));
}

GUI_APP_MAIN
{
	testOnglet().Run();
}

