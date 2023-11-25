#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS MyImages
#define IMAGEFILE  <Image02b/images.iml>
#include <Draw/iml.h>

class MyApp : public TopWindow {
public:
	MyApp(Iml& iml) : m_iml(iml) {
		SetRect(0, 0, 200, 200);
	}
	
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), SColorPaper());
		for(int i = 0; i < m_iml.GetCount(); i++) {
			w.DrawImage(50, 80 + 20 * i, m_iml.Get(i));
			w.DrawText(80, 80 + 20 * i, m_iml.GetId(i));
		}
	}
	
private:
	Iml& m_iml;
};

GUI_APP_MAIN
{
	MyApp(MyImages::Iml()).Run();
}
