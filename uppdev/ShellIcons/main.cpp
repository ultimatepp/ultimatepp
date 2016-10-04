#include <CtrlLib/CtrlLib.h>

using namespace Upp;

Image GetShellIcon(int ii, bool large)
{
	HICON hIcon = NULL;
	const char *kpath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Icons";
	Vector<String> s = Split(GetWinRegString(AsString(ii), kpath), ',');
	String dll = "shell32.dll";
	if(s.GetCount() == 2) {
		dll = s[0];
		ii = atoi(s[1]);
	}
	return Win32DllIcon(dll, ii, large);
}

struct App : TopWindow {
	ArrayCtrl h;
	
	App() {
		Sizeable().Zoomable();
		h.AddColumn("Icon no.");
		h.AddColumn("Small icon").SetDisplay(CenteredImageDisplay());
		h.AddColumn("Large icon").SetDisplay(CenteredImageDisplay());
		h.SetLineCy(32);
		for(int i = 0; i < 200; i++)
			h.Add(i, GetShellIcon(i, false), GetShellIcon(i, true));
		Add(h.SizePos());
	}
};

GUI_APP_MAIN {
	App().Run();
}