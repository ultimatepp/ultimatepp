#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	EditString es;
	TopWindow win;
	es <<= "devout eeeev";
	es.AlignRight();
	win.Add(es.HSizePos().TopPos(0, Ctrl::STDSIZE));
	win.Run();
	
	DUMP(StoreAsXML(es));
	DUMP(StoreAsJson(es, true));
}
