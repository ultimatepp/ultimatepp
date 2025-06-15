#include <RichEdit/RichEdit.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow app;
	app.Sizeable().Zoomable();
	DiagramEditor de;
	app.Add(de.SizePos());
	String fn = "c:/xxx/test.qdf";
	de.Load(LoadFile(fn));
	app.Run();
	SaveFile(fn, de.Save());
}
