#include <RichEdit/RichEdit.h>

using namespace Upp;

String FileName()
{
//	return "d:/test.qtf";
	return GetExeDirFile("test.qtf");
}


GUI_APP_MAIN
{
//	ChClassicSkin();
//	ChStdSkin();
//	SetLanguage(LNG_('P','L','P','L'));

//	InstallCrashDump();

	DUMP(GUI_PopUpEffect());
	RichEditWithToolBar e;

	DUMP(ScrollBarSize());

	e.Pick(ParseQTF(LoadFile(FileName())));
//	e.Pick(ParseQTF(AsQTF(CreateImageObject(CtrlImg::exclamation)) + " hahahaha"));
	TopWindow w;
	w.SetRect(0, 0, 700, 500);
	w.Sizeable().Zoomable();
	w.Add(e.SizePos());
	w.Run();
/*	
	String strQTF = e.GetQTF(CHARSET_ISO8859_1);
	e.SetQTF(strQTF);
	w.Run();
*/
	String x = e.GetQTF();
	SaveFile(FileName(), x);
	LOG("Exit");
}
