#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	EditString es;
	TopWindow win;
<<<<<<< .mine
	String h;
	h << "CCCCCCCC";
	es <<= h;
=======
	es <<= "CCCCCCCCCCCCCCC";
>>>>>>> .r5593
	win.Add(es.HSizePos().TopPos(0, Ctrl::STDSIZE));
	win.Run();
	
	DDUMP(StoreAsXML(es));
	DDUMP(StoreAsJson(es, true));
}
