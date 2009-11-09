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
	LaunchWebBrowser(GetTempPath());
	DUMP(GUI_PopUpEffect());
	RichEditWithToolBar e;
	Vector<int> h;
	for(int i = 1; i < Font::GetFaceCount(); i++)
		if((Font::GetFaceInfo(i) & Font::SCALEABLE) && !(Font::GetFaceInfo(i) & Font::SPECIAL))
			h.Add(i);
	e.FontFaces(h);

	DUMP(ScrollBarSize());

	e.Pick(ParseQTF(LoadFile(FileName())));
	RichEdit::PosInfo f;
	LoadFromFile(f, ConfigFile("pos"));
	e.SetPosInfo(f);
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
	f = e.GetPosInfo();
	StoreToFile(f, ConfigFile("pos"));
	LOG("Exit");
}
