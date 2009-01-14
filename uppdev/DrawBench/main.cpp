#include <CtrlLib/CtrlLib.h>

#include "bench.h"

using namespace Upp;



////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define N 1
#else
#define N 10
#endif


struct App : TopWindow {
	Point p;

	virtual void MouseMove(Point _p, dword keyflags) {
		p = _p;
		Refresh();
	}

	virtual void Paint(Draw& w) {
		RTIMING("Paint");
		Size sz = GetSize();
		ImageBuffer ib(sz);
		Fill(~ib, White(), ib.GetLength());
		
		for(int i = 0; i < N; i++) {
			{
				BufferPainter sw(ib);
				sw.Clear(White());
				{
					RTIMING("AGGUPP tree");
					DrawPythagorasTree(GetSize(), &sw, NULL);
				}
				{
					RTIMING("AGGUPP lion");
					PaintLion(&sw, NULL, NULL);
				}
			}
			{
				Cairo ca;
				ca.SetSurface(ib);
				ca.Translate(300, 0);
				{
					RTIMING("Cairo tree");
					DrawPythagorasTree(GetSize(), NULL, &ca);
				}
				{
					RTIMING("Cairo lion");
					PaintLion(NULL, &ca, NULL);
				}
			}
		}
		
		
		w.DrawImage(0, 0, ib);
	}
	
	App() { Sizeable().Zoomable(); p = Point(0, 0); }
};

/*#include <plugin/cairo/CairoCtrl.h>
struct TestCairoCtrl : CairoCtrl {
	Point p;

	virtual void MouseMove(Point _p, dword keyflags) {
		p = _p;
		Refresh();
	}
	
	virtual void CairoPaint(Cairo &ca) {
		RTIMING("CairoPaint");
		
		ca.Clear(White());
		PaintLion(NULL, &ca, NULL);
	}
};

struct App2 : TopWindow {
	TestCairoCtrl ctrl;

	void CairoPaint(Cairo& ca) {
	}
	
	App2() { 
		Sizeable().Zoomable(); 
		
		ctrl.SizePos();
		Add(ctrl);
	}
};*/

GUI_APP_MAIN
{
	App().Run();
}
