#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	String text = "αβγδ АБГД 吗嗎 弒";
	for(int i = 0; i < 30; i++) {
		Font fnt(i, 16);
		w.DrawText(0, i * 20, fnt.GetFaceName() + " " + text, fnt);
	}
}

GUI_APP_MAIN
{
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		Font fnt(i, 10);
		LOG(fnt.GetFaceName() << " " << (fnt.IsSerif() ? "[serif] " : "")
		    << (fnt.IsScript() ? "[script] " : ""));
	}
	MyApp().Run();
}
