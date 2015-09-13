#include "CtrlLib/CtrlLib.h"

using namespace Upp;

#define LAYOUTFILE <Cpp11/Cpp11.lay>
#include <CtrlCore/lay.h>

#ifdef CPP_11

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

#else

struct MyApp : WithMyAppLayout<TopWindow> {
	typedef MyApp CLASSNAME;

	void Add() {
		if(list.Find(~number) < 0)
			list.Add(~number);
	}
	void Sel() {
		number <<= list.GetKey();
	}
	MyApp() {
		CtrlLayout(*this, "C++ demo");
		list.NoHeader().AddColumn();
		Vector<int> x;
		x << 1 << 2 << 12 << 34 << 15 << 11;
		for(int i = 0; i < x.GetCount(); i++)
			list.Add(x[i]);
		add <<= THISBACK(Add);
		list.WhenSel = THISBACK(Sel);
	}
};

GUI_APP_MAIN
{
	MyApp().Execute();;
}

#endif
