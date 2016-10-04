#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetSize(), White());
		Font fnt = Arial(16);
		int fcx = fnt['X'];
		String txt;
		for(int i = 0; i < 30; i++)
			txt.Cat(i + 'A');
		{
			RTIMING("Optimized");
			for(int q = 0; q < 10000; q++)
				w.DrawText(0, 0, txt, fnt);
		}
		{
			RTIMING("One char");
			for(int q = 0; q < 10000; q++)
				for(int i = 0; i < 30; i++)
					w.DrawText(i * fcx, 40, String('A' + i, 1), fnt);
		}
		w.DrawText(100, 100, "Finished");
	}
};
 
GUI_APP_MAIN
{
	MyApp().Run();
}

