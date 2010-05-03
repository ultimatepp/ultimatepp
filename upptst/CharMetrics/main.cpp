#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	DropList fontlist;

	void Paint(Draw& w) {
		w.DrawRect(GetSize(), White);
		int y = 50;
		for(int p = 32; p < 8192; p += 64) {
			Font fnt(~fontlist, 30);
			FontInfo fi = fnt.Info();
			int x = 0;
			for(int ch = p; ch < p + 64; ch++) {
				int width = fi[ch];
				w.DrawRect(x, y, width - 1, fi.GetAscent(), Color(255, 255, 200));
				w.DrawRect(x, y + fi.GetAscent(), width - 1, fi.GetDescent(), Color(255, 200, 255));
				w.DrawRect(x + width - 1, y, 1, fi.GetHeight(), Black());
				w.DrawText(x, y, WString(ch, 1), fnt);
				x += width;
			}
			y += fi.GetHeight() + 20;
		}
	}

	void NewFont() {
		Refresh();
	}

	typedef MyApp CLASSNAME;

	MyApp() {
		for(int i = 0; i < Font::GetFaceCount(); i++)
			fontlist.Add(i, Font::GetFaceName(i));
		Add(fontlist.TopPos(0, MINSIZE).LeftPosZ(0, 200));
		fontlist <<= 0;
		fontlist <<= THISBACK(NewFont);
	}
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
