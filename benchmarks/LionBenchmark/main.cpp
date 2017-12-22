#include "LionBenchmark.h"

double tm[2];

struct MyApp : public TopWindow {
	virtual void Paint(Draw& w) {
		ImageBuffer ib(GetSize());
		BufferPainter sw(ib);
//		sw.Co();
		sw.Clear(White());
		sw.Scale(2);
		PaintLion(sw);
		w.DrawImage(0, 0, ib);
		w.DrawText(500, 0, Format("Standard %.4f", tm[0]));
		w.DrawText(500, 50, Format("Multithreaded %.4f", tm[1]));
	}
};

GUI_APP_MAIN
{
#if 0
	ImageBuffer ib(1000, 1000);
	BufferPainter sw(ib);
	PaintLion(sw);
	for(int pass = 0; pass < 2; pass++) {
		int time0 = msecs();
		int n = 0;
		BufferPainter sw(ib);
		sw.Scale(2);
		sw.Opacity(0.3);
		while(msecs(time0) < 1000) {
			n++;
			PaintLion(sw);
		}
		tm[pass] = (double)msecs(time0) / n;
	}
#endif
	MyApp().Run();
}
