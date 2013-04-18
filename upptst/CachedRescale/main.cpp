#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <CachedRescale/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <CachedRescale/test.iml>
#include <Draw/iml_source.h>

using namespace Upp;

struct MyApp : TopWindow {
	typedef MyApp CLASSNAME;

	DropList method;
	
	virtual void Paint(Draw& w)
	{
		w.DrawImage(0, 0, CachedRescale(TestImg::test(), GetSize(), (int)~method));
	}
	
	void Sync()
	{
		Refresh();
	}
	                   
	MyApp() {
		Sizeable().Zoomable();
		SetRect(0, 0, 200, 200);
		method.Add(Null, "Rylek");
		method.Add(FILTER_NEAREST, "Nearest");
		method.Add(FILTER_BILINEAR, "Bilinear");
		method.Add(FILTER_BSPLINE, "Bspline");
		method.Add(FILTER_COSTELLO, "Costello");
		method.Add(FILTER_BICUBIC_MITCHELL, "Bicubic Mitchell");
		method.Add(FILTER_BICUBIC_CATMULLROM, "Bicubic Catmull Rom");
		method.Add(FILTER_LANCZOS2, "Lanczos 2");
		method.Add(FILTER_LANCZOS3, "Lanczos 3");
		method.Add(FILTER_LANCZOS4, "Lanczos 4");
		method.Add(FILTER_LANCZOS5, "Lanczos 5");
		method <<= THISBACK(Sync);
		Add(method.TopPos(0, STDSIZE).RightPos(0, 200));
		method <<= FILTER_NEAREST;
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
