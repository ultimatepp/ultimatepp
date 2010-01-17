#include "TabBarCtrlTest.h"

class TabBarTest : public WithTabBarTestLayout<TopWindow>
{
	private:
		TabBarCtrl tabCtrl;
		WithFirstLayout<ParentCtrl> first;
		WithSecondLayout<ParentCtrl> second;
		WithBaseLayout<ParentCtrl> base;
		
		void actionCb(void);
		
		bool cancelCloseCb(Vector<int>tabs);
		void closeCb(Vector<int>tabs);
		
	public:
		typedef TabBarTest CLASSNAME;

		TabBarTest();
};

TabBarTest::TabBarTest()
{
	CtrlLayout(*this, "Window title");
	CtrlLayout(first);
	CtrlLayout(second);
	CtrlLayout(base);
	Add(tabCtrl.SizePos());
	Sizeable().Zoomable();

	// add a fixed control to TabBarCtrl
	tabCtrl.Add(base);
	
	// setup actions
	tabCtrl.WhenSet = THISBACK(actionCb);
	tabCtrl.WhenClose = THISBACK(closeCb);
	tabCtrl.CancelClose = THISBACK(cancelCloseCb);
	
	tabCtrl.Add(first, "First");
	tabCtrl.Add(second, "Second");
	tabCtrl.Add("Third");
	for(int i = 3; i < 7; i++)
		tabCtrl.Add("Item#" + FormatInt(i));
	
}

void TabBarTest::actionCb(void)
{
	int i = tabCtrl.Get();
	base.txt.SetText(base.txt.GetText() + Format("Pressed tab %d\n", i));
}

bool TabBarTest::cancelCloseCb(Vector<int>tabs)
{
	String s = "Close item(s) ";
	for(int i = 0; i < tabs.GetCount(); i++)
		s += FormatInt(tabs[i]) + ", ";
	s = s.Left(s.GetCount() - 2) + " ?";
	if(PromptYesNo(s))
		return false;
	return true;
}

void TabBarTest::closeCb(Vector<int>tabs)
{
	String s = "Closing item(s) ";
	for(int i = 0; i < tabs.GetCount(); i++)
		s += FormatInt(tabs[i]) + ", ";
	s = s.Left(s.GetCount() - 2);
	PromptOK(s);
}

GUI_APP_MAIN
{
	TabBarTest tabBarTest;
	
	tabBarTest.Run();
}

