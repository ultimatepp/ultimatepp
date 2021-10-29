#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) {
	//	String text = "Image: opening page of 訓 (훈)";
		String text = "Kanji and Hangul: 訓民正音 (훈민정음) ";
		text << WString(0x1F603, 1).ToString();
		
		w.DrawRect(GetSize(), White());
		int y = 10;
		int x = 10;
		for(int i = 0; i < Font::GetFaceCount(); i++) {
			Font fnt(i, 30);
			String n = AsString(i) + ": " + fnt.GetFaceName() + ": ";
			if(fnt.IsSpecial())
				n << " (special)";
			w.DrawText(x, y, n, StdFont());
			w.DrawText(x + GetTextSize(n, StdFont()).cx, y, text, fnt);
			y += fnt.GetLineHeight() + 10;
			if(y > GetSize().cy) {
				if(x > 10)
					break;
				x = GetSize().cx / 2;
				y = 10;
			}
		}
	}
};

GUI_APP_MAIN {
	Vector<String> h;
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		Font fnt(i, 40);
		h.Add(fnt.GetFaceName());
		if(!fnt.IsNormal('4'))
			DLOG(fnt.GetFaceName());
	}
	DLOG("===========");
	Sort(h);
	DDUMPC(h);
	MyApp().Sizeable().Zoomable().Run();
}