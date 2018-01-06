#include "LionBenchmark.h"

double tm[4];

struct MyApp : public TopWindow {
	bool co = true;

	virtual void LeftDown(Point p, dword keyflags)
	{
		co = !co;
		Refresh();
	}

	virtual void Paint(Draw& w) {
		RLOG("========== PAINT");
		ImageBuffer ib(GetSize());
		{
			BufferPainter sw(ib);
			sw.Co(co);
			sw.Clear(White());
//			sw.Scale(2);
//			sw.Opacity(0.98);
			PaintLion(sw);
		}
		w.DrawImage(0, 0, ib);
		w.DrawText(500, 0, Format("Standard %.4f", tm[0]));
		w.DrawText(500, 50, Format("Multithreaded %.4f", tm[1]));
		w.DrawText(500, 100, Format("Standard / Multithreaded %.4f", tm[0] / tm[1]));
		w.DrawText(500, 150, Format("Preclipped Standard %.4f", tm[2]));
		w.DrawText(500, 200, Format("Preclipped Multithreaded %.4f", tm[3]));
		w.DrawText(500, 250, co ? "MT" : "");
	}
};

GUI_APP_MAIN
{
#if 1 && !defined(_DEBUG)
	RDUMP(MemoryUsedKb());
	PeakMemoryProfile();
	ImageBuffer ib(1000, 1000);
	BufferPainter sw(ib);
	PaintLion(sw);
	for(int pass = 0; pass < 4; pass++) {
		int time0 = msecs();
		int n = 0;
		while(msecs(time0) < 1000) {
			n++;
			BufferPainter sw(ib);
			sw.Co(pass & 1);
			sw.PreClip(pass & 2);
//			sw.Scale(2);
//			sw.Opacity(0.3);
			PaintLion(sw);
		}
		tm[pass] = (double)msecs(time0) / n;
	}
	RLOG("=========================");
	RLOG(*PeakMemoryProfile());
	RLOG("Standard " << tm[0]);
	RLOG("MT " << tm[1]);
#endif
	MyApp().Run();
}
