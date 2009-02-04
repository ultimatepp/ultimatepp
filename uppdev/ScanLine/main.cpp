#include "ScanLine.h"

Image PaintLion(Size sz);

struct Raget : VertexTarget {
	Rasterizer& r;

	virtual void Line(Pointf p)   { r.Line(p.x, p.y); DUMP(p); }
	virtual void Move(Pointf p)   { r.Move(p.x, p.y); }
	virtual void End() {}

	Raget(Rasterizer& r) : r(r) {}
};

struct App : TopWindow {
	double x1, y1, x2, y2, x3, y3;

	String Text() {
		return Format("r.Move(%f, %f);\nr.Line(%f, %f);\nr.Line(%f, %f);\nr.Line(%f, %f);\n", x1, y1, x2, y2, x3, y3, x1, y1);
	}
	
	virtual void LeftDown(Point p, dword keyflags)
	{
		x1 = p.x;
		y1 = p.y;
		Refresh();
		ClearClipboard();
		AppendClipboardText(Text());
	}
	virtual void RightDown(Point p, dword)
	{
		x2 = p.x;
		y2 = p.y;
		Refresh();
		ClearClipboard();
		AppendClipboardText(Text());
	}
	virtual void MouseMove(Point p, dword keyflags)
	{
		x3 = p.x;
		y3 = p.y;
		Refresh();
		ClearClipboard();
		AppendClipboardText(Text());
	}

	virtual void Paint(Draw& w) {
		ImageBuffer ib(600, 600);
		Fill(~ib, White(), ib.GetLength());
/*		Apply(ib[20], 100, Black(), a);
		a.x = 10;
		Apply(ib[30], 100, Blue(), a);
		for(int i = 0; i < 20; i++) {
			b = Pack(i, line1, __countof(line1));
			Apply(ib[50 + 2 * i], 100, Black(), And(a, b));
		}
*/		
		Rasterizer r(600, 600);
//		r.SetClip(RectfC(100, 100, 200, 200));


#if 0
		r.Move(195.000000, 65.000000);
		r.Line(0.000000, 0.000000);
		r.Line(183.000000, 501.000000);
		r.Line(195.000000, 65.000000);
#endif
#if 0
		r.Move(x1, y1);
		r.Line(x2, y2);
		r.Line(x3, y3);
		r.Line(x1, y1);
#endif

		Raget q(r);
#if 1
		r.Move(200, 300);
		ApproximateQuadratic(q, Pointf(200, 300), Pointf(400, 50), Pointf(600, 300), 0.3);
		r.Line(200, 300);
		Render(ib, r, Red(), false);
#endif
		r.Move(100, 200);
		ApproximateCubic(q, Pointf(100, 200), Pointf(100, 100), Pointf(250, 100), Pointf(250, 200), 2);
		r.Line(150, 400);
		r.Line(100, 200);
		Render(ib, r, Blue(), false);
	
		LOG("---------");
/*
		r.Move(200, 300);
		Quadratic(q, Pointf(200, 300), Pointf(400, 50), Pointf(600, 300), 10);
		r.Line(200, 300);
		Render(ib, r, Blue(), false);
*/
		w.DrawRect(GetSize(), White());
		w.DrawImage(0, 0, ib);
		w.DrawText(0, GetSize().cy - 40, Text());
//		w.DrawImage(300, 300, PaintLion(GetSize()));
	}

	App() {
		x1 = y1 = x2 = y2 = 0;
	}
};

GUI_APP_MAIN {
	App().Run();
#ifdef _DEBUG
//	App().Run();
#else _DEBUG
	Size sz(800, 600);
	int time;
	int time0 = GetTickCount();
	int n = 0;
	for(;;) {
		time = GetTickCount();
		if(time - time0 > 1000) break;
		PAINTER_TIMING("All");
		PaintLion(sz);
		n++;
	}
	PromptOK("Benchmark: " + AsString(double(time - time0) / n) + " ms, " + AsString(n) + " times");
#endif

#if 0
	ScanLine a, b, c;
	static byte line1[] = { 1, 50, 100, 128, 128, 128, 128, 128, 100, 50, 1 };
	a = Pack(0, line1, __countof(line1));
	DUMP(a);
	for(int i = 0; i < 16; i++) {
		b = Pack(i, line1, __countof(line1));
		DUMP(b);
		DUMP(And(a, b));
		DUMP(And(b, a));
	}

	int x = 0;
	b = Pack(3, line1, __countof(line1));
	for(int i = 0; i < 10000000; i++) {
		RTIMING("And");
		x += And(a, b).x;
	}
#endif
}
