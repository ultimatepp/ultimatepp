#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	
	TopWindow win;

	int y = 0;
	auto add = [&](Ctrl& c) {
		y += 25;
		win.Add(c.TopPos(y, Zx(16)).LeftPos(10, Zx(100)));
	};
	
	DropList a, b, c;
	
	a.Add(1);
	add(a);
	

	b.Add(1);
	b.AddButton().Left().SetLabel("1");
	add(b);

	c.Add(1);
	c.AddButton().Left().SetLabel("1");
	c.AddButton().Left().SetLabel("2");
	c.AddButton().SetLabel("3");
	add(c);

	WithDropChoice<EditInt> aa, bb, cc;
	
	aa.AddList(1);
	add(aa);
	

	bb.AddList(1);
	bb.AddButton().Left().SetLabel("1");
	add(bb);

	cc.AddList(1);
	cc.AddButton().Left().SetLabel("1");
	cc.AddButton().Left().SetLabel("2");
	cc.AddButton().SetLabel("3");
	add(cc);
	
	win.Run();
}
