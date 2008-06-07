#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS MyImages
#define IMAGEFILE  <Image01/images.iml>
#include <Draw/iml.h>

class MyApp : public TopWindow {
public:
	virtual void Paint(Draw& draw);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorFace());
	w.DrawImage(50, 50, MyImages::MyImage());
}

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
