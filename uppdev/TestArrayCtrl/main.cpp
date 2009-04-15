#include "Test.h"

TestTabCtrl::TestTabCtrl(){

	CtrlLayout( *this, "Test" );
	CtrlLayout( _testArrayCtrl );

	_tab.Add( _testArrayCtrl, t_("Table of SQL Array") );
	_tab.Set(0);
	
	Sizeable();
}


GUI_APP_MAIN
{	
	TestTabCtrl t;
	t.Open();
	t.Run();
	TopWindow win;
	StaticRect r;
	win.Add(t._testArrayCtrl.SizePos());
//	r.Add(t._testArrayCtrl.HSizePos(150, 150).VSizePos(100, 100).SizePos());
	win.SetRect(100, 100, 300, 200);
	win.Sizeable().Run();
}
