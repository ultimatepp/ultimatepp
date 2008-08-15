#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	TopWindow w;
	LNGCtrl c;
	w.Add(c.SizePos());
	w.Run();
	DUMP(LNGAsText(~c));
}
