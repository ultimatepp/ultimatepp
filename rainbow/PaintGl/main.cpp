#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

#define LLOG(x)       //LOG(x)
#define LDUMP(x)      //DDUMP(x)

struct App : public Ctrl {
	EditString x;
	ArrayCtrl a, b;
	DropList dl;
	
	StaticRect popup;
	
	void Paint(Draw& w)
	{
		LLOG("Paint");
		Size sz = GetSize();
		LDUMP(sz);
		w.DrawRect(0, 0, sz.cx, sz.cy, SRed);
		w.DrawRect(10, 10, 30, 30, SRed);
		w.DrawLine(45, 45, 80, 120, 4, Blue);
		w.DrawLine(80, 90, 400, 0, PEN_DASHDOT);
		w.DrawEllipse(200, 200, 50, 100, Green);
		w.DrawImage(200, 10, CtrlImg::HandCursor());
		//const char *text = "This text is centered";
		//Size tsz = GetTextSize(text, Arial(25).Bold());
		//w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, text, Arial(27).Bold(), SBlue);
		//w.Clipoff(200, 50, 95, 100);
		//w.DrawText(0, 80, "CLIPPED", Roman(25));
		//w.End();
	}
	
	void LeftDown(Point p, dword)
	{
		popup.SetRect(p.x, p.y, 100, 400);
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
		InitArray(a);
		InitArray(b);
		popup.SetFrame(BlackFrame());
		popup.Add(b.HSizePos(10, 10).VSizePos(10, 10));
		popup.SetRect(800, 100, 100, 400);
		
		Add(dl.LeftPos(10, 300).TopPos(10, 30));
		for(int i = 0; i < 100; i++)
			dl.Add(i);
//		Sizeable();
	}
};

#define EDITOR 0

GUI_APP_MAIN
{
	App app;
	ChStdSkin();
#if defined(GUI_FB) || defined(GUI_WINGL)
	Ctrl::SetDesktop(app);
	app.SetFocus();
#else
	//make app a TopWindow, that EventLoop can handle
	//not needed it app is a TopWindow itself
	TopWindow top;
	top.Add(app.SizePos());
	top.Open();
#endif
	Ctrl::EventLoop();
}
