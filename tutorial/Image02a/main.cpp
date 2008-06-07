#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS MyImages
#define IMAGEFILE  <Image02a/images.iml>
#include <Draw/iml.h>

class MyApp : public TopWindow {
public:
	virtual void Paint(Draw& draw);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorPaper());
	for(int i = 0; i < MyImages::GetCount(); i++) {
		w.DrawImage(50, 80 + 20 * i, MyImages::Get(i));
		w.DrawText(80, 80 + 20 * i, MyImages::GetId(i));
	}
	w.DrawImage(20, 0, 50, 50, MyImages::Get(MyImages::I_Circle));
	w.DrawText(80, 0, AsString(MyImages::Find("Circle")));
}

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
	Image m = MyImages::Circle();
	MyImages::Set(MyImages::I_Circle, MyImages::Triangle());
	MyImages::Set(MyImages::I_Triangle, m);
	MyApp().Sizeable().Run();
}
