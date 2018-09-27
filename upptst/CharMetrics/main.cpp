#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	DropList fontlist;

	void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, White);
		Font fnt(~fontlist, 50);
		w.DrawText(Zx(220), 0, String() << fnt.GetFaceName()
		                                << ", ascent: " << fnt.GetAscent()
		                                << ", descent: " << fnt.GetDescent()
		                                << ", external: " << fnt.GetExternal()
		                                << (fnt.IsSerif() ? ", [serif]" : "")
		                                << (fnt.IsScript() ? ", [script]" : ""));
		int y = 50;
		for(int p = 32; p < 8192 && y < sz.cy; p += 64) {
			int x = 0;
			for(int ch = p; ch < p + 64; ch++) {
				int width = fnt[ch];
				w.DrawRect(x, y, width - 1, fnt.GetAscent(), Color(255, 255, 200));
				w.DrawRect(x, y + fnt.GetAscent(), width - 1, fnt.GetDescent(), Color(255, 200, 255));
				w.DrawRect(x + width - 1, y, 1, fnt.GetCy(), Black());
				w.DrawText(x, y, WString(ch, 1), fnt);
				x += width;
			}
			y += fnt.GetLineHeight();
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
