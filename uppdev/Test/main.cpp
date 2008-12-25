#include "Test.h"



Test::Test()
{
	CtrlLayout(*this, "Window title");
	
	TreeCtrl::Node p;
	
	int n;
	
	n = t.Add(0, "DUMMY1");
	t.SetLabel(n, "Label");
	p = t.GetNode(n);
	p.Set("real1");
	t.SetNode(n, p);
	
	n = t.Add(0, "DUMMY2");
	p = t.GetNode(n);
	p.Set("real2");
	t.SetNode(n, p);
	
	n = t.Add(0, "DUMMY3");
	p = t.GetNode(n);
	p.Set("real3");
	t.SetNode(n, p);
	
	t.Open(0);
}

GUI_APP_MAIN
{
	for(int i = -100; i < 100; i++)
		DLOG(i << " " << i % 10 << " " << i % -10);
	return;
	Test().Run();
}

