#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	DropList font_list;

	void Paint(Draw& w) override {
		const String text = "Programming is fun";
		Font fnt(~font_list, 60);
		
		w.DrawRect(GetSize(), White);
		w.Offset(50, 50);
		int x = 0;
		Vector<int> dx;
		for(char letter : text) {
			int width = fnt[letter];
			w.DrawRect(x, 0, width - 1, fnt.GetAscent(), Color(255, 255, 200));
			w.DrawRect(x, fnt.GetAscent(), width - 1, fnt.GetDescent(), Color(255, 200, 255));
			w.DrawRect(x + width - 1, 0, 1, fnt.GetHeight(), Black());
			dx.Add(width + 4);
			x += width;
		}
		w.DrawRect(0, 0, 4, 4, Black());
		w.DrawText(0, 0, text, fnt);
		w.DrawText(0, 70, text, fnt, Blue(), dx.GetCount(), dx.Begin());
		w.End();
	}

	void NewFont() {
		Refresh();
	}

	MyApp() {
		for(int i = 0; i < Font::GetFaceCount(); i++)
			font_list.Add(i, Font::GetFaceName(i));
		Add(font_list.TopPos(0, MINSIZE).LeftPosZ(0, 200));
		font_list <<= 0;
		font_list << [=] { NewFont(); };
	}
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
