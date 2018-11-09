#include "LionBenchmark.h"

double tm[4];

void Task(Painter& sw)
{
	RTIMING("Total");
	sw.Begin();
//	sw.Scale(2);
//	sw.Opacity(0.3);
//	sw.Translate(20, 0);
//	sw.Rotate(0.1);
//	PaintLion(sw);
//	PythagorasTree(sw);
//	Demo(sw);
//	ClippingExample(sw);
//	PolyPolygon(sw);
//	TextOnPath(sw);
	Dash(sw);

	sw.End();
}

struct MyApp : public TopWindow {
	bool co = true;

	virtual void LeftDown(Point p, dword keyflags)
	{
		co = !co;
		Refresh();
	}

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());

		for(int pass = 0; pass < 1; pass++) {
			ImageBuffer ib(1200, 800);
			{
				BufferPainter sw(ib);
				if(pass)
					sw.Co();
				sw.PreClip();
				sw.Clear(White());
				Task(sw);
			}
			w.DrawImage(0, 800 * pass, ib);
		}

		int x = 1200;
		int y = 0;
		w.DrawText(x, y += 50, Format("Standard %.4f", tm[0]));
		w.DrawText(x, y += 50, Format("Multithreaded %.4f", tm[1]));
		w.DrawText(x, y += 50, Format("Standard / Multithreaded %.4f", tm[0] / tm[1]));
		w.DrawText(x, y += 50, Format("Preclipped Standard %.4f", tm[2]));
		w.DrawText(x, y += 50, Format("Preclipped Multithreaded %.4f", tm[3]));
		w.DrawText(x, y += 50, co ? "MT" : "");
	}
};

struct DashInfo {};

	struct BaseAttr {
		Xform2D                         mtx;
		bool                            evenodd;
		byte                            join;
		byte                            cap;
		bool                            invert;
		double                          miter_limit;
		double                          opacity;
		DashInfo                       *dash;
	};

BaseAttr a, b;

GUI_APP_MAIN
{
#if 1 && !defined(_DEBUG)
	RDUMP(MemoryUsedKb());
	
	RDUMP(sizeof(Rasterizer));
	
//	PeakMemoryProfile();
	ImageBuffer ib(1200, 1200);
	BufferPainter sw(ib);
	sw.Co();
//	for(int i = 0; i < 10; i++)
		PaintLion(sw);
	for(int pass = 0; pass < 2; pass++) {
		int time0 = msecs();
		int n = 0;
		{
			BufferPainter sw(ib);
			sw.Co(pass & 1);
			sw.PreClip(pass & 2);
			while(msecs(time0) < 1000) {
				n++;
				Task(sw);
			}
		}
		tm[pass] = (double)msecs(time0) / n;
		RLOG(pass << ": " << n << ", " << tm[pass] << " ms");
	}
	RLOG("=========================");
//	RLOG(*PeakMemoryProfile());
	RLOG("Standard " << tm[0]);
	RLOG("MT " << tm[1]);
#endif
	MyApp().Title("6").Run();
}
