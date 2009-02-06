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
	
	Pointf p1, p2, p3, p4;

	String Text() {
		return Format("r.Move(%f, %f);\nr.Line(%f, %f);\nr.Line(%f, %f);\nr.Line(%f, %f);\n", x1, y1, x2, y2, x3, y3, x1, y1);
	}
	
	virtual void LeftDown(Point p, dword keyflags)
	{
		(keyflags & K_ALT ? p4 : p1) = p;
		Refresh();
	}
	virtual void RightDown(Point p, dword)
	{
		p2 = p;
		Refresh();
	}
	virtual void MouseMove(Point p, dword keyflags)
	{
		p2 = p;
		Refresh();
	}
	virtual Image CursorImage(Point p, dword keyflags) { return Image::Cross(); }

	virtual void Paint(Draw& w);

	virtual void Paint1(Draw& w) {
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

#if 1
		r.Move(200, 300);
//		ApproximateQuadratic(q, Pointf(200, 300), Pointf(400, 50), Pointf(600, 300), 0.3);
		r.Line(200, 300);
		Render(ib, r, Red(), false);
#endif
		r.Move(100, 200);
//		ApproximateCubic(q, Pointf(100, 200), Pointf(100, 100), Pointf(250, 100), Pointf(250, 200), 2);
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
		p1 = p4 = Pointf(100, 100);
		p2 = Pointf(200, 100);
		p3 = Pointf(150, 200);
	}
};

struct RasterizerTarget : VertexTarget {
	Rasterizer&  r;
	ImageBuffer& ib;
	
	virtual void Line(const Pointf& p)
	{
		r.Line(p.x, p.y);
	}
	virtual void Move(const Pointf& p)
	{
		r.Move(p.x, p.y);
	}
	virtual void End()
	{
		Render(ib, r, Black(), false);
	}
	
	RasterizerTarget(ImageBuffer& ib, Rasterizer& r) : ib(ib), r(r) {}
};

#if 1
void App::Paint(Draw& w)
{
	Size sz = GetSize();
	ImageBuffer ib(sz.cx, sz.cy);
	Rasterizer r(sz.cx, sz.cy);
	Fill(~ib, White(), ib.GetLength());
	RasterizerTarget tgt(ib, r);

	Stroker s(20, 4, 0.3, LINECAP_ROUND, LINEJOIN_ROUND);

	s|tgt;
	
	s.Move(p1);
	s.Line(p2);
//	s.Line(p3);
//	s.Line(p4);
	s.End();

	w.DrawImage(0, 0, ib);
}

#else
struct LineTarget : VertexTarget {
	Draw& w;
	Pointf p;
	
	virtual void Line(const Pointf& p1)
	{
		w.DrawLine(p.x, p.y, p1.x, p1.y);
		p = p1;
	}
	virtual void Move(const Pointf& p1)
	{
		p = p1;
	}
	virtual void End() {}
	
	LineTarget(Draw& w) : w(w) {}
};

void App::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	LineTarget ltg(w);
	Stroker s(20, 4, 0.3, LINECAP_ROUND, LINEJOIN_ROUND);
	s|ltg;
	s.Move(p1);
	s.Line(p2);
//	s.Line(p3);
//	s.Line(p4);
	s.End();
}
#endif

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
