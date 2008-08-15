#include <CtrlLib/CtrlLib.h>

Image CreateBall(int r, Color color)
{
	int rr = 2 * r;
	int r2 = r * r;
	ImageBuffer b(rr, rr);
	b.SetHotSpot(Point(4, 4));
	for(int y = 0; y < rr; y++)
		for(int x = 0; x < rr; x++) {
			RGBA& a = b[y][x];
			a.r = color.GetR();
			a.g = color.GetG();
			a.b = color.GetB();
			int q = ((x - r) * (x - r) + (y - r) * (y - r)) * 256 / r2;
			a.a = q <= 255 ? q : 0;
		}
	return b;
}

struct Tester : Ctrl {
	String lbl, lbl2;
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetSize(), White);
		w.DrawText(0, 0, lbl + ", " + lbl2 + ": " + AsString(GetMousePos()), GetSize().cy);
	}

	virtual void LeftDown(Point p, dword keyflags)
	{
		lbl = String().Cat() << "LEFTDOWN " << p;
		Refresh();
	}

	void LeftDouble(Point p, dword keyflags)
	{
		lbl = String().Cat() << "LEFTDOUBLE " << p;
		Refresh();
	}

	virtual void LeftUp(Point p, dword keyflags)
	{
		lbl = String().Cat() << "LEFTUP " << p;
		Refresh();
	}

	virtual void MouseMove(Point p, dword keyflags)
	{
		lbl = String().Cat() << "MOUSEMOVE " << p;
		Refresh();
	}

	virtual void MouseEnter(Point p, dword keyflags)
	{
		lbl2 = "MOUSEENTER";
		Refresh();
	}

	virtual void MouseLeave()
	{
		lbl2 = "MOUSELEAVE";
		Refresh();
	}
};

struct CETest : TopWindow {
	DropList h;
	Option   option;
	Image    ball;
	EditString editor;
	Button   button;
	Tester   tst;

	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, LtGray);
		w.DrawRect(sz / 2, LtBlue);
		w.DrawText(10, 10, "Hello", Arial(12));
		w.DrawImage(50, 50, CtrlImg::exclamation());
		w.DrawImage(100, 50, CtrlImg::exclamation(), Blue);
		w.DrawImage(50, 150, CtrlsImg::O1h());
		w.DrawImage(150, 150, CtrlsImg::O1h());
		w.Clipoff(50, 180, 40, 40);
		w.DrawImage(0, 0, 50, 50, CtrlsImg::O1h());
		w.End();
		w.DrawImage(100, 100, ball);
		w.DrawImage(200, 200, 40, 40, ball);
	}

	CETest() {
		Add(tst.BottomPos(40, 20).HSizePos());
		Add(h.TopPos(0, 20).HSizePos(20, 20));
		Add(option.SetLabel("Option").TopPos(20, 20).LeftPos(100, 100));
		Add(editor.TopPos(40, 20).LeftPos(0, 100));
		Add(button.SetLabel("U++").TopPos(200, 20).LeftPos(0, 100));
		for(int i = 0; i < 200; i++)
			h.Add(i);
		ball = CreateBall(80, Cyan);
		editor <<= Breaker();
		button <<= Breaker();
	}

};

GUI_APP_MAIN {
	CETest().Run();
	return;
	TopWindow w;
	Button button;
	button.SetLabel("Hello world!");
	button <<= w.Breaker();
	w << button.HSizePos(20, 20).VSizePos(20, 20);
	w.Run();
}
