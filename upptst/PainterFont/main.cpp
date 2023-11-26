#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	DropList dl;
	
	virtual void Paint(Draw& w) {
		DrawPainter iw(w, GetSize());
		iw.Clear(SWhite());
		iw.DrawText(Zx(100), Zx(100), "Hello world!", (Font)~dl);
		iw.Text(Zx(100), Zx(200), "Hello world again!", (Font)~dl).Fill(Black());
	}
	
	MyApp() {
		Add(dl.TopPos(10).LeftPos(10, Zx(200)));
		for(int face = 0; face < Font::GetFaceCount(); face++)
		{
			dl.Add(Font(face, 200), Font::GetFaceName(face));
		}
		dl.GoBegin();
		dl << [=] { Refresh(); };
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
