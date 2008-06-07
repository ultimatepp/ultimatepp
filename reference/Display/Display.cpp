#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct FontFaceDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		Font fnt = Font(q, r.Height() - 2);
		String txt = Font::GetFaceName(q);
		w.DrawRect(r, paper);
		w.DrawText(r.left + 2, r.top + (r.Height() - GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink);
	}
};

struct MyApp : TopWindow {
	DropList dl;

	MyApp()
	{
		Add(dl.HSizePos().TopPos(5, Ctrl::STDSIZE));
		dl.SetDisplay(Single<FontFaceDisplay>());
		for(int i = 0; i < Font::GetFaceCount(); i++)
			dl.Add(i);
		SetRect(0, 0, 200, 70);
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
