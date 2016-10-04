#include <CtrlLib/CtrlLib.h>

using namespace Upp;

namespace Upp {
Image GetFileIcon(const char *path, bool dir, bool force);
};

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.SetLineCy(30);
	a.AddColumn("Icon").SetDisplay(CenteredImageDisplay());
	a.AddColumn("File");
	for(int d = 'A'; d < 'Z'; d++)
		a.Add(GetFileIcon(String(d, 1) + ":\\", false, true), "");

	DUMP(GetFileIcon("C:\\", false, true).GetSize());

	FindFile ff("g:\\*.*");
	while(ff) {
		String p = "g:\\" + ff.GetName();
		a.Add(GetFileIcon(p, false, false), p);
		DUMP(GetFileIcon(p, false, false).GetSize());
		ff.Next();
	}
	TopWindow win;
	win.Add(a.SizePos());
	win.Run();
}
