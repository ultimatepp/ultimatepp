#include <RichEdit/RichEdit.h>

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture

#include <limits.h>
//#include <gtk/gtk.h>
//#include <gdk/gdkx.h>

#undef Picture
#undef Time
#undef Font
#undef Display

using namespace Upp;

String FileName()
{
//	return "d:/test.qtf";
	return GetExeDirFile("test.qtf");
}

struct MyApp : TopWindow {
	virtual void Activate() { Title("Active"); TopWindow::Activate(); }
	virtual void Deactivate() { Title("Deactivated"); TopWindow::Deactivate(); }
};


GUI_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
//	ChClassicSkin();
//	ChStdSkin();
//	SetLanguage(LNG_('P','L','P','L'));

//	InstallCrashDump();
	DUMP(GetStdFontCy());
	DUMP(GUI_PopUpEffect());
	
//	String htxt;
//	if(!EditText(htxt, "xxx", "xxxx"))
//		return;
	
//	PromptOK("[H20L1 &&&");

	DUMPM(Environment());
	
	RichEditWithToolBar e;
//	e.SetPage(Size(6074, 3968));
	Vector<int> h;
	for(int i = 1; i < Font::GetFaceCount(); i++)
		if((Font::GetFaceInfo(i) & Font::SCALEABLE) && !(Font::GetFaceInfo(i) & Font::SPECIAL))
			h.Add(i);
	e.FontFaces(h);

//	GUI_PopUpEffect_Write(GUIEFFECT_NONE);

	DUMP(ScrollBarSize());
	
	
//	e.Pick(ParseRTF(LoadFile("C:/xxx/data.bin")));

	e.Pick(ParseQTF(LoadFile(FileName())));
//	e.SetQTF(LoadFile("U:\\xxx\\PainterBug.qtf"));
//	e.ClipZoom(4, 1);
//	Ctrl::ShowRepaint(50);
	RichEdit::PosInfo f;
	LoadFromFile(f, ConfigFile("pos"));
	e.SetPosInfo(f);
//	e.Pick(ParseQTF(AsQTF(CreateImageObject(CtrlImg::exclamation)) + " hahahaha"));
	MyApp w;
//	w.ToolWindow();
	w.SetRect(0, 0, 700, 500);
	w.Sizeable().Zoomable();
//	e.SetReadOnly();
	StaticRect r;
#if 1
	r.Add(e.SizePos());
	w.Add(r.SizePos());
#else
	w.Add(e.SizePos());
#endif
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
	
	SaveFile(GetHomeDirFile("test.rtf"), EncodeRTF(e.Get()));
	
	LOG("Exit");
}
