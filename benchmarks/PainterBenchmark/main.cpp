#include "LionBenchmark.h"

Tuple<const char *, void (*)(Painter& sw)> tasks[] =
{
#define TASK(x) { #x, x, }
	TASK(PaintLionLarge),
#if 0
	TASK(PaintLion),
	TASK(TextRotated),
	TASK(PythagorasTree),
	TASK(Demo),
	TASK(ClippingExample),
	TASK(PolyPolygon),
	TASK(TextOnPath),
	TASK(Dash),
	TASK(SomeRocks),
#endif
};

void Task(Painter& sw, int i)
{
	RTIMING("Total");
	sw.Begin();
//	sw.Scale(1.2);
//	sw.Opacity(0.3);
//	sw.Translate(20, 0);
//	sw.Rotate(0.1);
	(*tasks[clamp(i, 0, __countof(tasks) - 1)].b)(sw);
	sw.End();
}

double tm[__countof(tasks)][16];

String FormatOpts(int pass)
{
	String r = pass & 8 ? "4K" : "1K";
	if(pass & 4)
		r << " Cleared";
	if(pass & 2)
		r << " PreClip";
	if(pass & 1)
		r << " Co";
	r << ": ";
	return r;
}

String FormatPass(int pass, double tm)
{
	return FormatOpts(pass) << Format("%.3f ms", tm);
}

struct MyApp : public TopWindow {
	bool co = true;
	DropList task;

	virtual void LeftDown(Point p, dword keyflags)
	{
		co = !co;
		Refresh();
	}

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());

		for(int pass = 0; pass < 2; pass++) {
			ImageBuffer ib(1200, 800);
			{
				BufferPainter sw(ib);
				if(pass)
					sw.Co();
				sw.PreClip();
				sw.Clear(White());
				Task(sw, ~task);
			}
			w.DrawImage(0, 800 * pass, ib);
		}

		int x = 1200;
		int y = 0;
		int lcy = StdFont().GetCy();
		for(int i = 0; i < 16; i++) {
			w.DrawText(x, y += lcy, FormatPass(i, tm[~task][i]));
			if((i & 3) == 3)
				y += lcy / 2;
		}
	}
	
	MyApp() {
		for(int i = 0; i < __countof(tasks); i++)
			task.Add(i, tasks[i].a);
		task << [=] { Refresh(); };
		Add(task.RightPosZ(8, 200).TopPosZ(8));
		task <<= 0;
	}
};

GUI_APP_MAIN
{
#ifdef _DEBUG
	int duration = 100;
#else
	int duration = 500;
#endif
	Progress pi("Tasks", 16 * __countof(tasks));
	for(int ti = 0; ti < __countof(tasks); ti++) {
		pi.SetText(tasks[ti].a);
		for(int pass = 0; pass < 16; pass++) {
			if(pi.StepCanceled())
				return;
			int time0 = msecs();
			int n = 0;
			int sz = pass & 8 ? 2048 : 512;
			ImageBuffer ib(sz, sz);
			{
				BufferPainter sw(ib);
				sw.Co(pass & 1);
				sw.PreClip(pass & 2);
				while(msecs(time0) < duration) {
					n++;
					if(pass & 4)
						sw.Clear(White());
					Task(sw, ti);
				}
			}
			tm[ti][pass] = (double)msecs(time0) / n;
			RLOG(tasks[ti].a << " " << FormatPass(pass, tm[ti][pass]));
		}
		RLOG("========");
	}
	MyApp().Run();
}
