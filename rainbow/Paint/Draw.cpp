#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

struct App : public Ctrl {
	EditString x, y;
	ArrayCtrl a, b;
	DropList dl;
	
	StaticRect popup;
	StaticRect popup2, popup3;
	
	void Paint(Draw& w)
	{
		Size sz = GetSize();
		DDUMP(sz);
		w.DrawRect(0, 0, sz.cx, sz.cy, SWhite);
		w.DrawRect(10, 10, 30, 30, SRed);
		w.DrawLine(45, 45, 80, 120, 4, Blue);
		w.DrawLine(80, 90, 400, 0, PEN_DASHDOT);
		w.DrawEllipse(200, 200, 50, 100, Green);
		w.DrawImage(200, 10, CtrlImg::HandCursor());
		const char *text = "This text is centered";
		Size tsz = GetTextSize(text, Arial(25).Bold());
		w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, text, Arial(27).Bold(), SBlue);
		w.DrawRect(80, 480, 100, 100, Blue());
		w.Clipoff(200, 50, 95, 100);
		w.DrawText(0, 80, "CLIPPED", Roman(25));
		w.End();
	}
	
	void LeftDown(Point p, dword)
	{
		popup.SetRect(p.x, p.y, 300, 400);
		popup.SetForeground();
	}

	void RightDown(Point p, dword)
	{
		popup3.SetRect(p.x, p.y, 100, 100);
		popup3.SetForeground();
	}
	
	void InitArray(ArrayCtrl& a)
	{
		a.AddColumn("first");
		a.AddColumn("second");
		for(int i = 0; i < 100; i++)
			a.Add(i, FormatIntRoman(i));
	}

	App()
	{
		x <<= "Hello world!";
		Add(x.LeftPos(100, 100).TopPos(500, 20));
		Add(a.LeftPos(300, 150).TopPos(10, 300));
		Add(dl.LeftPos(10, 300).TopPos(10, 30));
		InitArray(a);
		InitArray(b);
		popup.SetFrame(BlackFrame());
		popup.Add(b.HSizePos(10, 200).VSizePos(10, 10));
		b.SetFrame(ViewFrame());
		popup.Add(y.HSizePos(210, 10).TopPos(10, 30));
		popup.SetRect(800, 100, 300, 400);
		
		popup2.SetRect(500, 50, 300, 400);
		popup2.SetFrame(BlackFrame());
		popup2.Color(Magenta());
		
		popup3.SetFrame(OutsetFrame());
		popup3.Color(Cyan());
		
	//	Add(popup);
		
		for(int i = 0; i < 100; i++)
			dl.Add(i);
//		Sizeable();
	}
};

#define EDITOR 1

GUI_APP_MAIN
{
#if EDITOR
	RichEditWithToolBar app;
#else	
	App app;
#endif
	ChStdSkin();
	Ctrl::SetDesktop(app);
	app.SetFocus();
#if !EDITOR
	app.popup.PopUp();
	app.popup2.PopUp(&app.popup);
	app.popup3.PopUp();
#endif
//	Ctrl::SetRenderingMode(MODE_NOAA);
	Ctrl::SetRenderingMode(MODE_SUBPIXEL);
	Ctrl::EventLoop();
}
