#include <CtrlLib/CtrlLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TopWindow top;
	Label lbl;
	EditInt edit;
	StaticRect list;
	auto Print = [&] {
		DLOG("==============================");
		for(Ctrl& q : top)
			DLOG(typeid(q).name());
	};

	top << lbl << edit;
	Print();
	top.AddChild(&list, &lbl);
	Print();
	lbl.Remove();
	Print();
	edit.Ctrl::Remove();
	Print();
	list.Ctrl::Remove();
	Print();
	
	CheckLogEtalon();
}
