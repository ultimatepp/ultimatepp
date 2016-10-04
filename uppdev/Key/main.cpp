#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Label l;
	String k;

	void Do() {
		static int ii;
		String x;
		if(GetCtrl())
			x << "Ctrl ";
		if(GetAlt())
			x << "Alt ";
		if(GetShift())
			x << "Shift ";
		x << k << ' ' << GetMousePos();
		l = x;
	}

	bool Key(dword key, int count) {
		k = GetKeyDesc(key) + ' ' + FormatIntHex(key);
		if(key < 256)
			k << '\"' << (char)key << '\"';
		Do();
	}

	typedef MyApp CLASSNAME;

	MyApp() {
		Add(l.SizePos());
		SetTimeCallback(-100, THISBACK(Do));
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
