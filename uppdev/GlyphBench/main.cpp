#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetSize(), White());
		Font fnt = Arial(16);
		int fcx = fnt['X'];
		{
			RTIMING("Optimized");
			for(int q = 0; q < 100000; q++)
				w.DrawText(0, 0, String('X', 30), fnt);
		}
		{
			RTIMING("One char");
			for(int q = 0; q < 100000; q++)
				for(int i = 0; i < 30; i++)
					w.DrawText(i * fcx, 40, String('X', 1), fnt);
		}
		w.DrawText(100, 100, "Finished");
	}
};
 
GUI_APP_MAIN
{
	MyApp().Run();
}

