#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Test : TopWindow {
	virtual void Paint(Draw& w) {
		WString s;
		s << "Napishite zdesʹ tekst dlya perevoda";
		Buffer<int> wd(s.GetLength());
		for(int i = 0; i < s.GetCount(); i++)
			wd[i] = StdFont()[s[i]];
		w.DrawRect(GetSize(), White);
		w.DrawText(100, 100, ~s, StdFont(), Black, s.GetLength(), wd);
		w.DrawText(50, 200, ~s, StdFont(), Black, s.GetLength(), wd);
	}
};

GUI_APP_MAIN
{
	DDUMP("Here");
	DocEdit e;
	e.Set(WString("Napishite zdesʹ tekst dlya perevoda"));
	Test t;
	t.Add(e.TopPos(0, 50).HSizePos());
	t.Run();
}
