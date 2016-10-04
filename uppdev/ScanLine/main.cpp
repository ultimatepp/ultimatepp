#include "ScanLine.h"

Image PaintLion(Size sz);

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


	App() {
		p1 = p4 = Pointf(100, 100);
		p2 = Pointf(200, 100);
		p3 = Pointf(150, 200);
	}
};

#if 1
void App::Paint(Draw& w)
{
	Size sz = GetSize();
	ImageBuffer ib(sz.cx, sz.cy);
	Rasterizer r(sz.cx, sz.cy);
	Fill(~ib, White(), ib.GetLength());

	Stroker s(20, 4, 0.3, LINECAP_BUTT, LINEJOIN_MITER);
	Vector<double> dash;
	dash << 4 << 2;
	Dasher d(20, dash, GetMousePos().y);

	d|s|r;
	
	d.Move(p1);
	d.Line(p2);
	d.Line(p3);
	d.Line(p4);
	d.End();

	Render(ib, r, Black(), false);

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

void Test(const Xform2D& m)
{
	DDUMP(m.Transform(Pointf(0, 1)));
	DDUMP(m.Transform(Pointf(1, 1)));
	DDUMP(m.Transform(Pointf(1, 0)));
}

GUI_APP_MAIN {
	DDUMP(Xform2D::Identity().IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Translation(20, 20)).IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Translation(20, 20)).GetScale());
	DDUMP((Xform2D::Identity() * Xform2D::Scale(20)).IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Scale(20)).GetScale());
	DDUMP((Xform2D::Identity() * Xform2D::Scale(20, 10)).IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Rotation(0.7)).IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Rotation(0.7)).GetScale());
	DDUMP((Xform2D::Identity() * Xform2D::Rotation(0.7) * Xform2D::Scale(20)).IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Sheer(0.3)).IsRegular());
	Test((Xform2D::Identity() * Xform2D::Sheer(0.3)));
	DDUMP((Xform2D::Identity() * Xform2D::Rotation(0.7) * Xform2D::Scale(20) * Xform2D::Sheer(0.1)).IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Rotation(0.7) * Xform2D::Scale(20) * Xform2D::Sheer(0.1)).GetScale());
	DDUMP((Xform2D::Identity() * Xform2D::Rotation(0.7) * Xform2D::Scale(20, 1)).IsRegular());
	DDUMP((Xform2D::Identity() * Xform2D::Rotation(0.7) * Xform2D::Scale(20, 1)).IsRegular());
	
	Xform2D m = (Xform2D::Identity() * Xform2D::Rotation(0.7) * Xform2D::Scale(20) * Xform2D::Sheer(0.1));
	DDUMP(m.Transform(Pointf(100, 100)));
	DDUMP(Inverse(m).Transform(m.Transform(Pointf(100, 100))));
	
	return;
	
#ifdef _DEBUG
	App().Run();
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
