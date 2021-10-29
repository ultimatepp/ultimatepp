#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) {
	//	String text = "Image: opening page of 訓 (훈)";
		String text = "Kanji and Hangul: 訓民正音 (훈민정음) ";
		text << WString(0x1F603, 1).ToString();
		
		w.DrawRect(GetSize(), White());
		int y = 10;
		for(int i = 0; i < Font::GetFaceCount(); i++) {
			Font fnt(i, 40);
			w.DrawText(10, y, fnt.GetFaceName() + ": " + text, fnt);
			y += 50;
			if(y > GetSize().cy)
				break;
		}
	}
};

GUI_APP_MAIN {
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		Font fnt(i, 40);
		if(!fnt.IsNormal('4'))
			DLOG(fnt.GetFaceName());
	}
	MyApp().Run();
}