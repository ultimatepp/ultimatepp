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

	RichEditWithToolBar e;
	e.Pick(ParseQTF(LoadFile(FileName())));
	e.SetPage(Size(4000, 600));
	MyApp w;
	w.SetRect(0, 0, 700, 500);
	w.Sizeable().Zoomable();
	StaticRect r;
	w.Add(e.SizePos());

	w.Run();
	String x = e.GetQTF();
	SaveFile(FileName(), x);
	
	LOG("Exit");
}
