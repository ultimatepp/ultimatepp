#include "CtrlLib/CtrlLib.h"

using namespace Upp;

#define LAYOUTFILE <Cpp11/Cpp11.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	WithMyAppLayout<TopWindow> dlg;
	CtrlLayout(dlg, "C++11 demo");
	dlg.list.NoHeader().AddColumn();
	Vector<int> x = { 1, 2, 12, 34, 15, 11 };
	for(auto i : x)
		dlg.list.Add(i);
	dlg.add << [&] {
		if(dlg.list.Find(~dlg.number) < 0)
			dlg.list.Add(~dlg.number);
	};
	dlg.list.WhenSel << [&] {
		dlg.number <<= dlg.list.GetKey();
	};
	dlg.Execute();
}
