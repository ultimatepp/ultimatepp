#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS MyImages
#define IMAGEFILE  <Image02b/images.iml>
#include <Draw/iml.h>

class MyApp : public TopWindow {
	Iml& iml;

public:
	virtual void Paint(Draw& draw);

	MyApp(Iml& _iml) : iml(_iml) {}
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorPaper());
	for(int i = 0; i < iml.GetCount(); i++) {
		w.DrawImage(50, 80 + 20 * i, iml.Get(i));
		w.DrawText(80, 80 + 20 * i, iml.GetId(i));
	}
}

GUI_APP_MAIN
{
	MyApp(MyImages::Iml()).Sizeable().Run();
}
